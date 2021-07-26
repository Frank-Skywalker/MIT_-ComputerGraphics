#ifndef _GRID_H_
#define _GRID_H_

#include "object3d.h"
#include "marchinginfo.h"
#include "plane.h"
#include "material.h"

#define GRID_EPSILON 0.00001

class Grid :public Object3D
{
public:
	Grid(BoundingBox* bb, int nx, int ny, int nz):nx(nx),ny(ny),nz(nz)
	{

		material = new PhongMaterial(Vec3f(0.5, 0, 0), Vec3f(), 0);

		cout << "bb min" << bb->getMin() << endl;
		cout << "bb max" << bb->getMax() << endl;
		cout << "nx ny nz: " << nx << " " << ny << " " << nz << endl;
		boundingBox = bb;
		boundingBox->Get(gridMinVertex, gridMaxVertex);

		Vec3f subVertex = gridMaxVertex - gridMinVertex;
		subVertex.Divide(nx, ny, nz);
		gridStep = subVertex;
		cout << "grid step: " << gridStep<<endl;

		voxelHalfDiagonalLength = gridStep.Length() / 2;


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

		//init
		cubeVertices[0]=Vec3f(0, 0, 0);
		cubeVertices[1] = Vec3f(1, 0, 0);
		cubeVertices[2] = Vec3f(0, 1, 0);
		cubeVertices[3] = Vec3f(1, 1, 0);
		cubeVertices[4] = Vec3f(0, 0, 1);
		cubeVertices[5] = Vec3f(1, 0, 1);
		cubeVertices[6] = Vec3f(0, 1, 1);
		cubeVertices[7] = Vec3f(1, 1, 1);

		cubeNormals[0] = Vec3f(0, 0, -1);
		cubeNormals[1] = Vec3f(0, 0, 1);
		cubeNormals[2] = Vec3f(0, -1, 0);
		cubeNormals[3] = Vec3f(0, 1, 0);
		cubeNormals[4] = Vec3f(-1, 0, 0);
		cubeNormals[5] = Vec3f(1, 0, 0);



		for (int i = 0; i < 8; i++)
		{
			cubeVertices[i] =cubeVertices[i]*gridStep;
			cubeVertices[i] = cubeVertices[i] + gridMinVertex;
		}

		//set six edge planes of the grid
		Material* m = new PhongMaterial(Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1);
		Vec3f normals[3];
		normals[0].Set(1, 0, 0);
		normals[1].Set(0, 1, 0);
		normals[2].Set(0, 0, 1);
		gridPlanes[0] = new Plane(normals[0], gridMinVertex.x(),m);
		gridPlanes[1] = new Plane(normals[0], gridMaxVertex.x(), m);
		gridPlanes[2] = new Plane(normals[1], gridMinVertex.y(), m);
		gridPlanes[3] = new Plane(normals[1], gridMaxVertex.y(), m);
		gridPlanes[4] = new Plane(normals[2], gridMinVertex.z(), m);
		gridPlanes[5] = new Plane(normals[2], gridMaxVertex.z(), m);


	}


	void setVoxel(int i, int j, int k)
	{
		opaque[i][j][k] = true;
	}


	//void setVoxels(Vec3f minVertex, Vec3f maxVertex)
	//{
	//	cout << endl;
	//	cout << "minVertex: " << minVertex<<endl;
	//	cout << "maxVertex: " << maxVertex<<endl;
	//	int minI = floor((minVertex.x() - gridMinVertex.x()) / gridStep.x());
	//	cout << "minI: " << minI<<endl;

	//	int maxI = floor((maxVertex.x() - gridMinVertex.x()) / gridStep.x());
	//	if (maxI >=nx)
	//	{
	//		maxI = nx-1;
	//	}
	//	cout << "maxI: " << maxI << endl;


	//	int minJ = floor((minVertex.y() - gridMinVertex.y()) / gridStep.y());
	//	cout << "minJ: " << minJ << endl;

