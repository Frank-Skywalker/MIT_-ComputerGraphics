#ifndef _GROUP_H_
#define _GROUP_H_

#include "object3d.h"
#include <vector>

using namespace std;

class Group : public Object3D {
public:
	Group(){}
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
	}

	virtual bool intersect(const Ray& r, Hit& h, float tmin)
	{
		bool flag=false;
		//int i=0;
		//cout << "ray: " << r.getDirection() << endl;
		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			//cout << "round: " <<i<< endl;
			//cout << "sphere material: " << (*it)->getMaterial() << endl;
			//cout << "h.t: " << h.getT() << endl;
			//cout << "sphere material:" << (*it)->getMaterial() << endl;
			//if ((*it)->intersect(r, h, tmin))
			//{
			//	//cout << "intersect!" << endl;
			//	//flag = true;
			//}
			//i++;
			//
			flag|=(*it)->intersect(r, h, tmin);
		}
		//cout << "h.t: " << h.getT() << endl;

		return flag;
		
	}

	virtual void paint(void)
	{
		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			(*it)->paint();
		}
	}

private:
	int objectsNum;
	vector<Object3D*> objects;

};


#endif