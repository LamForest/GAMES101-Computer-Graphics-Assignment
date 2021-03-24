#include <eigen3/Eigen/Eigen>
#include <algorithm>
#include <iostream>

using namespace Eigen;

using array_2dtriangle = std::array<Vector2f, 3>;
using array_3dtriangle = std::array<Vector3f, 3>;
using std::cout;
using std::endl;
// 0, + -> true; - -> false
// static bool get_sign(float a){
//     return a >= 0;
// }

static bool same_sign(float a, float b, float c){
    // return (a >= 0 && b >= 0 && c>= 0) || (a <= 0 && b <= 0 && c <= 0); //三角形边上的点也算在三角形内
    // return (a > 0 && b > 0 && c> 0) || (a < 0 && b < 0 && c < 0); //三角形边上的点不算在三角形内
    return (a < 0 && b < 0 && c < 0); //三角形边上的点不算在三角形内

}


static bool all_negative(float a, float b, float c){
    // return (a >= 0 && b >= 0 && c>= 0) || (a <= 0 && b <= 0 && c <= 0); //三角形边上的点也算在三角形内
    return (a < 0 && b < 0 && c < 0); //三角形边上的点不算在三角形内

}


static bool insideTriangle_complex(float x, float y, const array_2dtriangle v_xy)
{   
    // TODO : Implement this function to check if the point (x, y) is inside the triangle represented by _v[0], _v[1], _v[2]
    // const auto v_xy = tri.v;
    const int num = v_xy.size(); //应该为3
    array_3dtriangle v_xyz;
    std::transform(v_xy.begin(), v_xy.end(), v_xyz.begin(), [](const Vector2f & xy)->Vector3f{
        return Vector3f(xy.x(), xy.y(), 0.0f);
    });


    Eigen::Vector3f side_1 = v_xyz[1] - v_xyz[0];
    Eigen::Vector3f side_2 = v_xyz[2] - v_xyz[1];
    Eigen::Vector3f side_3 = v_xyz[0] - v_xyz[2];

    std::vector<Vector3f> sides{side_1,side_2,side_3};

    Eigen::Vector3f p(x,y,0);

    std::vector<float> cross_product_zs{0,0,0};
    for (int i = 0; i < num; ++i){
        Vector3f cross_result = sides[i].cross((p - v_xyz[i]));
        cross_product_zs[i] = cross_result.z();
        // printf("cross with sides[%d] = ", i);
        // cout << cross_result << endl;
    }

    return same_sign(cross_product_zs[0], cross_product_zs[1], cross_product_zs[2]);
    
}

static bool insideTriangle(float x, float y, const array_2dtriangle v_xy)
{   
    // TODO : Implement this function to check if the point (x, y) is inside the triangle represented by _v[0], _v[1], _v[2]
    // const auto v_xy = tri.v;
    Eigen::Vector2f side_1 = v_xy[1] - v_xy[0];
    Eigen::Vector2f side_2 = v_xy[2] - v_xy[1];
    Eigen::Vector2f side_3 = v_xy[0] - v_xy[2];

    std::vector<Vector2f> sides{side_1,side_2,side_3};

    Eigen::Vector2f p(x,y);

    std::vector<float> cross_product_zs{0,0,0}; //3次叉乘的z的结果

    const int num = v_xy.size();
    for (int i = 0; i < num; ++i){
        Vector2f v = p - v_xy[i]; //AP, BP, CP，顶点到p的向量

        //这里我们不用把p sides都变成3维的向量
        //因为叉乘的结果x,y都为0，我们只关注z即可
        float cross_result_z = sides[i].x() * v.y() - sides[i].y() * v.x();
        cross_product_zs[i] = cross_result_z;
        // printf("cross with sides[%d] = ", i);
        // cout << cross_result_z << endl;
    }

    return all_negative(cross_product_zs[0], cross_product_zs[1], cross_product_zs[2]);
    
}

int main(){
    // Vector2f a(1.0f, 0.0f);
    // Vector2f b(0.0f, 0.0f);
    // Vector2f c(0.0f, 1.0f);

    // array_2dtriangle tri1{a,b,c};

    // Vector2f p1(1.0f, 1.0f); //out
    // Vector2f p2(-1.0f, -1.0f); //out
    // Vector2f p3(0.2f, 0.2f); //int
    // Vector2f p4(1.0f, -1.0f); //out
    // Vector2f p5(0.5f, 0.5f); // depends
    // Vector2f p6(0.0f, 0.5f); // depends
    // Vector2f p7(0.5f, 0.1f); // in

    // std::vector<Vector2f> tests{p1,p2,p3,p4,p5,p6,p7};
    // for(auto &t : tests){
    //     cout << insideTriangle(t.x(), t.y(), tri1) <<endl;

    // }


    Vector2f a(591.42f, 350.0f);
    Vector2f b(350.0f, 591.42f);
    Vector2f c(108.58f, 350.0f);

    array_2dtriangle tri1{a,b,c};

    Vector2f p1(.0f, 1.0f); //out
    Vector2f p2(-1.0f, -1.0f); //out
    Vector2f p3(0.2f, 0.2f); //int
    Vector2f p4(1.0f, -1.0f); //out
    Vector2f p5(0.5f, 0.5f); // depends
    Vector2f p6(0.0f, 0.5f); // depends
    Vector2f p7(0.5f, 0.1f); // in

    std::vector<Vector2f> tests{p1,p2,p3,p4,p5,p6,p7};
    for(auto &t : tests){
        cout << insideTriangle(t.x(), t.y(), tri1) <<endl;

    }



}