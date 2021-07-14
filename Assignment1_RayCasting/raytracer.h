#ifndef _RAYTRACER_
#define _RAYTRACER_

#include "scene_parser.h"
#include "image.h"
#include "hit.h"
#include "camera.h"
#include "group.h"

class Raytracer
{
public:
    Raytracer()
    {
        assert(0);
    }

    Raytracer(char *input_file, int width, int height, char *output_file, char *depth_file) : input_file(input_file), width(width), height(height), output_file(output_file),depth_file(depth_file)
    {
        scene = new SceneParser(input_file);
        hits= new Hit[width * height];
    }

    void doRaytrace()
    {
        Material mat(Vec3f(1, 0, 0));
        for (int i = 0; i < width * height; i++)
        {
            //cout << "i=" << i << endl;

            //cout << "hits[i]" << hits[i].getT() << " " << hits[i].getMaterial() << " " << hits[i].getIntersectionPoint() << endl;
            bool hasIntersect =scene->getGroup()->intersect(GenerateRayAtIndex(i), hits[i], scene->getCamera()->getTMin());
            //cout << "hits[i]" << hits[i].getT() << " " << hits[i].getMaterial() << " " << hits[i].getIntersectionPoint() << endl;

            //cout << "hits[i]" << hits[i].getMaterial()->getDiffuseColor()<<endl;
            if (!hasIntersect)
            {
                hits[i].setBackgroundMaterial(scene->getBackgroundMaterial());
            }
            float nowT = hits[i].getT();
            if (nowT != INFINITY&&nowT > maxT )
            {
                maxT = nowT;
            }
            if (nowT != INFINITY && nowT < minT)
            {
                minT = nowT;
            }
        }

        Image outputImage(width, height);
        for (int i = 0; i < width * height; i++)
        {
            int x = i % width;
            int y = i / width;
            outputImage.SetPixel(x, y, hits[i].getMaterial()->getDiffuseColor());
        }
        outputImage.SaveTGA(output_file);

        Image depthImage(width, height);
        for (int i = 0; i < width * height; i++)
        {
            int x = i % width;
            int y = i / width;
            //float t = hits[i].getT();
            float t = (maxT - hits[i].getT()) /(maxT-minT);
            //cout << "t " << t << endl;
            Vec3f depthColor(t, t, t);
            depthImage.SetPixel(x, y, depthColor);
        }

        depthImage.SaveTGA(depth_file);

    }


    ~Raytracer()
    {
        delete scene;
        delete[] hits;
    }

private:
    char *input_file;
    SceneParser *scene;
    char *output_file;
    char *depth_file;
    int width;
    int height;
    Hit* hits;
    float maxT=-INFINITY;
    float minT=INFINITY;


    Ray GenerateRayAtIndex(int index)
    {
        int xindex=index%width;
        int yindex = index / width;
        Vec2f point((float)xindex / (float)(width-1), (float)yindex / (float)(height-1));
        //float d0, d1;
        //point.Get(d0, d1);
        //cout << "pointInfo:" << d0<< " " <<d1<< endl;
        return scene->getCamera()->generateRay(point);
    }

};

#endif