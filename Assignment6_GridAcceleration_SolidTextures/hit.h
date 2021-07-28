#ifndef _HIT_H
#define _HIT_H

#include "vectors.h"
#include "ray.h"

class Material;

// ====================================================================
// ====================================================================

class Hit {
  
public:

  // CONSTRUCTOR & DESTRUCTOR
    Hit() 
    {
        material = NULL; 
        t = INFINITY;
        normal = Vec3f(0, 0, 0);
    }
  Hit(float _t, Material *m, Vec3f n) { 
    t = _t; material = m; normal = n; }
  Hit(const Hit &h) { 
    t = h.t; 
    material = h.material; 
    normal = h.normal; 
    intersectionPoint = h.intersectionPoint; }
  ~Hit() {}

  // ACCESSORS
  float getT() const { return t; }
  Material* getMaterial() const { return material; }
  Vec3f getNormal() const { return normal; }
  Vec3f getIntersectionPoint() const { return intersectionPoint; }
  Ray getRay()const { return ray; }
  
  // MODIFIER
  void set(float _t, Material *m, Vec3f n, const Ray &ray) {
    t = _t; material = m; normal = n; 
    this->ray = ray;
    intersectionPoint = ray.pointAtParameter(t); 
  }

  void set(Hit temph)
  {
      t = temph.getT();
      material = temph.getMaterial();
      normal = temph.getNormal();
      ray = temph.getRay();
      intersectionPoint = temph.getIntersectionPoint();
  }

  //void set(float _t, const Ray& ray)
  //{
  //    t = _t; 
  //    this->ray = ray;
  //    intersectionPoint = ray.pointAtParameter(t);
  //}
  //void setBackgroundMaterial(Material* m)
  //{
  //    material = m;
  //}

  
  //void setT(float _t)
  //{
  //    t = _t;
  //}
  //void setNormal(Vec3f norm)
  //{
  //    normal = norm;
  //}

private: 

  // REPRESENTATION
  float t;
  Material *material;
  Vec3f normal;
  Vec3f intersectionPoint;
  Ray ray;

};

inline ostream &operator<<(ostream &os, const Hit &h) {
  os << "Hit <" <<h.getT()<<", "<<h.getNormal()<<">";
  return os;
}
// ====================================================================
// ====================================================================

#endif