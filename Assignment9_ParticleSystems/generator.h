#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include "particle.h"
#include "particleSet.h"
#include "random.h"


#include <gl/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>



class Generator
{
public:


	~Generator()
	{
		delete(random);
	}

	// initialization
	void SetColors(Vec3f color, Vec3f dead_color, float color_randomness)
	{
		this->color = color;
		this->deadColor = dead_color;
		this->colorRandomness = color_randomness;
	}
	void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles)
	{
		this->lifespan = lifespan;
		this->lifespanRandomness = lifespan_randomness;
		this->desiredNumParticles = desired_num_particles;
	}
	void SetMass(float mass, float mass_randomness)
	{
		this->mass = mass;
		this->massRandomness = mass_randomness;
	}

	// on each timestep, create some particles
	// (desired_num_particles), dt * desired_num_particles / lifespan particles should be created on each timestep.
	virtual int numNewParticles(float current_time, float dt) const
	{
		return ceilf(dt * (float)desiredNumParticles / lifespan);
	}
	virtual Particle* Generate(float current_time, int i) = 0;

	// for the gui
	virtual void Paint() const
	{
	}

	void Restart()
	{
		delete random;
		random = new Random;
	}


protected:
	Vec3f color;
	Vec3f deadColor;
	float colorRandomness;
	float lifespan;
	float lifespanRandomness;
	int desiredNumParticles;
	float mass;
	float massRandomness;
	Random* random;

};



class HoseGenerator : public Generator
{
public:
	HoseGenerator(Vec3f position, float position_randomness, Vec3f velocity, float velocity_randomness):
		position(position), positionRandomness(position_randomness),velocity(velocity), velocityRandomness(velocity_randomness)
	{

	}

	virtual Particle* Generate(float current_time, int i)
	{
		Vec3f pPosition = position + positionRandomness * random->randomVector();
		Vec3f pVelocity = velocity + velocityRandomness * random->randomVector();
		Vec3f pColor = color + colorRandomness * random->randomVector();
		Vec3f pDeadColor = deadColor + colorRandomness * random->randomVector();
		float pMass = mass + massRandomness * random->next();
		float pLifespan = lifespan + lifespanRandomness * random->next();

		return new Particle(pPosition, pVelocity, pColor, pDeadColor, pMass, pLifespan);
	}
	

private:
	Vec3f position;
	float positionRandomness;
	Vec3f velocity;
	float velocityRandomness;

};


class RingGenerator : public Generator
{
public:
	RingGenerator(float position_randomness, Vec3f velocity, float velocity_randomness):
		positionRandomness(position_randomness),velocity(velocity),velocityRandomness(velocity_randomness)
	{
	}


	virtual Particle* Generate(float current_time, int i)
	{
		float radius = current_time;
		Vec3f pPosition(radius*sinf(i)+positionRandomness*random->next(),0,radius*cosf(i)+positionRandomness*random->next());
		Vec3f pVelocity = velocity + velocityRandomness * random->randomVector();
		Vec3f pColor = color + colorRandomness * random->randomVector();
		Vec3f pDeadColor = deadColor + colorRandomness * random->randomVector();
		float pMass = mass + massRandomness * random->next();
		float pLifespan = lifespan + lifespanRandomness * random->next();

		return new Particle(pPosition, pVelocity, pColor, pDeadColor, pMass, pLifespan);
	}


	virtual int numNewParticles(float current_time, float dt) const
	{
		return ceilf(current_time*(dt * (float)desiredNumParticles / lifespan));
	}




	virtual void Paint() const
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
		glVertex3f(10, 0, 10);
		glVertex3f(10, 0, -10);
		glVertex3f(-10, 0, -10);
		glVertex3f(10, 0, -10);
		glEnd();
	}

private:
	float positionRandomness;
	Vec3f velocity;
	float velocityRandomness;
};




#endif