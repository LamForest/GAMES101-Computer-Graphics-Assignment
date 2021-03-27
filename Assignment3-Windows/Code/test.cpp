#include <eigen3/Eigen/Eigen>
#include <stdio.h>
#include <iostream>

using namespace std;

int main(){
    Eigen::Vector3f a(3,4,1);
    Eigen::Vector3f b(2,1,3);
    Eigen::Vector3f c = a*b;
    cout << c <<endl;
    return 0;
}

