//
// Created by goksu on 4/6/19.
//

#include <algorithm>
#include "rasterizer.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>


rst::pos_buf_id rst::rasterizer::load_positions(const std::vector<Eigen::Vector3f> &positions)
{
    auto id = get_next_id();
    pos_buf.emplace(id, positions);

    return {id};
}

rst::ind_buf_id rst::rasterizer::load_indices(const std::vector<Eigen::Vector3i> &indices)
{
    auto id = get_next_id();
    ind_buf.emplace(id, indices);

    return {id};
}

rst::col_buf_id rst::rasterizer::load_colors(const std::vector<Eigen::Vector3f> &cols)
{
    auto id = get_next_id();
    col_buf.emplace(id, cols);

    return {id};
}

rst::col_buf_id rst::rasterizer::load_normals(const std::vector<Eigen::Vector3f>& normals)
{
    auto id = get_next_id();
    nor_buf.emplace(id, normals);

    normal_id = id;

    return {id};
}


// Bresenham's line drawing algorithm
void rst::rasterizer::draw_line(Eigen::Vector3f begin, Eigen::Vector3f end)
{
    auto x1 = begin.x();
    auto y1 = begin.y();
    auto x2 = end.x();
    auto y2 = end.y();

    Eigen::Vector3f line_color = {255, 255, 255};

    int x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;

    dx=x2-x1;
    dy=y2-y1;
    dx1=fabs(dx);
    dy1=fabs(dy);
    px=2*dy1-dx1;
    py=2*dx1-dy1;

    if(dy1<=dx1)
    {
        if(dx>=0)
        {
            x=x1;
            y=y1;
            xe=x2;
        }
        else
        {
            x=x2;
            y=y2;
            xe=x1;
        }
        Eigen::Vector2i point = Eigen::Vector2i(x, y);
        set_pixel(point,line_color);
        for(i=0;x<xe;i++)
        {
            x=x+1;
            if(px<0)
            {
                px=px+2*dy1;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    y=y+1;
                }
                else
                {
                    y=y-1;
                }
                px=px+2*(dy1-dx1);
            }
//            delay(0);
            Eigen::Vector2i point = Eigen::Vector2i(x, y);
            set_pixel(point,line_color);
        }
    }
    else
    {
        if(dy>=0)
        {
            x=x1;
            y=y1;
            ye=y2;
        }
        else
        {
            x=x2;
            y=y2;
            ye=y1;
        }
        Eigen::Vector2i point = Eigen::Vector2i(x, y);
        set_pixel(point,line_color);
        for(i=0;y<ye;i++)
        {
            y=y+1;
            if(py<=0)
            {
                py=py+2*dx1;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    x=x+1;
                }
                else
                {
                    x=x-1;
                }
                py=py+2*(dx1-dy1);
            }
//            delay(0);
            Eigen::Vector2i point = Eigen::Vector2i(x, y);
            set_pixel(point,line_color);
        }
    }
}

auto to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
{
    return Vector4f(v3.x(), v3.y(), v3.z(), w);
}

static bool insideTriangle(int x, int y, const Vector4f* _v){
    Vector3f v[3];
    for(int i=0;i<3;i++)
        v[i] = {_v[i].x(),_v[i].y(), 1.0};
    Vector3f f0,f1,f2;
    f0 = v[1].cross(v[0]);
    f1 = v[2].cross(v[1]);
    f2 = v[0].cross(v[2]);
    Vector3f p(x,y,1.);
    if((p.dot(f0)*f0.dot(v[2])>0) && (p.dot(f1)*f1.dot(v[0])>0) && (p.dot(f2)*f2.dot(v[1])>0))
        return true;
    return false;
}

//在2d屏幕上，也就是说，三角形顶点只用到了x,y坐标
static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Vector4f* v){
    float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
    float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
    float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
    return {c1,c2,c3};
}

struct InterpolateWithPerspectiveCorrect{
    float alpha;
    float beta;
    float  gamma;
    float w_reciprocal;

