#ifndef _GENERATOR_H_
#define _GENERATOR_H_
#include "particle.h"

class Generator
{
public:
	// initialization
	virtual void SetColors(Vec3f color, Vec3f dead_color, float color_randomness)
	{

	}
	void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles)
	{

	}
	virtual void SetMass(float mass, float mass_randomness)
	{
		
	}

	// on each timestep, create some particles

	// (desired_num_particles), dt * desired_num_particles / lifespan particles should be created on each timestep.
	int numNewParticles(float current_time, float dt) const;
	Particle* Generate(float current_time, int i);

	// for the gui
	void Paint() const;
	void Restart();


protected:



};



class HoseGenerator : public Generator
{
public:
	HoseGenerator(Vec3f position, float position_randomness, Vec3f velocity, float velocity_randomness):
		position(position), positionRandomness(position_randomness),velocity(velocity), velocityRandomness(velocity_randomness)
	{

	}

private:
	Vec3f position;
	float positionRandomness;
	Vec3f velocity;
	float velocityRandomness
};


class RingGenerator : public Generator
{
public:
	RingGenerator(float position_randomness, Vec3f velocity, float velocity_randomness):
		positionRandomness(position_randomness),velocity(velocity),velocityRandomness(velocity_randomness)
	{

	}

private:
	float positionRandomness;
	Vec3f velocity;
	float velocityRandomness

};




#endif