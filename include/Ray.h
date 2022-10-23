#ifndef RAY_H
#define RAY_H

#include "glm/glm.hpp"

class Ray
{
    public:
        Ray();
        Ray(glm::vec4 org, glm::vec4 di, float len);
        ~Ray();
        glm::vec4 origin;
        glm::vec4 dir;
        float length;

};

#endif // RAY_H
