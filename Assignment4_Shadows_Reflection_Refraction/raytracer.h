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

class RayTracer
{
public:
    RayTracer()
    {
        assert(0);
    }

    RayTracer(char *input_file, int width, int height,float depth_min,float depth_max) : input_file(input_file), width(width), height(height),depth_min(depth_min),depth_max(depth_max)
    {
        scene = new SceneParser(input_file);
        hits= new Hit[width * height];
        rays = new Ray[width * height];
        assert(scene != NULL);
        ambientLight = scene->getAmbientLight();
    }

    //Assignment4
    RayTracer(char* input_file, int width, int height, int max_bounces, float cutoff_weight, bool shadows, bool shadeback) : 
        input_file(input_file), width(width), height(height), maxBounces(max_bounces), cutoffWeight(cutoff_weight), shadeShadows(shadows),shadeBack(shadeback)
    {
        scene = new SceneParser(input_file);
        hits = new Hit[width * height];
        rays = new Ray[width * height];
        assert(scene != NULL);
        ambientLight = scene->getAmbientLight();
    }

    //RayTracer(SceneParser* s, int max_bounces, float cutoff_weight, bool shadows, ...);


    ~RayTracer()
    {
        delete scene;
        delete[] hits;
    }


    
    void colorShader(char* outputFile)
    {
        doRayCasting();
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
        doRayCasting();
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
        doRayCasting();
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
        doRayCasting();
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


    void phongShader(char* outputFile)
    {
        doRayCasting();
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


    //Assignment4
    void raytraceShader(char* outputFile)
    {
        Image outputImage(width, height);
        for (int i = 0; i < width * height; i++)
        {
            int x = i % width;
            int y = i / width;
            Hit hit;
            Ray ray = generateRayAtIndex(i);
            outputImage.SetPixel(x, y, traceRay(ray, scene->getCamera()->getTMin(), maxBounces, 1, 1, hit));
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

    //Assignment4
    int maxBounces;
    float cutoffWeight;
    bool shadeShadows;
    bool shadeBack;
    Vec3f ambientLight;

    


    Ray generateRayAtIndex(int index)
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


    void doRayCasting(void)
    {
        for (int i = 0; i < width * height; i++)
        {
            scene->getGroup()->intersect(generateRayAtIndex(i), hits[i], scene->getCamera()->getTMin());
        }
    }


    //Assignment4
    Vec3f mirrorDirection(const Vec3f& normal, const Vec3f& incoming)
    {
        Vec3f mirror = incoming - 2 * normal.Dot3(incoming) * normal;
        mirror.Normalize();
        return mirror;
    }


    bool transmittedDirection(const Vec3f& normal, const Vec3f& incoming,
        float index_i, float index_t, Vec3f& transmitted)
    {
        Vec3f inverseIncoming = incoming;
        inverseIncoming.Negate();
        float yitaR = index_i / index_t;
        transmitted = (yitaR * normal.Dot3(inverseIncoming) - sqrtf(1 - powf(yitaR, 2) * (1 - powf(normal.Dot3(inverseIncoming), 2)))) * normal - yitaR * inverseIncoming;
        transmitted.Normalize();
        return true;
    }
   
    Vec3f traceRay(Ray& ray, float tmin, int bounces, float weight,
        float indexOfRefraction, Hit& hit) const
    {
        if (bounces > maxBounces || weight < cutoffWeight)
        {
            return Vec3f(0, 0, 0);
        }

        scene->getGroup()->intersect(ray, hit, tmin);
        //no intersection
        if (hit.getT() == INFINITY)
        {
            return weight * scene->getBackgroundColor();
        }

        Vec3f normal = hit.getNormal();
        //shade back and ray inside object
        if (shadeBack && ray.getDirection().Dot3(normal) > 0)
        {
            normal.Negate();
        }

        //no shade back and ray inside object
        if (!shadeBack && ray.getDirection().Dot3(normal) > 0)
        {
            return Vec3f(0, 0, 0);
        }

        Vec3f objectColor = hit.getMaterial()->getDiffuseColor();
        Vec3f ambientColor = ambientLight * objectColor;
        Vec3f diffuseSpecularColor(0, 0, 0);
        for (int j = 0; j < scene->getNumLights(); j++)
        {
            Vec3f dirToLight;
            Vec3f lightColor;
            float distanceToLight;
            scene->getLight(j)->getIllumination(hit.getIntersectionPoint(), dirToLight, lightColor, distanceToLight);
            diffuseSpecularColor += hit.getMaterial()->Shade(hit.getRay(), hit, dirToLight, lightColor);
        }

        Vec3f pixelColor = diffuseSpecularColor + ambientColor;
        return weight * pixelColor;
    }
   

};

#endif