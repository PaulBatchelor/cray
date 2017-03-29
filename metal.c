#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "hitablelist.h"
#include "material.h"
#include "rand.h"

static int metal(cray_material *m, 
        cray_hitable *hit, 
        cray_ray *r_in, 
        vec3 *attenuation, 
        cray_ray *scattered)
{
    vec3 reflected;
    vec3 fuzz;
    CRAYFLT tmp; 
    cray_metal *met;

    met = m->ud;
    VEC3_UNIT_VECTOR(cray_ray_direction(r_in), tmp, reflected);
    reflected = cray_reflect(&reflected, &hit->normal);
    fuzz = random_in_unit_sphere();
    VEC3_MULS(fuzz, met->fuzz, fuzz);
    VEC3_ADD(reflected, fuzz, reflected);
    RAY_SET(*scattered, hit->p, reflected);
    *attenuation = met->albedo;
    return (VEC3_DOT(cray_ray_direction(scattered), hit->normal) > 0);
}

void cray_metal_init(cray_metal *m)
{
    cray_material_init(&m->mat);
    m->mat.type = CRAY_METAL;
    m->mat.ud = m;
    m->mat.scatter = metal;
    VEC3_SET(m->albedo, 1.0, 1.0, 1.0);
    m->fuzz = 0.0;
}

void cray_metal_color(cray_metal *m, CRAYFLT r, CRAYFLT g, CRAYFLT b)
{
    VEC3_SET(m->albedo, r, g, b);
}

void cray_metal_fuzz(cray_metal *m, CRAYFLT fuzz)
{
    m->fuzz = fuzz;
}
