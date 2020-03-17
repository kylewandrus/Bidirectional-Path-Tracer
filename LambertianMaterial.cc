
#include "LambertianMaterial.h"
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

LambertianMaterial::LambertianMaterial(const Color& color, float Kd, float Ka, float Ks, int n, bool isReflective)
  :color(color), Kd(Kd), Ka(Ka), Ks(Ks), n(n), isReflective(isReflective)
{
    setType(LAMBERTIAN);
}

LambertianMaterial::~LambertianMaterial()
{
}

bool LambertianMaterial::getReflective() const {
    return isReflective;
}

Color LambertianMaterial::getColor() const {
    return color;
}

float LambertianMaterial::getKs() const {
    return Ks;
}

Color LambertianMaterial::shade(const RenderContext& context,
                               const Ray& ray, const HitRecord& hit, const Light* light)
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

    Color result = scene->getAmbient()*Ka;

//#if 0
//    for(vector<Light*>::const_iterator iter = lights.begin(); iter != lights.end(); iter++){
//#else
//    Light*const* begin = &lights[0];
//    Light*const* end = &lights[0] + lights.size();
//    while(begin != end){
//#endif

    // compute direction vector from light to hitpos
    Color light_color;
    Vector light_direction;
    double dist = light->getLight(light_color, light_direction, context, hitpos);
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
                result += light_color * (Kd * cosphi + Ks * pow(cosbet, n));
            }
            else {
                result += light_color * (Kd * cosphi);
            }
        }
    }
    //}

    return result * color;
}

// source for the following functions: https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html#importancesamplingmaterials

void LambertianMaterial::scatter(Point hitpos, Vector normal, Ray& scattered, float& pdf) const
{
    // generate 3 random numbers between -1 and 1
    double rand1 = double(rand()) / double(RAND_MAX);
    rand1 = -1.0 + 2.0 * rand1;
    double rand2 = double(rand()) / double(RAND_MAX);
    rand2 = -1.0 + 2.0 * rand2;
    double rand3 = double(rand()) / double(RAND_MAX);
    rand3 = -1.0 + 2.0 * rand3;
    Vector randVec = Vector(rand1, rand2, rand3);

    Vector point = Vector(hitpos);
    Vector target = normal + point + randVec;
    Vector dir = Vector(target - point);
    dir.normalize();
    scattered = Ray(hitpos, dir);
    pdf = Dot(normal, scattered.direction()) / M_PI;
}
