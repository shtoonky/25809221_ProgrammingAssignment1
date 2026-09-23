#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "ray.hpp"

class HitRecord {
    public:
        Point3 p;
        Vec3 normal;
        double t;
        bool front_face;

        void SetFaceNormal(const Ray& r, const Vec3& outward_normal) {
            // outward_normal is assumed to have unity length.

            front_face = Dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
};

class Hittable {
    public:
        virtual ~Hittable() = default;

        virtual bool Hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord& rec) const = 0;
};

#endif