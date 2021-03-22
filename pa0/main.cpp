#include<cmath>
#include<eigen3/Eigen/Core>
#include<eigen3/Eigen/Dense>
#include<iostream>
#include <cmath>

constexpr float PI = 3.1415926;
using std::endl; using std::cout;
int main(){

    // Basic Example of cpp
    std::cout << "Example of cpp \n";
    float a = 1.0, b = 2.0;
    std::cout << a << std::endl;
    std::cout << a/b << std::endl;
    std::cout << std::sqrt(b) << std::endl;
    std::cout << std::acos(-1) << std::endl;
    std::cout << std::sin(30.0/180.0*acos(-1)) << std::endl;

    // Example of vector
    std::cout << "Example of vector \n";
    // vector definition
    Eigen::Vector3f v(1.0f,2.0f,3.0f);
    Eigen::Vector3f w(1.0f,0.0f,0.0f);
    // vector output
    std::cout << "Example of output \n";
    std::cout << v << std::endl;
    // vector add
    std::cout << "Example of add \n";
    std::cout << v + w << std::endl;
    // vector scalar multiply
    std::cout << "Example of scalar multiply \n";
    std::cout << v * 3.0f << std::endl;
    std::cout << 2.0f * v << std::endl;

    // Example of matrix
    std::cout << "Example of matrix \n";
    // matrix definition
    Eigen::Matrix3f i,j;
    i << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0;
    j << 2.0, 3.0, 1.0, 4.0, 6.0, 5.0, 9.0, 7.0, 8.0;
    // matrix output
    std::cout << "Example of output \n";
    std::cout << i << std::endl;
    // matrix add i + j
    // matrix scalar multiply i * 2.0
    // matrix multiply i * j
    // matrix multiply vector i * v
    // 作业
    using Point = Eigen::Vector3f;
    Point p(2,1,1);
    Point p2(1,0,1);
    Point p3(1,1,1);
    Eigen::Matrix3f m;
    float theta = 45 / 180.0 * PI;
    //只旋转
    m << cos(theta) ,  -sin(theta), 0,
     sin(theta), cos(theta), 0 ,
     0, 0, 1;

     //只平移
    Eigen::Matrix3f m2;
    m2 << 1 ,  0, 1,
     0, 1, 2 ,
     0, 0, 1;

     //平移+旋转
    Eigen::Matrix3f m3;
    m3 << cos(theta) ,  -sin(theta), 1,
     sin(theta), cos(theta), 2 ,
     0, 0, 1;


     std::cout << " m " <<  m <<std::endl;
     std::cout << "m * p " << endl << m3 * p <<std::endl;
     std::cout <<" M * p2" << endl << m3 * p2 <<std::endl;
     std::cout <<" M * p3" << endl << m3 * p3 <<std::endl;
     std::cout <<" M2 * p3" << endl << m2 * p3 <<std::endl;
    

    return 0;
}