	//	int maxJ = floor((maxVertex.y() - gridMinVertex.y()) / gridStep.y());
	//	if (maxJ >= ny)
	//	{
	//		maxJ = ny - 1;
	//	}
	//	cout << "maxJ: " << maxJ << endl;


	//	int minK = floor((minVertex.z() - gridMinVertex.z()) / gridStep.z());
	//	cout << "minK: " << minK << endl;

	//	int maxK = floor((maxVertex.z() - gridMinVertex.z()) / gridStep.z());
	//	if (maxK >= nz)
	//	{
	//		maxK = nz - 1;
	//	}
	//	cout << "maxK: " << maxK << endl;

	//
	//	for (int i = minI; i <= maxI; i++)
	//	{
	//		for (int j = minJ; j <= maxJ; j++)
	//		{
	//			for (int k = minK; k <= maxK; k++)
	//			{
	//				opaque[i][j][k] = true;
	//			}
	//		}
	//	}

	//}

	virtual void paint(void)
	{
		getMaterial()->glSetMaterial();
		glBegin(GL_QUADS);
		for (int i = 0; i < nx; i++)
		{
			for (int j = 0; j < ny; j++)
			{
				for (int k = 0; k < nz; k++)
				{
					if (opaque[i][j][k] == true)
					{
						Vec3f offset = gridStep * Vec3f(i, j, k);
						for (int f = 0; f < 6; f++)
						{
							glNormal3f(cubeNormals[f].x(), cubeNormals[f].y(), cubeNormals[f].z());
							for (int v = 0; v < 4; v++)
							{
								Vec3f vertex = cubeVertices[cubeFaces[f][v]]+ offset;
								glVertex3f(vertex.x(), vertex.y(), vertex.z());
							}
						}
					}
				}
			}
		}
		glEnd();
	}

	virtual bool intersectShadowRay(const Ray& r, float tmin, float distanceToLight)
	{

		return false;
	}

	virtual bool intersect(const Ray& r, Hit& h, float tmin)
	{
		MarchingInfo mi;
		initializeRayMarch(mi,r,tmin);
		if (mi.getTmin() == INFINITY)
		{
			return false;
		}

		int i,j,k;
		mi.getGridIndex(i,j,k);
		while (i < nx && j<ny && k<nz)
		{
			if (opaque[i][j][k])
			{
				h.set(mi.getTmin(), getMaterial(),mi.getNormal() , r);
				return true;
			}
			mi.nextCell();
			mi.getGridIndex(i,j,k);
		}

		return false;

	}


