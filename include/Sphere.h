#ifndef SPHERE_H
#define SPHERE_H

#include <Primitive.h>
#include "Ray.h"
#include "glm/glm.hpp"

class Sphere : public Primitive
{
    public:
        Sphere();
        Sphere(glm::vec4 ps,float rad, glm::vec3 col);
        ~Sphere();
        bool intersectRay(Ray& ray, HitInfo& hit_info) override;
        glm::vec4 pos;
        glm::vec3 color;
        float radius;

    private:

};

#endif // SPHERE_H
