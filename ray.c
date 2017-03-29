#include "vec3.h"
#include "ray.h"

vec3 cray_ray_origin(cray_ray *ray)
{
    return ray->A;
}

vec3 cray_ray_direction(cray_ray *ray)
{

    return ray->B;
}

vec3 cray_ray_point_at_param(cray_ray *ray, CRAYFLT t)
{
    vec3 tmp;
    /* A + t*B */
    VEC3_MULS(ray->B, t, tmp);
    VEC3_ADD(ray->A, tmp, tmp);
    return tmp;
}
