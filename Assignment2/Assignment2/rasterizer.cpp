// clang-format off
//
// Created by goksu on 4/6/19.
//

#include <algorithm>
#include <vector>
#include "rasterizer.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>

using std::endl;
using std::cout;

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

auto to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
{
    return Vector4f(v3.x(), v3.y(), v3.z(), w);
}

//用于判断三角形inside时，需要保证顶点以顺时针方向保存
static bool all_negative(float a, float b, float c){
    // return (a < 0 && b < 0 && c < 0); //三角形边上的点不算在三角形内
    return (a <= 0 && b <= 0 && c <= 0); //三角形边上的点算在三角形内
}

//用于判断三角形inside时，需要保证顶点以逆时针方向保存
static bool all_positive(float a, float b, float c){
    // return (a < 0 && b < 0 && c < 0); //三角形边上的点不算在三角形内
    return (a >= 0 && b >= 0 && c >= 0); //三角形边上的点算在三角形内
}

static bool insideTriangle(float x, float y, const Triangle& tri)
{   
    // TODO : Implement this function to check if the point (x, y) is inside the triangle represented by _v[0], _v[1], _v[2]
    // const auto v_xy = tri.v;
    const auto &v_xy = tri.toVector2();
    Eigen::Vector2f side_1 = v_xy[1] - v_xy[0];
    Eigen::Vector2f side_2 = v_xy[2] - v_xy[1];
    Eigen::Vector2f side_3 = v_xy[0] - v_xy[2];

    std::vector<Vector2f> sides{side_1,side_2,side_3};

    Eigen::Vector2f p(x,y);

    std::vector<float> cross_product_zs{0,0,0}; //3次叉乘的z的结果

    // const int num = v_xy.size();
    const int num = 3;
    for (int i = 0; i < num; ++i){
        Vector2f v = p - v_xy[i]; //AP, BP, CP，顶点到p的向量

        //这里我们不用把p sides都变成3维的向量
        //因为叉乘的结果x,y都为0，我们只关注z即可
        float cross_result_z = sides[i].x() * v.y() - sides[i].y() * v.x();
        cross_product_zs[i] = cross_result_z;
        // printf("cross with sides[%d] = ", i);
        // cout << cross_result_z << endl;
    }

    return all_positive(cross_product_zs[0], cross_product_zs[1], cross_product_zs[2]);
    
}

static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Vector3f* v)
{
    float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
    float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
    float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
    return {c1,c2,c3};
}

void rst::rasterizer::draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, col_buf_id col_buffer, Primitive type)
{
    auto& buf = pos_buf[pos_buffer.pos_id];
    auto& ind = ind_buf[ind_buffer.ind_id];
    auto& col = col_buf[col_buffer.col_id];

    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;

    Eigen::Matrix4f mvp = projection * view * model;
    for (auto& i : ind)
    {
        Triangle t;
        Eigen::Vector4f v[] = {
                mvp * to_vec4(buf[i[0]], 1.0f),
                mvp * to_vec4(buf[i[1]], 1.0f),
                mvp * to_vec4(buf[i[2]], 1.0f)
        };
        //Homogeneous division
        for (auto& vec : v) {
            // cout << "Before division Tri " <<  "vertices:" <<  endl << vec << endl;
            vec /= vec.w(); //返回vec[3]即齐次坐标
            // cout << "After:Tri " <<  "vertices:" <<  endl << vec << endl;
        }
        
        //Viewport transformation
        for (auto & vert : v)
        {
            vert.x() = 0.5*width*(vert.x()+1.0);
            vert.y() = 0.5*height*(vert.y()+1.0);
            vert.z() = vert.z() * f1 + f2;
        }

        for (int i = 0; i < 3; ++i)
        {
            t.setVertex(i, v[i].head<3>()); //这里应该是只获得xyz坐标，不要w
            t.setVertex(i, v[i].head<3>());
            t.setVertex(i, v[i].head<3>());
        }

        auto col_x = col[i[0]];
        auto col_y = col[i[1]];
        auto col_z = col[i[2]];

        t.setColor(0, col_x[0], col_x[1], col_x[2]);
        t.setColor(1, col_y[0], col_y[1], col_y[2]);
        t.setColor(2, col_z[0], col_z[1], col_z[2]);

        rasterize_triangle(t);
    }
}

