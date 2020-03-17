
#ifndef AreaLight_h
#define AreaLight_h

#include "Light.h"
#include "Point.h"
#include "Color.h"
#include "Ray.h"

class AreaLight : public Light {
 public:
  AreaLight(const Point& position, const Color& color, const Vector& a, const Vector& b, const Color& emit);
  virtual ~AreaLight();

  virtual void preprocess();
  virtual double getLight(Color& light_color, Vector& light_direction,
                          const RenderContext& context, const Point& pos) const;
  virtual Point makeRandomPoint() const;
  virtual Vector makeNormal(Point basePt);
  virtual bool intersect(const Ray& ray, Point& hitpos);

  virtual Color getColor() const {
      return color;
  }

  //Color areaShade(const Ray& ray, Vector normal) const;

 private:
  AreaLight(const AreaLight&);
  AreaLight& operator=(const AreaLight&);

  Point cornerPos;
  Color color;
  Vector a, b; // represent dimensions of rectangle
};

#endif

