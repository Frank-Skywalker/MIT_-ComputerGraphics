#ifndef _SPHERE_H_
#define _SPHERE_H_


#include "Object3D.h"
#include "ray.h"
#include "vectors.h"
#include <math.h>
#include "grid.h"

#define M_PI acos(-1)

using namespace std;

class Sphere : public Object3D
{

public:
    Sphere(const Vec3f center, const float radius, Material* mat) : Object3D(mat), center(center), radius(radius) 
    {
        //generate bounding box
        Vec3f R(radius, radius, radius);
        boundingBox = new BoundingBox(center-R,center+R);
    }


    virtual bool intersect(const Ray& r, Hit& h, float tmin)
    {
        //cout << "Sphere center: " << this->center << endl;
        Ray raySphereSpace(r.getOrigin() - center, r.getDirection());
        float disRayOrigin = raySphereSpace.getOrigin().Length();

        float a = raySphereSpace.getDirection().Dot3(raySphereSpace.getDirection());
        float b = 2 * raySphereSpace.getDirection().Dot3(raySphereSpace.getOrigin());
        float c = raySphereSpace.getOrigin().Dot3(raySphereSpace.getOrigin()) - radius * radius;
        float delta = b * b - 4 * a * c;
        if (delta < 0)
        {
            return false;
        }
        delta = sqrtf(delta);
        float t1 = (-b - delta) / (2 * a);
        float t2 = (-b + delta) / (2 * a);
        Vec3f p1 = r.pointAtParameter(t1);
        Vec3f p2 = r.pointAtParameter(t2);
        Vec3f n1 = p1 - center;
        Vec3f n2 = p2 - center;
        n1.Normalize();
        n2.Normalize();
    /*    cout << n1 << endl;
        cout << n2 << endl;*/
        //cout << "t1:" << t1 << endl;
        //cout << "t2:" << t2 << endl;
        if (t1 > tmin )
        {
            if (t1 < h.getT())
            {
                h.set(t1, getMaterial(), n1, r);
                //cout << "hit with sphere: " << this->center<<endl;
               /* cout << "t: " << t1 << endl;
                cout << "N: " << n1 << endl;*/
                return true;
            }
            //return true;
        }
        else if (t2 > tmin)
        {
            if (t2 < h.getT() )
            {
                h.set(t2, getMaterial(), n2, r);
                //cout << "hit with sphere: " << this->center << endl;
                //cout << "t: " << t1 << endl;
                //cout << "N: " << n1 << endl;
                return true;
            }
            //return true;
        }
        //cout << "false" << endl;
        return false;
    }



    virtual bool intersectShadowRay(const Ray& r, float tmin,float distanceToLight)
    {
        //cout << "Sphere center: " << this->center << endl;
        Ray raySphereSpace(r.getOrigin() - center, r.getDirection());
        float disRayOrigin = raySphereSpace.getOrigin().Length();

        float a = raySphereSpace.getDirection().Dot3(raySphereSpace.getDirection());
        float b = 2 * raySphereSpace.getDirection().Dot3(raySphereSpace.getOrigin());
        float c = raySphereSpace.getOrigin().Dot3(raySphereSpace.getOrigin()) - radius * radius;
        float delta = b * b - 4 * a * c;
        if (delta < 0)
        {
            return false;
        }
        delta = sqrtf(delta);
        float t1 = (-b - delta) / (2 * a);
        float t2 = (-b + delta) / (2 * a);
        Vec3f p1 = r.pointAtParameter(t1);
        Vec3f p2 = r.pointAtParameter(t2);
        Vec3f n1 = p1 - center;
        Vec3f n2 = p2 - center;
        n1.Normalize();
        n2.Normalize();
        if (t1 > tmin && t1 <= distanceToLight )
        {
            return true;
        }
        else if (t2 > tmin && t2 <= distanceToLight)
        {
            return true;
        }
        return false;
    }

