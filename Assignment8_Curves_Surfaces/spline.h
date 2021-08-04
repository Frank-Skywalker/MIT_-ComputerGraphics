#ifndef _SPLINE_H_
#define _SPLINE_H_

#include <gl/glew.h>
#include <gl/GL.h>
#include<gl/GLU.h>
#include <gl/glut.h>
#include <vector>

#include "triangle_mesh.h"
#include "matrix.h"
#include "arg_parser.h"

#define PI acos(-1)

class Spline
{
public:

	Spline()
	{
		B_Bezier = getB_Bezier();
		B_BSpline = getB_BSpline();
		B_BezierToB_BSpline = getB_BesierToB_BSpline();
		B_BSplineToB_Bezier = getB_BSplinerToB_Bezier();
	}



	// FOR VISUALIZATION
	virtual void Paint(ArgParser* args)
	{

	}

	// FOR CONVERTING BETWEEN SPLINE TYPES
	virtual void OutputBezier(FILE* file)
	{

	}
	virtual void OutputBSpline(FILE* file)
	{

	}

	// FOR CONTROL POINT PICKING
	virtual int getNumVertices()
	{
		return controlPoints.size();
	}
	virtual Vec3f getVertex(int i)
	{
		return controlPoints[i];
	}

	// FOR EDITING OPERATIONS
	virtual void moveControlPoint(int selectedPoint, float x, float y)
	{
		controlPoints[selectedPoint].Set(0, x);
		controlPoints[selectedPoint].Set(1, y);
	}


	virtual void set(int index, Vec3f point)
	{
		controlPoints.insert(controlPoints.begin() + index, point);
		updateNumSplines();

	}

	virtual void addControlPoint(int selectedPoint, float x, float y)
	{
		controlPoints.insert(controlPoints.begin() + selectedPoint, Vec3f(x, y, 0));
		updateNumSplines();

	}

	virtual void deleteControlPoint(int selectedPoint)
	{
		controlPoints.erase(controlPoints.begin() + selectedPoint);
		updateNumSplines();
	}

	// FOR GENERATING TRIANGLES
	virtual TriangleMesh* OutputTriangles(ArgParser* args)
	{
		return NULL;
	}


	virtual Vec3f Q(float t)
	{
		return Vec3f(1, 1, 1);
	}


protected:
	Matrix B_Bezier;
	Matrix B_BSpline;
	Matrix B;
	Matrix B_BezierToB_BSpline;
	Matrix B_BSplineToB_Bezier;
	vector<Vec3f> controlPoints;
	int numSplines;







	Vec4f getT(float t)
	{
		//assert(t >= 0 && t <= 1);
		float power = 1;
		Vec4f T;
		for (int i = 0; i < 4; i++)
		{
			T.Set(i, power);
			power *= t;
		}
		return T;
	}


private:
	virtual void updateNumSplines()
	{

	}


	//static Matrix getB_Bezier(void)
	//{
	//	Matrix B_Bezier;
	//	B_Bezier.Set(0, 0, -1);
	//	B_Bezier.Set(0, 1, 3);
	//	B_Bezier.Set(0, 2, -3);
	//	B_Bezier.Set(0, 3, 1);

	//	B_Bezier.Set(1, 0, 3);
	//	B_Bezier.Set(1, 1, -6);
	//	B_Bezier.Set(1, 2, 3);
	//	B_Bezier.Set(1, 3, 0);

	//	B_Bezier.Set(2, 0, -3);
	//	B_Bezier.Set(2, 1, 3);
	//	B_Bezier.Set(2, 2, 0);
	//	B_Bezier.Set(2, 3, 0);

	//	B_Bezier.Set(3, 0, 1);
	//	B_Bezier.Set(3, 1, 0);
	//	B_Bezier.Set(3, 2, 0);
	//	B_Bezier.Set(3, 3, 0);
	//	return B_Bezier;
	//}

	//static Matrix getB_BSpline(void)
	//{
	//	Matrix B_BSpline;
	//	B_BSpline.Set(0, 0, -1);
	//	B_BSpline.Set(0, 1, 3);
	//	B_BSpline.Set(0, 2, -3);
	//	B_BSpline.Set(0, 3, 1);

