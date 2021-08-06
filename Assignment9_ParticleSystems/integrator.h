#ifndef _INTEGRATOR_H_
#define _INTEGRATOR_H_

#include "particle.h"
#include "forcefield.h"


class Integrator
{
public:
	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt) = 0;
	virtual Vec3f getColor() = 0;

};



//pn+1 = pn + vn * dt
//vn + 1 = vn + a(pn, t) * dt
class EulerIntegrator : public Integrator
{
public:
	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt)
	{
		particle->increaseAge(dt);
		Vec3f velocityN = particle->getVelocity();
		Vec3f positionN = particle->getPosition();
		float mass = particle->getMass();

		Vec3f newPosition = positionN + velocityN * dt;
		Vec3f newVelocity = velocityN + forcefield->getAcceleration(positionN, mass, t)*dt;

		particle->setPosition(newPosition);
		particle->setVelocity(newVelocity);


	}

	virtual Vec3f getColor() 
	{
		return Vec3f(1, 0, 0);
	}

};



//pm = pn + vn * dt / 2
//vm = vn + a(pn, t) * dt / 2
//
//pn + 1 = pn + vm * dt
//vn + 1 = vn + a(pm, t + dt / 2) * dt
class MidpointIntegrator : public Integrator
{
public:
	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt)
	{
		particle->increaseAge(dt);

		Vec3f velocityN = particle->getVelocity();
		Vec3f positionN = particle->getPosition();
		float mass = particle->getMass();

		Vec3f positionM = positionN + velocityN * (dt / 2.0f);
		Vec3f velocityM = velocityN + forcefield->getAcceleration(positionN, mass, t) * (dt / 2.0f);

		Vec3f newPosition = positionN + velocityM * dt;
		Vec3f newVelocity = velocityN + forcefield->getAcceleration(positionM, mass, t + dt/2.0f ) * dt;

		particle->setPosition(newPosition);
		particle->setVelocity(newVelocity);
	}


	virtual Vec3f getColor()
	{
		return Vec3f(0, 1, 0);
	}

};



//not implemented
class RungeKuttaIntegrator :public Integrator
{
	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt)
	{
		particle->increaseAge(dt);

		Vec3f velocityN = particle->getVelocity();
		Vec3f positionN = particle->getPosition();
		float mass = particle->getMass();

		Vec3f positionM = positionN + velocityN * (dt / 2.0f);
		Vec3f velocityM = velocityN + forcefield->getAcceleration(positionN, mass, t) * (dt / 2.0f);

		Vec3f newPosition = positionN + velocityM * dt;
		Vec3f newVelocity = velocityN + forcefield->getAcceleration(positionM, mass, t + dt / 2.0f) * dt;

		particle->setPosition(newPosition);
		particle->setVelocity(newVelocity);
	}


	virtual Vec3f getColor()
	{
		return Vec3f(0, 0, 1);
	}
};



#endif