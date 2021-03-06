#include <iostream>
#include <opencv2/opencv.hpp>

#include "global.hpp"
#include "rasterizer.hpp"
#include "Triangle.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "OBJ_Loader.h"
#include "myutils.h"

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1,0,0,-eye_pos[0],
                 0,1,0,-eye_pos[1],
                 0,0,1,-eye_pos[2],
                 0,0,0,1;

    view = translate*view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float angle)
{
    Eigen::Matrix4f rotation;
    angle = angle * MY_PI / 180.f;
    rotation << cos(angle), 0, sin(angle), 0,
                0, 1, 0, 0,
                -sin(angle), 0, cos(angle), 0,
                0, 0, 0, 1;

    Eigen::Matrix4f scale;
    scale << 2.5, 0, 0, 0,
              0, 2.5, 0, 0,
              0, 0, 2.5, 0,
              0, 0, 0, 1;

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    return translate * rotation * scale;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
    // TODO: Use the same projection matrix from the previous assignments
    Eigen::Matrix4f projection;
    //复制了Ass1中的代码
    float n = zNear, f = zFar;
    float foY = (eye_fov * MY_PI / 180.0);
    float t = abs(n) * tan(foY / 2.0);//abs必要，t一定大于0
    float r = t * aspect_ratio; //r一定大于0

    //github
    projection << n / r ,   0,              0,          0,
                    0   ,n /t,              0,          0,
                    0   ,   0, (n+f) / (n-f) , 2*n*f / (n-f),
                    0   ,   0,              -1,          0; 
    return projection;
}

Eigen::Vector3f vertex_shader(const vertex_shader_payload& payload)
{
    return payload.position;
}

//normal shader就是根据不同的法向量生成不同的颜色
//法向量方向相同，则颜色相同，方向不同则颜色不同
//最后的效果就是随着牛牛表面法向量方向的改变，颜色也逐渐改变
Eigen::Vector3f normal_fragment_shader(const fragment_shader_payload& payload)
{
    //+(1,1,1)什么意思？，为了使颜色更好看？
    //原本normal的xyz在[-1,1]之间,这么操作,在[0,1]之内]}
    Eigen::Vector3f return_color = (payload.normal.head<3>().normalized() + Eigen::Vector3f(1.0f, 1.0f, 1.0f)) / 2.f;
    Eigen::Vector3f result;
    result << return_color.x() * 255, return_color.y() * 255, return_color.z() * 255;
    return result;
}

static Eigen::Vector3f reflect(const Eigen::Vector3f& vec, const Eigen::Vector3f& axis)
{
    auto costheta = vec.dot(axis);
    return (2 * costheta * axis - vec).normalized();
}

struct light
{
    Eigen::Vector3f position;
    Eigen::Vector3f intensity;
};

/*
与Phongshading完全一致，
唯一的区别是，kd != 某个常数，(u,v)处的kd = texture在(u,v)处的颜色值
*/
Eigen::Vector3f texture_fragment_shader(const fragment_shader_payload& payload)
{
    Eigen::Vector3f return_color = {0, 0, 0};
    if (payload.texture)
    {
        // TODO: Get the texture value at the texture coordinates of the current fragment
        // range_check(payload.tex_coords[0]);
        // range_check(payload.tex_coords[1]);
        // payload.tex_coords[1] = std::min(1, std::max(0, payload.tex_coords[1]));

        return_color = payload.texture->getColor(payload.tex_coords[0], payload.tex_coords[1]);

    }
    Eigen::Vector3f texture_color;
    texture_color << return_color.x(), return_color.y(), return_color.z();

    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = texture_color / 255.f;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{10, 10, 10};
    Eigen::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    // Eigen::Vector3f color = texture_color;
    Eigen::Vector3f point = payload.view_pos;
    // Eigen::Vector3f normal = payload.normal;
    //有些向量相对于光源不变
    Eigen::Vector3f n = payload.normal.normalized();
    Vector3f v = (eye_pos - point).normalized();

    Eigen::Vector3f result_color = {0, 0, 0};
    for (auto& light : lights)
    {
        // TODO: For each light source in the code, calculate what the *ambient*, *diffuse*, and *specular* 
        // components are. Then, accumulate that result on the *result_color* object.
        float attenuation = 1.0f / std::pow((point -  light.position).norm(), 2);  // 1/r^2 
        Vector3f attenuation_intensity = attenuation * light.intensity; 
        Vector3f l = (light.position - point).normalized(); //光照方向，自shading point到光源
        Vector3f h = (v+l).normalized();


        //环境光
        Vector3f ambient_color = amb_light_intensity.cwiseProduct(ka);

        //漫反射光
        float diff_cos = std::max(0.0f, l.dot(n));
        Vector3f diffuse_color = kd.cwiseProduct(attenuation_intensity) * diff_cos;

        //镜面反射
        float spec_cos = std::max(0.0f, h.dot(n));
        Vector3f specular_color = ks.cwiseProduct(attenuation_intensity) * std::pow(spec_cos, p);
        // printf("spec = %.3f %.3f %.3f, cos = %.4f, h.dot(n) = %.4f ", specular_color[0], specular_color[1], specular_color[2], spec_cos, h.dot(n));
        // printf("diff = %.3f %.3f %.3f, ", diffuse_color[0], diffuse_color[1], diffuse_color[2]);
        // printf("amb = %.3f %.3f %.3f\n", ambient_color[0], ambient_color[1], ambient_color[2]);
        result_color += specular_color + diffuse_color + ambient_color;
        // result_color += diffuse_color + ambient_color;
        // result_color += specular_color;
        // result_color += diffuse_color;
        // result_color += ambient_color;
    }
    // printf("Result_color = %.3f %.3f %.3f\n", result_color[0], result_color[1], result_color[2]);
    result_color *= 255.f;
    // printf("Result_color = %.3f %.3f %.3f\n", result_color[0], result_color[1], result_color[2]);
    return result_color;

}

