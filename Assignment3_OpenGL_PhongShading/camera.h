#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <limits>
#include <gl/glew.h>
#include <gl/GL.h>
#include<gl/GLU.h>

#include "ray.h"
#include "vectors.h"
#include <gl/glut.h>
#include "matrix.h"


class Camera {
public:
	Camera() {}
	~Camera(){}
	virtual Ray generateRay(Vec2f point) = 0;
	virtual float getTMin() const = 0;
	virtual void glInit(int w, int h) = 0;
	virtual void glPlaceCamera(void) = 0;
	virtual void dollyCamera(float dist) = 0;
	virtual void truckCamera(float dx, float dy) = 0;
	virtual void rotateCamera(float rx, float ry) = 0;
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

// ====================================================================
// Create an orthographic camera with the appropriate dimensions that
// crops the screen in the narrowest dimension.
// ====================================================================
	void glInit(int w, int h)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (w > h)
			glOrtho(-size / 2.0, size / 2.0, -size * (float)h / (float)w / 2.0, size * (float)h / (float)w / 2.0, 0.5, 40.0);
		else
			glOrtho(-size * (float)w / (float)h / 2.0, size * (float)w / (float)h / 2.0, -size / 2.0, size / 2.0, 0.5, 40.0);
	}

// ====================================================================
// Place an orthographic camera within an OpenGL scene
// ====================================================================
	void glPlaceCamera(void)
	{
		gluLookAt(center.x(), center.y(), center.z(), center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(), up.x(), up.y(), up.z());
	}

// ====================================================================
// dollyCamera, truckCamera, and RotateCamera
// // Asumptions:
// - up is really up (i.e., it hasn't been changed
// to point to "screen up")
// - up and direction are normalized
// Special considerations:
// - If your constructor precomputes any vectors for
// use in 'generateRay', you will likely to recompute those
// values at athe end of the these three routines
// ====================================================================
// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================
	void dollyCamera(float dist)
	{
		center += direction * dist;

		// ===========================================
		// ASSIGNMENT 3: Fix any other affected values 
		// ===========================================
	}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================
	void truckCamera(float dx, float dy)
	{
		//Vec3f horizontal;
		//Vec3f::Cross3(horizontal, direction, up);
		//horizontal.Normalize();
		Vec3f screenUp;
		Vec3f::Cross3(screenUp, horizontal, direction);
		center += horizontal * dx + screenUp * dy;
		// ===========================================
		// ASSIGNMENT 3: Fix any other affected values
		// ===========================================
	}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================
	void rotateCamera(float rx, float ry)
	{
		//Vec3f horizontal;
		//Vec3f::Cross3(horizontal, direction, up);
		//horizontal.Normalize();

		// Don't let the model flip upside-down (There is a singularity 
		// at the poles when 'up' and 'direction' are aligned)
		float tiltAngle = acos(up.Dot3(direction));
		if (tiltAngle - ry > 3.13)
			ry = tiltAngle - 3.13;
		else if (tiltAngle - ry < 0.01)
			ry = tiltAngle - 0.01;
		Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
		rotMat *= Matrix::MakeAxisRotation(horizontal, ry);
		rotMat.Transform(center);
		rotMat.TransformDirection(direction);

		// ===========================================
		// ASSIGNMENT 3: Fix any other affected values
		// ===========================================
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

// ====================================================================
// Create a perspective camera with the appropriate dimensions that
// crops or stretches in the x-dimension as necessary
// ====================================================================
	void glInit(int w, int h)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(angle * 180.0 / 3.14159, (float)w / float(h), 0.5, 40.0);
	}

// ====================================================================
// Place a perspective camera within an OpenGL scene
// ====================================================================
	void glPlaceCamera(void)
	{
		gluLookAt(center.x(), center.y(), center.z(), center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(), up.x(), up.y(), up.z());
	}

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================
	void dollyCamera(float dist)
	{
		center += direction * dist;
		// ===========================================
		// ASSIGNMENT 3: Fix any other affected values
		// ===========================================
	}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================
	void truckCamera(float dx, float dy)
	{
		Vec3f horizontal;
		Vec3f::Cross3(horizontal, direction, up);
		horizontal.Normalize();
		Vec3f screenUp;
		Vec3f::Cross3(screenUp, horizontal, direction);
		center += horizontal * dx + screenUp * dy;
		// ===========================================
		// ASSIGNMENT 3: Fix any other affected values
		// ===========================================
	}


// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================
	void rotateCamera(float rx, float ry)
	{
		Vec3f horizontal;
		Vec3f::Cross3(horizontal, direction, up);
		horizontal.Normalize();
		// Don't let the model flip upside-down (There is a singularity
		// at the poles when 'up' and 'direction' are aligned)
		float tiltAngle = acos(up.Dot3(direction));
		if (tiltAngle - ry > 3.13)
			ry = tiltAngle - 3.13;
		else if (tiltAngle - ry < 0.01)
			ry = tiltAngle - 0.01;
		Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
		rotMat *= Matrix::MakeAxisRotation(horizontal, ry);
		rotMat.Transform(center);
		rotMat.TransformDirection(direction);
		//direction.Normalize(); 
		// ===========================================
		// ASSIGNMENT 3: Fix any other affected values
		// ===========================================
		this->direction.Normalize();
		this->up = this->up - this->direction * this->up.Dot3(this->direction);
		this->up.Normalize();
		Vec3f::Cross3(this->horizontal, this->direction, this->up);
		this->horizontal.Normalize();
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