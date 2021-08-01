#ifndef _GROUP_H_
#define _GROUP_H_

#include "object3d.h"
#include <vector>

using namespace std;

class Group : public Object3D {
public:
	Group()
	{
	}
	Group(int num)
	{
		objectsNum = num;
		objects.reserve(objectsNum);
	}
	~Group()
	{
		for(int i=0;i<objects.size();i++)
		{
			delete objects[i];
		}
	}
	void addObject(int index, Object3D* obj)
	{
		objects.insert(objects.begin()+index, obj);
		//cout << "sdds: " << objectsNum << endl;
		//cout << "objects number: " << objects.size() << endl;

		//generate bounding box
		if (index == objectsNum - 1)
		{
			bool hasOtherThanPlane = false;
			Vec3f min(INFINITY, INFINITY, INFINITY);
			Vec3f max(-INFINITY, -INFINITY, -INFINITY);
			for (auto it = objects.begin(); it != objects.end(); it++)
			{
				if ((*it)->getBoundingBox() == NULL)
				{
					continue;
				}
				hasOtherThanPlane = true;
				Vec3f::Min(min, min, (*it)->getBoundingBox()->getMin());
				Vec3f::Max(max, max, (*it)->getBoundingBox()->getMax());
			}

			if(!hasOtherThanPlane)
			{ 
				return;
			}
			boundingBox = new BoundingBox(min, max);
		}
	}

	virtual bool intersect(const Ray& r, Hit& h, float tmin)
	{
		bool flag=false;
		//int i=0;
		//cout << "ray: " << r.getDirection() << endl;
		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			flag|=(*it)->intersect(r, h, tmin);
		}
		//cout << "h.t: " << h.getT() << endl;

		return flag;
		
	}


	virtual bool intersectShadowRay(const Ray& r, float tmin, float distanceToLight)
	{
		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			if ((*it)->intersectShadowRay(r, tmin,distanceToLight))
			{
				return true;
			}
		}
		return false;
	}




	virtual void paint(void)
	{
		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			(*it)->paint();
		}
	}


	//Assignment5
	virtual void insertIntoGrid(Grid* g, Matrix* m)
	{
		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			Matrix* tempM = NULL;
			if (m != NULL)
			{
				tempM = new Matrix(*m);
			}
			(*it)->insertIntoGrid(g, tempM);
			if (tempM != NULL)
			{
				delete tempM;
			}
		}
	}

private:
	int objectsNum;
	vector<Object3D*> objects;

};


#endif