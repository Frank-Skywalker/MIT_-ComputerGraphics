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
#include "rayTree.h"
#include "grid.h"
#include "raytracing_stats.h"
#include "film.h"
#include "sample.h"
#include "sampler.h"
#include "filter.h"

#define EPSILON 0.001
#define VACUUM_REFRACTION_INDEX 1
#define WEIGHT_STEP_DECREASE 0.05

class SceneParser;

class RayTracer
{
public:


    //Assignment4
    RayTracer(char* input_file, int width, int height, 
        int max_bounces, float cutoff_weight, bool shadows, bool shadeback, 
        bool useGrid,int nx, int ny, int nz,bool renderGrid,
        int sampleMode, int numSamples, int filterMode,float filterRadius) :
        input_file(input_file), width(width), height(height), 
        maxBounces(max_bounces), cutoffWeight(cutoff_weight), shadeShadows(shadows),shadeBack(shadeback),
        sampleMode(sampleMode),numSamples(numSamples),filterMode(filterMode),filterRadius(filterRadius)
    {
        squareLength = max(width, height);
        scene = new SceneParser(input_file);

        assert(scene != NULL);
        ambientLight = scene->getAmbientLight();
        
        //cout << "here2" << endl;
        //Assignment5

        //select ray tracing method
        if (useGrid)
        {
            //Matrix *m=new Matrix;
            //m->SetToIdentity();
            grid = new Grid(scene->getGroup()->getBoundingBox(), nx, ny, nz);
            scene->getGroup()->insertIntoGrid(grid, NULL);

            if (renderGrid)
            {
                tracerayFunction = &RayTracer::traceRayGrid;
            }
            else
            {
                tracerayFunction = &RayTracer::traceRayFast;
            }
            //traceRayFunction = traceRayFast;
        }
        else
        {
            grid = NULL;
            tracerayFunction = &RayTracer::traceRay;
        }
        RayTracingStats::Initialize(width, height, scene->getGroup()->getBoundingBox(), nx, ny, nz);

        //Assignment7
        film = new Film(width, height, numSamples);

        sampler = getSampler(sampleMode,numSamples);

        filter = getFilter(filterMode, filterRadius);

    }

    //RayTracer(SceneParser* s, int max_bounces, float cutoff_weight, bool shadows, ...);


    ~RayTracer()
    {
        delete scene;
        if (grid != NULL)
        {
            delete grid;
        }
        if (film != NULL)
        {
            delete film;
        }
        if (sampler != NULL)
        {
            delete sampler;
        }
        if (filter != NULL)
        {
            delete filter;
        }
    }




    SceneParser* getScene(void)
    {
        return scene;
    }

    Grid* getGrid(void)
    {
        return grid;
    }




	////Assignment6
	//void RayCast(char* outputFile)
	//{
	//	Image outputImage(width, height);
	//	int xoffset = 0;
	//	int yoffset = 0;

	//	//deal with width!=height
	//	if (width > height)
	//	{
	//		yoffset = (squareLength - height) / 2;
	//	}
	//	else if (width < height)
	//	{
	//		xoffset = (squareLength - width) / 2;
	//	}

	//	for (int i = 0; i < width * height; i++)
	//	{
	//		int x = i % width;
	//		int y = i / width;
	//		int xrayIndex = x + xoffset;
	//		int yrayIndex = y + yoffset;

	//		Hit hit;
	//		Ray ray = generateRayAtIndex(xrayIndex, yrayIndex);
	//		RayTracingStats::IncrementNumNonShadowRays();
	//		outputImage.SetPixel(x, y, traceRay(ray, scene->getCamera()->getTMin(), 0, 1, VACUUM_REFRACTION_INDEX, hit));
	//	}
	//	outputImage.SaveTGA(outputFile);
	//}

	//void RayCastFast(char* outputFile)
	//{
	//	Image outputImage(width, height);
	//	int xoffset = 0;
	//	int yoffset = 0;

	//	//deal with width!=height
	//	if (width > height)
	//	{
	//		yoffset = (squareLength - height) / 2;
	//	}
	//	else if (width < height)
	//	{
	//		xoffset = (squareLength - width) / 2;
	//	}

