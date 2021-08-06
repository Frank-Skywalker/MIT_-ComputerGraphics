#ifndef _IFS_H_
#define _IFS_H_

#include "matrix.h"
#include "stdlib.h"

class IFS
{
public:
	IFS(int numTransforms, int numPoints, int numIters):numTransforms(numTransforms),numPoints(numPoints),numIters(numIters)
	{
		transforms = new Matrix[numTransforms];
		probabilities = new float[numTransforms];
	}


	void SetTransform(int index, float probability, Matrix transform)
	{
		probabilities[index] = probability;
		transforms[index] = transform;
	}


	void PerformIFS(char* outputFile, int imageSize)
	{

		srand((unsigned int)(time(NULL)));
		Image outputImage(imageSize, imageSize);
		outputImage.SetAllPixels(Vec3f(1, 1, 1));

		for (int i = 0; i < numPoints; i++)
		{
			Vec2f point((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
			for (int k = 0; k < numIters; k++)
			{
				Matrix transform;
				float temp = (float)rand() / (float)RAND_MAX;
				float total=0;
				for (int j = 0; j < numTransforms; j++)
				{
					total += probabilities[j];
					if (total >= temp)
					{
						transform = transforms[j];
						break;
					}
				}

				transform.Transform(point);

			}
			
			point.Set(floorf(point[0] * imageSize), floorf(point[1] * imageSize));
			//if (point[0] < 0)
			//{
			//	point.Set(0, point[1]);
			//}
			//if (point[1] < 0)
			//{
			//	point.Set(point[0], 0);
			//}
			//if (point[0] >= imageSize)
			//{
			//	point.Set(imageSize - 1, point[1]);
			//}
			//if (point[1] >= imageSize)
			//{
			//	point.Set(point[0], imageSize-1);
			//}


			outputImage.SetPixel(point[0], point[1], Vec3f(0, 0, 0));
		}


		if (outputFile != NULL)
			outputImage.SaveTGA(outputFile);
	}


private:
	int numTransforms;
	Matrix *transforms;
	float *probabilities;
	int numPoints;
	int numIters;

};



#endif