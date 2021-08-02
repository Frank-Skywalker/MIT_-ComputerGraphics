#ifndef _FILTER_H_
#define _FILTER_H_

#include "vectors.h"
#include "film.h"
#include "sample.h"

#define NO_FILTER 0
#define BOX_FILTER 1
#define TENT_FILTER 2
#define GAUSSIAN_FILTER 3


class Filter
{

public:
	Filter(){}



	Vec3f getColor(int x, int y, Film* film)
	{
		int supportRadius=getSupportRadius();
		int numSamples = film->getNumSamples();
		int width = film->getWidth();
		int height = film->getHeight();
		float totalWeight = 0;
		Vec3f color;
		for (int i = x - supportRadius; i<= x + supportRadius; i++)
		{
			if (i < 0)
			{
				continue;
			}
			else if (i >= width)
			{
				break;
			}
			for (int j = y - supportRadius; j <= y + supportRadius; j++)
			{
				if (j < 0)
				{
					continue;
				}
				else if(j>=height)
				{
					break;
				}
				for (int k = 0; k < numSamples; k++)
				{
					Sample sample=film->getSample(i, j, k);
					Vec2f samplePosition(i+ sample.getPosition().x(),j+sample.getPosition().y());
					float xoffset = samplePosition.x() - x - 0.5;
					float yoffset = samplePosition.y() - y - 0.5;
					float weight = getWeight(xoffset, yoffset);
					color+=sample.getColor() * weight;
					totalWeight += weight;
				}
			}
		}
		
		return color*(1.0/totalWeight);
	}


	virtual float getWeight(float x, float y) = 0;
	virtual int getSupportRadius() = 0;






private:
};


class BoxFilter : public Filter
{
public:
	BoxFilter(float radius) :radius(radius)
	{

	}

	virtual float getWeight(float x, float y)
	{
		if (fabs(x) > radius || fabs(y) > radius)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	virtual int getSupportRadius()
	{
		return ceil(radius-0.5);
	}

private:
	float radius;
};

class TentFilter : public Filter
{

public:
	TentFilter(float radius) :radius(radius)
	{

	}
	virtual float getWeight(float x, float y)
	{
		Vec2f vec(fabs(x),fabs(y));
		return fmax(0 , 1.0 -vec.Length() / radius);
	}
	virtual int getSupportRadius()
	{
		return ceil(radius - 0.5);
	}


private:
	float radius;

};

class GaussianFilter :public Filter
{

public:
	GaussianFilter(float sigma) :sigma(sigma)
	{
	}
	virtual float getWeight(float x, float y)
	{
		Vec2f vec(fabs(x), fabs(y));
		float d = vec.Length();
		//if (d > 2 * sigma)
		//{
		//	return 0;
		//}

		return exp(-powf(d, 2) / (2 * powf(sigma, 2)));
	}
	virtual int getSupportRadius()
	{
		return ceil(2*sigma - 0.5);
	}

private:
	float sigma;
};






#endif