
#include "AreaLight.h"
#include <cstdlib>

AreaLight::AreaLight(const Point& position, const Color& color, const Vector& a, const Vector& b, const Color& emit)
  : cornerPos(position), color(color), a(a), b(b)
{
    emission = emit;
}

AreaLight::~AreaLight()
{
}

void AreaLight::preprocess()
{
}

double AreaLight::getLight(Color& light_color, Vector& light_direction,
                            const RenderContext&, const Point& hitpos) const
{
    // generate sample point on the light
    Point samplePt = makeRandomPoint();

    light_color = color;
    Vector dir = samplePt - hitpos;
    double len = dir.normalize();
    light_direction = dir;
    return len;
}

Point AreaLight::makeRandomPoint() const
{
    double rand1 = double(rand()) / double(RAND_MAX);
    double rand2 = double(rand()) / double(RAND_MAX);
    Point samplePt = cornerPos + rand1 * a + rand2 * b;
    return samplePt;
}

Vector AreaLight::makeNormal(Point basePt)
{
    Point pt1 = basePt + a;
    Point pt2 = basePt + b;

    Vector vec1 = pt1 - basePt;
    Vector vec2 = pt2 - basePt;
    return Cross(vec1, vec2);
}

bool AreaLight::intersect(const Ray& ray, Point& hitpos)
{
    Vector normal = makeNormal(cornerPos);

    double denom = Dot(ray.direction(), normal);
    if (Abs(denom) < 1.e-6 * -1) {
        float rayLen = (Dot(cornerPos - ray.origin(), normal) / denom);
        hitpos = ray.origin() + rayLen * ray.direction();
        Vector intVec = hitpos - cornerPos;

        // project vector onto sides of the light
        float lenProj1 = Dot(intVec, a) / a.length();
        float lenProj2 = Dot(intVec, b) / b.length();

        if (0 <= lenProj1 && lenProj1 <= a.length() && 0 <= lenProj2 && lenProj2 <= b.length()) {
            return true;
        }
    }
    return false;
}