	//	for (int i = 0; i < width * height; i++)
	//	{
	//		int x = i % width;
	//		int y = i / width;
	//		int xrayIndex = x + xoffset;
	//		int yrayIndex = y + yoffset;



	//		Hit hit;
	//		Ray ray = generateRayAtIndex(xrayIndex, yrayIndex);
	//		RayTracingStats::IncrementNumNonShadowRays();
	//		outputImage.SetPixel(x, y, traceRayFast(ray, scene->getCamera()->getTMin(), 0, 1, VACUUM_REFRACTION_INDEX, hit));
	//	}
	//	outputImage.SaveTGA(outputFile);
	//}

	//void RayCastGrid(char* outputFile)
	//{
	//	Image outputImage(width, height);
	//	int xoffset = 0;
	//	int yoffset = 0;

	//	//deal with width!=height
	//	if (width > height)
	//	{
	//		yoffset = (squareLength - height) / 2;
	//	}
	//	else if (width < height)
	//	{
	//		xoffset = (squareLength - width) / 2;
	//	}

	//	for (int i = 0; i < width * height; i++)
	//	{
	//		int x = i % width;
	//		int y = i / width;
	//		int xrayIndex = x + xoffset;
	//		int yrayIndex = y + yoffset;
	//		Hit hit;
	//		Ray ray = generateRayAtIndex(xrayIndex, yrayIndex);
	//		outputImage.SetPixel(x, y, traceRayGrid(ray, scene->getCamera()->getTMin(), 0, 1, VACUUM_REFRACTION_INDEX, hit));

	//	}
	//	outputImage.SaveTGA(outputFile);
	//}


	//void RayCastNormal(char* outputFile)
	//{
	//	Image outputImage(width, height);
	//	int xoffset = 0;
	//	int yoffset = 0;

	//	//deal with width!=height
	//	if (width > height)
	//	{
	//		yoffset = (squareLength - height) / 2;
	//	}
	//	else if (width < height)
	//	{
	//		xoffset = (squareLength - width) / 2;
	//	}

	//	for (int i = 0; i < width * height; i++)
	//	{
	//		int x = i % width;
	//		int y = i / width;
	//		int xrayIndex = x + xoffset;
	//		int yrayIndex = y + yoffset;
	//		Hit hit;
	//		Ray ray = generateRayAtIndex(xrayIndex, yrayIndex);
	//		traceRay(ray, scene->getCamera()->getTMin(), 0, 1, VACUUM_REFRACTION_INDEX, hit);
	//		Vec3f N = hit.getNormal();
	//		Vec3f depthColor(fabsf(N.x()), fabsf(N.y()), fabsf(N.z()));
	//		outputImage.SetPixel(x, y, depthColor);
	//	}
	//	outputImage.SaveTGA(outputFile);
	//}



	//void RayCastFastNormal(char* outputFile)
	//{
	//	Image outputImage(width, height);
	//	int xoffset = 0;
	//	int yoffset = 0;

	//	//deal with width!=height
	//	if (width > height)
	//	{
	//		yoffset = (squareLength - height) / 2;
	//	}
	//	else if (width < height)
	//	{
	//		xoffset = (squareLength - width) / 2;
	//	}

	//	for (int i = 0; i < width * height; i++)
	//	{
	//		int x = i % width;
	//		int y = i / width;
	//		int xrayIndex = x + xoffset;
	//		int yrayIndex = y + yoffset;
	//		Hit hit;
	//		Ray ray = generateRayAtIndex(xrayIndex, yrayIndex);
	//		traceRayFast(ray, scene->getCamera()->getTMin(), 0, 1, VACUUM_REFRACTION_INDEX, hit);
	//		Vec3f N = hit.getNormal();
	//		Vec3f depthColor(fabsf(N.x()), fabsf(N.y()), fabsf(N.z()));
	//		outputImage.SetPixel(x, y, depthColor);
	//	}
	//	outputImage.SaveTGA(outputFile);
	//}






