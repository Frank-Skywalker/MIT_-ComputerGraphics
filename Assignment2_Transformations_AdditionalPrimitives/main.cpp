#include<iostream>
#include<assert.h>
#include "raytracer.h"
using namespace std;

int main(int argc, char* argv[])
{
	// ======================================================== // ======================================================== // Some sample code you might like to use for parsing // command line arguments

	char* input_file = NULL;
	int width = 100;
	int height = 100;
	char* output_file = NULL;
	float depth_min = 0;
	float depth_max = 1;
	char* depth_file = NULL;
	char* normal_file = NULL;
	bool shade_back = false;



	// sample command line: // raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga

	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-input"))
		{
			i++;
			assert(i < argc);
			input_file = argv[i];
		}
		else if (!strcmp(argv[i], "-size"))
		{
			i++;
			assert(i < argc);
			width = atoi(argv[i]);
			i++;
			assert(i < argc);
			height = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-output"))
		{
			i++; 
			assert(i < argc);
			output_file = argv[i];
		}
		else if (!strcmp(argv[i], "-depth"))
		{
			i++;
			assert(i < argc);
			depth_min = atof(argv[i]);
			i++;
			assert(i < argc);
			depth_max = atof(argv[i]);
			i++;
			assert(i < argc);
			depth_file = argv[i];
		}
		else if (!strcmp(argv[i], "-normals"))
		{
			i++;
			assert(i < argc);
			normal_file = argv[i];
		}
		else if (!strcmp(argv[i], "-shade_back"))
		{
			assert(i < argc);
			shade_back = true;
		}
		else
		{
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}

	// ======================================================== // ========================================================
	
	Raytracer raytracer(input_file,width,height,depth_min,depth_max);
	raytracer.doRaytrace();
	if (output_file != NULL)
	{
		raytracer.diffuseShader(output_file,shade_back);
	}

	if (depth_file != NULL)
	{
		raytracer.depthShader(depth_file);
	}

	if (normal_file != NULL)
	{
		raytracer.normalShader(normal_file);
	}



   
	
	return 0;
}