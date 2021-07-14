#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.h"
class Camera {
public:
	Camera() {}
	Camera(float tmin) :tmin(tmin) {}
	virtual Ray generateRay(Vec2f point) = 0;
	virtual float getTMin() const = 0;
private:
	float tmin;

};

#endif