	//Assignment7 
	void RayCastSample(char* outputFile)
	{
        Image outputImage(width, height);
        int xoffset = 0;
        int yoffset = 0;

        //deal with width!=height
        yoffset =max( 0, (squareLength - height) / 2);
        xoffset =max(0, (squareLength - width) / 2);

        for (int i = 0; i < width * height; i++)
        {
            int x = i % width;
            int y = i / width;
            int xrayIndex = x + xoffset;
            int yrayIndex = y + yoffset;

            //sampling
            for (int s = 0; s < numSamples; s++)
            {
                Vec2f sampleOffset=sampler->getSamplePosition(s);
                
                Hit hit;
                Ray ray = generateRayAtIndexWithOffset(xrayIndex, yrayIndex, sampleOffset);
                
                Vec3f color = (this->*tracerayFunction)(ray, scene->getCamera()->getTMin(), 0, 1, VACUUM_REFRACTION_INDEX, hit);
                //cout << "color: " << color<<endl;
                film->setSample(x, y, s, sampleOffset, color);
            }
        }

        for (int i = 0; i < width * height; i++)
        {
            int x = i % width;
            int y = i / width;
            outputImage.SetPixel(x,y,filter->getColor(x, y, film));
        }
        if(outputFile!=NULL)
            outputImage.SaveTGA(outputFile);
	}



    void RenderSample(char* sampleFile, int sampleZoom)
    {
        film->renderSamples(sampleFile, sampleZoom);
    }

    void RenderFilter(char* filterFile, int filterZoom)
    {
        assert(filter != NULL);
        film->renderFilter(filterFile, filterZoom,filter);
    }



    Vec3f traceRay(Ray& ray, float tmin, int bounces, float weight,
        float indexOfRefraction, Hit& hit) const
    {
        //cout << "raydirection before everrything: " << ray.getDirection() << endl;

        if (bounces > maxBounces || weight < cutoffWeight)
        {
            return Vec3f(0, 0, 0);
        }

        scene->getGroup()->intersect(ray, hit, tmin);

        Vec3f intersectionPoint = hit.getIntersectionPoint();

        //render main segment
        if (bounces == 0)
        {
            RayTree::SetMainSegment(ray, tmin, hit.getT());
        }
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

        Vec3f objectColor = hit.getMaterial()->getDiffuseColor(intersectionPoint);
        Vec3f ambientColor = ambientLight * objectColor;
        Vec3f diffuseSpecularColor(0, 0, 0);
        for (int j = 0; j < scene->getNumLights(); j++)
        {
            Vec3f dirToLight;
            Vec3f lightColor;
            float distanceToLight;
            
            scene->getLight(j)->getIllumination(hit.getIntersectionPoint(), dirToLight, lightColor, distanceToLight);

            if (!shadeShadows)
            {
                //cout << "raydirection: " << hit.getRay().getDirection() << endl;
                //return Vec3f(fabsf(hit.getRay().getDirection().x()), fabsf(hit.getRay().getDirection().y()), fabsf(hit.getRay().getDirection().z()));
                diffuseSpecularColor += hit.getMaterial()->Shade(hit.getRay(), hit, dirToLight, lightColor);
                continue;
            }

            //shade shadows
            Ray shadowRay(hit.getIntersectionPoint(), dirToLight);
            RayTree::AddShadowSegment(shadowRay, 0, distanceToLight);
            RayTracingStats::IncrementNumShadowRays();
            if (!scene->getGroup()->intersectShadowRay(shadowRay, EPSILON,distanceToLight))
            {
                diffuseSpecularColor += hit.getMaterial()->Shade(hit.getRay(), hit, dirToLight, lightColor);
            }

        }

        //deal with reflection color
        Vec3f reflectColor = hit.getMaterial()->getReflectiveColor(intersectionPoint);
        if (reflectColor != Vec3f(0, 0, 0))
        {
            Ray reflectRay(hit.getIntersectionPoint(), mirrorDirection(normal, ray.getDirection()));
            Hit reflectHit;
            RayTracingStats::IncrementNumNonShadowRays();
            Vec3f reflectResult = traceRay(reflectRay, EPSILON, bounces + 1, weight- WEIGHT_STEP_DECREASE, indexOfRefraction, reflectHit);
            reflectColor = reflectColor * reflectResult;
            RayTree::AddReflectedSegment(reflectRay, 0, reflectHit.getT());
        }

        //deal with refraction color
        Vec3f refractColor = hit.getMaterial()->getTransparentColor(intersectionPoint);
        if (refractColor != Vec3f(0, 0, 0))
        {
            Vec3f refractDirection;
            Vec3f refractResult;
            //ray from inside object
            if (ray.getDirection().Dot3(hit.getNormal()) > 0)
            {
                transmittedDirection(normal, ray.getDirection(), indexOfRefraction, VACUUM_REFRACTION_INDEX, refractDirection);
                Ray refractRay(hit.getIntersectionPoint(), refractDirection);
                Hit refractHit;
                RayTracingStats::IncrementNumNonShadowRays();
                refractResult = traceRay(refractRay, EPSILON, bounces + 1, weight- WEIGHT_STEP_DECREASE, VACUUM_REFRACTION_INDEX, refractHit);
                RayTree::AddTransmittedSegment(refractRay, 0, refractHit.getT());

            }
            //ray from outside object
            else
            {
                transmittedDirection(normal, ray.getDirection(), indexOfRefraction, hit.getMaterial()->getIndexOfRefraction(intersectionPoint), refractDirection);
                Ray refractRay(hit.getIntersectionPoint(), refractDirection);
                Hit refractHit;
                RayTracingStats::IncrementNumNonShadowRays();
                refractResult = traceRay(refractRay, EPSILON, bounces + 1, weight- WEIGHT_STEP_DECREASE, hit.getMaterial()->getIndexOfRefraction(intersectionPoint), refractHit);
                RayTree::AddTransmittedSegment(refractRay, 0, refractHit.getT());
            }
            refractColor = refractColor * refractResult;

        }

        Vec3f resultColor = refractColor + reflectColor + diffuseSpecularColor + ambientColor;
        return weight * resultColor;
    }


 


