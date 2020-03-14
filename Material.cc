
#include "Material.h"

Material::Material()
{
}

Material::~Material()
{
}

void Material::preprocess()
{
}

float Material::scattering_pdf(const Vector& normal, const Ray& scattered) const
{
    Vector scatteredDir = scattered.direction();
    scatteredDir.normalize();
    float cosine = Dot(normal, scatteredDir);
    if (cosine < 0)
        return 0;
    return cosine / M_PI;
}