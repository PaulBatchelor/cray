#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "hitablelist.h"
#include "material.h"
#include "rand.h"

static int lambertian (cray_material *m, 
        cray_hitable *hit, 
        cray_ray *ray_in, 
        vec3 *attenuation, 
        cray_ray *scattered)
{
    vec3 tmp[2];

    /* target = rec.p + rec.normal + random_in_unit_sphere() */
    VEC3_ADD(hit->p, hit->normal, tmp[0]);
    tmp[1] = random_in_unit_sphere();
    VEC3_ADD(tmp[0], tmp[1], tmp[0]);
    /* target - rec.p */
    VEC3_SUB(tmp[0], hit->p, tmp[0]);
    RAY_SET(*scattered, hit->p, tmp[0]);

    /* *attenuation = l->albedo; */

    m->value(m, 0, 0, &hit->p, attenuation);

    return 1;
}

void cray_lambertian_init(cray_lambertian *l)
{
    cray_material_init(&l->mat);
    l->mat.type = CRAY_LAMBERTIAN;
    l->mat.ud = l;
    l->mat.scatter = lambertian;
}

void cray_lambertian_color(cray_lambertian *l, CRAYFLT r, CRAYFLT g, CRAYFLT b)
{
    cray_material_albedo(&l->mat, r, g, b);
}