    Vec3f traceRayFast(Ray& ray, float tmin, int bounces, float weight,
        float indexOfRefraction, Hit& hit) const
    {
        //cout << "raydirection before everrything: " << ray.getDirection() << endl;

        if (bounces > maxBounces || weight < cutoffWeight)
        {
            return Vec3f(0, 0, 0);
        }

        grid->intersectObjects(ray, hit, tmin);
        Vec3f intersectionPoint = hit.getIntersectionPoint();
            //cout << "hit!" << endl;
            //cout << "ray: " << ray.getDirection()<<endl;
            //cout << "hit: " << hit.getNormal()<<endl;
            //cout << "hit: " << hit.getMaterial() << endl;
        //render main segment
        if (bounces == 0)
        {
            RayTree::SetMainSegment(ray, tmin, hit.getT());
        }
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

        Vec3f objectColor = hit.getMaterial()->getDiffuseColor(intersectionPoint);
        Vec3f ambientColor = ambientLight * objectColor;
        Vec3f diffuseSpecularColor(0, 0, 0);
        for (int j = 0; j < scene->getNumLights(); j++)
        {
            Vec3f dirToLight;
            Vec3f lightColor;
            float distanceToLight;
            scene->getLight(j)->getIllumination(hit.getIntersectionPoint(), dirToLight, lightColor, distanceToLight);
            if (!shadeShadows)
            {
                //cout << "raydirection: " << hit.getRay().getDirection() << endl;
                //return Vec3f(fabsf(hit.getRay().getDirection().x()), fabsf(hit.getRay().getDirection().y()), fabsf(hit.getRay().getDirection().z()));
                diffuseSpecularColor += hit.getMaterial()->Shade(hit.getRay(), hit, dirToLight, lightColor);
                continue;
            }

            //shade shadows
            Ray shadowRay(hit.getIntersectionPoint(), dirToLight);
            RayTree::AddShadowSegment(shadowRay, 0, distanceToLight);
            RayTracingStats::IncrementNumShadowRays();
            if (!grid->intersectObjectsShadow(shadowRay, EPSILON, distanceToLight))
            {
                diffuseSpecularColor += hit.getMaterial()->Shade(hit.getRay(), hit, dirToLight, lightColor);
            }

        }

        //deal with reflection color
        Vec3f reflectColor = hit.getMaterial()->getReflectiveColor(intersectionPoint);
        if (reflectColor != Vec3f(0, 0, 0))
        {
            Ray reflectRay(hit.getIntersectionPoint(), mirrorDirection(normal, ray.getDirection()));
            Hit reflectHit;
            RayTracingStats::IncrementNumNonShadowRays();
            Vec3f reflectResult = traceRayFast(reflectRay, EPSILON, bounces + 1, weight - WEIGHT_STEP_DECREASE, indexOfRefraction, reflectHit);
            reflectColor = reflectColor * reflectResult;
            RayTree::AddReflectedSegment(reflectRay, 0, reflectHit.getT());
        }

        //deal with refraction color
        Vec3f refractColor = hit.getMaterial()->getTransparentColor(intersectionPoint);
        if (refractColor != Vec3f(0, 0, 0))
        {
            Vec3f refractDirection;
            Vec3f refractResult;
            //ray from inside object
            if (ray.getDirection().Dot3(hit.getNormal()) > 0)
            {
                transmittedDirection(normal, ray.getDirection(), indexOfRefraction, VACUUM_REFRACTION_INDEX, refractDirection);
                Ray refractRay(hit.getIntersectionPoint(), refractDirection);
                Hit refractHit;
                RayTracingStats::IncrementNumNonShadowRays();
                refractResult = traceRayFast(refractRay, EPSILON, bounces + 1, weight - WEIGHT_STEP_DECREASE, VACUUM_REFRACTION_INDEX, refractHit);
                RayTree::AddTransmittedSegment(refractRay, 0, refractHit.getT());

            }
            //ray from outside object
            else
            {
                transmittedDirection(normal, ray.getDirection(), indexOfRefraction, hit.getMaterial()->getIndexOfRefraction(intersectionPoint), refractDirection);
                Ray refractRay(hit.getIntersectionPoint(), refractDirection);
                Hit refractHit;
                RayTracingStats::IncrementNumNonShadowRays();
                refractResult = traceRayFast(refractRay, EPSILON, bounces + 1, weight - WEIGHT_STEP_DECREASE, hit.getMaterial()->getIndexOfRefraction(intersectionPoint), refractHit);
                RayTree::AddTransmittedSegment(refractRay, 0, refractHit.getT());
            }
            refractColor = refractColor * refractResult;

        }

        Vec3f resultColor = refractColor + reflectColor + diffuseSpecularColor + ambientColor;
        return weight * resultColor;
    }



