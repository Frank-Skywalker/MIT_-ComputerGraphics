#ifndef _MARCHING_INFO_H_
#define _MARCHING_INFO_H_
#include "vectors.h"

class MarchingInfo
{
public:
	MarchingInfo():tmin(INFINITY)
	{
		threeNormals[0].Set(-1, 0, 0);
		threeNormals[1].Set(0, -1, 0);
		threeNormals[2].Set(0, 0, -1);
	}

	float getTmin()
	{
		return tmin;
	}
	void setTmin(float tmin)
	{
		this->tmin = tmin;
	}

	void setGridIndex(int index[])
	{
		for (int i = 0; i < 3; i++)
		{
			gridIndex[i] = index[i];
		}
	}

	void getGridIndex(int rindex[])
	{
		for (int i = 0; i < 3; i++)
		{
			rindex[i] = gridIndex[i];
		}
	}

	void getGridIndex(int& i, int& j, int& k)
	{
		i = gridIndex[0];
		j = gridIndex[1];
		k = gridIndex[2];
	}

	void setDT(Vec3f dt)
	{
		this->dt = dt;
	}
	void setTnext(Vec3f tnext)
	{
		this->tnext = tnext;
	}

	void setSign(int sign[])
	{
		for (int i = 0; i < 3; i++)
		{
			assert(sign[i] == 1 || sign[i] == -1 || sign[i]==0);
			this->sign[i] = sign[i];
		}
		//assert(ysign == 1 || ysign == -1);
		//assert(zsign == 1 || zsign == -1);
		//sign[0] = xsign;
		//sign[1] = ysign;
		//sign[2] = zsign;
	}

	//no dealing with offset
	void nextCell()
	{
		int minI;
		float temp=INFINITY;
		for (int i = 0; i < 3; i++)
		{
			if (tnext[i] < temp)
			{
				temp = tnext[i];
				minI = i;
			}
		}
		gridIndex[minI] += sign[minI];
		tmin = tnext[minI];
		tnext.Set(minI, tnext[minI] + dt[minI]);
		normal = threeNormals[minI] * sign[minI];
	}

	
	Vec3f getNormal()
	{
		return normal;
	}
	
	void setNormal(int axis)
	{
		this->normal = threeNormals[axis]*sign[axis];
	}




	//To render the occupied grid cells for visualization you will also need to 
	//store the surface normal of the cell face which was crossed to enter the current cell.
	
private:
	float tmin;
	int gridIndex[3];
	Vec3f tnext;
	Vec3f dt;
	int sign[3];
	Vec3f normal;

	Vec3f threeNormals[3];

};


#endif