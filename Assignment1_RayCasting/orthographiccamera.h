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

	OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size) : Camera(-INFINITY), center(center), direction(direction), up(up), size(size)
	{
		this->direction.Normalize();
		this->up = this->up - this->direction * this->up.Dot3(this->direction);
		this->up.Normalize();
		Vec3f::Cross3(this->horizontal, this->direction, this->up);
		this->horizontal.Normalize();
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