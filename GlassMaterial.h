
#ifndef GlassMaterial_h
#define GlassMaterial_h

#include "Material.h"
#include "Color.h"
#include "Point.h"
#include "Ray.h"

class GlassMaterial : public Material {
 public:
  GlassMaterial(const Color& color, float Kd, float Ka, float Ks, int n, bool isReflective);
  virtual ~GlassMaterial();

  virtual Color shade(const RenderContext& context, const Ray& ray,
                     const HitRecord& hit, const Color& atten, int depth) const;
  virtual bool getReflective() const;
  virtual Color getColor() const;
  virtual float getKs() const;

  virtual void scatter(Point hitpos, Vector normal, Ray& scattered, float& pdf) const;

 private:
     GlassMaterial(const GlassMaterial&);
     GlassMaterial& operator=(const GlassMaterial&);

  Color color;
  float Kd;
  float Ka;
  float Ks;
  int n;
  bool isReflective;
};

class onb
{
public:
    onb() {}
    inline Vector operator[](int i) const { return axis[i]; }
    Vector u() const { return axis[0]; }
    Vector v() const { return axis[1]; }
    Vector w() const { return axis[2]; }
    Vector local(float a, float b, float c) const { return a * u() + b * v() + c * w(); }
    Vector local(const Vector& a) const { return a.x() * u() + a.y() * v() + a.z() * w(); }
    void build_from_w(Vector&);
    Vector axis[3];
};

#endif