    Vec3f traceRayGrid(Ray& ray, float tmin, int bounces, float weight,
        float indexOfRefraction, Hit& hit) const
    {
        grid->intersect(ray, hit, tmin);
        Vec3f intersectionPoint = hit.getIntersectionPoint();
        if (hit.getT() == INFINITY)
        {
            return scene->getBackgroundColor();
        }
        Vec3f N = hit.getNormal();
        //cout << "N: " << N << endl;
        //if shade back
        if (shadeBack && ray.getDirection().Dot3(N) > 0)
        {
            N.Negate();
        }
        //no shade back and ray inside object
        if (!shadeBack && ray.getDirection().Dot3(N) > 0)
        {
            return Vec3f(0, 0, 0);
        }

        Vec3f objectColor = hit.getMaterial()->getDiffuseColor(intersectionPoint);
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
        return diffuseSpecularColor + ambientColor;
    }


private:
    char *input_file;
    SceneParser *scene;
    int width;
    int height;
    int squareLength;
    float depth_min;
    float depth_max;
    //GLCanvas *glCanvas;

    //Assignment4
    int maxBounces;
    float cutoffWeight;
    bool shadeShadows;
    bool shadeBack;
    Vec3f ambientLight;

    //Assignment5
    Grid* grid;
    //bool shadeGrid;

