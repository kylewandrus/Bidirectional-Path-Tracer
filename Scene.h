
#ifndef Scene_h
#define Scene_h

#include "Color.h"
#include "PathNode.h"
#include <string>
#include <vector>

using namespace std;

class Background;
class Camera;
class Color;
class Image;
class Light;
class Object;
class RenderContext;
class Ray;

class Scene {
 public:
  Scene();
  virtual ~Scene();

  inline Object* getObject() const {
    return object;
  }
  void setObject(Object* obj) {
    object = obj;
  }

  inline Background* getBackground() const {
    return background;
  }
  void setBackground(Background* bg) {
    background = bg;
  }

  inline Camera* getCamera() const {
    return camera;
  }
  void setCamera(Camera* cam) {
    camera = cam;
  }

  inline Image* getImage() const {
    return image;
  }
  void setImage(Image* im) {
    image = im;
  }

  void addLight(Light* light) {
    lights.push_back(light);
  }
  const std::vector<Light*>& getLights() const {
    return lights;
  }

  Color getAmbient() const {
    return ambient;
  }
  void setAmbient(const Color& amb) {
    ambient = amb;
  }

  int getMaxRayDepth() const {
    return maxRayDepth;
  }
  void setMaxRayDepth(int rd) {
    maxRayDepth = rd;
  }
  void setMinRayDepth(int rd) {
      minRayDepth = rd;
  }
  double getMinAttenuation() const {
    return minAttenuation;
  }
  void setMinAttenuation(double atten) {
    minAttenuation = atten;
  }
  void setNumSamples(int num) {
      numSamples = num;
  }

  void preprocess();
  void render();
  Color backwardTraceRay(const RenderContext& context, Ray& ray, vector<PathNode*> lightPath) const;
  vector<PathNode*> forwardTraceRay(const RenderContext& context) const;

  Color connectPaths(const RenderContext& context, const Point& hitpos, const Vector& normal, vector<PathNode*> lightPath) const;

 private:
  Scene(const Scene&);
  Scene& operator=(const Scene&);

  Background* background;
  Camera* camera;
  Color ambient;
  Image* image;
  Object* object;
  std::vector<Light*> lights;
  int maxRayDepth;
  int minRayDepth;
  int numSamples;
  double minAttenuation;
};

#endif