Eigen::Vector3f phong_fragment_shader(const fragment_shader_payload& payload)
{
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005); //环境光
    Eigen::Vector3f kd = payload.color; //漫反射系数
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937); //镜面反射系数

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{10, 10, 10}; //I_a，即RGB下的强度
    Eigen::Vector3f eye_pos{0, 0, 10}; //

    float p = 150;

    // Eigen::Vector3f color = payload.color;
    Eigen::Vector3f point = payload.view_pos;

    //有些向量相对于光源不变
    Eigen::Vector3f n = payload.normal.normalized();
    Vector3f v = (eye_pos - point).normalized();

    Eigen::Vector3f result_color = {0, 0, 0};
    for (auto& light : lights)
    {
        // TODO: For each light source in the code, calculate what the *ambient*, *diffuse*, and *specular* 
        // components are. Then, accumulate that result on the *result_color* object.
        float attenuation = 1.0f / std::pow((point -  light.position).norm(), 2);  // 1/r^2 
        Vector3f attenuation_intensity = attenuation * light.intensity; 
        Vector3f l = (light.position - point).normalized(); //光照方向，自shading point到光源
        Vector3f h = (v+l).normalized();


        //环境光
        Vector3f ambient_color = amb_light_intensity.cwiseProduct(ka);

        //漫反射光
        float diff_cos = std::max(0.0f, l.dot(n));
        Vector3f diffuse_color = kd.cwiseProduct(attenuation_intensity) * diff_cos;

        //镜面反射
        float spec_cos = std::max(0.0f, h.dot(n));
        Vector3f specular_color = ks.cwiseProduct(attenuation_intensity) * std::pow(spec_cos, 150);
        // printf("spec = %.3f %.3f %.3f, cos = %.4f, h.dot(n) = %.4f ", specular_color[0], specular_color[1], specular_color[2], spec_cos, h.dot(n));
        // printf("diff = %.3f %.3f %.3f, ", diffuse_color[0], diffuse_color[1], diffuse_color[2]);
        // printf("amb = %.3f %.3f %.3f\n", ambient_color[0], ambient_color[1], ambient_color[2]);
        result_color += specular_color + diffuse_color + ambient_color;
        // result_color += diffuse_color + ambient_color;
        // result_color += specular_color;
        // result_color += diffuse_color;
        // result_color += ambient_color;
    }
    // printf("Result_color = %.3f %.3f %.3f\n", result_color[0], result_color[1], result_color[2]);
    result_color *= 255.f;
    // printf("Result_color = %.3f %.3f %.3f\n", result_color[0], result_color[1], result_color[2]);
    return result_color;

}