    //Assignment7
    int sampleMode;
    int numSamples;
    int filterMode;
    float filterRadius;
    Film* film;
    Sampler* sampler;
    Filter* filter;

    Vec3f(RayTracer::* tracerayFunction)(Ray& ray, float tmin, int bounces, float weight,
        float indexOfRefraction, Hit& hit) const;


    //Ray generateRayAtIndex(int index)
    //{
    //    int xindex=index% squareLength;
    //    int yindex = index / squareLength;
    //    Vec2f point((float)xindex / (float)(squareLength -1), (float)yindex / (float)(squareLength -1));
    //    //float d0, d1;
    //    //point.Get(d0, d1);
    //    //cout << "pointInfo:" << d0<< " " <<d1<< endl;
    //    rays[index] = scene->getCamera()->generateRay(point);
    //    return rays[index];
    //}

    Ray generateRayAtIndex(int xindex,int yindex)
    {
        Vec2f point((float)xindex / (float)(squareLength - 1), (float)yindex / (float)(squareLength - 1));
        //float d0, d1;
        //point.Get(d0, d1);
        //cout << "pointInfo:" << d0<< " " <<d1<< endl;
        return scene->getCamera()->generateRay(point);
    }

    Ray generateRayAtIndexWithOffset(float xrayIndex, float yrayIndex,Vec2f sampleOffset)
    {
        Vec2f point(((float)xrayIndex + sampleOffset.x()) / (float)squareLength, ((float)yrayIndex + sampleOffset.y()) / (float)squareLength );
        return scene->getCamera()->generateRay(point);

    }


    //void doRayCasting(void)
    //{
    //    for (int i = 0; i < width * height; i++)
    //    {
    //        scene->getGroup()->intersect(generateRayAtIndex(i), hits[i], scene->getCamera()->getTMin());
    //    }
    //}


    //Assignment4
    Vec3f mirrorDirection(const Vec3f& normal, const Vec3f& incoming) const
    {
        Vec3f mirror = incoming - 2 * normal.Dot3(incoming) * normal;
        mirror.Normalize();
        return mirror;
    }


    bool transmittedDirection(const Vec3f& normal, const Vec3f& incoming,
        float index_i, float index_t, Vec3f& transmitted) const
    {
        Vec3f inverseIncoming = incoming;
        inverseIncoming.Negate();
        float yitaR = index_i / index_t;
        transmitted = (yitaR * normal.Dot3(inverseIncoming) - sqrtf(1 - powf(yitaR, 2) * (1 - powf(normal.Dot3(inverseIncoming), 2)))) * normal - yitaR * inverseIncoming;
        transmitted.Normalize();
        return true;
    }
   




    Sampler* getSampler(int sampleMode, int numSamples)
    {
        Sampler* sampler;
        if (sampleMode == NO_SAMPLE)
        {
            sampler = new UniformSampler(1);
        }
        else if (sampleMode == UNIFORM_SAMPLE)
        {
            sampler = new UniformSampler(numSamples);
        }
        else if (sampleMode == RANDOM_SAMPLE)
        {
            sampler = new RandomSampler(numSamples);
        }
        else if (sampleMode == JITTERED_SAMPLE)
        {
            sampler = new JitteredSampler(numSamples);
        }
        else
        {
            sampler = NULL;
        }

        return sampler;
    }




    Filter* getFilter(int filterMode, float radius)
    {
        Filter* filter = NULL;
        if (filterMode == NO_FILTER)
        {
            filter = new BoxFilter(0.5);
        }
        else if (filterMode == BOX_FILTER)
        {
            filter = new BoxFilter(radius);
        }
        else if (filterMode == TENT_FILTER)
        {
            filter = new TentFilter(radius);
        }
        else if (filterMode == GAUSSIAN_FILTER)
        {
            filter = new GaussianFilter(radius);
        }
        else
        {
            filter = NULL;
        }

        return filter;

    }

};

#endif