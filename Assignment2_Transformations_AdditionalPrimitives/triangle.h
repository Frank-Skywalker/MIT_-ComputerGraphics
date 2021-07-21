#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object3d.h"
#include "plane.h"
#include "matrix.h"

class Triangle :public Object3D
{
public:
	Triangle(Vec3f& a, Vec3f& b, Vec3f& c, Material* m) :Object3D(m), a(a), b(b), c(c)
	{
		Vec3f::Cross3(normal, b - a, c - b);
		normal.Normalize();
	}
	~Triangle() {};
	bool intersect(const Ray& r, Hit& h, float tmin)
	{
		Vec3f Ro = r.getOrigin();
		Vec3f Rd = r.getDirection();
		float detA = Matrix::det3x3(a.x() - b.x(), a.x() - c.x(), Rd.x(),
						a.y()-b.y(),a.y()-c.y(),Rd.y(),
			a.z() - b.z(), a.z() - c.z(), Rd.z());
		float detBeta= Matrix::det3x3(a.x() - Ro.x(), a.x() - c.x(), Rd.x(),
			a.y() - Ro.y(), a.y() - c.y(), Rd.y(),
			a.z() - Ro.z(), a.z() - c.z(), Rd.z());
		float detGamma = Matrix::det3x3(a.x() - b.x(), a.x() - Ro.x(), Rd.x(),
			a.y() - b.y(), a.y() - Ro.y(), Rd.y(),
			a.z() - b.z(), a.z() - Ro.z(), Rd.z());

		float detT= Matrix::det3x3(a.x() - b.x(), a.x() - c.x(), a.x()-Ro.x(),
			a.y() - b.y(), a.y() - c.y(), a.y()-Ro.y(),
			a.z() - b.z(), a.z() - c.z(), a.z()-Ro.z());

		float beta = detBeta / detA;
		float gamma = detGamma / detA;
		float t = detT / detA;
		if (t > tmin && beta>0 && gamma > 0 && beta + gamma < 1)
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
	Vec3f a;
	Vec3f b;
	Vec3f c;
	Vec3f normal;

};

#endif