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
		//transform ray from world space to object space
		Matrix inverseTransform = transform;
		assert(inverseTransform.Inverse());
		Vec3f rayOriginOS = r.getOrigin();
		Vec3f rayDirectionOS = r.getDirection();

		inverseTransform.Transform(rayOriginOS);
		inverseTransform.TransformDirection(rayDirectionOS);

		//store the transform ratio
		float ratio = r.getDirection().x() / rayDirectionOS.x();
		rayDirectionOS.Normalize();
		Ray rayOS(rayOriginOS,rayDirectionOS);

		//do intersect in object space
		Hit temph(h);
		temph.set(temph.getT() / ratio, rayOS);
		if (!object->intersect(rayOS, temph, tmin))
		{
			return false;
		}

		//transform normal from object space to world space
		inverseTransform.Transpose();
		Vec3f normalWS = temph.getNormal();
		inverseTransform.TransformDirection(normalWS);
		h.set(temph.getT()*ratio,temph.getMaterial(),normalWS,r);
		return true;
	}

private:
	Matrix transform;
	Object3D* object;

};

#endif