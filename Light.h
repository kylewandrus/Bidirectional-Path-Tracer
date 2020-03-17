
#ifndef Light_h
#define Light_h

#include "Color.h"

class Color;
class Point;
class Ray;
class RenderContext;
class Vector;
class HitRecord;

class Light {
 public:
  Light();
  virtual ~Light();

  virtual void preprocess();
  virtual double getLight(Color& light_color, Vector& light_direction,
                          const RenderContext& context, const Point& pos) const = 0;
  virtual Point makeRandomPoint() const = 0;
  virtual Vector makeNormal(Point basePt) = 0;
  Color getEmission() const {
      return emission;
  }
  virtual Color getColor() const = 0;
  //virtual Color areaShade(const Ray& ray, Vector normal) const = 0;
  virtual bool intersect(const Ray& ray, Point& hitpos) = 0;

 private:
  Light(const Light&);
  Light& operator=(const Light&);
protected:
  Color emission;
};

#endif

