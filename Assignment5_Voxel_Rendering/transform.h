#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_
#include "object3d.h"
#include "matrix.h"
class Transform :public Object3D
{
public:
	Transform(Matrix& m, Object3D* o):transform(m),object(o)
	{

		//generate bounding box
		Vec3f objectMin;
		Vec3f objectMax;

		//object is a plane
		if (object->getBoundingBox() == NULL)
		{
			return;
		}
		//get sub object bounding box info
		object->getBoundingBox()->Get(objectMin, objectMax);

		//get eight vertices of sub object bounding box
		Vec3f transVertices[8];
		transVertices[0] = objectMin;
		transVertices[1] = Vec3f(objectMin.x(), objectMin.y(), objectMax.z());
		transVertices[2] = Vec3f(objectMin.x(), objectMax.y(), objectMin.z());
		transVertices[3] = Vec3f(objectMin.x(), objectMax.y(), objectMax.z());
		transVertices[4] = Vec3f(objectMax.x(), objectMin.y(), objectMin.z());
		transVertices[5] = Vec3f(objectMax.x(), objectMin.y(), objectMax.z());
		transVertices[6] = Vec3f(objectMax.x(), objectMax.y(), objectMin.z());
		transVertices[7] = Vec3f(objectMax.x(), objectMax.y(), objectMax.z());

		Vec3f min(INFINITY,INFINITY,INFINITY);
		Vec3f max(-INFINITY,-INFINITY,-INFINITY);
		for (int i = 0; i < 8; i++)
		{
			transform.Transform(transVertices[i]);
			Vec3f::Min(min, min, transVertices[i]);
			Vec3f::Max(max, max, transVertices[i]);
		}
		boundingBox = new BoundingBox(min, max);
	}
	~Transform()
	{
		delete object;
	}

	virtual bool intersect(const Ray& r, Hit& h, float tmin)
	{
		//cout << "here" << endl;
		//transform ray from world space to object space
		Matrix inverseTransform = transform;
		assert(inverseTransform.Inverse());
		Vec3f rayOriginOS = r.getOrigin();
		Vec3f rayDirectionOS = r.getDirection();

		inverseTransform.Transform(rayOriginOS);
		inverseTransform.TransformDirection(rayDirectionOS);

		float ratio = rayDirectionOS.Length();

		//store the transform ratio
		//float ratio = r.getDirection().x() / rayDirectionOS.x();
		rayDirectionOS.Normalize();
		Ray rayOS(rayOriginOS,rayDirectionOS);

		//do intersect in object space
		Hit temph(h);
		temph.set(h.getT() * ratio, h.getMaterial(), h.getNormal(), rayOS);
		if (!object->intersect(rayOS, temph, tmin*ratio))
		{
			return false;
		}

		//transform normal from object space to world space
		inverseTransform.Transpose();
		Vec3f normalWS = temph.getNormal();
		inverseTransform.TransformDirection(normalWS);
		normalWS.Normalize();
		h.set(temph.getT()/ratio,temph.getMaterial(),normalWS,r);
		return true;
	}

	virtual bool intersectShadowRay(const Ray& r, float tmin,float distanceToLight)
	{
		//cout << "here" << endl;
		//transform ray from world space to object space
		Matrix inverseTransform = transform;
		assert(inverseTransform.Inverse());
		Vec3f rayOriginOS = r.getOrigin();
		Vec3f rayDirectionOS = r.getDirection();

		inverseTransform.Transform(rayOriginOS);
		inverseTransform.TransformDirection(rayDirectionOS);

		float ratio = rayDirectionOS.Length();

		//store the transform ratio
		//float ratio = r.getDirection().x() / rayDirectionOS.x();
		rayDirectionOS.Normalize();
		Ray rayOS(rayOriginOS, rayDirectionOS);

		//do intersect in object space
		if (object->intersectShadowRay(rayOS, tmin * ratio, distanceToLight*ratio))
		{
			return true;
		}
		return false;
	}

	virtual void paint(void)
	{
		glPushMatrix();
		GLfloat* glMatrix = transform.glGet();
		glMultMatrixf(glMatrix);
		delete[] glMatrix;
		object->paint();
		glPopMatrix();
	}

	virtual void insertIntoGrid(Grid* grid, Matrix* m)
	{
		*m =(*m) * transform;
		object->insertIntoGrid(grid, m);
	}


private:
	Matrix transform;
	Object3D* object;

};

#endif