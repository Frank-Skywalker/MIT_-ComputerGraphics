#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
// include glCanvas.h to access the preprocessor variable SPECULAR_FIX
#include "glCanvas.h"
#include "hit.h"
#include "vectors.h"
#include "ray.h"
#include "matrix.h"
#include "perlin_noise.h"

#include <math.h>
#include <gl/glew.h>
#include <gl/GL.h>
#include<gl/GLU.h>
#include <gl/glut.h>

#ifdef SPECULAR_FIX
// OPTIONAL: global variable allows (hacky) communication
// with glCanvas::display extern int SPECULAR_FIX_WHICH_PASS;
extern int SPECULAR_FIX_WHICH_PASS;
#endif

class Material {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Material(){}
  Material(const Vec3f &d_color) { diffuseColor = d_color; }
  virtual ~Material() {}

  // ACCESSORS
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }
  virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor) const = 0;
  virtual void glSetMaterial(void) const = 0;
  virtual Vec3f getSpecularColor() const { return Vec3f(0, 0, 0); }
  virtual Vec3f getReflectiveColor() const { return Vec3f(0, 0, 0); }
  virtual Vec3f getTransparentColor() const { return Vec3f(0, 0, 0); }
  virtual float getIndexOfRefraction() const { return 0; }

protected:

  // REPRESENTATION
  Vec3f diffuseColor;
  
};

class PhongMaterial : public Material {
public:

    PhongMaterial(const Vec3f& diffuseColor, const Vec3f& specularColor, float exponent) :Material(diffuseColor), specularColor(specularColor), exponent(exponent)
    {}

    PhongMaterial(const Vec3f& diffuseColor,
        const Vec3f& specularColor,
        float exponent,
        const Vec3f& reflectiveColor,
        const Vec3f& transparentColor,
        float indexOfRefraction):Material(diffuseColor), specularColor(specularColor), exponent(exponent),
        reflectiveColor(reflectiveColor),transparentColor(transparentColor),indexOfRefraction(indexOfRefraction)
    {}

    Vec3f getSpecularColor() const { return specularColor; }
    Vec3f getReflectiveColor() const { return reflectiveColor; }
    Vec3f getTransparentColor() const { return transparentColor; }
    float getIndexOfRefraction() const { return indexOfRefraction; }



	virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor) const
	{
        Vec3f normal = hit.getNormal();
        Vec3f dirToView = ray.getDirection();
        dirToView.Negate();
        Vec3f h = dirToView + dirToLight;
        h.Normalize();
        float cosBeta = normal.Dot3(h);
//#if !SPECULAR_FIX
        if (normal.Dot3(dirToLight)< 0)
        {
            cosBeta = 0;
        }
        //ignore r^2 and ks
        Vec3f specularComponent=powf(cosBeta, exponent)* lightColor*specularColor;
//#else
        /*
        In the examples below we illustrate an artifact that occurs at grazing angles
        for wide specular lobes (small exponents).
        To solve this problem, the specular component can be multiplied by the dot
        product of the normal and direction to the light instead of simply clamping
        it to zero when this dot product is negative.

        */
//        Vec3f specularComponent;
//        if (normal.Dot3(dirToLight) < 0)
//        {
//            specularComponent = powf(cosBeta, exponent) * lightColor * specularColor * normal.Dot3(dirToLight);
//        }
//        else
//        {
//            specularComponent = powf(cosBeta, exponent) * lightColor * specularColor;
//        }
//        //ignore r^2 and ks
//#endif

        float cosTheta = normal.Dot3(dirToLight);
        if (cosTheta < 0)
        {
            cosTheta = 0;
        }
        Vec3f diffuseComponent = cosTheta * lightColor * diffuseColor;

        //no ambientComponent

        return specularComponent + diffuseComponent;


	}

   

