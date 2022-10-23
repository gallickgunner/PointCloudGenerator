#include "Sphere.h"

Sphere::Sphere()
{
    //ctor
}

Sphere::Sphere(glm::vec4 ps, float rad, glm::vec3 col) : pos(ps), radius(rad), color(col)
{
    //ctor
}

Sphere::~Sphere()
{
    //dtor
}

bool Sphere::intersectRay(Ray& ray, HitInfo& hit_info)
{
    glm::vec4 normal = ray.origin - pos;
    float a = dot(ray.dir, ray.dir);
    float b = dot(normal, ray.dir);
    float c = dot(normal, normal) - radius*radius;
    float disc = b * b - c;

    float t = -1.0f;
    if(disc >= 0.0f)
    t = -b - glm::sqrt(disc);

    if (t >= 0.0 && t < ray.length)
    {
        ray.length = t;
        hit_info.hitpoint = ray.origin + ray.dir * t;
        hit_info.color = color;
        return true;
    }
    return false;
}
