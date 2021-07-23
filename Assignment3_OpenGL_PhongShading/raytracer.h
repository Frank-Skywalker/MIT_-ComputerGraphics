#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include "scene_parser.h"
#include "image.h"
//#include "image.h"
//#include "hit.h"
#include "camera.h"
#include "light.h"
#include "group.h"
#include "Ray.h"
#include "hit.h"
#include "material.h"
#include "glCanvas.h"

class SceneParser;

class Raytracer
{
public:
    Raytracer()
    {
        assert(0);
    }

    Raytracer(char *input_file, int width, int height,float depth_min,float depth_max) : input_file(input_file), width(width), height(height),depth_min(depth_min),depth_max(depth_max)
    {
        scene = new SceneParser(input_file);
        hits= new Hit[width * height];
        rays = new Ray[width * height];

    }

    void doRaytrace(void)
    {
        //Material mat(Vec3f(1, 0, 0));
        for (int i = 0; i < width * height; i++)
        {
            //cout << "i=" << i << endl;

            //cout << "hits[i]" << hits[i].getT() << " " << hits[i].getMaterial() << " " << hits[i].getIntersectionPoint() << endl;
            //cout << "hits[i]" << hits[i].getT() << " " << hits[i].getMaterial() << " " << hits[i].getIntersectionPoint() << endl;
            //cout << "hits[i]" << hits[i].getMaterial()->getDiffuseColor()<<endl;
            if (!scene->getGroup()->intersect(GenerateRayAtIndex(i), hits[i], scene->getCamera()->getTMin()))
            {
                //hits[i].setBackgroundMaterial(scene->getBackgroundMaterial());
            }
            //cout << "hits[i].Normal: " << hits[i].getNormal() << endl;
            //cout << "hits[i].T: " << hits[i].getT() << endl;

        }



      

    }


    ~Raytracer()
    {
        delete scene;
        delete[] hits;
    }




    void colorShader(char* outputFile)
    {
        Image outputImage(width, height);
        for (int i = 0; i < width * height; i++)
        {
            int x = i % width;
            int y = i / width;
            outputImage.SetPixel(x, y, hits[i].getMaterial()->getDiffuseColor());
        }
        outputImage.SaveTGA(outputFile);
    }

    void depthShader(char* outputFile)
    {
        Image outputImage(width, height);
        for (int i = 0; i < width * height; i++)
        {
            int x = i % width;
            int y = i / width;
            //float t = hits[i].getT();
            float t = (depth_max - hits[i].getT()) / (depth_max - depth_min);
            //cout << "t " << t << endl;
            Vec3f depthColor(t, t, t);
            outputImage.SetPixel(x, y, depthColor);
        }

        outputImage.SaveTGA(outputFile);
    }


    void normalShader(char* outputFile)
    {
        Image outputImage(width, height);
        for (int i = 0; i < width * height; i++)
        {
            int x = i % width;
            int y = i / width;
            //if (hits[i].getT() == INFINITY)
            //{
            //    outputImage.SetPixel(x, y, scene->getBackgroundColor());
            //    continue;
            //}
            Vec3f N = hits[i].getNormal();
            Vec3f depthColor(fabsf(N.x()), fabsf(N.y()), fabsf(N.z()));
            outputImage.SetPixel(x, y, depthColor);
        }
        outputImage.SaveTGA(outputFile);
    }



    SceneParser* getScene(void)
    {
        return scene;
    }



/*
	d	= L . N     	if L . N > 0
 	 	= 0	otherwise
    cpixel  =  cambient * cobject + SUMi [ clamped(Li . N) * clighti * cobject ]
*/
    void diffuseShader(char* outputFile,bool shadeBack)
    {
        Image outputImage(width, height);
        for (int i = 0; i < width * height; i++)
        {
            int x = i % width;
            int y = i / width;
            if (hits[i].getT() == INFINITY)
            {
                outputImage.SetPixel(x, y, scene->getBackgroundColor());
                continue;
			}
            Vec3f N = hits[i].getNormal();
            //cout << "N: " << N << endl;
            //if shade back
			if (shadeBack && rays[i].getDirection().Dot3(N) > 0)
            {
                N.Negate();
            }
            //no shade back and ray inside object
            if (!shadeBack && rays[i].getDirection().Dot3(N) > 0)
            {
                outputImage.SetPixel(x, y, Vec3f(0,0,0));
                continue;
            }

            Vec3f objectColor = hits[i].getMaterial()->getDiffuseColor();
            Vec3f ambientColor = scene->getAmbientLight() * objectColor;
            Vec3f diffuseColor(0,0,0);
            for (int j = 0; j < scene->getNumLights(); j++)
            {
                Vec3f L;
                Vec3f lightColor;
                float distanceToLight;
                scene->getLight(j)->getIllumination(hits[i].getIntersectionPoint(), L, lightColor, distanceToLight);
                float temp = L.Dot3(N);
                if (temp < 0)
                    temp = 0;
                diffuseColor += temp * lightColor * objectColor;
            }
            Vec3f pixelColor =ambientColor+diffuseColor;
            outputImage.SetPixel(x, y, pixelColor);
        }
        outputImage.SaveTGA(outputFile);
    }


    void PhongShader(char* outputFile, bool shadeBack)
    {
        Image outputImage(width, height);
        for (int i = 0; i < width * height; i++)
        {
            int x = i % width;
            int y = i / width;
            if (hits[i].getT() == INFINITY)
            {
                outputImage.SetPixel(x, y, scene->getBackgroundColor());
                continue;
            }
            Vec3f N = hits[i].getNormal();
            //cout << "N: " << N << endl;
            //if shade back
            if (shadeBack && rays[i].getDirection().Dot3(N) > 0)
            {
                N.Negate();
            }
            //no shade back and ray inside object
            if (!shadeBack && rays[i].getDirection().Dot3(N) > 0)
            {
                outputImage.SetPixel(x, y, Vec3f(0, 0, 0));
                continue;
            }



            Vec3f objectColor = hits[i].getMaterial()->getDiffuseColor();
            Vec3f ambientColor = scene->getAmbientLight() * objectColor;
            Vec3f diffuseSpecularColor(0, 0, 0);
            for (int j = 0; j < scene->getNumLights(); j++)
            {
                Vec3f dirToLight;
                Vec3f lightColor;
                float distanceToLight;
                scene->getLight(j)->getIllumination(hits[i].getIntersectionPoint(), dirToLight, lightColor, distanceToLight);
                diffuseSpecularColor+= hits[i].getMaterial()->Shade(hits[i].getRay(), hits[i],dirToLight,lightColor);
            }

            Vec3f pixelColor = diffuseSpecularColor + ambientColor;
            outputImage.SetPixel(x, y, pixelColor);

        }
        outputImage.SaveTGA(outputFile);
    }




private:
    char *input_file;
    SceneParser *scene;
    Ray* rays;
    int width;
    int height;
    Hit* hits;
    float depth_min;
    float depth_max;
    //GLCanvas *glCanvas;


    Ray GenerateRayAtIndex(int index)
    {
        int xindex=index%width;
        int yindex = index / width;
        Vec2f point((float)xindex / (float)(width-1), (float)yindex / (float)(height-1));
        //float d0, d1;
        //point.Get(d0, d1);
        //cout << "pointInfo:" << d0<< " " <<d1<< endl;
        rays[index] = scene->getCamera()->generateRay(point);
        return rays[index];
    }

};

#endif