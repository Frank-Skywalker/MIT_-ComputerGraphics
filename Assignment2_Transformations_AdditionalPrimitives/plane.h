#ifndef _PLANE_H_
#define _PLANE_H_
#include "object3d.h"

class Plane :public Object3D
{
public: 
	Plane(Vec3f& normal, float d, Material* m):Object3D(m),normal(normal),d(d)
	{
		this->normal.Normalize();
	}

	Plane(Vec3f a, Vec3f b, Vec3f c,Material* m):Object3D(m)
	{
		Vec3f::Cross3(normal, b - a, c - b);
		normal.Normalize();
		d = fabsf(a.Dot3(normal));
	}
	~Plane(){}
	bool intersect(const Ray& r, Hit& h, float tmin)
	{
		if (normal.Dot3(r.getDirection()) == 0)
		{
			return false;
		}

		float t = (d - normal.Dot3(r.getOrigin())) / normal.Dot3(r.getDirection());
		if (t>tmin)
		{
			if (t < h.getT())
			{
				h.set(t, getMaterial(), normal, r);
				return true;
			}
			//return true;
		}
		return false;
	}

private:
	Vec3f normal;
	float d;

};


#endif