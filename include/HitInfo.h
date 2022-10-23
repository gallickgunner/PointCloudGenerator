#ifndef HITINFO_H
#define HITINFO_H
#include "glm/vec4.hpp"

class HitInfo
{
    public:
        HitInfo() {}
        HitInfo(glm::vec4 hp, glm::vec3 col) : hitpoint(hp), color(col) {}
        glm::vec4 hitpoint;
        glm::vec3 color;

};

#endif // HITINFO_H
