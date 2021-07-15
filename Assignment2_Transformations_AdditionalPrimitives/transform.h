#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_
#include "object3d.h"
#include "matrix.h"
class Transform :public Object3D
{
public:
	Transform(Matrix& m, Object3D* o):transform(m),object(o)
	{
	}
	~Transform()
	{
		delete object;
	}

	virtual bool intersect(const Ray& r, Hit& h, float tmin)
	{
		Ray rayObjectSpace;
		
	}

private:
	Matrix transform;
	Object3D* object;

};

#endif