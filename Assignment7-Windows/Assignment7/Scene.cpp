//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

//随机在某个发光物体表面A上取1个点pos, pdf = 1.0/area_A
void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

//outofdated， use scene::intersect instead
//有了BVH后不该用这个
//这个是最笨的穷举方法。
bool Scene::trace(
        const Ray &ray,
        const std::vector<Object*> &objects,
        float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm here
    Intersection p = this->intersect(ray);
    if(!p.happened) {
        //真的会不发生碰撞吗？人眼这面有墙吗？
        return Vector3f(0.0f);
    }
    //处理发光物体
    if(p.obj->hasEmit()){
        if(depth == 0){
            // printf("eye hit Emiting obj, depth = %d, emit radiance : (%.4f, %.4f, %.4f)\n", depth, p.obj.emit.x, p.emit.y, p.emit.z);
            return p.m->getEmission();
        }else{
            return Vector3f(0.0f);
        }
        
    }
    // auto p = inter.coords;
    
    //1. direct 
    Vector3f L_dir(0.0f);
    {
        Intersection light; float pdf_light; //for面光源，永远1/A
        this->sampleLight(light, pdf_light);
        Ray p_to_light(p.coords, normalize(light.coords - p.coords));
        Intersection middle = this->intersect(p_to_light);
        //是否有物体介于sampled light和p之间
        //可能判断distance即t是否相等更好，
        //但是Intersection light只有coords normal emit是有意义的，其他都没有被设置
        if(isSamePoint(middle.coords, light.coords)){ 
            auto L_i = light.emit;
            //w_i light -> p 
            Vector3f w_s = normalize(light.coords - p.coords), w_o(ray.direction);
            float cos_theta = dotProduct(p.normal, w_s), cos_theta_prime = dotProduct(light.normal,  -w_s);

            auto f_brdf = p.m->eval(w_o, w_s, p.normal);
            float dist_square = std::pow((light.coords - p.coords).norm(), 2);
            
            L_dir = (L_i * f_brdf * cos_theta * cos_theta_prime / dist_square) / pdf_light; 
        }
    }


    //2. indirect
    Vector3f L_indir(0.0f);
    {
        //2.0 RR
        // printf("random : %.4f\n", get_random_float());
        if(get_random_float() < this->P_RR) {
            //2.1 sample a point on semiphere
            Vector3f w_o(ray.direction); 
            //这里参数ray.directiond的正负无所谓，反正不会用到
            Vector3f w_i = p.m->sample(ray.direction, p.normal); //For diffuse, uniform sample on the hemisphere
            //这里3个参数都无所谓
            float pdf_semiphere = p.m->pdf(w_o, w_i, p.normal); //For diffuse, always 1/2pi
            
            auto L_i = castRay(Ray(p.coords, w_i), depth + 1); //符号?
            Vector3f f_brdf = p.m->eval(w_o, w_i, p.normal);
            float cos_theta = dotProduct(p.normal, w_i);
            L_indir = (L_i * f_brdf * cos_theta) / pdf_semiphere / P_RR;
            // printf(" < P_RR, L_indir = (%.4f, %.4f, %.4f), depth = %d\n", L_indir.x, L_indir.y, L_indir.z, depth);
        }

        
    }


    Vector3f L_o = L_dir + L_indir;
    return L_o;


    

}