Eigen::Vector3f displacement_fragment_shader(const fragment_shader_payload& payload)
{
    
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = payload.color;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{10, 10, 10};
    Eigen::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Eigen::Vector3f color = payload.color; 
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f n = payload.normal.normalized();

    float kh = 0.2, kn = 0.1;
    
    // TODO: Implement displacement mapping here
    // Let n = normal = (x, y, z)
    // Vector t = (x*y/sqrt(x*x+z*z),sqrt(x*x+z*z),z*y/sqrt(x*x+z*z))
    // Vector b = n cross product t
    // Matrix TBN = [t b n]
    // dU = kh * kn * (h(u+1/w,v)-h(u,v))
    // dV = kh * kn * (h(u,v+1/h)-h(u,v))
    // Vector ln = (-dU, -dV, 1)
    // Position p = p + kn * n * h(u,v)
    // Normal n = normalize(TBN * ln)

    float x = n.x(), y = n.y(), z = n.z();
    float w = payload.texture->width;
    float h = payload.texture->height;
    float u = payload.tex_coords[0];
    float v = payload.tex_coords[1];
    // range_check(u);
    // range_check(u);

    Vector3f t{x*y/std::sqrt(x*x+z*z),std::sqrt(x*x+z*z),z*y/std::sqrt(x*x+z*z)};
    Vector3f b = n.cross(t);
    Matrix3f TBN;
    // TBN.col(0) = t;
    // TBN.col(1) = b;
    // TBN.col(2) = n;
    TBN<<t,b,n;
    float new_u = u + 1.0f/w, new_v = v + 1.0f / h; 
    //防止new_u new_v超出uv边界
    if (new_u > 1 || new_u < 0){
        new_u = u;
    }
    if (new_v > 1 || new_v < 0){
        new_v = v;
    }

    float dU = kh * kn * (payload.texture->getColor(new_u, v).norm() - payload.texture->getColor(u, v).norm());
    float dV = kh * kn * (payload.texture->getColor(u , new_v).norm() - payload.texture->getColor(u, v).norm());

    Vector3f ln{-dU, -dV, 1.0f};

    // point = point + kn * n.cwiseProduct(payload.texture->getColor(u, v));
    //注意这里相比于bump mapping 真正改变了点在view space中的位置
    point = point + kn * n * (payload.texture->getColor(u, v)).norm();
    
    
    //perturbed之后的法向量
    n = (TBN * ln).normalized();
    
    Eigen::Vector3f result_color = {0, 0, 0};

    Vector3f view_v = (eye_pos - point).normalized();

    for (auto& light : lights)
    {
        // TODO: For each light source in the code, calculate what the *ambient*, *diffuse*, and *specular* 
        // components are. Then, accumulate that result on the *result_color* object.
        float attenuation = 1.0f / std::pow((point -  light.position).norm(), 2);  // 1/r^2 
        Vector3f attenuation_intensity = attenuation * light.intensity; 
        Vector3f l = (light.position - point).normalized(); //光照方向，自shading point到光源
        Vector3f h = (view_v+l).normalized();


        //环境光
        Vector3f ambient_color = amb_light_intensity.cwiseProduct(ka);

        //漫反射光
        float diff_cos = std::max(0.0f, l.dot(n));
        Vector3f diffuse_color = kd.cwiseProduct(attenuation_intensity) * diff_cos;

        //镜面反射
        float spec_cos = std::max(0.0f, h.dot(n));
        Vector3f specular_color = ks.cwiseProduct(attenuation_intensity) * std::pow(spec_cos, p);
        // printf("spec = %.3f %.3f %.3f, cos = %.4f, h.dot(n) = %.4f ", specular_color[0], specular_color[1], specular_color[2], spec_cos, h.dot(n));
        // printf("diff = %.3f %.3f %.3f, ", diffuse_color[0], diffuse_color[1], diffuse_color[2]);
        // printf("amb = %.3f %.3f %.3f\n", ambient_color[0], ambient_color[1], ambient_color[2]);
        // result_color += specular_color + diffuse_color + ambient_color;
        // result_color += diffuse_color + ambient_color;
        result_color += specular_color;
        // result_color += diffuse_color;
        // result_color += ambient_color;

    }

    return result_color * 255.f;
}


Eigen::Vector3f bump_fragment_shader(const fragment_shader_payload& payload)
{
    
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = payload.color;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{10, 10, 10};
    Eigen::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Eigen::Vector3f color = payload.color; 
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f n = payload.normal.normalized();


    float kh = 0.2, kn = 0.1;


    // TODO: Implement bump mapping here
    // Let n = normal = (x, y, z)
    // Vector t = (x*y/sqrt(x*x+z*z),sqrt(x*x+z*z),z*y/sqrt(x*x+z*z))
    // Vector b = n cross product t
    // Matrix TBN = [t b n]
    // dU = kh * kn * (h(u+1/w,v)-h(u,v))
    // dV = kh * kn * (h(u,v+1/h)-h(u,v))
    // Vector ln = (-dU, -dV, 1)
    // Normal n = normalize(TBN * ln)
    float x = n.x(), y = n.y(), z = n.z();
    float w = payload.texture->width;
    float h = payload.texture->height;
    float u = payload.tex_coords[0];
    float v = payload.tex_coords[1];
    // range_check(u);
    // range_check(u);

    Vector3f t{x*y/std::sqrt(x*x+z*z),std::sqrt(x*x+z*z),z*y/std::sqrt(x*x+z*z)};
    Vector3f b = n.cross(t);
    Matrix3f TBN;
    TBN.col(0) = t;
    TBN.col(1) = b;
    TBN.col(2) = n;
    float new_u = u + 1.0f/w, new_v = v + 1.0f / h; 
    //防止new_u new_v超出uv边界
    if (new_u > 1 || new_u < 0){
        new_u = u;
    }
    if (new_v > 1 || new_v < 0){
        new_v = v;
    }
    //
    // try{

    // range_check(new_u);
    // range_check(new_v);
    // }catch(std::exception e){
    //     printf("u,v = %.2f, %.2f; new_u, new_v = %.2f, %.2f\n", u, v, new_u, new_v);
    //     assert(false);
    // }
    // printf("u,v = %.2f, %.2f; new_u, new_v = %.2f, %.2f\n", u, v, new_u, new_v);
    // assert(new_u < 0 || new_u >=1.0 || new_v < 0 || new_v >= 1.0);

    float dU = kh * kn * (payload.texture->getColor(new_u, v).norm() - payload.texture->getColor(u, v).norm());
    float dV = kh * kn * (payload.texture->getColor(u , new_v).norm() - payload.texture->getColor(u, v).norm());

    Vector3f ln{-dU, -dV, 1.0f};
    Vector3f perturbed_n = (TBN * ln).normalized();
    






    Eigen::Vector3f result_color = {0, 0, 0};
    result_color = perturbed_n;

    return result_color * 255.f;
}


