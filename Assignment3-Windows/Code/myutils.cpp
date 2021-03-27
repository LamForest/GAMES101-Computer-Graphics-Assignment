#include "myutils.h"
    void range_check(float uv){
        if (uv >= 1 || uv < 0){
            printf("u or v is out_of_range: %.5f\n", uv);
            throw std::out_of_range("out of range");
        }
    };