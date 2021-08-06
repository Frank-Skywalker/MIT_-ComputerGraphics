#include <assert.h>
#include <iostream>
#include<stdlib.h>
#include "matrix.h"
#include "image.h"
#include "ifs.h"

char* input_file = NULL; 
int num_points = 10000; 
int num_iters = 10; 
int imageSize = 100; 
char* output_file = NULL;


int main(int argc, char* argv[])
{
	// sample command line: // ifs -input fern.txt -points 10000 -iters 10 -size 100 -output fern.tga

	

	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-input")) 
		{
			i++; 
			assert(i < argc);
			input_file = argv[i]; 
		}
		else if (!strcmp(argv[i], "-points")) 
		{
			i++;
			assert(i < argc);
			num_points = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-iters"))
		{
			i++;
			assert(i < argc);
			num_iters = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-size")) 
		{
			i++; 
			assert(i < argc); 
			imageSize = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-output")) 
		{
			i++; 
			assert(i < argc); 
			output_file = argv[i];
		}
		else 
		{ 
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]); 
			assert(0); 
		}
	}

	// open the file
	FILE *input = fopen(input_file,"r");
	assert(input != NULL);

	// read the number of transforms 
	int num_transforms; 
	fscanf(input,"%d",&num_transforms);
	// < DO SOMETHING WITH num_transforms >
	assert(num_transforms > 0);
	IFS ifs(num_transforms,num_points,num_iters);

	// read in the transforms 
	for (int i = 0; i < num_transforms; i++) 
	{ 
		float probability;
		fscanf (input,"%f",&probability); 
		Matrix m;
		m.Read3x3(input); 
		// < DO SOMETHING WITH probability and m >
		ifs.SetTransform(i, probability, m);
	}

	// close the file 
	fclose(input);

	ifs.PerformIFS(output_file,imageSize);


}