//Screen space rasterization
void rst::rasterizer::rasterize_triangle(const Triangle& t) {
    //三角形的三个顶点的齐次坐标表示
    //v.size() == 3, 每个元素是Vector4
    auto v = t.toVector4();

    
    // TODO : Find out the bounding box of current triangle.
    // iterate through the pixel and find if the current pixel is inside the triangle
    //1. find bounding box
    float max_x = 0.0, max_y = 0.0, min_x = this->width - 1, min_y = this->height - 1;
    for (const auto & vertex:v){
        min_x = std::min(vertex.x(), min_x);
        min_y = std::min(vertex.y(), min_y);
        max_x = std::max(vertex.x(), max_x);
        max_y = std::max(vertex.y(), max_y);
    }

    int i_min_x = std::floor(min_x);
    int i_min_y = std::floor(min_y);
    int i_max_x = std::ceil(max_x);
    int i_max_y = std::ceil(max_y);

    printf("Tri: [(%.2f, %.2f, %.2f), (%.2f, %.2f, %.2f), (%.2f, %.2f, %.2f) ];\nBBBox : x[%d, %d] x y[%d, %d]\n",
        v[0].x(), v[0].y(), v[0].z(),
        v[1].x(), v[1].y(), v[1].z(),
        v[2].x(), v[2].y(), v[2].z(),
        i_min_x, i_max_x, i_min_y, i_max_y
    );
    //generate offsets
    std::vector<std::array<float, 2> > offsets ;
    //msaa_coef = msaa_argv ^ 2
    offsets.resize(this->msaa_coef);
    float unit = 1.0f / (this->msaa_argv * 2.0f);
    for(int i = 0; i < this->msaa_argv; ++i){
        for(int j = 0; j < this->msaa_argv; ++j){
            offsets[i * this->msaa_argv + j] = 
                {(i*2 + 1)*unit, (j*2 + 1) * unit};
            // printf("offset[%d, %d] : %.2f, %.2f", i, j, offsets[i*msaa_argv + j][0], offsets[i*msaa_argv + j][1]);
        }
    }

    assert(offsets.size() == this->msaa_coef);

    //2. z-buffer
    int inside_cnt = 0;
    for(int x = i_min_x; x < i_max_x; ++x){
        for(int y = i_min_y; y < i_max_y; ++y){
            //msaa sample
            for(int ith_sample = 0; ith_sample < this->msaa_coef; ++ith_sample){
                float sample_x = (float)x + offsets[ith_sample][0];
                float sample_y = (float)y + offsets[ith_sample][1];
                // float center_x = x + 0.5, center_y = y + 0.5;
                bool is_inside = insideTriangle(sample_x, sample_y, t);

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
                    auto [alpha, beta, gamma] = computeBarycentric2D(sample_x, sample_y, t.v);
                    float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                    float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
                    z_interpolated *= w_reciprocal;
                    //断言，防止访问depth_buf越界
                    assert(x < this->width );
                    assert(y < this->height );
                    if(z_interpolated < this->depth_buf[get_msaa_index(x, y, ith_sample)] ){
                        this->depth_buf[get_msaa_index(x, y, ith_sample)] = z_interpolated;

                        //将frame_buf对应像素涂成getColor()
                        set_sample_color(x, y, ith_sample, t.getColor() );
                    } 
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

    //3. average sample colors to get pixel color
    for(int x = i_min_x; x < i_max_x; ++x){
        for(int y = i_min_y; y < i_max_y; ++y){
            Vector3f avg_color(0.0f, 0.0f, 0.0f);
            for(int ith_sample = 0; ith_sample < this->msaa_coef; ++ith_sample){
                //向量应该使可加的吧
                avg_color += this->msaa_frame_buf[get_msaa_index(x,y,ith_sample)];
            }
            avg_color /= float(this->msaa_coef);
            this->set_pixel(x, y, avg_color);
        }
    }
     



    // If so, use the following code to get the interpolated z value.
    //auto[alpha, beta, gamma] = computeBarycentric2D(x, y, t.v);
    //float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
    //float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
    //z_interpolated *= w_reciprocal;

    // TODO : set the current pixel (use the set_pixel function) to the color of the triangle (use getColor function) if it should be painted.
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

//1. z-buffer矩阵被初始化为无穷大
//msaa修改，使其成为4倍大
void rst::rasterizer::clear(rst::Buffers buff)
{
    if ((buff & rst::Buffers::Color) == rst::Buffers::Color)
    {
        std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f{0, 0, 0});
        std::fill(msaa_frame_buf.begin(), msaa_frame_buf.end(), Eigen::Vector3f{0, 0, 0});
    }
    if ((buff & rst::Buffers::Depth) == rst::Buffers::Depth)
    {
        std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity());
    }
}

rst::rasterizer::rasterizer(int w, int h, int msaa) : 
width(w), height(h), msaa_coef(msaa * msaa), msaa_argv(msaa)
{
    frame_buf.resize(w * h); //用一维vector模拟二维屏幕
    msaa_frame_buf.resize(w * h * msaa_coef ); //用一维vector模拟二维屏幕
    depth_buf.resize(w * h * msaa_coef); //同上
}

//排列方式为 :
//首先是y = height - 1这一行的所有pixel
//然后是y = height - 2这一行的所有pixel
//以此类推
int rst::rasterizer::get_index(int x, int y)
{
    return (height-1-y)*width + x;
}

//三维数组偏移
int rst::rasterizer::get_msaa_index(int x, int y, int ith_sample)
{
    return (get_index(x, y)) * msaa_coef + ith_sample;
}

// int rst::rasterizer::get_index(int x, int y)
// {
//     return (height-1-y)*width + x;
// }

void rst::rasterizer::set_pixel(int x, int y, const Eigen::Vector3f& color)
{
    //old index: auto ind = point.y() + point.x() * width;
    // auto ind = (height-1-point.y())*width + point.x();
    int ind = get_index(x, y);
    frame_buf[ind] = color;

}

//set color in msaa_frame_buf
void rst::rasterizer::set_sample_color(int x, int y, int ith_sample, const Eigen::Vector3f& color)
{
    //old index: auto ind = point.y() + point.x() * width;
    // auto ind = (height-1-point.y())*width + point.x();
    int ind = get_msaa_index(x, y, ith_sample);
    msaa_frame_buf[ind] = color;

}

// clang-format on