    const Vector4f *v;

    InterpolateWithPerspectiveCorrect(float alpha, float beta, float gamma, float w_reciprocal, const Vector4f *v) 
    : alpha(alpha), beta(beta), gamma(gamma), w_reciprocal(w_reciprocal), v(v)
    {
        
    }
    
    template <typename T>
    T interpolate(T I_a, T I_b, T I_c){
        T I_interpolated = alpha * I_a / v[0].w() + beta * I_b / v[1].w() + gamma * I_c / v[2].w();
        I_interpolated *= w_reciprocal;
        return I_interpolated;
    }

};

void rst::rasterizer::draw(std::vector<Triangle *> &TriangleList) {

    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;
    printf("len of TriList is %d\n", TriangleList.size());
    Eigen::Matrix4f mvp = projection * view * model;
    for (const auto& t:TriangleList)
    {
        //clone of tri
        Triangle newtri = *t; 

        std::array<Eigen::Vector4f, 3> mm {
                (view * model * t->v[0]),
                (view * model * t->v[1]),
                (view * model * t->v[2])
        };

        //view坐标
        std::array<Eigen::Vector3f, 3> viewspace_pos;

        //View/model 变换不会改变w的值，w仍为1，所以可以直接舍弃w的坐标
        std::transform(mm.begin(), mm.end(), viewspace_pos.begin(), [](auto& v) {
            return v.template head<3>();
        });

        //mvp变换后的坐标
        Eigen::Vector4f v[] = {
                mvp * t->v[0],
                mvp * t->v[1],
                mvp * t->v[2]
        };
        //Homogeneous division
        //Ass2这里是vec /= vec.w();
        //Ass3这里就变成了不处理vec.w()
        //疑惑？
        for (auto& vec : v) {
            vec.x()/=vec.w();
            vec.y()/=vec.w();
            vec.z()/=vec.w();
        }

        //法向量变换矩阵
        Eigen::Matrix4f inv_trans = (view * model).inverse().transpose();

        //view space 中每个顶点的法向量，需要normalize吗？还是根本不关心
        Eigen::Vector4f n[] = {
                inv_trans * to_vec4(t->normal[0], 0.0f),
                inv_trans * to_vec4(t->normal[1], 0.0f),
                inv_trans * to_vec4(t->normal[2], 0.0f)
        };

        //Viewport transformation
        for (auto & vert : v)
        {
            vert.x() = 0.5*width*(vert.x()+1.0);
            vert.y() = 0.5*height*(vert.y()+1.0);
            vert.z() = vert.z() * f1 + f2;
        }


        //newtri是三角形再在屏幕空间中的表示
        for (int i = 0; i < 3; ++i)
        {
            //screen space coordinates
            newtri.setVertex(i, v[i]);
        }

        //newtri在屏幕空间，但是NormalVector是在view Space
        for (int i = 0; i < 3; ++i)
        {
            //view space normal
            newtri.setNormal(i, n[i].head<3>());
        }

        newtri.setColor(0, 148,121.0,92.0);
        newtri.setColor(1, 148,121.0,92.0);
        newtri.setColor(2, 148,121.0,92.0);

        // Also pass view space vertice position
        rasterize_triangle(newtri, viewspace_pos);
    }
}

static Eigen::Vector3f interpolate(float alpha, float beta, float gamma, const Eigen::Vector3f& vert1, const Eigen::Vector3f& vert2, const Eigen::Vector3f& vert3, float weight)
{
    return (alpha * vert1 + beta * vert2 + gamma * vert3) / weight;
}

static Eigen::Vector2f interpolate(float alpha, float beta, float gamma, const Eigen::Vector2f& vert1, const Eigen::Vector2f& vert2, const Eigen::Vector2f& vert3, float weight)
{
    auto u = (alpha * vert1[0] + beta * vert2[0] + gamma * vert3[0]);
    auto v = (alpha * vert1[1] + beta * vert2[1] + gamma * vert3[1]);

    u /= weight;
    v /= weight;

    return Eigen::Vector2f(u, v);
}

