#include "Ray.h"

Ray::Ray()
{
    //ctor
}

Ray::Ray(glm::vec4 org, glm::vec4 di, float len) : origin(org), dir(di), length(len)
{

}

Ray::~Ray()
{
    //dtor
}