	void initializeRayMarch(MarchingInfo& mi, const Ray& r, float tmin) const
	{
		//Three cases:
		//bool rayOriginInGrid = true;
		//bool startPointInGrid = true;
		//float startT=0;
		//bool intersect = false;
		int startIndex[3] = { 0,0,0 };

		Vec3f startPoint = r.getOrigin() + tmin * r.getDirection();

		if (!getVoxelIndex(startPoint, startIndex))
		{
			Hit hits[6];
			//never think about parallel
			for (int i = 0; i < 6; i++)
			{
				gridPlanes[i]->intersect(r, hits[i], tmin);
			}

			float tNear = -INFINITY;
			float tFar = INFINITY;
			if (r.getDirection().x() > 0)
			{
				tNear = fmaxf(tNear, hits[0].getT());
				tFar = fminf(tFar, hits[1].getT());
			}
			else
			{
				tNear = fmaxf(tNear, hits[1].getT());
				tFar = fminf(tFar, hits[0].getT());
			}

			if (r.getDirection().y() > 0)
			{
				tNear = fmaxf(tNear, hits[2].getT());
				tFar = fminf(tFar, hits[3].getT());
			}
			else
			{
				tNear = fmaxf(tNear, hits[3].getT());
				tFar = fminf(tFar, hits[2].getT());
			}


			if (r.getDirection().z() > 0)
			{
				tNear = fmaxf(tNear, hits[4].getT());
				tFar = fminf(tFar, hits[5].getT());
			}
			else
			{
				tNear = fmaxf(tNear, hits[5].getT());
				tFar = fminf(tFar, hits[4].getT());
			}


			//no intersection
			if (!(tNear < tFar && tNear >= tmin))
			{
				//cout << "Ray has no intersection with grid" << endl;
				mi.setTmin(INFINITY);
				return;
			}

			//add epsilon
			startPoint = r.getOrigin() + r.getDirection() * (tNear + GRID_EPSILON);
			assert(getVoxelIndex(startPoint, startIndex));
			tmin = tmin + GRID_EPSILON;
		}

		mi.setGridIndex(startIndex);
		//cout << "Start grid index: " << startIndex << endl;

		int sign[3];
		r.getDirection().Sign(sign);
		mi.setSign(sign);
		//cout << "Init Sign: " << sign[0] << " " << sign[1] << " " << sign[2] << endl;

		Vec3f dt;
		float arraydt[3];
		for (int i = 0; i < 3; i++)
		{
			if (sign[i] == 0)
			{
				arraydt[i] = INFINITY;
			}
			else
			{
				arraydt[i] = gridStep[i] / fabs(r.getDirection()[i]);
			}
		}
		dt.Set(arraydt[0], arraydt[1], arraydt[2]);
		mi.setDT(dt);
		//cout << "Init d_t: " << dt << endl;

		mi.setTmin(tmin);
		//cout << "Init tmin: " << tmin << endl;


		//get tnext
		Vec3f tempVertex = getVoxelMinByIndex(startIndex[0] + sign[0], startIndex[1] + sign[1], startIndex[2] + sign[2]);
		
		Vec3f offset = tempVertex - startPoint;
		Vec3f percent(offset.x() / gridStep.x(), offset.y() / gridStep.y(), offset.z() / gridStep.z());
		Vec3f toffset = percent * dt;
		Vec3f tnext(toffset.x()+tmin,toffset.y()+tmin,toffset.z()+tmin);
		mi.setTnext(tnext);

		//set normal
		Vec3f tempVertex2 = getVoxelMinByIndex(startIndex[0], startIndex[1], startIndex[2]);
		Vec3f offset2 =  startPoint-tempVertex2;
		Vec3f percent2(offset2.x() / gridStep.x(), offset2.y() / gridStep.y(), offset2.z() / gridStep.z());
		
		int minI;
		float temp2 = INFINITY;
		for (int i = 0; i < 3; i++)
		{
			if (percent2[i] < temp2)
			{
				temp2 = tnext[i];
				minI = i;
			}
		}
		mi.setNormal(minI);



		//Vec3f startSubMin = r.getOrigin() - gridMinVertex;
		//Vec3f startSubMax = r.getOrigin() - gridMaxVertex;

		////Vec3f rayOriginSubMin = r.getOrigin() - gridMinVertex;
		////Vec3f rayOriginSubMax = r.getOrigin() - gridMaxVertex;



		//if (startSubMin.x() > 0&& startSubMin.y()>0&& startSubMin.z()>0
		//	&& startSubMax.x() < 0 && startSubMax.y() < 0 && startSubMax.z() < 0)
		//{
		//	startPointInGrid = true;
		//}
		//else
		//{
		//	startPointInGrid = false;
		//}

		////ray origin inside grid
		//if (startPointInGrid)
		//{
		//	//set start index
		//	startIndex[0] = floor(startSubMin.x() / gridStep.x());
		//	startIndex[1]= floor(startSubMin.y() / gridStep.y());
		//	startIndex[2] = floor(startSubMin.z() / gridStep.z());
		//	mi.setGridIndex(startIndex);
		//	cout << "Start grid index: " << startIndex << endl;

		//	int sign[3];
		//	r.getDirection().Sign(sign);
		//	mi.setSign(sign);
		//	cout << "Init Sign: " << sign[0] << " " << sign[1] << " " << sign[2] << endl;

		//	Vec3f dt(gridStep.x() / r.getDirection().x() , gridStep.y()/r.getDirection().y(),gridStep.z()/r.getDirection().z());
		//	cout << "Init d_t: " << dt << endl;
		//	mi.setDT(dt);

		//	mi.setTmin(tmin);

		//	//intersect = true;
		//	//startT = tmin;

		//}

		//ray origin outside grid
		//else
		//{
		//	

		//}

		
	}


