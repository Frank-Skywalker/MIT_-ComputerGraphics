#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Object3D.h"
#include "ray.h"
#include "vectors.h"
#include <math.h>

using namespace std;

class Sphere : public Object3D
{

public:
    Sphere(const Vec3f center, const float radius, Material* mat) : Object3D(mat), center(center), radius(radius) {}
    virtual bool intersect(const Ray& r, Hit& h, float tmin)
    {
        Ray raySphereSpace(r.getOrigin() - center, r.getDirection());
        float disRayOrigin = raySphereSpace.getOrigin().Length();

        float a = raySphereSpace.getDirection().Dot3(raySphereSpace.getDirection());
        float b = 2 * raySphereSpace.getDirection().Dot3(raySphereSpace.getOrigin());
        float c = raySphereSpace.getOrigin().Dot3(raySphereSpace.getOrigin()) - radius * radius;
        float delta = b * b - 4 * a * c;
        if (delta < 0)
        {
            return false;
        }
        delta = sqrtf(delta);
        float t1 = (-b - delta) / (2 * a);
        float t2 = (-b + delta) / (2 * a);
        //cout << "t1:" << t1 << endl;
        //cout << "t2:" << t2 << endl;
        if (t1 > tmin && t1 < h.getT())
        {
            h.set(t1, getMaterial(), r);
            return true;
        }
        else if (t2 > tmin && t2 < h.getT())
        {
            h.set(t2, getMaterial(), r);
            return true;
        }
        return false;
    }

private:
    Vec3f center;
    float radius;
};

#endif