#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Ray.h"
#include "HitInfo.h"

class Primitive
{
    public:
        Primitive();
        ~Primitive();
        virtual bool intersectRay(Ray& ray, HitInfo& hit_info) = 0;

    protected:

    private:
};

#endif // PRIMITIVE_H
