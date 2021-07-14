#ifndef _ORTHOGRAPHICCAMERA_
#define _ORTHOGRAPHICCAMERA_

#include "Camera.h"
#include "vectors.h"
#include <limits>

class OrthographicCamera : public Camera
{
public:
	OrthographicCamera() : Camera(-INFINITY) {}
	Ray generateRay(Vec2f point)
	{
		float pointX, pointY;
		point.Get(pointX, pointY);

		Vec3f origin = center + (pointX - 0.5) * size * horizontal + (pointY - 0.5) * size * up;
		//cout << "Origin: " << origin<<endl;
		return Ray(origin, direction);
	}

	virtual float getTMin() const
	{
		return tmin;
	}

	OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size) : Camera(-INFINITY), center(center)
	{
		this->direction = direction;
		this->direction.Normalize();
		this->up = up - direction * up.Dot3(direction);
		up.Normalize();
		Vec3f::Cross3(this->horizontal, this->direction, this->up);
		this->horizontal.Normalize();
		this->size = size;
	}

private:
	Vec3f center;
	float size;
	Vec3f horizontal;
	Vec3f up;
	Vec3f direction;
	float tmin;
};

#endif