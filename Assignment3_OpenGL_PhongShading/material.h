#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
// include glCanvas.h to access the preprocessor variable SPECULAR_FIX
#include "glCanvas.h"
#include "hit.h"
#include "vectors.h"
#include "ray.h"

#ifdef SPECULAR_FIX
// OPTIONAL: global variable allows (hacky) communication
// with glCanvas::display extern int SPECULAR_FIX_WHICH_PASS;
#endif

class Material {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Material(const Vec3f &d_color) { diffuseColor = d_color; }
  virtual ~Material() {}

  // ACCESSORS
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }
  virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor) const = 0;
  virtual void glSetMaterial(void) const = 0;

protected:

  // REPRESENTATION
  Vec3f diffuseColor;
  
};

class PhongMaterial : public Material {
public:
	PhongMaterial(const Vec3f& diffuseColor, const Vec3f& specularColor, float exponent) :Material(diffuseColor), specularColor(specularColor), exponent(exponent)
	{}
	virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor) const
	{
        Vec3f normal = hit.getNormal();
        Vec3f dirToView = ray.getDirection();
        dirToView.Negate();
        Vec3f h = dirToView + dirToLight;
        h.Normalize();
        float cosBeta = normal.Dot3(h);
        if (cosBeta < 0)
        {
            cosBeta = 0;
        }
        //ignore r^2 and ks
        Vec3f specularComponent=powf(cosBeta, exponent)* lightColor*specularColor;

        float cosTheta = normal.Dot3(dirToLight);
        if (cosTheta < 0)
        {
            cosTheta = 0;
        }
        Vec3f diffuseComponent = cosTheta * lightColor * diffuseColor;

        //no ambientComponent

        return specularComponent + diffuseComponent;
	}

    Vec3f getSpecularColor() const { return specularColor; }

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

};


// ====================================================================
// ====================================================================

#endif