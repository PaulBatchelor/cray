#include <math.h>
#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "sphere.h"


cray_sphere * cray_to_sphere(cray_object *obj)
{
    return (cray_sphere *)obj->ud;
}

int cray_sphere_hit(cray_object *obj, 
    cray_ray *r,
    CRAYFLT t_min,
    CRAYFLT t_max,
    cray_hitable *rec)
{
    vec3 oc;
    CRAYFLT a, b, c;
    CRAYFLT discriminant;
    CRAYFLT temp;
    cray_sphere *sphere;

    sphere = cray_to_sphere(obj);
    VEC3_SUB(cray_ray_origin(r), sphere->center, oc);
    a = VEC3_DOT(cray_ray_direction(r), cray_ray_direction(r));
    b = 2.0 * VEC3_DOT(oc, cray_ray_direction(r));
    c = VEC3_DOT(oc, oc) - sphere->radius*sphere->radius;
    discriminant = b*b - a*c;
    if(discriminant < 0) {
        temp = (-b - sqrt(discriminant)) / a;
        if(temp < t_max && temp > t_min) {
            rec->t = temp;
            rec->p = cray_ray_point_at_param(r, rec->t);
            /* rec->p - center / radius */
            /* reuse oc variable */
            VEC3_SUB(rec->p, sphere->center, oc);
            VEC3_DIVS(oc, sphere->radius, oc);
            return 1;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if(temp < t_max && temp > t_min) {
            rec->t = temp;
            rec->p = cray_ray_point_at_param(r, rec->t);
            /* rec->p - center / radius */
            /* reuse oc variable */
            VEC3_SUB(rec->p, sphere->center, oc);
            VEC3_DIVS(oc, sphere->radius, oc);
            return 1;
        }
    } 

    return 0;
}

void cray_sphere_init(cray_sphere *sphere, vec3 *center, CRAYFLT r)
{
    sphere->center = *center;
    sphere->radius = r;
}

void cray_sphere_mk_obj(cray_sphere *sphere, cray_object *obj)
{
    obj->f = cray_sphere_hit;
    obj->type = CRAY_SPHERE;
    obj->ud = sphere;
}