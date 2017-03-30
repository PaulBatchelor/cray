#include <math.h>
#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "hitablelist.h"
#include "material.h"
#include "rand.h"

static CRAYFLT schlick(CRAYFLT cosine, CRAYFLT ref_idx)
{
    CRAYFLT r0;
    r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}

static int refract(vec3 *v, vec3 *n, CRAYFLT ni_over_nt, vec3 *refracted)
{
    vec3 uv;
    vec3 tmp;
    CRAYFLT var;
    CRAYFLT dt;
    CRAYFLT discriminant;
    
    VEC3_UNIT_VECTOR(*v, var, uv);
    dt = VEC3_DOT(uv, *n);
    discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);

    if(discriminant > 0) {
        /*refracted = ni_over_nt*(uv-n*dt)-n*sqrt(discriminant)*/ 
        VEC3_MULS(*n, dt, tmp);
        VEC3_SUB(uv, tmp, tmp);
        VEC3_MULS(tmp, ni_over_nt, tmp);
        var = cray_sqrtf(discriminant);
        VEC3_MULS(*n, var, *refracted);
        VEC3_SUB(tmp, *refracted, *refracted);
        return 1;
    } else {
        return 0;
    }

}

static int dielectric(cray_material *m, 
        cray_hitable *hit, 
        cray_ray *r_in, 
        vec3 *attenuation, 
        cray_ray *scattered)
{
    vec3 outward_normal;
    vec3 reflected;
    vec3 refracted;
    vec3 ray_dir;
    CRAYFLT ni_over_nt;
    CRAYFLT cosine;
    CRAYFLT reflect_prob;
    cray_dielectric *d;
   
    d = m->ud;

    VEC3_SET(*attenuation, 1.0, 1.0, 1.0);

    ray_dir = cray_ray_direction(r_in);

    reflected = cray_reflect(&ray_dir, &hit->normal);

    if(VEC3_DOT(ray_dir, hit->normal) > 0) {
        VEC3_MULS(hit->normal, -1.0, outward_normal);
        ni_over_nt = d->ref_idx;
        cosine = d->ref_idx * VEC3_DOT(ray_dir, hit->normal);
    } else {
        outward_normal = hit->normal;
        ni_over_nt = 1.0 / d->ref_idx;
        cosine = -VEC3_DOT(ray_dir, hit->normal) / VEC3_LENGTH(ray_dir);
    }


    if(refract(&ray_dir, &outward_normal, ni_over_nt, &refracted)) {
        reflect_prob = schlick(cosine, d->ref_idx);
    } else {
        reflect_prob = 1.0;
    }

    if(cray_rand() < reflect_prob) {
        RAY_SET(*scattered, hit->p, reflected);
    } else {
        RAY_SET(*scattered, hit->p, refracted);
    }

    return 1;
}

void cray_dielectric_init(cray_dielectric *d)
{
    cray_material_init(&d->mat);
    d->mat.type = CRAY_DIELECTRIC;
    d->mat.ud = d;
    d->mat.scatter = dielectric;
    d->ref_idx = 1.5;
}

void cray_dielectric_refraction(cray_dielectric *d, CRAYFLT ref_idx)
{
    d->ref_idx = ref_idx;
}
