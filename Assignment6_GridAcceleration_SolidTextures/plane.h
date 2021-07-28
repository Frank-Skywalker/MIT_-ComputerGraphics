#ifndef _PLANE_H_
#define _PLANE_H_
#include "object3d.h"
#define LENGTH 100000

class Plane :public Object3D
{
public: 
	Plane(Vec3f& normal, float d, Material* m):Object3D(m),normal(normal),d(d)
	{
		this->normal.Normalize();
		//Assignment5
		boundingBox = NULL;
	}

	Plane(Vec3f a, Vec3f b, Vec3f c,Material* m):Object3D(m)
	{
		Vec3f::Cross3(normal, b - a, c - b);
		normal.Normalize();
		d = fabsf(a.Dot3(normal));
		//Assignment5
		boundingBox = NULL;
	}
	~Plane(){}
	bool intersect(const Ray& r, Hit& h, float tmin)
	{
		RayTracingStats::IncrementNumIntersections();
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

	virtual bool intersectShadowRay(const Ray& r, float tmin, float distanceToLight) 
	{
		RayTracingStats::IncrementNumIntersections();
		if (normal.Dot3(r.getDirection()) == 0)
		{
			return false;
		}

		float t = (d - normal.Dot3(r.getOrigin())) / normal.Dot3(r.getDirection());
		if (t > tmin && t <= distanceToLight)
		{
			return true;
		}
		return false;
	}

	void paint(void)
	{
		//origin projection on the plane
		//true whether d is positive or negative
		Vec3f o=normal*d;
		
		Vec3f v;
		//parellel
		if (normal.Dot3(Vec3f(1, 0, 0))<1)
		{
			v.Set(1, 0, 0);
		}
		else
		{
			v.Set(0, 1, 0);
		}

		Vec3f basis1;
		Vec3f::Cross3(basis1, v, normal);
		basis1.Normalize();
		Vec3f basis2;
		Vec3f::Cross3(basis2, normal, basis1);
		basis2.Normalize();

		Vec3f points[4];
		points[0] = o + basis1 * LENGTH;
		points[1] = o + basis2 * LENGTH;
		points[2] = o - basis1 * LENGTH;
		points[3] = o - basis2 * LENGTH;

		getMaterial()->glSetMaterial();
		glBegin(GL_QUADS);
		glNormal3f(normal.x(), normal.y(), normal.z());
		glVertex3f(points[0].x(), points[0].y(), points[0].z());
		glVertex3f(points[1].x(), points[1].y(), points[1].z());
		glVertex3f(points[2].x(), points[2].y(), points[2].z());
		glVertex3f(points[3].x(), points[3].y(), points[3].z());
		glEnd();
	}

private:
	Vec3f normal;
	float d;

};


#endif