	//	B_BSpline.Set(1, 0, 3);
	//	B_BSpline.Set(1, 1, -6);
	//	B_BSpline.Set(1, 2, 0);
	//	B_BSpline.Set(1, 3, 4);

	//	B_BSpline.Set(2, 0, -3);
	//	B_BSpline.Set(2, 1, 3);
	//	B_BSpline.Set(2, 2, 3);
	//	B_BSpline.Set(2, 3, 1);

	//	B_BSpline.Set(3, 0, 1);
	//	B_BSpline.Set(3, 1, 0);
	//	B_BSpline.Set(3, 2, 0);
	//	B_BSpline.Set(3, 3, 0);

	//	B_BSpline *= 1 / 6;
	//	return B_BSpline;
	//}




	static Matrix getB_Bezier(void)
	{
		Matrix B_Bezier;
		B_Bezier.Set(0, 0, 1);
		B_Bezier.Set(1, 0, -3);
		B_Bezier.Set(2, 0, 3);
		B_Bezier.Set(3, 0, -1);

		B_Bezier.Set(0, 1, 0);
		B_Bezier.Set(1, 1, 3);
		B_Bezier.Set(2, 1, -6);
		B_Bezier.Set(3, 1, 3);

		B_Bezier.Set(0, 2, 0);
		B_Bezier.Set(1, 2, 0);
		B_Bezier.Set(2, 2, 3);
		B_Bezier.Set(3, 2, -3);

		B_Bezier.Set(0, 3, 0);
		B_Bezier.Set(1, 3, 0);
		B_Bezier.Set(2, 3, 0);
		B_Bezier.Set(3, 3, 1);
		return B_Bezier;
	}

	static Matrix getB_BSpline(void)
	{
		Matrix B_BSpline;
		B_BSpline.Set(0, 0, 1);
		B_BSpline.Set(1, 0, -3);
		B_BSpline.Set(2, 0, 3);
		B_BSpline.Set(3, 0, -1);

		B_BSpline.Set(0, 1, 4);
		B_BSpline.Set(1, 1, 0);
		B_BSpline.Set(2, 1, -6);
		B_BSpline.Set(3, 1, 3);

		B_BSpline.Set(0, 2, 1);
		B_BSpline.Set(1, 2, 3);
		B_BSpline.Set(2, 2, 3);
		B_BSpline.Set(3, 2, -3);

		B_BSpline.Set(0, 3, 0);
		B_BSpline.Set(1, 3, 0);
		B_BSpline.Set(2, 3, 0);
		B_BSpline.Set(3, 3, 1);
		//cout << "B_BSpline: " << endl;
		B_BSpline *= 1.0 / 6.0;
		return B_BSpline;
	}





	//get the transform matrix from the spline basis of Besier to the spline basis of BSpline
	//B_BSpline=Transform * B_Besier
	static Matrix getB_BesierToB_BSpline(void)
	{
		Matrix B_Bezier = getB_Bezier();
		Matrix B_BSpline = getB_BSpline();
		B_Bezier.Inverse();
		return B_BSpline * B_Bezier;
	}



	//get the transform matrix from the spline basis of BSpline to the spline basis of Bezier
	//B_Bezier=Transform * B_BSpline
	static Matrix getB_BSplinerToB_Bezier(void)
	{
		Matrix B_Bezier = getB_Bezier();
		Matrix B_BSpline = getB_BSpline();
		B_BSpline.Inverse();
		return B_Bezier*B_BSpline;
	}


	
};





