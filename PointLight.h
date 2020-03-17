
#ifndef PointLight_h
#define PointLight_h

#include "Light.h"
#include "Point.h"
#include "Color.h"

class PointLight : public Light {
 public:
  PointLight(const Point& position, const Color& color, const Color& emit);
  virtual ~PointLight();

  virtual void preprocess();
  virtual double getLight(Color& light_color, Vector& light_direction,
                          const RenderContext& context, const Point& pos) const;

  virtual Point makeRandomPoint() const { return Point(-1, -1, -1); }
  virtual Vector makeNormal(Point basePt) { return Vector(-1, -1, -1); }
  virtual bool intersect(const Ray& ray, Point& hitpos) { return false; }

  virtual Color getColor() const {
      return color;
  }

 private:
  PointLight(const PointLight&);
  PointLight& operator=(const PointLight&);

  Point position;
  Color color;
};

#endif

