#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <limits>
#include "ray.h"
#include "vectors.h"

class Camera {
public:
	Camera() {}
	~Camera(){}
	virtual Ray generateRay(Vec2f point) = 0;
	virtual float getTMin() const = 0;

};


class OrthographicCamera : public Camera
{
public:
	OrthographicCamera() 
	{
		assert(0);
	}
	~OrthographicCamera(){}
	Ray generateRay(Vec2f point)
	{
		float pointX, pointY;
		point.Get(pointX, pointY);

		Vec3f rayOrigin = center + (pointX - 0.5) * size * horizontal + (pointY - 0.5) * size * up;
		//cout << "Origin: " << origin<<endl;
		return Ray(rayOrigin, direction);
	}

	virtual float getTMin() const
	{
		return tmin;
	}

	OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size) : center(center),direction(direction),up(up),size(size)
	{
		this->direction.Normalize();
		this->up = this->up - this->direction * this->up.Dot3(this->direction);
		this->up.Normalize();
		Vec3f::Cross3(this->horizontal, this->direction, this->up);
		this->horizontal.Normalize();
		this->tmin = -INFINITY;
	}

private:
	Vec3f center;
	float size;
	Vec3f horizontal;
	Vec3f up;
	Vec3f direction;
	float tmin;

};

class PerspectiveCamera :public Camera
{
public:
	PerspectiveCamera()
	{
		assert(0);
	}
	PerspectiveCamera(Vec3f center, Vec3f& direction, Vec3f& up, float angle):center(center),direction(direction),up(up),angle(angle)
	{
		this->direction.Normalize();
		this->up = this->up - this->direction * this->up.Dot3(this->direction);
		this->up.Normalize();
		Vec3f::Cross3(this->horizontal, this->direction, this->up);
		this->horizontal.Normalize();
		virtualScreenSize = 2 * tanf(angle / (float)2);
		this->tmin = 0;
	}
	~PerspectiveCamera() {}
	Ray generateRay(Vec2f point)
	{
		float pointX, pointY;
		point.Get(pointX, pointY);

		Vec3f rayOrigin = center;
		Vec3f rayDirection = direction+(pointX - 0.5) * virtualScreenSize * horizontal + (pointY - 0.5) * virtualScreenSize * up;
		rayDirection.Normalize();
		//cout << "Origin: " << origin<<endl;
		return Ray(rayOrigin, rayDirection);
	}

	virtual float getTMin() const
	{
		return tmin;
	}


private:
	Vec3f center;
	Vec3f horizontal;
	Vec3f up;
	Vec3f direction;
	float angle;
	float virtualScreenSize;
	float virtualScreenDistance = 1;
	float tmin;

};



#endif