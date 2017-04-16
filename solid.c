#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "hitablelist.h"
#include "material.h"

static int solid(cray_material *m, 
        cray_hitable *hit, 
        cray_ray *r_in, 
        vec3 *attenuation, 
        cray_ray *scattered)
{
    cray_solid *s;
    s = m->ud;
    RAY_SET(*scattered, hit->p, hit->normal);
    *attenuation = s->albedo;
    return 1;
}

void cray_solid_init(cray_solid *s)
{
    cray_material_init(&s->mat);
    s->mat.type = CRAY_SOLID;
    s->mat.ud = s;
    s->mat.scatter = solid;
    VEC3_SET(s->albedo, 1.0, 1.0, 1.0);
}

void cray_solid_color(cray_solid *s, CRAYFLT r, CRAYFLT g, CRAYFLT b)
{
    VEC3_SET(s->albedo, r, g, b);
}
