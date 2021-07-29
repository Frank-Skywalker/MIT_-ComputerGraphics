#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object3d.h"
#include "plane.h"
#include "matrix.h"
#include <math.h>


class Triangle :public Object3D
{
public:
	Triangle(Vec3f& a, Vec3f& b, Vec3f& c, Material* m) :Object3D(m), a(a), b(b), c(c)
	{
		Vec3f::Cross3(normal, b - a, c - b);
		normal.Normalize();


		//generate bounding box
		Vec3f min;
		Vec3f::Min(min, a, b);
		Vec3f::Min(min, min, c);

		Vec3f max;
		Vec3f::Max(max, a, b);
		Vec3f::Max(max, max, c);


		boundingBox = new BoundingBox(min,max);
	}
	~Triangle() {};
	bool intersect(const Ray& r, Hit& h, float tmin)
	{
		RayTracingStats::IncrementNumIntersections();
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


	virtual bool intersectShadowRay(const Ray& r, float tmin, float distanceToLight)
	{
		RayTracingStats::IncrementNumIntersections();
		Vec3f Ro = r.getOrigin();
		Vec3f Rd = r.getDirection();
		float detA = Matrix::det3x3(a.x() - b.x(), a.x() - c.x(), Rd.x(),
			a.y() - b.y(), a.y() - c.y(), Rd.y(),
			a.z() - b.z(), a.z() - c.z(), Rd.z());
		float detBeta = Matrix::det3x3(a.x() - Ro.x(), a.x() - c.x(), Rd.x(),
			a.y() - Ro.y(), a.y() - c.y(), Rd.y(),
			a.z() - Ro.z(), a.z() - c.z(), Rd.z());
		float detGamma = Matrix::det3x3(a.x() - b.x(), a.x() - Ro.x(), Rd.x(),
			a.y() - b.y(), a.y() - Ro.y(), Rd.y(),
			a.z() - b.z(), a.z() - Ro.z(), Rd.z());

		float detT = Matrix::det3x3(a.x() - b.x(), a.x() - c.x(), a.x() - Ro.x(),
			a.y() - b.y(), a.y() - c.y(), a.y() - Ro.y(),
			a.z() - b.z(), a.z() - c.z(), a.z() - Ro.z());

		float beta = detBeta / detA;
		float gamma = detGamma / detA;
		float t = detT / detA;
		if (t > tmin && t <= distanceToLight && beta > 0 && gamma > 0 && beta + gamma < 1)
		{
			return true;
		}
		return false;
	}

	virtual void paint(void)
	{
		getMaterial()->glSetMaterial();
		glBegin(GL_TRIANGLES);
		glNormal3f(normal.x(), normal.y(), normal.z());
		glVertex3f(a.x(), a.y(), a.z());
		glVertex3f(b.x(), b.y(), b.z());
		glVertex3f(c.x(), c.y(), c.z());
		glEnd();
	}

	virtual void insertIntoGrid(Grid* grid, Matrix* m)
	{
		Vec3f minVertex;
		Vec3f maxVertex;
		boundingBox->Get(minVertex, maxVertex);
		if (m != NULL)
		{
			//get eight vertices of sub object bounding box
			Vec3f transVertices[3];
			transVertices[0] = a;
			transVertices[1] = b;
			transVertices[2] = c;

			minVertex.Set(INFINITY, INFINITY, INFINITY);
			maxVertex.Set(-INFINITY, -INFINITY, -INFINITY);
			for (int i = 0; i < 3; i++)
			{
				m->Transform(transVertices[i]);
				Vec3f::Min(minVertex, minVertex, transVertices[i]);
				Vec3f::Max(maxVertex, maxVertex, transVertices[i]);
			}
		}
		int mini, minj, mink;
		int maxi, maxj, maxk;
		assert(grid->getVoxelIndex(minVertex, mini, minj, mink));
		//cout << "minVertex: " << minVertex << endl;
		//cout << "indices: " << mini << " " << minj << " " << mink << endl;
		//cout << "triangle: " << a << endl;
		assert(grid->getVoxelIndex(maxVertex, maxi, maxj, maxk));
		//cout << "maxVertices: " << maxVertex << endl;
		//cout << "indices: " << maxi << " " << maxj << " " << maxk << endl;
		for (int i = mini; i <= maxi; i++)
		{
			for (int j = minj; j <= maxj; j++)
			{
				for (int k = mink; k <= maxk; k++)
				{
					grid->addObjectToVoxel(i, j, k, this);
				}
			}
		}
	}

private:
	Vec3f a;
	Vec3f b;
	Vec3f c;
	Vec3f normal;

};

#endif