//Screen space rasterization
void rst::rasterizer::rasterize_triangle(const Triangle& t, const std::array<Eigen::Vector3f, 3>& view_pos) 
{
    // TODO: From your HW3, get the triangle rasterization code.
    // TODO: Inside your rasterization loop:
    //    * v[i].w() is the vertex view space depth value z.
    //    * Z is interpolated view space depth for the current pixel
    //    * zp is depth between zNear and zFar, used for z-buffer

    // float Z = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
    // float zp = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
    // zp *= Z;


    
    // TODO : Find out the bounding box of current triangle.
    // iterate through the pixel and find if the current pixel is inside the triangle
    //1. find bounding box
    float max_x = 0.0, max_y = 0.0, min_x = this->width - 1, min_y = this->height - 1;
    for (const auto & vertex:t.v){
        min_x = std::min(vertex.x(), min_x);
        min_y = std::min(vertex.y(), min_y);
        max_x = std::max(vertex.x(), max_x);
        max_y = std::max(vertex.y(), max_y);
    }

    int i_min_x = std::floor(min_x);
    int i_min_y = std::floor(min_y);
    int i_max_x = std::ceil(max_x);
    int i_max_y = std::ceil(max_y);

    const auto &v = t.v; //只是为了少写一个t.
    // printf("Tri: [(%.2f, %.2f, %.2f, %.2f), (%.2f, %.2f, %.2f, %.2f), (%.2f, %.2f, %.2f, %.2f) ];\nBBBox : x[%d, %d] x y[%d, %d]\n",
    //     v[0].x(), v[0].y(), v[0].z(),v[0].w(),
    //     v[1].x(), v[1].y(), v[1].z(),v[1].w(),
    //     v[2].x(), v[2].y(), v[2].z(),v[2].w(),
    //     i_min_x, i_max_x, i_min_y, i_max_y
    // );


    //2. z-buffer
    int inside_cnt = 0;
    for(int x = i_min_x; x < i_max_x; ++x){
        for(int y = i_min_y; y < i_max_y; ++y){
            float center_x = x + 0.5, center_y = y + 0.5;
            bool is_inside = insideTriangle(x, y, t.v);

            if(is_inside){
                //debug
                // printf("P[%.2f, %.2f] in Tri[(%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f) ]\n", 
                //     center_x, center_y, 
                //     v[0].x(), v[0].y(),
                //     v[1].x(), v[1].y(),
                //     v[2].x(), v[2].y()
                // );
                inside_cnt += 1;

                //因为我们只知道三角形每个顶点的z
                //但是不知道每个像素的z
                //所以要插值计算
                //注意Ass2这里v.w()都是1
                //而Ass3中v.w()不为1，不过computeBarycentric2D
                float alpha, beta, gamma; //ERROR:structure binding cannot be captured
                std::tie(alpha, beta, gamma) = computeBarycentric2D(center_x, center_y, t.v);
                
                //这里其实做了一个透视插值误差修正：
                //Ass2中在homogenous division这一步，将w也变换为了1，所以Ass2中其实没有修正
                //Ass3中w没有做除法，所以有修正。
                //这和https://zhuanlan.zhihu.com/p/144331875 的Eq. 16完全一致
                float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                // float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
                // z_interpolated *= w_reciprocal;


                InterpolateWithPerspectiveCorrect interpolator(alpha,beta,gamma, w_reciprocal, v);
                // auto correctTextureFloat = [=](float I_a, float I_b, float I_c) ->float{
                //     float I_interpolated = alpha * I_a / v[0].w() + beta * I_b / v[1].w() + gamma * I_c / v[2].w();
                //     I_interpolated *= w_reciprocal;
                //     return I_interpolated;
                // };

                // auto correctTextureVector3f = [=](Vector3f I_a, Vector3f I_b, Vector3f I_c) ->Vector3f{
                    
                //     Vector3f I_interpolated = alpha * I_a / v[0].w() + beta * I_b / v[1].w() + gamma * I_c / v[2].w();
                //     I_interpolated *= w_reciprocal;
                //     return I_interpolated;
                // };

                float z_interpolated = interpolator.interpolate<float>(v[0].z(), v[1].z(), v[2].z());

                //断言，防止访问depth_buf越界
                assert(x < this->width );
                assert(y < this->height );
                if(z_interpolated < this->depth_buf[get_index(x, y)] ){
                    this->depth_buf[get_index(x, y)] = z_interpolated;

                    //插值得到x,y处的法向量
                    Vector3f interpolated_normal = interpolator.interpolate<Vector3f>(t.normal[0], t.normal[1], t.normal[2]);
                    Vector3f interpolated_color = interpolator.interpolate<Vector3f>(t.color[0], t.color[1], t.color[2]);
                    Vector3f interpolated_view_pos = interpolator.interpolate<Vector3f>(view_pos[0], view_pos[1], view_pos[2]);

                    //构造shader_payload传给this->fragment_shader
                    fragment_shader_payload payload;
                    payload.normal = interpolated_normal;
                    payload.color = interpolated_color;
                    payload.view_pos = interpolated_view_pos;

                    //调用shader
                    Vector3f pixel_color = this->fragment_shader(payload);

                    //设置颜色
                    this->frame_buf[get_index(x,y)] = pixel_color;
                    
                } 
            }
        }
    }
    // printf("inside cnt [%d] for Tri[(%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f) \n",
    //     inside_cnt,
    //     v[0].x(), v[0].y(),
    //     v[1].x(), v[1].y(),
    //     v[2].x(), v[2].y()
    // );

    // TODO: Interpolate the attributes:
    // auto interpolated_color
    // auto interpolated_normal
    // auto interpolated_texcoords
    // auto interpolated_shadingcoords

    // Use: fragment_shader_payload payload( interpolated_color, interpolated_normal.normalized(), interpolated_texcoords, texture ? &*texture : nullptr);
    // Use: payload.view_pos = interpolated_shadingcoords;
    // Use: Instead of passing the triangle's color directly to the frame buffer, pass the color to the shaders first to get the final color;
    // Use: auto pixel_color = fragment_shader(payload);

 
}

