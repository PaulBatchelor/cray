#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "hitablelist.h"
#include "material.h"
#include "rand.h"

static vec3 random_in_unit_sphere() {
    vec3 p;
    vec3 tmp[2];
    do {
        VEC3_SET(tmp[0], cray_rand(), cray_rand(), cray_rand());
        VEC3_MULS(tmp[0], 2.0, tmp[0]);

        VEC3_SET(tmp[1], 1, 1, 1);
        VEC3_SUB(tmp[0], tmp[1], p);
    } while(VEC3_SQUARED_LENGTH(p) >= 1.0);

    return p;
}

static int lambertian (cray_material *m, 
        cray_hitable *hit, 
        cray_ray *ray_in, 
        vec3 *attenuation, 
        cray_ray *scattered)
{
    vec3 tmp[2];
    cray_lambertian *l;

    l = m->ud;
    /* target = rec.p + rec.normal + random_in_unit_sphere() */
    VEC3_ADD(hit->p, hit->normal, tmp[0]);
    tmp[1] = random_in_unit_sphere();
    VEC3_ADD(tmp[0], tmp[1], tmp[0]);
    /* target - rec.p */
    VEC3_SUB(tmp[0], hit->p, tmp[0]);
    RAY_SET(*scattered, hit->p, tmp[0]);

    *attenuation = l->albedo;

    return 1;
}

void cray_lambertian_make(cray_lambertian *l)
{
    cray_material_init(&l->mat);
    l->mat.type = CRAY_LAMBERTIAN;
    l->mat.ud = l;
    l->mat.scatter = lambertian;
    VEC3_SET(l->albedo, 1.0, 1.0, 1.0);
}

void cray_lambertian_color(cray_lambertian *l, CRAYFLT r, CRAYFLT g, CRAYFLT b)
{
    VEC3_SET(l->albedo, r, g, b);
}
