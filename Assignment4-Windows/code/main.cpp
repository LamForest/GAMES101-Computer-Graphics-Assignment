#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

std::vector<cv::Point2f> control_points;

const int B = 4;

void mouse_handler(int event, int x, int y, int flags, void *userdata) 
{
    if (event == cv::EVENT_LBUTTONDOWN && control_points.size() < B) 
    {
        std::cout << "Left button of the mouse is clicked - position (" << x << ", "
        << y << ")" << '\n';
        control_points.emplace_back(x, y);
    }     
}

void naive_bezier(const std::vector<cv::Point2f> &points, cv::Mat &window) 
{
    auto &p_0 = points[0];
    auto &p_1 = points[1];
    auto &p_2 = points[2];
    auto &p_3 = points[3];

    for (double t = 0.0; t <= 1.0; t += 0.001) 
    {
        auto point = std::pow(1 - t, 3) * p_0 + 3 * t * std::pow(1 - t, 2) * p_1 +
                 3 * std::pow(t, 2) * (1 - t) * p_2 + std::pow(t, 3) * p_3;

        window.at<cv::Vec3b>(point.y, point.x)[2] = 255;
    }
}

cv::Point2f recursive_bezier(const std::vector<cv::Point2f> &control_points, float t) 
{
    // TODO: Implement de Casteljau's algorithm
    if(control_points.size() < 2){
        throw std::invalid_argument("控制点个数最少为2");
        return cv::Point2f();
    }
    else if(control_points.size() == 2){
        return (1-t) * control_points[0] + t * control_points[1];
    }else{
        std::vector<cv::Point2f> next_level_control_points;
        // next_level_control_points.resize(co)
        for (int i = 0; i < control_points.size() - 1; ++i){
            cv::Point2f mid = recursive_bezier(
                std::vector<cv::Point2f>(control_points.begin() + i, control_points.begin() + i + 2),
                t
            );
            next_level_control_points.push_back(std::move(mid)); //构造cv::Point时，使用cv::Point的移动构造函数，Point内部不用再分配空间，直接窃取就行
        }
        assert(next_level_control_points.size() + 1 == control_points.size());
        return recursive_bezier(next_level_control_points, t);
    }

    // return cv::Point2f();

}

void bezier(const std::vector<cv::Point2f> &control_points, cv::Mat &window) 
{
    // TODO: Iterate through all t = 0 to t = 1 with small steps, and call de Casteljau's 
    // recursive Bezier algorithm.
    float sample_interval = 0.001f;
    for(float t = 0.0; t <= 1.0f; t += sample_interval){
        cv::Point2f point_at_t = recursive_bezier(control_points, t);
        window.at<cv::Vec3b>(point_at_t.y, point_at_t.x)[1] = 255;
    }
}

int main() 
{
    cv::Mat window = cv::Mat(700, 700, CV_8UC3, cv::Scalar(0));
    cv::cvtColor(window, window, cv::COLOR_BGR2RGB);
    cv::namedWindow("Bezier Curve", cv::WINDOW_AUTOSIZE);

    cv::setMouseCallback("Bezier Curve", mouse_handler, nullptr);
    int key = -1;
    while (key != 27) 
    {
        for (int i = 0; i < control_points.size();++i) 
        {
            cv::circle(window, control_points[i], i, {255, 255, 255}, 3);
        }

        if (control_points.size() == B) 
        {
            naive_bezier(control_points, window);
              bezier(control_points, window);

            cv::imshow("Bezier Curve", window);
            cv::imwrite("my_bezier_curve.png", window);
            key = cv::waitKey(0);

            return 0;
        }

        cv::imshow("Bezier Curve", window);
        key = cv::waitKey(20);
    }

return 0;
}
