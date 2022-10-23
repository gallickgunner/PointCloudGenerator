#ifndef QUAD_H
#define QUAD_H

#include <Primitive.h>
#include "Ray.h"
#include "glm/glm.hpp"

class Quad : public Primitive
{
    public:
        Quad();
        Quad(glm::vec4 ps, glm::vec4 norm, glm::vec4 ew, glm::vec4 el, glm::vec3 col);
        ~Quad();
        bool intersectRay(Ray& ray, HitInfo& hit_info) override;

        glm::vec4 pos;
        glm::vec4 normal;
        glm::vec4 edge_l;
        glm::vec4 edge_w;
        glm::vec3 color;
};

#endif // QUAD_H