class Curve : public Spline
{
public:
	Curve()
	{

	}
	virtual void Paint(ArgParser* args)
	{

		if (numSplines <= 0)
		{
			return;
		}

		glLineWidth(3);
		glColor3f(0, 0, 1);
		glBegin(GL_LINE_STRIP);
		for (auto i = controlPoints.begin(); i != controlPoints.end(); i++)
		{
			glVertex3f(i->x(), i->y(), i->z());
		}
		glEnd();

		glLineWidth(3);
		glColor3f(0, 1, 0);
		glBegin(GL_LINE_STRIP);
		float step= 1.0 / (float)args->curve_tessellation;
		float t = 0;
		
		for (int i = 0; i < numSplines; i++)
		{
			t = i;
			for (int j = 0; j <= args->curve_tessellation; j++)
			{
				Vec3f vertex = Q(t);
				glVertex3f(vertex[0], vertex[1], vertex[2]);
				t += step;
			}
		}
		glEnd();

		//for (int i = 0; i <= args->curve_tessellation; i++)
		//{
		//	Vec3f vertex = Q(t);
		//	glVertex3f(vertex[0], vertex[1], vertex[2]);
		//	t += step;
		//}


		//control points
		glPointSize(5);
		glColor3f(1, 0, 0);
		glBegin(GL_POINTS);
		for (auto i = controlPoints.begin(); i != controlPoints.end(); i++)
		{
			glVertex3f(i->x(), i->y(), i->z());
		}
		glEnd();
	}



	// FOR GENERATING TRIANGLES
	virtual TriangleMesh* OutputTriangles(ArgParser* args)
	{
		TriangleNet* triangleNet=new TriangleNet(args->revolution_tessellation,args->curve_tessellation*numSplines);


		float uStep = 2.0f * PI / (float) args->revolution_tessellation;
		float vStep = 1.0f / (float)args->curve_tessellation;
		float phi = 0;

		for (int k = 0; k <= args->revolution_tessellation; k++)
		{
			float cosPhi = cosf(phi);
			float sinPhi = sinf(phi);
			for (int i = 0; i < numSplines; i++)
			{
				float t = i;
				for (int j = 0; j <= args->curve_tessellation; j++)
				{
					Vec3f vertex = Q(t);
					float length = sqrtf(powf(vertex.x(), 2) + powf(vertex.z(), 2));
					triangleNet->SetVertex( k , i * args->curve_tessellation + j, Vec3f(length*cosPhi,vertex.y(), length *sinPhi));
					t += vStep;
				}
			}

			phi += uStep;
		}
		//FILE* file = fopen(args->output_file,"w");
		//triangleNet.Output(file);
		//fclose(file);
		return triangleNet;

	}

private:


};


class BezierCurve : public Curve
{
public:
	BezierCurve(int num_vertices)
	{
		controlPoints.reserve(num_vertices);
		B = B_Bezier;
	}



