#ifndef _RAY_H
#define _RAY_H

#include <iostream>
using namespace std;

#include "vectors.h"

// ====================================================================
// ====================================================================

// Ray class mostly copied from Peter Shirley and Keith Morley

class Ray {

public:

  // CONSTRUCTOR & DESTRUCTOR
  Ray () {}
  Ray (const Vec3f &orig, const Vec3f &dir) {
    origin = orig; 
    direction = dir; 
    inverseDirection.Set(1/direction.x(),1/direction.y(),1/direction.z());
    for (int i = 0; i < 3; i++)
    {
        sign[i] = direction[i] > 0 ? 1 : (direction[i] < 0 ? -1 : 0);
    }

  }
  Ray (const Ray& r) {*this=r;}

  // ACCESSORS
  const Vec3f& getOrigin() const { return origin; }
  const Vec3f& getDirection() const { return direction; }
  const Vec3f& getInverseDirection() const { return inverseDirection; }
  const void getSign(int* rsign)const 
  { 
      rsign[0] = sign[0];
      rsign[1] = sign[1];
      rsign[2] = sign[2];
  }
  Vec3f pointAtParameter(float t) const {
    return origin+direction*t; }

private:

  // REPRESENTATION
  Vec3f origin;
  Vec3f direction;
  Vec3f inverseDirection;
  int sign[3];
};

inline ostream &operator<<(ostream &os, const Ray &r) {
  os << "Ray <o:" <<r.getOrigin()<<", d:"<<r.getDirection()<<">";
  return os;
}

// ====================================================================
// ====================================================================

#endif