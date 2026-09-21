#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "ray.hpp"

class HitRecord {
    public:
        point3 p;
        Vec3 normal;
        double t;
        bool front_face;


};

class Hittable {
    public:
        virtual ~Hittable() = default;

        virtual bool Hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord& rec) const = 0;
};

#endif