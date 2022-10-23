#include "Quad.h"

Quad::Quad()
{
    //ctor
}

Quad::Quad(glm::vec4 ps, glm::vec4 norm, glm::vec4 el, glm::vec4 ew, glm::vec3 col) : pos(ps), normal(norm), edge_l(el), edge_w(ew), color(col)
{
    //ctor
}


Quad::~Quad()
{
    //dtor
}

bool Quad::intersectRay(Ray& ray, HitInfo& hit_info)
{
    float DdotN = glm::dot(ray.dir, normal);
    if(glm::abs(DdotN) > 0.0001)
    {
        float t = glm::dot(normal, pos - ray.origin) / DdotN;
        if(t > 0.0 && t < ray.length)
        {
            float proj1, proj2, la, lb;
            glm::vec4 temp;

            temp = ray.origin + (ray.dir * t);
            temp = temp - pos;
            proj1 = dot(temp, edge_l);
            proj2 = dot(temp, edge_w);
            la = glm::length(edge_l);
            lb = glm::length(edge_w);

            // Projection of the vector from rectangle corner to hitpoint on the edges.
            proj1 /= la;
            proj2 /= lb;

            if( (proj1 >= 0.0 && proj2 >= 0.0)  && (proj1 <= la && proj2 <= lb)  )
            {
                ray.length = t;
                hit_info.hitpoint = ray.origin + ray.dir * t;
                hit_info.color = color;
                return true;
            }
        }
    }
    return false;
}
