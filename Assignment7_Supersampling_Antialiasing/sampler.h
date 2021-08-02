#ifndef _SAMPLER_H_
#define _SAMPLER_H_
#include "vectors.h"

//sample modes
#define NO_SAMPLE 0
#define UNIFORM_SAMPLE 1
#define RANDOM_SAMPLE 2
#define JITTERED_SAMPLE 3

#define MAX_OFFSET 1
#define MIN_OFFSET 0

class Sampler
{
public:
	Sampler(int n) :n(n) {}
	
	virtual Vec2f getSamplePosition(int i) = 0;


protected:
	int n;
};


class RandomSampler :public Sampler
{
public:
	RandomSampler(int i):Sampler(n){}
	virtual Vec2f getSamplePosition(int i)
	{
		return Vec2f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
	}


};




class UniformSampler : public Sampler
{
public:

	UniformSampler(int n) :Sampler(n) 
	{
		axisN = sqrtf(n);
		step = 1.0 / (float)axisN;
		offset = step / 2;
	}
	virtual Vec2f getSamplePosition(int i)
	{
		assert(i < n);
		int x = i % axisN;
		int y = i / axisN;
		return Vec2f(offset + x * step, offset + y * step);
	}


private:
	int axisN;
	float step;
	float offset;
};



class JitteredSampler: public Sampler
{

public:

	JitteredSampler(int n) :Sampler(n) 
	{
		axisN = sqrtf(n);
		step = 1.0 / (float)axisN;
		offset = step / 2;
	}
	virtual Vec2f getSamplePosition(int i)
	{
		assert(i < n);
		int x = i % axisN;
		int y = i / axisN;
		Vec2f center(x * step, y * step);
		Vec2f random((float)rand() / (float)RAND_MAX * step, (float)rand() / (float)RAND_MAX * step);
		return Vec2f(center.x()+random.x(),center.y()+random.y());
	}

private:
	int axisN;
	float step;
	float offset;
};






#endif