#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "material.h"
#include "ray.h"
#include "hit.h"

class Object3D
{

public:
    virtual bool intersect(const Ray& r, Hit& h, float tmin) = 0;
    virtual bool intersectShadowRay(const Ray& r, float tmin,float distanceToLight) = 0;

    Object3D(Material* mat) : material(mat) {};
    Object3D() {};
    ~Object3D() {};

    Material* getMaterial()
    {
        return material;
    }

// ====================================================================
// Add the following pure virtual function to Object3D and define it
// in each of its subclasses. The paint routine is responsible for
// making the OpenGL calls to draw the object to the screen.
// ==================================================================== #include
    virtual void paint(void) = 0;

private:
    Material* material;
};

#endif