// ====================================================================
// Set the OpenGL parameters to render with the given material
// attributes.
// ====================================================================

    void glSetMaterial(void) const
    {

        GLfloat one[4] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat zero[4] = { 0.0, 0.0, 0.0, 0.0 };
        GLfloat specular[4] = { getSpecularColor().r(), getSpecularColor().g(), getSpecularColor().b(), 1.0 };
        GLfloat diffuse[4] = { getDiffuseColor().r(), getDiffuseColor().g(), getDiffuseColor().b(), 1.0 };

        // NOTE: GL uses the Blinn Torrance version of Phong...
        float glexponent = exponent;
        if (glexponent < 0)
            glexponent = 0;
        if (glexponent > 128)
            glexponent = 128;

#if !SPECULAR_FIX

        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

#else

        // OPTIONAL: 3 pass rendering to fix the specular highlight
        // artifact for small specular exponents (wide specular lobe)

        if (SPECULAR_FIX_WHICH_PASS == 0)
        {
            // First pass, draw only the specular highlights
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
        }
        else if (SPECULAR_FIX_WHICH_PASS == 1)
        {
            // Second pass, compute normal dot light
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
        }
        else
        {
            // Third pass, add ambient & diffuse terms
            assert(SPECULAR_FIX_WHICH_PASS == 2);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
        }

#endif 
    }



private:
	//Vec3f diffuseColor;
	Vec3f specularColor;
	float exponent;
    Vec3f reflectiveColor;
    Vec3f transparentColor;
    float indexOfRefraction;
};



//Asssignment6


class Checkerboard :public Material
{
public:
    Checkerboard(Matrix* m, Material* mat1, Material* mat2):matrix(m),material1(mat1),material2(mat2)
    {

    }

    void glSetMaterial(void) const
    {
        material1->glSetMaterial();
    }


    virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor) const
    {
        Vec3f vertex = hit.getIntersectionPoint();
        matrix->Transform(vertex);
        int sum = floor(vertex.x()) + floor(vertex.y()) + floor(vertex.z());
        if (sum%2)
        {
            return material2->Shade(ray, hit, dirToLight, lightColor);
        }
        else
        {
            return material1->Shade(ray, hit, dirToLight, lightColor);
        }
    }

private:
    Matrix* matrix;
    Material* material1;
    Material* material2;
};



class Noise : public Material
{
public:
    Noise(Matrix* m, Material* mat1, Material* mat2, int octaves):matrix(m),material1(mat1),material2(mat2),octaves(octaves)
    {
    }

    void glSetMaterial(void) const
    {
        material1->glSetMaterial();
    }

    virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor) const
    {
        Vec3f vertex = hit.getIntersectionPoint();
        matrix->Transform(vertex);
        float n=N(vertex.x(), vertex.y(), vertex.z(),octaves);
        Vec3f color =n* material1->Shade(ray, hit, dirToLight, lightColor)+ (1-n)*material2->Shade(ray, hit, dirToLight, lightColor);
        color.Clamp();
        return color;
    }

    static float N(float x, float y, float z,int octaves)
    {
        float result=0;
        float pow2 = 1;
        for (int i = 0; i < octaves; i++)
        {
            result += PerlinNoise::noise(pow2 * x, pow2 * y, pow2 * z)/pow2;
            pow2 *= 2;
        }
        return result;
    }
private:
    Matrix* matrix;
    Material* material1;
    Material* material2;
    int octaves;
};

class Marble :public Material
{
public:
    Marble(Matrix* m, Material* mat1, Material* mat2, int octaves, float frequency, float amplitude):matrix(m), material1(mat1), material2(mat2), octaves(octaves),frequency(frequency),amplitude(amplitude)
    {

    }

    void glSetMaterial(void) const
    {
        material1->glSetMaterial();
    }

    virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor) const
    {
        Vec3f vertex = hit.getIntersectionPoint();
        matrix->Transform(vertex);
        float m = M(vertex.x(), vertex.y(), vertex.z());
        Vec3f color = m * material1->Shade(ray, hit, dirToLight, lightColor) + (1 - m) * material2->Shade(ray, hit, dirToLight, lightColor);
        color.Clamp();
        return color;
    }

    float M(float x, float y, float z) const
    {
        return sinf(frequency * x + amplitude * Noise::N(x, y, z, octaves));
    }

private:
    Matrix* matrix;
    Material* material1;
    Material* material2;
    int octaves;
    float frequency;
    float amplitude;

};


class Wood :public Material
{
public:
    Wood(Matrix* m, Material* mat1, Material* mat2, int octaves, float frequency, float amplitude) :matrix(m), material1(mat1), material2(mat2), octaves(octaves), frequency(frequency), amplitude(amplitude)
    {

    }
    void glSetMaterial(void) const
    {
        material1->glSetMaterial();
    }

    virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor) const
    {
        return Vec3f(1, 1, 1);
    }

private:
    Matrix* matrix;
    Material* material1;
    Material* material2;
    int octaves;
    float frequency;
    float amplitude;


};

// ====================================================================
// ====================================================================

#endif