#include "scene_parser.h"
#include "hit.h"
#include "group.h"
#include "light.h"

class rTracer
{
public:
	rTracer(SceneParser* s, int max_bounces, float cutoff_weight, bool shadows):scene(s),maxBounces(max_bounces),cutoffWeight(cutoff_weight),shadeShadows(shadows)
	{
        assert(scene != NULL);
        ambientLight = scene->getAmbientLight();
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

        Vec3f normal=hit.getNormal();
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





    //void raytraceShader(void)
    //{
    //    for (int i = 0; i < width * height; i++)
    //    {

    //        Vec3f diffuseSpecularColor(0, 0, 0);
    //        //deal with shadow
    //        for (int j = 0; j < scene->getNumLights(); j++)
    //        {
    //            Vec3f dirToLight;
    //            Vec3f lightColor;
    //            float distanceToLight;
    //            scene->getLight(j)->getIllumination(hits[i].getIntersectionPoint(), dirToLight, lightColor, distanceToLight);
    //            diffuseSpecularColor += hits[i].getMaterial()->Shade(hits[i].getRay(), hits[i], dirToLight, lightColor);
    //        }

    //        //deal with reflection
    //        Vec3f reflectDirection = mirrorDirection(hits[i].getNormal(), hits[i].getRay().getDirection());
    //        Ray reflectRay(hits[i].getIntersectionPoint(), reflectDirection);
    //        //traceRay(reflectRay,)
    //    }
    //}





private:
	SceneParser* scene;
	int maxBounces;
	float cutoffWeight;
	bool shadeShadows;
    bool shadeBack;
    Vec3f ambientLight;


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
};