	//G_Bezier = G_BSpline * B_BezierToB_BSpline
	virtual void OutputBezier(FILE* file)
	{
		fprintf(file, "bezier ");
		fprintf(file, "num_vertices %d",controlPoints.size());
		for (int i=0;i<controlPoints.size(); i++)
		{
			fprintf(file, " %f %f %f",controlPoints[i][0], controlPoints[i][1], controlPoints[i][2]);
		}

		fprintf(file, "\n");

	}


	
	//G_BSpline = G_Bezier * B_BSplineToB_Bezier
	virtual void OutputBSpline(FILE* file)
	{
		Matrix G_Bezier;
		G_Bezier.Clear();
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				G_Bezier.Set(j, i, controlPoints[j][i]);
			}
		}

		Matrix G_BSpline = G_Bezier * B_BSplineToB_Bezier;
		fprintf(file, "bspline ");
		fprintf(file, "num_vertices %d", 4);
		for (int i = 0; i < 4; i++)
		{
			fprintf(file, " %f %f %f", G_BSpline.Get(i,0), G_BSpline.Get(i, 1), G_BSpline.Get(i, 2));
		}
		fprintf(file, "\n");



		//Matrix B_BSpline_Inverse = B_BSpline;
		//B_BSpline_Inverse.Inverse();
		//Matrix transMatrix = B_Bezier * B_BSpline_Inverse;

		//fprintf(file, "bspline ");
		//fprintf(file, "num_vertices %d", controlPoints.size());
		//for (int i = 0; i < controlPoints.size(); i++)
		//{
		//	Vec3f point = controlPoints[i];
		//	transMatrix.Transform(point);
		//	fprintf(file, " %f %f %f", point[0], point[1], point[2]);
		//}
		//fprintf(file, "\n");


	}




	virtual Vec3f Q(float t)
	{
		assert(numSplines > 0);
		//assert(t >= 0 && t <= 1);

		int splineIndex;

		splineIndex = floor(t);

		if (splineIndex >= numSplines)
		{
			splineIndex = numSplines - 1;
		}
		else if (splineIndex < 0)
		{
			splineIndex = 0;
		}

		t = t - splineIndex;
		//cout << "numSplines: " << numSplines << endl;
		//cout << "t: " << t << endl;
		//cout << "splineIndex: " << splineIndex << endl;
		Vec3f localControlPoints[4];
		//cout << "controlPoints.size(): " << controlPoints.size() << endl;
		//cout << "splineIndex: " << splineIndex << endl;
		assert(controlPoints.size() > 3 * splineIndex);
		for (int i = 0; i < 4; i++)
		{
			if (controlPoints.size() > 3 * splineIndex + i)
			{
				localControlPoints[i] = controlPoints[3 * splineIndex + i];
				//cout << "controlPoints: " << localControlPoints[i]<<endl;
			}
			else
			{
				localControlPoints[i] = localControlPoints[i-1];
				//cout << "false: " << localControlPoints[i] << endl;

			}
		}

		Vec3f Q;
		Vec4f T = getT(t);
		//cout << "T: " << T << endl;
		//cout << "B: " << B<<endl;
		B.Transform(T);
		//cout << "B*T: " << T << endl;


		Matrix G;
		G.Clear();
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				G.Set(j, i, localControlPoints[j][i]);
			}
		}

		G.Transform(T);

		Q.Set(T[0], T[1], T[2]);

		return Q;
	}


private:

	virtual void updateNumSplines()
	{
		numSplines = max(0, (int)(controlPoints.size() - 1)) / 3;
		if (controlPoints.size() == 1)
		{
			numSplines = 1;
		}
		else if (controlPoints.size() > 0 && ((controlPoints.size() - 1) % 3) > 0)
		{
			numSplines++;
		}
	}
};


class BSplineCurve : public Curve
{
public:
	BSplineCurve(int num_vertices)
	{
		controlPoints.reserve(num_vertices);
		B = B_BSpline;

	}



	//G_Bezier = G_BSpline * B_BezierToB_BSpline
	virtual void OutputBezier(FILE* file)
	{
		Matrix G_BSpline;
		G_BSpline.Clear();
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				G_BSpline.Set(j, i, controlPoints[j][i]);
			}
		}
		Matrix G_Bezier = G_BSpline * B_BezierToB_BSpline;

		fprintf(file, "bezier ");
		fprintf(file, "num_vertices %d", 4);
		for (int i = 0; i < 4; i++)
		{
			fprintf(file, " %f %f %f", G_Bezier.Get(i, 0), G_Bezier.Get(i, 1), G_Bezier.Get(i, 2));
		}
		fprintf(file, "\n");

	}



	//G_BSpline = G_Bezier * B_BSplineToB_Bezier
	virtual void OutputBSpline(FILE* file)
	{

		fprintf(file, "bspline ");
		fprintf(file, "num_vertices %d", controlPoints.size());
		for (int i = 0; i < controlPoints.size(); i++)
		{
			fprintf(file, " %f %f %f", controlPoints[i][0], controlPoints[i][1], controlPoints[i][2]);
		}

	}




	virtual Vec3f Q(float t)
	{
		//assert(t >= 0 && t <= 1);
		assert(numSplines > 0);

		int splineIndex;

		splineIndex = floor(t);
		//cout << "numSpliens: " << numSplines<<endl;
		//cout <<"splineIndex: "<< splineIndex << endl;
		if (splineIndex >= numSplines)
		{
			splineIndex = numSplines - 1;
		}
		else if (splineIndex < 0)
		{
			splineIndex = 0;
		}

		t = t - splineIndex;

		Vec3f localControlPoints[4];
		assert(controlPoints.size() > 0);
		for (int i = 0; i < 4; i++)
		{

			if(controlPoints.size() > splineIndex + i)
			{
				localControlPoints[i] = controlPoints[splineIndex + i];
				//cout <<"localControlPoints[i]: " <<localControlPoints[i] << endl;
			}
			else
			{
				localControlPoints[i] = localControlPoints[i - 1];
			}
		}

		Vec3f Q;
		Vec4f T = getT(t);
		//cout << "B: " << B << endl;
		B.Transform(T);


		Matrix G;
		G.Clear();
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				G.Set(j, i, localControlPoints[j][i]);
			}
		}

		G.Transform(T);

		Q.Set(T[0], T[1], T[2]);

		return Q;
	}


