
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

#define ATTEN 0.001

GlassMaterial::GlassMaterial(const Color& color, float Kd, float Ka, float Ks, int n, bool isReflective)
  :color(color), Kd(Kd), Ka(Ka), Ks(Ks), n(n), isReflective(isReflective)
{
    setType(GLASS);
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
                               const Ray& ray, const HitRecord& hit, const Light* light)
{
    // relfection is used for color
    // refraction is used for color and next ray direction

    Color result(0, 0, 0);
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

    // compute direction vector from light to hitpos
    Color light_color;
    Vector light_direction;
    double dist = light->getLight(light_color, light_direction, context, hitpos);
    light_direction.normalize();
    double cosphi = Dot(normal, light_direction);

    // compute reflection
    Vector reflect_direction = 2 * Dot(normal, light_direction) * normal - light_direction;
    reflect_direction.normalize();
    
    // compute fresnel and assign indices of refraction
    float n1, n2;
    float kr = computeFresnel(ray.direction(), normal, n1, n2);

    if (kr < 1) {
        // compute refraction
        float cost = Dot(-1 * ray.direction(), normal);
        float sint = pow(n1 / n2, 2) * (1 - pow(cost, 2));
        Vector refraction_direction = (n1 / n2) * ray.direction() + ((n1 / n2) * cost - sqrt(1 - pow(sint, 2))) * normal;
        Point refractPos;

        // check if ray is inside glass or not
        if (Dot(ray.direction(), normal) < 0) {
            refractPos = hitpos - (ATTEN * normal);
        }
        else {
            refractPos = hitpos + (ATTEN * normal);
        }
        refractionRay = Ray(refractPos, refraction_direction);
    }
    

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
                //light += light_color * (Kd * cosphi + Ks * pow(cosbet, n));
            }
            else {
                //light += light_color * (Kd * cosphi);
            }
        }
    }

    return result * color;
}

void GlassMaterial::scatter(Point hitpos, Vector normal, Ray& scattered, float& pdf) const
{
    scattered = refractionRay;
}

float GlassMaterial::computeFresnel(const Vector& incident, const Vector& normal, float& n1, float& n2)
{
    // source: https://www.sc ratchapixel.com/lessons/3d-basic-rendering/introdu ction-to-shading/reflection-refraction-fresnel

    // n1 == incident index of refraction
    // n2 == refracted index of refraction

    float cosi = fmax(-1.0f, fmin(1.0f, Dot(incident, normal)));

    // Compute sini using Snell's law
    // air = 1.0, glass = 1.5
    if (cosi > 0) {
        n1 = 1.5;
        n2 = 1.0;
    }
    else {
        n1 = 1.0;
        n2 = 1.5;
    }
    float sint = n1 / n2 * sqrtf(fmax(0.f, 1 - cosi * cosi));

    // Total internal reflection
    if (sint >= 1) {
        return 1.0f;
    }
    else {
        float cost = sqrtf(fmax(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((n2 * cosi) - (n1 * cost)) / ((n2 * cosi) + (n1 * cost));
        float Rp = ((n1 * cosi) - (n2 * cost)) / ((n1 * cosi) + (n2 * cost));
        return (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, transmittance is given by:
    // kt = 1 - kr;
}