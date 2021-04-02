//
// Created by LEI XU on 5/16/19.
//

#ifndef RAYTRACING_BOUNDS3_H
#define RAYTRACING_BOUNDS3_H
#include "Ray.hpp"
#include "Vector.hpp"
#include <limits>
#include <array>

class Bounds3
{
  public:
    Vector3f pMin, pMax; // two points to specify the bounding box
    Bounds3()
    {
        double minNum = std::numeric_limits<double>::lowest();
        double maxNum = std::numeric_limits<double>::max();
        pMax = Vector3f(minNum, minNum, minNum);
        pMin = Vector3f(maxNum, maxNum, maxNum);
    }
    Bounds3(const Vector3f p) : pMin(p), pMax(p) {}
    Bounds3(const Vector3f p1, const Vector3f p2)
    {
        pMin = Vector3f(fmin(p1.x, p2.x), fmin(p1.y, p2.y), fmin(p1.z, p2.z));
        pMax = Vector3f(fmax(p1.x, p2.x), fmax(p1.y, p2.y), fmax(p1.z, p2.z));
    }
    //对角线的差
    Vector3f Diagonal() const { return pMax - pMin; }

    //longest axis 0 = x, 1 = y, 2 = z
    int maxExtent() const
    {
        Vector3f d = Diagonal();
        if (d.x > d.y && d.x > d.z)
            return 0;
        else if (d.y > d.z)
            return 1;
        else
            return 2;
    }

    //包围盒表面积
    double SurfaceArea() const
    {
        Vector3f d = Diagonal();
        return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
    }

    Vector3f Centroid() { return 0.5 * pMin + 0.5 * pMax; }

    //intersection of Bound3 : this and b
    Bounds3 Intersect(const Bounds3& b)
    {
        return Bounds3(Vector3f(fmax(pMin.x, b.pMin.x), fmax(pMin.y, b.pMin.y),
                                fmax(pMin.z, b.pMin.z)),
                       Vector3f(fmin(pMax.x, b.pMax.x), fmin(pMax.y, b.pMax.y),
                                fmin(pMax.z, b.pMax.z)));
    }

    Vector3f Offset(const Vector3f& p) const
    {
        Vector3f o = p - pMin;
        if (pMax.x > pMin.x)
            o.x /= pMax.x - pMin.x;
        if (pMax.y > pMin.y)
            o.y /= pMax.y - pMin.y;
        if (pMax.z > pMin.z)
            o.z /= pMax.z - pMin.z;
        return o;
    }

    bool Overlaps(const Bounds3& b1, const Bounds3& b2)
    {
        bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
        bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
        bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
        return (x && y && z);
    }

    bool Inside(const Vector3f& p, const Bounds3& b)
    {
        return (p.x >= b.pMin.x && p.x <= b.pMax.x && p.y >= b.pMin.y &&
                p.y <= b.pMax.y && p.z >= b.pMin.z && p.z <= b.pMax.z);
    }
    inline const Vector3f& operator[](int i) const
    {
        return (i == 0) ? pMin : pMax;
    }

    inline bool IntersectP(const Ray& ray, const Vector3f& invDir,
                           const std::array<int, 3>& dirisNeg) const;
};



inline bool Bounds3::IntersectP(const Ray& ray, const Vector3f& invDir,
                                const std::array<int, 3>& dirIsNeg) const
{
    // invDir: ray direction(x,y,z), invDir=(1.0/x,1.0/y,1.0/z), use this because Multiply is faster that Division
    // dirIsNeg: ray direction(x,y,z), dirIsNeg=[int(x>0),int(y>0),int(z>0)], use this to simplify your logic
    // TODO test if ray bound intersects

    // float t_enter_x t_enter_y= t_enter_z = ;
    // float t_enter_x, t_exit_x, t_enter_y, t_exit_y, t_enter_z, t_exit_z;

    // if(isZero(ray.direction.x)){
    //     if( ray.origin.x <= px_max && ray.origin.x >= px_min){
    //         t_enter_x = - std::numeric_limits<float>::infinity();
    //         t_exit_x = std::numeric_limits<float>::infinity();
    //     }else{
    //         t_enter_x = std::numeric_limits<float>::infinity();
    //         t_exit_x = -std::numeric_limits<float>::infinity();
    //     }
    // }else{

    
    // if(isZero(ray.direction.y)){
    //     if( ray.origin.y <= py_max && ray.origin.y >= py_min){
    //         t_enter_y = - std::numeric_limits<float>::infinity();
    //         t_exit_y = std::numeric_limits<float>::infinity();
    //     }else{
    //         t_enter_y = std::numeric_limits<float>::infinity();
    //         t_exit_y = -std::numeric_limits<float>::infinity();
    //     }
    // }else{

    // }

    // if(isZero(ray.direction.z)){
    //     if( ray.origin.z <= pz_max && ray.origin.z >= pz_min){
    //         t_enter_z = - std::numeric_limits<float>::infinity();
    //         t_exit_z = std::numeric_limits<float>::infinity();
    //     }else{
    //         t_enter_z = std::numeric_limits<float>::infinity();
    //         t_exit_z = -std::numeric_limits<float>::infinity();
    //     }
    // }else{

    // }

    // printf("t_enter = %.3f, t_exit = %.3f\n", t_enter, t_exit);


    
    // t_min & t_max along x/y/z-axis
    //TODO:是否需要考虑invDir = inf的情况？
    //暂时不考虑太过于细枝末节的东西
    Vector3f t_pmax = (pMax - ray.origin) * invDir;
    Vector3f t_pmin = (pMin - ray.origin) * invDir;
    
    //需要考虑光线正穿还是反穿
    //反穿指光线由x,y,z较大，射向xyz较小的地方，
    // 这样会先经过pMax，然后才是pMin
    Vector3f t_enter(
        std::min(t_pmin.x, t_pmax.x),
        std::min(t_pmin.y, t_pmax.y),
        std::min(t_pmin.z, t_pmax.z)
        );
    Vector3f t_exit(
        std::max(t_pmin.x, t_pmax.x),
        std::max(t_pmin.y, t_pmax.y),
        std::max(t_pmin.z, t_pmax.z)
        );

    float t_enter_max = std::max({t_enter.x, t_enter.y, t_enter.z});
    float t_exit_min  = std::min({t_exit.x, t_exit.y, t_exit.z});
    return (t_exit_min>t_enter_max) && (t_exit_min>0);
}

inline Bounds3 Union(const Bounds3& b1, const Bounds3& b2)
{
    Bounds3 ret;
    ret.pMin = Vector3f::Min(b1.pMin, b2.pMin);
    ret.pMax = Vector3f::Max(b1.pMax, b2.pMax);
    return ret;
}

inline Bounds3 Union(const Bounds3& b, const Vector3f& p)
{
    Bounds3 ret;
    ret.pMin = Vector3f::Min(b.pMin, p);
    ret.pMax = Vector3f::Max(b.pMax, p);
    return ret;
}

#endif // RAYTRACING_BOUNDS3_H