	void getGridInfo(Vec3f& rgridMinVertex, Vec3f& rgridMaxVertex, Vec3f& rgridStep)
	{
		rgridMinVertex = gridMinVertex;
		rgridMaxVertex = gridMaxVertex;
		rgridStep = gridStep;
	}

	Vec3f getGridStep()
	{
		return gridStep;
	}

	float getVoxelHalfDiagonalLength()
	{
		return voxelHalfDiagonalLength;
	}

	void getIndexByVertex(Vec3f vertex, int& i,int& j,int& k)
	{
		i = floor((vertex.x() - gridMinVertex.x()) / gridStep.x());
		j = floor((vertex.y() - gridMinVertex.y()) / gridStep.y());
		k = floor((vertex.z() - gridMinVertex.z()) / gridStep.z());
		if (i >= nx)
		{
			i = nx-1;
		}

		if (j >= ny)
		{
			j = ny - 1;
		}

		if (k >= nz)
		{
			k = nz - 1;
		}
	}

	Vec3f getVoxelMinByIndex(int i, int j, int k) const
	{
		Vec3f offset(gridStep.x() * i, gridStep.y() * j, gridStep.z() * k);
		return Vec3f(gridMinVertex + offset);
	}

	Vec3f getVoxelCenterByIndex(int i,int j,int k)const
	{
		Vec3f offset(gridStep.x() * i, gridStep.y() * j, gridStep.z() * k);
		Vec3f minVoxelCenter = gridMinVertex + Vec3f(gridStep.x()/2,gridStep.y()/2,gridStep.z()/2);
		return minVoxelCenter + offset;
	}

private:

	//int nx;
	//int ny;
	//int nz;
	Vec3f gridMinVertex;
	Vec3f gridMaxVertex;
	Vec3f gridStep;
	int nx, ny, nz;
	Vec3f cubeVertices[8];
	int cubeFaces[6][4] = {
		{ 0, 2, 3, 1 }, {4, 5, 7, 6},
		{0, 1, 5, 4}, {2, 6, 7, 3},
		{0, 4, 6, 2},{1, 3, 7, 5} };
	Vec3f cubeNormals[6];
	Plane *gridPlanes[6];

	float voxelHalfDiagonalLength;

	bool*** opaque;


	bool getVoxelIndex(Vec3f point, int index[]) const
	{
		Vec3f pointSubMin = point - gridMinVertex;
		Vec3f pointSubMax = point - gridMaxVertex;
		if (!(pointSubMin.x() >= 0 && pointSubMin.y() >= 0 && pointSubMin.z() >= 0
			&& pointSubMax.x() <= 0 && pointSubMax.y() <= 0 && pointSubMax.z() <= 0))
		{
			return false;
		}
		index[0] = floor(pointSubMin.x() / gridStep.x());
		index[1] = floor(pointSubMin.y() / gridStep.y());
		index[2] = floor(pointSubMin.z() / gridStep.z());
		if (index[0] >= nx)
		{
			index[0] = nx - 1;
		}

		if (index[1] >= ny)
		{
			index[1] = ny - 1;
		}

		if (index[2] >= nz)
		{
			index[2] = nz - 1;
		}

		return true;
	}

	
	
	
};

#endif