    void paint(void)
    {
        extern int thetaSteps;
        extern int phiSteps;
        extern bool gouraudShading;

        float thetaStepLength=(float)2*M_PI/thetaSteps;
        float phiStepLength=(float)M_PI/phiSteps;

        
        getMaterial()->glSetMaterial();
        glBegin(GL_QUADS);
        for (float iPhi = 0; iPhi < M_PI; iPhi += phiStepLength)
        {
            for (float iTheta = 0; iTheta < 2*M_PI; iTheta += thetaStepLength) 
            {
                Vec3f points[4];
                points[0].Set(radius * sinf(iPhi) * cosf(iTheta), radius * sinf(iPhi) * sinf(iTheta), radius * cosf(iPhi));
                points[1].Set(radius * sinf(iPhi) * cosf(iTheta + thetaStepLength), radius * sinf(iPhi) * sinf(iTheta + thetaStepLength), radius * cosf(iPhi));
                points[2].Set(radius * sinf(iPhi + phiStepLength) * cosf(iTheta + thetaStepLength), radius * sinf(iPhi + phiStepLength) * sinf(iTheta + thetaStepLength), radius * cosf(iPhi + phiStepLength));
                points[3].Set(radius * sinf(iPhi + phiStepLength) * cosf(iTheta), radius * sinf(iPhi + phiStepLength) * sinf(iTheta), radius * cosf(iPhi + phiStepLength));
                
                //points[0].Set(radius * cosf(iPhi) * cosf(iTheta), radius * cosf(iPhi) * sinf(iTheta), -radius * sinf(iPhi));
                //points[1].Set(radius * cosf(iPhi) * cosf(iTheta + thetaStepLength), radius * cosf(iPhi) * sinf(iTheta + thetaStepLength), -radius * sinf(iPhi));
                //points[2].Set(radius * cosf(iPhi + phiStepLength) * cosf(iTheta + thetaStepLength), radius * cosf(iPhi + phiStepLength) * sinf(iTheta + thetaStepLength), -radius * sinf(iPhi + phiStepLength));
                //points[3].Set(radius * cosf(iPhi + phiStepLength) * cosf(iTheta), radius * cosf(iPhi + phiStepLength) * sinf(iTheta), -radius * sinf(iPhi + phiStepLength));

                //cout << points[0]<<endl;
                //cout << points[1] << endl;
                //cout << points[2] << endl;
                //cout << points[3] << endl;
                //Gouraund interpolation
                if (gouraudShading)
                {
                    Vec3f normali;
                    for (int i = 0; i < 4; i++)
                    {
                        normali = points[i];
                        normali.Normalize();
                        glNormal3f(normali.x(), normali.y(), normali.z());
                        points[i] += center;
                        glVertex3f(points[i].x(), points[i].y(), points[i].z());
                    }
                }
                //flat shading 
                else 
                {
                    Vec3f line1;
                    Vec3f line2;
                    if (iPhi < M_PI / 2)
                    {
                        line1 = points[2] - points[1];
                        line2 = points[3] - points[2];
                    }
                    else
                    {
                        line1 = points[1] - points[0];
                        line2 = points[2] - points[1];
                    }
             
                    Vec3f flatNormal;
                    Vec3f::Cross3(flatNormal, line2, line1);
                    flatNormal.Normalize();
                    glNormal3f(flatNormal.x(), flatNormal.y(), flatNormal.z());
                    for (int i = 0; i < 4; i++)
                    {
                        points[i] += center;
                        glVertex3f(points[i].x(), points[i].y(), points[i].z());
                    }

                }
               
                // send gl vertex commands
                //glVertex3f(x0, y0, z0);
                //glVertex3f(x1, y1, z1);
                //glVertex3f(x2, y2, z2);
                //glVertex3f(x3, y3, z3);

                
            }
        }
        glEnd();
    }


    //Assignment5
    virtual void insertIntoGrid(Grid* grid, Matrix* m)
    {

        //transform
        if (m != NULL)
        {
            Vec3f minVertex;
            Vec3f maxVertex;
            boundingBox->Get(minVertex, maxVertex);

			//get eight vertices of sub object bounding box
			Vec3f transVertices[8];
			transVertices[0] = minVertex;
			transVertices[1] = Vec3f(minVertex.x(), minVertex.y(), maxVertex.z());
			transVertices[2] = Vec3f(minVertex.x(), maxVertex.y(), minVertex.z());
			transVertices[3] = Vec3f(minVertex.x(), maxVertex.y(), maxVertex.z());
			transVertices[4] = Vec3f(maxVertex.x(), minVertex.y(), minVertex.z());
			transVertices[5] = Vec3f(maxVertex.x(), minVertex.y(), maxVertex.z());
			transVertices[6] = Vec3f(maxVertex.x(), maxVertex.y(), minVertex.z());
			transVertices[7] = Vec3f(maxVertex.x(), maxVertex.y(), maxVertex.z());

			minVertex.Set(INFINITY, INFINITY, INFINITY);
			maxVertex.Set(-INFINITY, -INFINITY, -INFINITY);
			for (int i = 0; i < 8; i++)
			{
				m->Transform(transVertices[i]);
				Vec3f::Min(minVertex, minVertex, transVertices[i]);
				Vec3f::Max(maxVertex, maxVertex, transVertices[i]);
			}
            int mini, minj, mink;
            int maxi, maxj, maxk;
            assert(grid->getVoxelIndex(minVertex, mini, minj, mink));
            assert(grid->getVoxelIndex(maxVertex, maxi, maxj, maxk));
            for (int i = mini; i <= maxi; i++)
            {
                for (int j = minj; j <= maxj; j++)
                {
                    for (int k = mink; k <= maxk; k++)
                    {
                        grid->addObjectToVoxel(i, j, k, this);
                    }
                }
            }
            return;
        }
        

        //no transform
        float voxelHalfDiagonalLength = grid->getVoxelHalfDiagonalLength();

        Vec3f minVertex;
        Vec3f maxVertex;
        boundingBox->Get(minVertex, maxVertex);
        int mini, minj, mink;
        int maxi, maxj, maxk;
        grid->getVoxelIndex(minVertex, mini, minj, mink);
        grid->getVoxelIndex(maxVertex, maxi, maxj, maxk);
        for (int i = mini; i <= maxi; i++)
        {
            for (int j = minj; j <= maxj; j++)
            {
                for (int k = mink; k <= maxk; k++)
                {
                    Vec3f voxelCenter=grid->getVoxelCenterByIndex(i, j, k);
                    if ((voxelCenter - center).Length() <= radius + voxelHalfDiagonalLength)
                    {
                        grid->addObjectToVoxel(i, j, k, this);
                    }
                }
            }
        }
    }


private:
    Vec3f center;
    float radius;
};

#endif