#ifndef _GRID_H_
#define _GRID_H_

#include "object3d.h"

class Grid :public Object3D
{
public:
	Grid(BoundingBox* bb, int nx, int ny, int nz)
	{
		BoundingBox * sceneBBox = getBoundingBox();
		sceneBBox = bb;
		sceneBBox->Get(sceneMinVertex, sceneMaxVertex);

		n = Vec3f(nx, ny, nz);
		Vec3f subVertex = sceneMaxVertex - sceneMinVertex;
		subVertex.Divide(nx, ny, nz);
		gridStep = subVertex;

		opaque = new bool** [nx];
		for (int i = 0; i < nx; i++)
		{
			opaque[i] = new bool* [ny];
			for (int j = 0; j < ny; j++)
			{
				opaque[i][j]=new bool[nz];
				for (int k = 0; k < nz; k++)
				{
					opaque[i][j][k] = false;
				}
			}
		}
		
		
		
	}

	void SetVoxels(Vec3f minVertex, Vec3f maxVertex)
	{
		int minI = floor((minVertex.x() - sceneMinVertex.x()) / gridStep.x());
		cout << "minI: " << minI<<endl;

		int maxI = floor((maxVertex.x() - sceneMinVertex.x()) / gridStep.x());
		cout << "maxI: " << maxI << endl;


		int minJ = floor((minVertex.y() - sceneMinVertex.y()) / gridStep.y());
		cout << "minJ: " << minJ << endl;

		int maxJ = floor((maxVertex.y() - sceneMinVertex.y()) / gridStep.y());
		cout << "maxJ: " << maxJ << endl;


		int minK = floor((minVertex.z() - sceneMinVertex.z()) / gridStep.z());
		cout << "minK: " << minK << endl;

		int maxK = floor((maxVertex.z() - sceneMinVertex.z()) / gridStep.z());
		cout << "maxK: " << maxK << endl;

	
		for (int i = minI; i <= maxI; i++)
		{
			for (int j = minJ; j <= maxJ; j++)
			{
				for (int k = minK; k <= maxK; k++)
				{
					opaque[i][j][k] = true;
				}
			}
		}

	}

	virtual void paint(void)
	{
	}

	virtual bool intersectShadowRay(const Ray& r, float tmin, float distanceToLight)
	{
		return true;
	}

	virtual bool intersect(const Ray& r, Hit& h, float tmin)
	{
		return true;
	}


private:

	//int nx;
	//int ny;
	//int nz;
	Vec3f sceneMinVertex;
	Vec3f sceneMaxVertex;
	Vec3f gridStep;
	Vec3f n;

	bool*** opaque;

};

#endif