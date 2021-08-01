#ifndef _SAMPLER_H_
#define _SAMPLER_H_



class Sampler
{
public:
	Sampler(int n) :n(n) {}
	
	virtual Vec2f getSamplePosition(int n) = 0;


private:
	int n;
};


class RandomSampler :public Sampler
{



};


#endif