#ifndef _SPLINE_H_
#define _SPLINE_H_

#include <gl/glew.h>
#include <gl/GL.h>
#include<gl/GLU.h>
#include <gl/glut.h>
#include <vector>
#include "triangle_mesh.h"
#include "matrix.h"

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
	virtual void Paint(ArgParser* args)=0;

	// FOR CONVERTING BETWEEN SPLINE TYPES
	virtual void OutputBezier(FILE* file);
	virtual void OutputBSpline(FILE* file);

	// FOR CONTROL POINT PICKING
	virtual int getNumVertices();
	virtual Vec3f getVertex(int i);

	// FOR EDITING OPERATIONS
	virtual void moveControlPoint(int selectedPoint, float x, float y);
	virtual void addControlPoint(int selectedPoint, float x, float y);
	virtual void deleteControlPoint(int selectedPoint);

	// FOR GENERATING TRIANGLES
	virtual TriangleMesh* OutputTriangles(ArgParser* args);



protected:
	Matrix B_Bezier;
	Matrix B_BSpline;
	Matrix B;
	Matrix B_BezierToB_BSpline;
	Matrix B_BSplineToB_Bezier;
	vector<Vec3f> controlPoints;

	Vec3f Q(float t)
	{
		//assert(t >= 0 && t <= 1);
		Vec3f Q;
		Vec4f T = getT(t);
		B.Transform(T);

		Matrix G;
		G.Clear();
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				G.Set(i, j, controlPoints[j][i]);
			}
		}

		G.Transform(T);

		Q.Set(T[0], T[1], T[2]);
	}

	Vec4f getT(float t)
	{
		//assert(t >= 0 && t <= 1);
		float power = 1;
		Vec4f T;
		for (int i = 0; i < 4; i++)
		{
			T.Set(3 - i, power);
			power *= t;
		}
		return T;
	}


	static Matrix getB_Bezier(void)
	{
		Matrix B_Bezier;
		B_Bezier.Set(0, 0, -1);
		B_Bezier.Set(0, 1, 3);
		B_Bezier.Set(0, 2, -3);
		B_Bezier.Set(0, 3, 1);

		B_Bezier.Set(1, 0, 3);
		B_Bezier.Set(1, 1, -6);
		B_Bezier.Set(1, 2, 3);
		B_Bezier.Set(1, 3, 0);

		B_Bezier.Set(2, 0, -3);
		B_Bezier.Set(2, 1, 3);
		B_Bezier.Set(2, 2, 0);
		B_Bezier.Set(2, 3, 0);

		B_Bezier.Set(3, 0, 1);
		B_Bezier.Set(3, 1, 0);
		B_Bezier.Set(3, 2, 0);
		B_Bezier.Set(3, 3, 0);
		return B_Bezier;
	}

	static Matrix getB_BSpline(void)
	{
		Matrix B_BSpline;
		B_BSpline.Set(0, 0, -1);
		B_BSpline.Set(0, 1, 3);
		B_BSpline.Set(0, 2, -3);
		B_BSpline.Set(0, 3, 1);

		B_BSpline.Set(1, 0, 3);
		B_BSpline.Set(1, 1, -6);
		B_BSpline.Set(1, 2, 0);
		B_BSpline.Set(1, 3, 4);

		B_BSpline.Set(2, 0, -3);
		B_BSpline.Set(2, 1, 3);
		B_BSpline.Set(2, 2, 3);
		B_BSpline.Set(2, 3, 1);

		B_BSpline.Set(3, 0, 1);
		B_BSpline.Set(3, 1, 0);
		B_BSpline.Set(3, 2, 0);
		B_BSpline.Set(3, 3, 0);

		B_BSpline *= 1 / 6;
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
		//control points
		glBegin(GL_POINTS);
		glPointSize(2);
		glColor3f(1, 0, 0);
		for (auto i = controlPoints.begin(); i != controlPoints.end(); i++)
		{
			glVertex3f(i->x(), i->y(), i->z());
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
		glLineWidth(1);
		glColor3f(0, 0, 1);
		for (auto i = controlPoints.begin(); i != controlPoints.end(); i++)
		{
			glVertex3f(i->x(), i->y(), i->z());
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
		glLineWidth(1);
		glColor3f(0, 1, 0);
		float step=1.0/(float)args->curve_tessellation;
		float t = 0;
		for (int i = 0; i <= args->curve_tessellation; i++)
		{
			Vec3f vertex = Q(t);
			glVertex3f(vertex[0], vertex[1], vertex[2]);
			t += step;
		}
		glEnd();
	}

private:


};


class BezierCurve : public Curve
{
public:
	BezierCurve(int num_vertices)
	{
		B = B_Bezier;
	}


	//G_Bezier = G_BSpline * B_BezierToB_BSpline
	virtual void OutputBezier(FILE* file)
	{

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
				G_Bezier.Set(i, j, controlPoints[j][i]);
			}
		}

		Matrix G_BSpline = G_Bezier * B_BSplineToB_Bezier;
		
	}
};


class BSplineCurve : public Curve
{
public:
	BSplineCurve(int num_vertices)
	{
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
				G_BSpline.Set(i, j, controlPoints[j][i]);
			}
		}

		Matrix G_Bezier = G_BSpline * B_BezierToB_BSpline;


	}



	//G_BSpline = G_Bezier * B_BSplineToB_Bezier
	virtual void OutputBSpline(FILE* file)
	{


	}


};


class Surface : public Spline
{
public:

};


class SurfaceOfRevolution : public Surface
{
public:
	SurfaceOfRevolution(Curve *c)
	{

	}
};

class BezierPatch : public Surface
{
public:
	BezierPatch()
	{

	}
};

#endif