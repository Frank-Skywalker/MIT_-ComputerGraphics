#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "material.h"
#include "ray.h"
#include "hit.h"
#include "boundingbox.h"
#include "raytracing_stats.h"

#define INTERSECT_EPSILON 0.000001


class Grid;
class BoundingBox;
class Transform;

class Object3D{

public:

    Object3D(Material* mat) : material(mat) 
    {
        boundingBox = NULL;
        transform= NULL;
    }
    Object3D() 
    {
        boundingBox = NULL;
        transform = NULL;
    };
    ~Object3D()
    {
        //Assignment4
        if (boundingBox != NULL)
        {
            delete boundingBox;
        }

        //Assignment6
        if (transform != NULL)
        {
            delete transform;
        }
    };

    Material* getMaterial()
    {
        return material;
    }

    virtual bool intersect(const Ray& r, Hit& h, float tmin) = 0;

    //Assignment3
    virtual void paint(void) = 0;

    //Assignment4
    virtual bool intersectShadowRay(const Ray& r, float tmin,float distanceToLight) = 0;

    //Assignment5
    BoundingBox* getBoundingBox() 
    {
        return boundingBox;
    }


    virtual void insertIntoGrid(Grid* grid, Matrix* m)
    {
    }

// ====================================================================
// Add the following pure virtual function to Object3D and define it
// in each of its subclasses. The paint routine is responsible for
// making the OpenGL calls to draw the object to the screen.
// ==================================================================== #include

protected:
    Material* material;
    BoundingBox* boundingBox;
    Transform* transform;
};

#endif