int main(int argc, const char** argv)
{
    std::vector<Triangle*> TriangleList;

    float angle = 140.0;
    bool command_line = false;

    std::string filename = "output.png";
    objl::Loader Loader;
    std::string obj_path = "../models/spot/";

    // Load .obj File
    bool loadout = Loader.LoadFile("../models/spot/spot_triangulated_good.obj");
    for(auto mesh:Loader.LoadedMeshes)
    {
        for(int i=0;i<mesh.Vertices.size();i+=3)
        {
            Triangle* t = new Triangle();
            for(int j=0;j<3;j++)
            {

                t->setVertex(j,Vector4f(mesh.Vertices[i+j].Position.X,mesh.Vertices[i+j].Position.Y,mesh.Vertices[i+j].Position.Z,1.0));
                t->setNormal(j,Vector3f(mesh.Vertices[i+j].Normal.X,mesh.Vertices[i+j].Normal.Y,mesh.Vertices[i+j].Normal.Z));
                t->setTexCoord(j,Vector2f(mesh.Vertices[i+j].TextureCoordinate.X, mesh.Vertices[i+j].TextureCoordinate.Y));
            }
            TriangleList.push_back(t);
        }
    }

    rst::rasterizer r(700, 700);

    auto texture_path = "hmap.jpg";
    r.set_texture(Texture(obj_path + texture_path));

    std::function<Eigen::Vector3f(fragment_shader_payload)> active_shader = phong_fragment_shader;

    if (argc >= 2)
    {
        command_line = true;
        filename = std::string(argv[1]);

        if (argc == 3 && std::string(argv[2]) == "texture")
        {
            std::cout << "Rasterizing using the texture shader\n";
            active_shader = texture_fragment_shader;
            texture_path = "spot_texture.png";
            r.set_texture(Texture(obj_path + texture_path));
        }
        else if (argc == 3 && std::string(argv[2]) == "normal")
        {
            std::cout << "Rasterizing using the normal shader\n";
            active_shader = normal_fragment_shader;
        }
        else if (argc == 3 && std::string(argv[2]) == "phong")
        {
            std::cout << "Rasterizing using the phong shader\n";
            active_shader = phong_fragment_shader;
        }
        else if (argc == 3 && std::string(argv[2]) == "bump")
        {
            std::cout << "Rasterizing using the bump shader\n";
            active_shader = bump_fragment_shader;
        }
        else if (argc == 3 && std::string(argv[2]) == "displacement")
        {
            std::cout << "Rasterizing using the bump shader\n";
            active_shader = displacement_fragment_shader;
        }
    }

    Eigen::Vector3f eye_pos = {0,0,10};

    r.set_vertex_shader(vertex_shader);
    r.set_fragment_shader(active_shader);

    int key = 0;
    int frame_count = 0;

    if (command_line)
    {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);
        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45.0, 1, 0.1, 50));

        r.draw(TriangleList);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

        cv::imwrite(filename, image);

        return 0;
    }

    while(key != 27)
    {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45.0, 1, 0.1, 50));

        //r.draw(pos_id, ind_id, col_id, rst::Primitive::Triangle);
        r.draw(TriangleList);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

        cv::imshow("image", image);
        cv::imwrite(filename, image);
        key = cv::waitKey(10);

        if (key == 'a' )
        {
            angle -= 0.1;
        }
        else if (key == 'd')
        {
            angle += 0.1;
        }

    }
    return 0;
}
