#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
        -eye_pos[2], 0, 0, 0, 1;
    //view = T * R，R是单位矩阵，是因为相机摆在eye_pos，看向-z轴，不需要任何旋转
    //T也很容易算 t_x = -eye_pos[0], t_y = -eye_pos[1], t_z = -eye_pos[2]
    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    
    float theta = (rotation_angle * MY_PI / 180.0);

    model(0,0) = cos(theta);
    model(0,1) = -sin(theta);
    model(1,0) = sin(theta);
    model(1,1) = cos(theta);

    std::cout << "model M" << std::endl << model <<std::endl;


    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.
    /*
    zNear = -zNear; zFar = -zFar; //给出的zNear zFar都是整数，但实际上他们都应该在z轴负半轴，为负数
    projection(0,0) = zNear;
    projection(1,1) = zNear;

    projection(2,2) = zNear + zFar;
    projection(2,3) = -zNear * zFar;

    projection(3,3) = 0;
    projection(3,2) = 1;

    float n = zNear, f = zFar;
    float foY = (eye_fov * MY_PI / 180.0);
    float t = abs(n) * tan(foY / 2.0);//abs必要，t一定大于0
    float b = -t;
    printf("tan = %.2f, t = %.2f", tan(foY / 2.0), t);
    
    float r = t * aspect_ratio; //r一定大于0
    float l = -r;

    //不能auto？为什么
    Eigen::Matrix4f orthoProjection_translate = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f orthoProjection_scale = Eigen::Matrix4f::Identity();

    orthoProjection_translate(0,3) = -0.5 * (r+l);
    orthoProjection_translate(1,3) = -0.5 * (t+b);
    orthoProjection_translate(2,3) = -0.5 * (n+f);
    
    //先平移 再伸缩
    orthoProjection_scale(0,0) = 2.0 / (r-l);
    orthoProjection_scale(1,1) = 2.0 / (t-b);
    orthoProjection_scale(2,2) = 2.0 / (n-f);

    auto orthoProjection = orthoProjection_scale * orthoProjection_translate;

    
    projection = orthoProjection * projection;
    */
    float n = -zNear, f = -zFar;
    float foY = (eye_fov * MY_PI / 180.0);
    float t = abs(n) * tan(foY / 2.0);//abs必要，t一定大于0
    float r = t * aspect_ratio; //r一定大于0


    projection << n / r ,   0,              0,          0,
                    0   ,n /t,              0,          0,
                    0   ,   0, (n+f) / (n-f) , -2*n*f / (n-f),
                    0   ,   0,              1,          0;  

    return projection;
}

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
        else
            return 0;
    }

    //获得光栅化器
    rst::rasterizer r(700, 700);
    //view的坐标  
    Eigen::Vector3f eye_pos = {0, 0, 5};
    //等腰直角三角形
    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};
    //模型原点坐标，在三角形的中心/重心上
    //并不是，只是代表有这3个点 第0个点 第1个点 第2个点
    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    //
    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    //不显示交互式窗口，保存为图片退出。
    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        }
        else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}
