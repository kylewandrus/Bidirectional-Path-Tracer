
#include "GlassMaterial.h"
#include "HitRecord.h"
#include "Light.h"
#include "Point.h"
#include "Primitive.h"
#include "Ray.h"
#include "RenderContext.h"
#include "Scene.h"
#include "Vector.h"
#include "Math.h"
using namespace std;

GlassMaterial::GlassMaterial(const Color& color, float Kd, float Ka, float Ks, int n, bool isReflective)
  :color(color), Kd(Kd), Ka(Ka), Ks(Ks), n(n), isReflective(isReflective)
{
}

GlassMaterial::~GlassMaterial()
{
}

bool GlassMaterial::getReflective() const {
    return isReflective;
}

Color GlassMaterial::getColor() const {
    return color;
}

float GlassMaterial::getKs() const {
    return Ks;
}

Color GlassMaterial::shade(const RenderContext& context,
                               const Ray& ray, const HitRecord& hit, const Color&, int) const
{
    const Scene* scene = context.getScene();
    const vector<Light*>& lights = scene->getLights();
    Point hitpos = ray.origin() + ray.direction() * hit.minT();

    // compute normal at hitpos
    Vector normal;
    hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);
    normal.normalize();
    double costheta = Dot(normal, ray.direction());
    if(costheta > 0)
        normal = -normal;

    const Object* world = scene->getObject();

    float na = 1.0; // index of refraction for air
    float ng = 1.5; // index of refraction for glass

    //Color light = scene->getAmbient()*Ka;

#if 0
    for(vector<Light*>::const_iterator iter = lights.begin(); iter != lights.end(); iter++){
#else
    Light*const* begin = &lights[0];
    Light*const* end = &lights[0] + lights.size();
    while(begin != end){
#endif
        // compute direction vector from light to hitpos
        Color light_color;
        Vector light_direction;
        double dist = (*begin++)->getLight(light_color, light_direction, context, hitpos);
        light_direction.normalize();
        double cosphi = Dot(normal, light_direction);

        Vector reflect_direction = 2 * Dot(normal, light_direction) * normal - light_direction;
        reflect_direction.normalize();

        // multiply ray direction by -1 to use direction from hitpos back to eye
        double cosbet = Dot(reflect_direction, ray.direction() * -1);

        if(cosphi > 0){
            // Cast shadow rays...
            HitRecord shadowhit(dist);
            Ray shadowray(hitpos, light_direction);
            world->intersect(shadowhit, context, shadowray);

            if (!shadowhit.getPrimitive()) {
                // No shadows...
                if (cosbet > 0) {
                    light += light_color * (Kd * cosphi + Ks * pow(cosbet, n));
                }
                else {
                    light += light_color * (Kd * cosphi);
                }
            }
        }
    }

    return light * color;
}

void GlassMaterial::scatter(Point hitpos, Vector normal, Ray& scattered, float& pdf) const
{
     
}