#ifndef _SceneParser_H_
#define _SceneParser_H_
//#include "vectors.h"
//#include <assert.h>
//#include "image.h"
//#include "hit.h"
#include "camera.h"
#include "light.h"
#include "group.h"
//#include "material.h"
//#include "object3d.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "transform.h"

#include "vectors.h"
#include <assert.h>

class Camera;
class Light;
class Material;
class Object3D;
class Group;
class Sphere;
class Plane;
class Triangle;
class Transform;





#define MAX_PARSER_TOKEN_LENGTH 100

// ====================================================================
// ====================================================================

class SceneParser {

public:

  // CONSTRUCTOR & DESTRUCTOR
  SceneParser(const char *filename);
  ~SceneParser();

  // ACCESSORS
  //Camera* getCamera() const { return camera; }
  //Vec3f getBackgroundColor() const { return background_color; }
  //Vec3f getAmbientLight() const { return ambient_light; }
  //int getNumLights() const { return num_lights; }
  //Light* getLight(int i) const { 
  //  assert(i >= 0 && i < num_lights); 
  //  return lights[i]; }  
  //int getNumMaterials() const { return num_materials; }
  //Material* getMaterial(int i) const { 
  //  assert(i >= 0 && i < num_materials); 
  //  return materials[i]; }  
  //Group* getGroup() const { return group; }

  //Material* getBackgroundMaterial() const
  //{
  //    assert(background_material != NULL);
  //    return background_material;
  //};

  Camera* getCamera() const;
  Vec3f getBackgroundColor() const;
  Vec3f getAmbientLight() const;
  int getNumLights() const;
  Light* getLight(int i) const;
  int getNumMaterials() const;
  Material* getMaterial(int i) const;
  Group* getGroup() const;

  Material* getBackgroundMaterial() const;

private:

  SceneParser() { assert(0); } // don't use

  // PARSING
  void parseFile();
  void parseOrthographicCamera();
  void parsePerspectiveCamera();
  void parseBackground();
  void parseLights();
  Light* parseDirectionalLight();
  void parseMaterials();
  Material* parseMaterial();

  Object3D* parseObject(char token[MAX_PARSER_TOKEN_LENGTH]);
  Group* parseGroup();
  Sphere* parseSphere();
  Plane* parsePlane();
  Triangle* parseTriangle();
  Group* parseTriangleMesh();
  Transform* parseTransform();

  // HELPER FUNCTIONS
  int getToken(char token[MAX_PARSER_TOKEN_LENGTH]);
  Vec3f readVec3f();
  Vec2f readVec2f();
  float readFloat();
  int readInt();

  // ==============
  // REPRESENTATION
  FILE *file;
  Camera *camera;
  Vec3f background_color;
  Vec3f ambient_light;
  int num_lights;
  Light **lights;
  int num_materials;
  Material **materials;
  Material *current_material;
  Group *group;

  Material* background_material;


};

// ====================================================================
// ====================================================================

#endif