private:
	virtual void updateNumSplines()
	{
		numSplines = max(0, (int)(controlPoints.size() - 3));
		if (controlPoints.size() > 0 && controlPoints.size() <= 3 )
		{
			numSplines = 1;
		}
	}


};


class Surface : public Spline
{
public:

};


class SurfaceOfRevolution : public Surface
{
public:
	SurfaceOfRevolution(Curve *c):curve(c)
	{
	}

	virtual void Paint(ArgParser* args)
	{
		curve->Paint(args);
	}

	// FOR GENERATING TRIANGLES
	virtual TriangleMesh* OutputTriangles(ArgParser* args)
	{
		return curve->OutputTriangles(args);
	}


	virtual void addControlPoint(int selectedPoint, float x, float y)
	{
		curve->addControlPoint(selectedPoint, x, y);
	}

	virtual void deleteControlPoint(int selectedPoint)
	{
		curve->deleteControlPoint(selectedPoint);
	}


	virtual void moveControlPoint(int selectedPoint, float x, float y)
	{
		curve->moveControlPoint(selectedPoint, x, y);
	}



	// FOR CONVERTING BETWEEN SPLINE TYPES
	virtual void OutputBezier(FILE* file)
	{
		curve->OutputBezier(file);
	}
	virtual void OutputBSpline(FILE* file)
	{
		curve->OutputBSpline(file);
	}

	// FOR CONTROL POINT PICKING
	virtual int getNumVertices()
	{
		return curve->getNumVertices();
	}
	virtual Vec3f getVertex(int i)
	{
		return curve->getVertex(i);
	}



	virtual void set(int index, Vec3f point)
	{
		curve->set(index, point);
	}


private:
	Curve* curve;
};

class BezierPatch : public Surface
{
public:
	BezierPatch()
	{
		controlPoints.reserve(16);

		for (int i = 0; i < 4; i++)
		{
			uBezierCurves[i] = new BezierCurve(4);
			vBezierCurves[i] = new BezierCurve(4);
		}

	}


	virtual void set(int index, Vec3f point)
	{
		controlPoints.insert(controlPoints.begin() + index, point);
		if (controlPoints.size() == 16)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					uBezierCurves[i]->set(j, controlPoints[i * 4 + j]);
					vBezierCurves[i]->set(j, controlPoints[j * 4 + i]);
				}
			}
		}
	}

	virtual void Paint(ArgParser* args)
	{

		for (int i = 0; i < 4; i++)
		{
			uBezierCurves[i]->Paint(args);
			vBezierCurves[i]->Paint(args);
		}
	}


	virtual TriangleMesh* OutputTriangles(ArgParser* args)
	{
		float step = 1.0 / (float)args->patch_tessellation;
		TriangleNet* triangleNet = new TriangleNet(args->patch_tessellation, args->patch_tessellation);
		
		float u = 0;
		for (int i = 0; i <= args->patch_tessellation; i++)
		{
			BezierCurve newCurve(4);
			for (int k = 0; k < 4; k++)
			{
				newCurve.set(k, uBezierCurves[k]->Q(u));
			}

			float v = 0;
			for (int j = 0; j <= args->patch_tessellation; j++)
			{
				Vec3f vertex=newCurve.Q(v);
				triangleNet->SetVertex(i, j, vertex);
				v += step;
			}
			u += step;
		}

		return triangleNet;
	}




private:
	BezierCurve* uBezierCurves[4];
	BezierCurve* vBezierCurves[4];


};

#endif