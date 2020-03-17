
#ifndef Material_h
#define Material_h

#include "Point.h"
#include "Ray.h"
#include "Light.h"

class Color;
class HitRecord;
class Ray;
class RenderContext;

enum MatType {LAMBERTIAN, GLASS};

class Material {
 public:
  Material();
  virtual ~Material();

  virtual void preprocess();
  virtual Color shade(const RenderContext& context, const Ray& ray, const HitRecord& hit, 
      const Light* light) = 0;

  virtual bool getReflective() const = 0;
  virtual Color getColor() const = 0;
  virtual float getKs() const = 0;

  virtual void scatter(Point hitpos, Vector normal, Ray& scattered, float& pdf) const = 0;
  virtual float scattering_pdf(const Vector& normal, const Ray& scattered) const;

  virtual Vector emitted(float u, float v, const Vector& p) const {
      return Vector(0, 0, 0);
  }

  void setType(MatType matType) {
      type = matType;
  }
  bool isType(MatType matType) const {
      return type == matType;
  }

 private:
  Material(const Material&);
  Material& operator=(const Material&);

  MatType type;
};

#endif

