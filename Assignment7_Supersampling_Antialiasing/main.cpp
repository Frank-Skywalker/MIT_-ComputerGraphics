#include<iostream>
#include<assert.h>
#include "raytracer.h"
#include"glCanvas.h"





using namespace std;


RayTracer* raytracer;
char* input_file = NULL;
int width = 100;
int height = 100;
char* output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char* depth_file = NULL;
char* normal_file = NULL;
bool shade_back = false;
bool previsualize = false;
bool gouraudShading = false;
int thetaSteps;
int phiSteps;

//Assignment4
bool shadeShadows=false;
int maxBounces;
float cutoffWeight;

//Assignment5
int nx=0;
int ny=0;
int nz=0;
bool renderGrid = false;
bool useGrid = false;

//Assignment6
bool printStatistics = false;

//Assignment7
//sample
int sampleMode = NO_SAMPLE;
int numSamples = 1;

bool renderSample = false;
char* sampleFile = NULL;
int sampleZoom =1;

//filter
int filterMode = NO_FILTER;
float filterRadius=0.5;

bool renderFilter = false;
char* filterFile = NULL;
int filterZoom = 1;


void shade(void)
{
	assert(raytracer != NULL);

	raytracer->RayCastSample(output_file);


	if (printStatistics)
	{
		RayTracingStats::PrintStatistics();
	}
	if (renderSample == true)
	{
		raytracer->RenderSample(sampleFile, sampleZoom);
	}
	if (renderFilter == true)
	{
		raytracer->RenderFilter(filterFile, filterZoom);
	}

}

void trace(float x,float y)
{
	Vec2f point(x, y);
	Ray ray=raytracer->getScene()->getCamera()->generateRay(point);
	Hit hit;
	raytracer->traceRay(ray, raytracer->getScene()->getCamera()->getTMin(), 0, 1, 1, hit);
	if (renderGrid)
	{
		raytracer->getGrid()->intersect(ray, hit, raytracer->getScene()->getCamera()->getTMin());
	}

}



int main(int argc, char* argv[])
{
	// ======================================================== // ======================================================== // Some sample code you might like to use for parsing // command line arguments

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
		//Assignment3
		else if (!strcmp(argv[i], "-gui"))
		{
			previsualize = true;
		}
		else if (!strcmp(argv[i], "-gouraud"))
		{
			gouraudShading = true;
		}
		else if (!strcmp(argv[i], "-tessellation"))
		{
			i++;
			assert(i < argc);
			thetaSteps = atoi(argv[i]);
			i++;
			assert(i < argc);
			phiSteps = atoi(argv[i]);
		}
		//Assignment4
		else if (!strcmp(argv[i], "-shadows"))
		{
			shadeShadows = true;
		}
		else if (!strcmp(argv[i], "-bounces"))
		{
			i++;
			assert(i < argc);
			maxBounces = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-weight"))
		{
			i++;
			assert(i < argc);
			cutoffWeight = atof(argv[i]);
		}
		//Assignment5
		else if (!strcmp(argv[i], "-grid"))
		{	
			useGrid = true;
			i++;
			assert(i < argc);
			nx = atoi(argv[i]);
			i++;
			assert(i < argc);
			ny = atoi(argv[i]);
			i++;
			assert(i < argc);
			nz = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-visualize_grid"))
		{
			renderGrid = true;
		}
		//Assignment6
		else if (!strcmp(argv[i], "-stats"))
		{
			printStatistics = true;
		}
		//Assignment7

		//sample modes
		else if (!strcmp(argv[i], "-random_samples"))
		{
			sampleMode = RANDOM_SAMPLE;
			i++;
			assert(i < argc);
			numSamples = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-uniform_samples"))
		{
			sampleMode = UNIFORM_SAMPLE;
			i++;
			assert(i < argc);
			numSamples = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-jittered_samples"))
		{
			sampleMode = JITTERED_SAMPLE;
			i++;
			assert(i < argc);
			numSamples = atoi(argv[i]);
		}
		//should render samples
		else if (!strcmp(argv[i], "-render_samples"))
		{
			renderSample = true;
			i++;
			assert(i < argc);
			sampleFile = argv[i];
			i++;
			assert(i < argc);
			sampleZoom = atoi(argv[i]);
		}
		//filter modes
		else if (!strcmp(argv[i], "-box_filter"))
		{
			filterMode = BOX_FILTER;
			i++;
			assert(i < argc);
			filterRadius = atof(argv[i]);
		}
		else if (!strcmp(argv[i], "-tent_filter"))
		{
			filterMode = TENT_FILTER;
			i++;
			assert(i < argc);
			filterRadius = atof(argv[i]);
		}
		else if (!strcmp(argv[i], "-gaussian_filter"))
		{
			filterMode = GAUSSIAN_FILTER;
			i++;
			assert(i < argc);
			filterRadius = atof(argv[i]);
		}
		//should render filters
		else if (!strcmp(argv[i], "-render_filter"))
		{
			renderFilter = true;
			i++;
			assert(i < argc);
			filterFile = argv[i];
			i++;
			assert(i < argc);
			filterZoom = atoi(argv[i]);
		}
		else
		{
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}





	// ======================================================== // ========================================================
	raytracer = new RayTracer(input_file, width, height, maxBounces, cutoffWeight, shadeShadows, shade_back,useGrid,nx,ny,nz,renderGrid,sampleMode,numSamples,filterMode,filterRadius);
	//previsualize this scene with OpenGL
	if (previsualize)
	{
		GLCanvas glCanvas;
		glCanvas.initialize(raytracer->getScene(), shade, trace, raytracer->getGrid(), renderGrid);
	}
	else
	{
		shade();
	}
	
	return 0;
}