void rst::rasterizer::set_model(const Eigen::Matrix4f& m)
{
    model = m;
}

void rst::rasterizer::set_view(const Eigen::Matrix4f& v)
{
    view = v;
}

void rst::rasterizer::set_projection(const Eigen::Matrix4f& p)
{
    projection = p;
}

void rst::rasterizer::clear(rst::Buffers buff)
{
    if ((buff & rst::Buffers::Color) == rst::Buffers::Color)
    {
        std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f{0, 0, 0});
    }
    if ((buff & rst::Buffers::Depth) == rst::Buffers::Depth)
    {
        std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity());
    }
}

rst::rasterizer::rasterizer(int w, int h) : width(w), height(h)
{
    frame_buf.resize(w * h);
    depth_buf.resize(w * h);

    texture = std::nullopt;
}

int rst::rasterizer::get_index(int x, int y)
{
    return (height-y)*width + x;
}

void rst::rasterizer::set_pixel(const Vector2i &point, const Eigen::Vector3f &color)
{
    //old index: auto ind = point.y() + point.x() * width;
    int ind = (height-point.y())*width + point.x();
    frame_buf[ind] = color;
}

void rst::rasterizer::set_vertex_shader(std::function<Eigen::Vector3f(vertex_shader_payload)> vert_shader)
{
    vertex_shader = vert_shader;
}

void rst::rasterizer::set_fragment_shader(std::function<Eigen::Vector3f(fragment_shader_payload)> frag_shader)
{
    fragment_shader = frag_shader;
}

