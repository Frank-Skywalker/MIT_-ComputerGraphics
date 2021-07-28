#ifndef _GRID_H_
#define _GRID_H_

#include "object3d.h"
#include "marchinginfo.h"
#include "plane.h"
#include "material.h"
#include "rayTree.h"
#include "object3dvector.h"

#define GRID_EPSILON 0.00001
#define MATERIAL_NUM 16

class Grid :public Object3D
{
public:
	Grid(BoundingBox* bb, int nx, int ny, int nz):nx(nx),ny(ny),nz(nz)
	{

		//material = new PhongMaterial(Vec3f(0.5, 0, 0), Vec3f(), 0);

		//cout << "bb min" << bb->getMin() << endl;
		//cout << "bb max" << bb->getMax() << endl;
		//cout << "nx ny nz: " << nx << " " << ny << " " << nz << endl;
		boundingBox = bb;
		boundingBox->Get(gridMinVertex, gridMaxVertex);

		Vec3f subVertex = gridMaxVertex - gridMinVertex;
		subVertex.Divide(nx, ny, nz);
		gridStep = subVertex;
		//cout << "grid step: " << gridStep<<endl;

		voxelHalfDiagonalLength = gridStep.Length() / 2;


		voxels = new Object3DVector** [nx];
		for (int i = 0; i < nx; i++)
		{
			voxels[i] = new Object3DVector* [ny];
			for (int j = 0; j < ny; j++)
			{
				voxels[i][j] = new Object3DVector [nz];
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
		//Material* m = new PhongMaterial(Vec3f(0, 0, 0), Vec3f(0, 0, 0), 1);
		//Vec3f normals[3];
		//normals[0].Set(1, 0, 0);
		//normals[1].Set(0, 1, 0);
		//normals[2].Set(0, 0, 1);
		//gridPlanes[0] = new Plane(normals[0], gridMinVertex.x(),m);			//yz
		//gridPlanes[1] = new Plane(normals[0], gridMaxVertex.x(), m);		//yz
		//gridPlanes[2] = new Plane(normals[1], gridMinVertex.y(), m);		//xz
		//gridPlanes[3] = new Plane(normals[1], gridMaxVertex.y(), m);		//xz
		//gridPlanes[4] = new Plane(normals[2], gridMinVertex.z(), m);		//xy
		//gridPlanes[5] = new Plane(normals[2], gridMaxVertex.z(), m);		//xy


		//init material
		nowMaterialIndexFace = 0;
		nowMaterialIndexCell = 0;

		materials[0] = new PhongMaterial(Vec3f(1, 1, 1), Vec3f(0, 0, 0), 0);
		materials[1] = new PhongMaterial(Vec3f(0.95, 0.129, 0.98), Vec3f(0, 0, 0), 0);
		materials[2] = new PhongMaterial(Vec3f(0.6549, 0.149, 0.98), Vec3f(0, 0, 0), 0);
		materials[3] = new PhongMaterial(Vec3f(0.2235, 0.16, 0.98), Vec3f(0, 0, 0), 0);
		materials[4] = new PhongMaterial(Vec3f(0, 0.1647, 1), Vec3f(0, 0, 0), 0);
		materials[5] = new PhongMaterial(Vec3f(0, 0.34, 1), Vec3f(0, 0, 0), 0);
		materials[6] = new PhongMaterial(Vec3f(0.145, 0.7098, 0.99), Vec3f(0, 0, 0), 0);
		materials[7] = new PhongMaterial(Vec3f(0.30,1, 1), Vec3f(0, 0, 0), 0);

		materials[8] = new PhongMaterial(Vec3f(0.20, 0.65, 0.47), Vec3f(0, 0, 0), 0);
		materials[9] = new PhongMaterial(Vec3f(0.21, 0.647, 0.2392), Vec3f(0, 0, 0), 0);
		materials[10] = new PhongMaterial(Vec3f(0.2157, 0.647, 0.1333), Vec3f(0, 0, 0), 0);
		materials[11] = new PhongMaterial(Vec3f(0.29, 0.647, 0.133), Vec3f(0, 0, 0), 0);

		materials[12] = new PhongMaterial(Vec3f(0.745, 0.99, 0.2196), Vec3f(0, 0, 0), 0);
		materials[13] = new PhongMaterial(Vec3f(1, 0.99, 0.2157), Vec3f(0, 0, 0), 0);
		materials[14] = new PhongMaterial(Vec3f(0.99, 0.6863, 0.1294), Vec3f(0, 0, 0), 0);
		materials[15] = new PhongMaterial(Vec3f(0.96, 0.2784, 0), Vec3f(0, 0, 0), 0);




	}


	void addObjectToVoxel(int i, int j, int k, Object3D* obj)
	{
		//opaque[i][j][k] = true;
		voxels[i][j][k].addObject(obj);
	}



	virtual void paint(void)
	{
		glBegin(GL_QUADS);
		for (int i = 0; i < nx; i++)
		{
			for (int j = 0; j < ny; j++)
			{
				for (int k = 0; k < nz; k++)
				{
					//if (opaque[i][j][k] == true)
					int numObjects = voxels[i][j][k].getNumObjects();
					if(numObjects!=0)
					{
						int index = numObjects-1;
						if (index >= 16)
						{
							index = 15;
						}
						this->material = materials[index];
						getMaterial()->glSetMaterial();
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


	void addHitCell(int i, int j, int k)
	{


		Vec3f offset = gridStep * Vec3f(i, j, k);
		for (int f = 0; f < 6; f++)
		{
			Vec3f vertices[4];
			for (int v = 0; v < 4; v++)
			{
				vertices[v]= cubeVertices[cubeFaces[f][v]] + offset;
			}
			RayTree::AddHitCellFace(vertices[0], vertices[1], vertices[2], vertices[3], cubeNormals[f], materials[nowMaterialIndexCell]);
			
		}

		nowMaterialIndexCell++;
		if (nowMaterialIndexCell >= 16)
		{
			nowMaterialIndexCell -= 16;
		}

	}


	void addEnteredFace(int i, int j, int k, int axis, int* sign)
	{


		Vec3f offset = gridStep * Vec3f(i, j, k);

		int index = 2 - axis;
		if (sign[axis] == 1)
		{
			index = 2 * index ;
		}
		else if(sign[axis]==-1)
		{
			index = 2 * index+1;
		}

		Vec3f vertices[4];

		for (int v = 0; v < 4; v++)
		{
			vertices[v] = cubeVertices[cubeFaces[index][v]] + offset;
		}

		RayTree::AddEnteredFace(vertices[0], vertices[1], vertices[2], vertices[3], cubeNormals[index],materials[nowMaterialIndexFace]);
		nowMaterialIndexFace++;
		if (nowMaterialIndexFace >= 16)
		{
			nowMaterialIndexFace -= 16;
		}
	
	}
	

	virtual bool ShadowRay(const Ray& r, float tmin, float distanceToLight)
	{
		RayTracingStats::IncrementNumIntersections();
		return false;
	}

	virtual bool intersect(const Ray& r, Hit& h, float tmin)
	{
		RayTracingStats::IncrementNumIntersections();
		nowMaterialIndexCell = 0;
		nowMaterialIndexFace = 0;
		MarchingInfo mi;
		initializeRayMarch(mi,r,tmin);
		if (mi.getTmin() == INFINITY)
		{
			return false;
		}

		int i,j,k;
		mi.getGridIndex(i,j,k);
		while (i>=0 && i < nx &&j>=0&& j<ny &&k>=0&& k<nz)
		{
			RayTracingStats::IncrementNumGridCellsTraversed();
			addHitCell(i, j, k);
			addEnteredFace(i, j, k, mi.getAxis(), mi.getSign());
			//cout << "i,j,k: " << i << " " << j << " " << k << " " << endl;
			int numObjects = voxels[i][j][k].getNumObjects();
			if (numObjects !=0)
			{
				int index = numObjects-1;
				if (index >= 16)
				{
					index = 15;
				}
				h.set(mi.getTmin(), materials[index],mi.getNormal() , r);
				//cout << "Normal: " << mi.getNormal()<<endl;
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

		int sign[3];
		r.getSign(sign);
		Vec3f rayInvDir = r.getInverseDirection();
		Vec3f rayOrigin = r.getOrigin();

		int startIndex[3] = { 0,0,0 };

		Vec3f startPoint = r.getOrigin() + tmin * r.getDirection();

		if (!getVoxelIndex(startPoint, startIndex))
		{


			float tNear = -INFINITY;
			float tFar = INFINITY;

			//cout << endl << "origin: " << r.getOrigin() << endl;
			//cout <<"direcrtion: "<< r.getDirection() << endl;
			//cout <<"inverse direction: "<< r.getInverseDirection() << endl;
			for (int i = 0; i < 3; i++)
			{
				if (rayInvDir[i] >= 0)
				{
					tNear = fmaxf(tNear,( gridMinVertex[i] - rayOrigin[i]) * rayInvDir[i]);
					tFar = fminf(tFar,(gridMaxVertex[i] - rayOrigin[i]) * rayInvDir[i]);
				}
				else
				{
					tNear = fmaxf(tNear,( gridMaxVertex[i] - rayOrigin[i]) * rayInvDir[i]);
					//cout << "gridMaxVertex[i]: " << gridMaxVertex[i] << endl;
					//cout << "rayOrigin[i]: " << rayOrigin[i] << endl;
					//cout << "rayInvDir[i]: " << rayInvDir[i] << endl;
					//cout << "here near: " << tNear << endl;
					tFar = fminf(tFar, (gridMinVertex[i] - rayOrigin[i]) * rayInvDir[i]);
				}
			}
	
			//Hit hits[6];
			////never think about parallel
			//for (int i = 0; i < 6; i++)
			//{
			//	//gridPlanes[i]->intersect(r, hits[i], tmin);
			//}


			//float tNear = -INFINITY;
			//float tFar = INFINITY;


			//for (int i = 0; i < 3; i++)
			//{
			//	int index = 2 * i;
			//	if (sign[i] > 0)
			//	{
			//		tNear = fmaxf(tNear, hits[index].getT());
			//		tFar = fminf(tFar, hits[index + 1].getT());
			//	}
			//	else
			//	{
			//		tNear = fmaxf(tNear, hits[index+1].getT());
			//		tFar = fminf(tFar, hits[index].getT());
			//	}

			//}


			//cout << "tNear: " << tNear << endl;
			//cout << "tFar: " << tFar << endl;
			//no intersection
			if (!(tNear < tFar && tNear >= tmin))
			{
				//cout << "Ray has no intersection with grid" << endl;
				mi.setTmin(INFINITY);
				return;
			}

			//cout << "Ray intersect with grid" << endl;
			//add epsilon
			startPoint = r.getOrigin() + r.getDirection() * (tNear);
			assert(!getVoxelIndex(startPoint, startIndex));
			//cout << "startPoint: " << startPoint << endl;
			//cout << "t: " << tNear << endl;
			tmin = tNear;
			//cout << "Start Point: " << startPoint << endl;
		}

		mi.setGridIndex(startIndex);
		//cout << "Start grid index: " << startIndex << endl;

		
		mi.setSign(sign);

	
		//cout << "Init Sign: " << sign[0] << " " << sign[1] << " " << sign[2] << endl;

		Vec3f dt;
		float arraydt[3];
		for (int i = 0; i < 3; i++)
		{
			//if (sign[i] == 0)
			//{
			//	arraydt[i] = INFINITY;
			//}
			//else
			//{
			//	arraydt[i] = gridStep[i] / fabs(r.getDirection()[i]);
			//}
			arraydt[i] = gridStep[i] * fabs(rayInvDir[i]);
		}
		dt.Set(arraydt[0], arraydt[1], arraydt[2]);
		mi.setDT(dt);
		//cout << "Init d_t: " << dt << endl;

		mi.setTmin(tmin);
		//cout << "Init tmin: " << tmin << endl;


		//get tnext
		Vec3f nextVoxelConner = getVoxelCornerBySign(startIndex[0] , startIndex[1] , startIndex[2],sign);
		Vec3f offset = nextVoxelConner - startPoint;
		offset.Set(fabs(offset.x()), fabs(offset.y()), fabs(offset.z()));
		Vec3f percent(offset.x() / gridStep.x(), offset.y() / gridStep.y(), offset.z() / gridStep.z());
		Vec3f toffset = percent * dt;
		Vec3f tnext(toffset.x()+tmin,toffset.y()+tmin,toffset.z()+tmin);
		mi.setTnext(tnext);

		//set normal
		Vec3f voxelMin = getVoxelMinByIndex(startIndex[0], startIndex[1], startIndex[2]);
		Vec3f offset2 =  startPoint- voxelMin;
		offset2.Set(fabs(offset2.x()), fabs(offset2.y()), fabs(offset2.z()));
		Vec3f percent2(offset2.x() / gridStep.x(), offset2.y() / gridStep.y(), offset2.z() / gridStep.z());
		//cout << "percent2: " << percent2 << endl;

		int maxI;
		float temp2 = -INFINITY;
		for (int i = 0; i < 3; i++)
		{
			if (percent2[i] > temp2)
			{
				temp2 = percent2[i];
				maxI= i;
			}
		}
		mi.setNormal(maxI);
		//cout << "maxI: "<<maxI << endl;

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

	
	
	//void getIndexByVertex(Vec3f vertex, int& i,int& j,int& k)
	//{
	//	i = floor((vertex.x() - gridMinVertex.x()) / gridStep.x());
	//	j = floor((vertex.y() - gridMinVertex.y()) / gridStep.y());
	//	k = floor((vertex.z() - gridMinVertex.z()) / gridStep.z());
	//	if (i >= nx)
	//	{
	//		i = nx-1;
	//	}

	//	if (j >= ny)
	//	{
	//		j = ny - 1;
	//	}

	//	if (k >= nz)
	//	{
	//		k = nz - 1;
	//	}
	//}


	bool getVoxelIndex(Vec3f point, int& i,int& j, int& k) const
	{
		Vec3f pointSubMin = point - gridMinVertex;
		Vec3f pointSubMax = point - gridMaxVertex;
		if (!(pointSubMin.x() >= -GRID_EPSILON && pointSubMin.y() >= -GRID_EPSILON && pointSubMin.z() >= -GRID_EPSILON
			&& pointSubMax.x() <= GRID_EPSILON && pointSubMax.y() <= GRID_EPSILON && pointSubMax.z() <= GRID_EPSILON))
		{
			return false;
		}
		i = floor(pointSubMin.x() / gridStep.x());
		j = floor(pointSubMin.y() / gridStep.y());
		k = floor(pointSubMin.z() / gridStep.z());
		if (i < 0)
		{
			i = 0;
		}
		else if (i>= nx)
		{
			i= nx - 1;
		}

		if (j < 0)
		{
			j = 0;
		}
		else if (j>= ny)
		{
			j= ny - 1;
		}

		if (k < 0)
		{
			k = 0;
		}
		else if (k>= nz)
		{
			k= nz - 1;
		}

		return true;
	}

	bool getVoxelIndex(Vec3f point, int index[]) const
	{
		Vec3f pointSubMin = point - gridMinVertex;
		Vec3f pointSubMax = point - gridMaxVertex;
		if (!(pointSubMin.x() >= -GRID_EPSILON && pointSubMin.y() >= -GRID_EPSILON && pointSubMin.z() >= -GRID_EPSILON
			&& pointSubMax.x() <= GRID_EPSILON && pointSubMax.y() <= GRID_EPSILON && pointSubMax.z() <= GRID_EPSILON))
		{
			return false;
		}
		index[0] = floor(pointSubMin.x() / gridStep.x());
		index[1] = floor(pointSubMin.y() / gridStep.y());
		index[2] = floor(pointSubMin.z() / gridStep.z());
		if (index[0] < 0)
		{
			index[0] = 0;
		}
		else if (index[0] >= nx)
		{
			index[0] = nx - 1;
		}

		if (index[1] < 0)
		{
			index[1] = 0;
		}
		else if (index[1] >= ny)
		{
			index[1] = ny - 1;
		}

		if (index[2] < 0)
		{
			index[2] = 0;
		}
		else if (index[2] >= nz)
		{
			index[2] = nz - 1;
		}

		return true;
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

	Vec3f getVoxelCornerBySign(int i, int j, int k, int sign[]) const
	{
		Vec3f voxelCenter=getVoxelCenterByIndex(i, j, k);
		int mysign[3];
		//deal with sign 0
		for (int i = 0; i < 3; i++)
		{
			if (sign[i] == 0)
			{
				mysign[i] = 1;
			}
			else
			{
				mysign[i] = sign[i];
			}
		}
		Vec3f offset(mysign[0] * gridStep.x() / 2, mysign[1] * gridStep.y() / 2, mysign[2] * gridStep.z() / 2);

		return voxelCenter + offset;
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
		{ 0, 2, 3, 1 },		//xy	z	-1 
		{4, 5, 7, 6},		//xy	z	1
		{0, 1, 5, 4},		//xz	y	-1
		{2, 6, 7, 3},		//xz	y	1	
		{0, 4, 6, 2},		//yz	x	-1
		{1, 3, 7, 5} };		//yz	x	1
	Vec3f cubeNormals[6];
	//Plane *gridPlanes[6];

	float voxelHalfDiagonalLength;

	//bool*** opaque;

	Object3DVector ***voxels;

	Material* materials[16];
	int nowMaterialIndexCell;
	int nowMaterialIndexFace;
	

	

	
	
	
};

#endif