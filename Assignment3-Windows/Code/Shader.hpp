//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_SHADER_H
#define RASTERIZER_SHADER_H
#include <eigen3/Eigen/Eigen>
#include "Texture.hpp"


struct fragment_shader_payload
{
    fragment_shader_payload()
    {
        texture = nullptr;
    }

    fragment_shader_payload(const Eigen::Vector3f& col, const Eigen::Vector3f& nor,const Eigen::Vector2f& tc, Texture* tex) :
         color(col), normal(nor), tex_coords(tc), texture(tex) {}

    void set_tex_coords(Eigen::Vector2f tex_coords){
        float u =tex_coords[0];
        float v =tex_coords[1];
        if(u < 0 || u > 1.0f){
            u = std::min(1.0f, std::max(0.0f, u));
        }
        if(v < 0 || v > 1.0f){
            v = std::min(1.0f, std::max(0.0f, v));
        }
        this->tex_coords[0] = u;
        this->tex_coords[1] = v;
        return;
    }
    Eigen::Vector3f view_pos;
    Eigen::Vector3f color;
    Eigen::Vector3f normal;
    Eigen::Vector2f tex_coords;
    Texture* texture;
};

struct vertex_shader_payload
{
    Eigen::Vector3f position;
};

#endif //RASTERIZER_SHADER_H
