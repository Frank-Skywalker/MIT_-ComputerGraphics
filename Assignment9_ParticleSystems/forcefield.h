#ifndef _FORCEFIELD_H_
#define _FORCEFIELD_H_

#include "vectors.h"

#define G 9.8


class ForceField
{
public:
	virtual Vec3f getAcceleration(const Vec3f& position, float mass, float t) const = 0;

};


class GravityForceField : public ForceField
{
public:
	GravityForceField(Vec3f gravity):gravity(gravity)
	{

	}
	virtual Vec3f getAcceleration(const Vec3f& position, float mass, float t) const
	{
		return gravity;
	}
private:
	Vec3f gravity;
};



class ConstantForceField : public ForceField
{
public:

	ConstantForceField(Vec3f force):force(force)
	{

	}

	virtual Vec3f getAcceleration(const Vec3f& position, float mass, float t) const
	{
		return  (1.0f / mass)* force;
	}

private:
	Vec3f force;
};



class RadialForceField : public ForceField
{
public:


	RadialForceField(float magnitude):magnitude(magnitude)
	{
	}

	virtual Vec3f getAcceleration(const Vec3f& position, float mass, float t) const
	{
		Vec3f direction = -1*position;
		direction.Normalize();
		float distance = position.Length();
		return magnitude * distance * (1.0f / mass)*direction;
	}


private:
	float magnitude;

};


class VerticalForceField : public ForceField
{
public:

	VerticalForceField(float magnitude) :magnitude(magnitude)
	{

	}

	virtual Vec3f getAcceleration(const Vec3f& position, float mass, float t) const
	{
		Vec3f direction(0, -position.y(), 0);
		direction.Normalize();
		float distance = fabs(position.y());
		return magnitude * distance * (1.0f / mass) * direction;
	}

private:
	float magnitude;

};





//not implemented
class WindForceField :public ForceField
{
public:
	WindForceField(float magnitude) : magnitude(magnitude)
	{

	}


	virtual Vec3f getAcceleration(const Vec3f& position, float mass, float t) const
	{
		Vec3f direction(0, -position.y(), 0);
		direction.Normalize();
		float distance = fabs(position.y());
		return magnitude * distance * (1.0f / mass) * direction;
	}


private:
	float magnitude;
};


#endif