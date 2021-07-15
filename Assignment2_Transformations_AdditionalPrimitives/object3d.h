#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "material.h"
#include "ray.h"
#include "hit.h"

class Object3D
{

public:
    virtual bool intersect(const Ray& r, Hit& h, float tmin) = 0;

    Object3D(Material* mat) : material(mat) {};
    Object3D() {};
    ~Object3D() {};

    Material* getMaterial()
    {
        return material;
    }

private:
    Material* material;
};

#endif