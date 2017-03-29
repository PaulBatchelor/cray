#include "vec3.h"
#include "ray.h"
#include "camera.h"

void cray_camera_init(cray_camera *cam)
{
    VEC3_SET(cam->lower_left_corner, -2.0, -1.0, -1.0);
    VEC3_SET(cam->horizontal, 4.0, 0.0, 0.0);
    VEC3_SET(cam->vertical, 0.0, 2.0, 0.0);
    VEC3_SET(cam->origin, 0.0, 0.0, 0.0);
}

cray_ray cray_camera_get_ray(cray_camera *cam, CRAYFLT u, CRAYFLT v)
{
    cray_ray r;
    vec3 tmp[2];

    VEC3_MULS(cam->horizontal, u, tmp[0]);
    VEC3_MULS(cam->vertical, v, tmp[1]);
    VEC3_ADD(tmp[0], tmp[1], tmp[0]);
    VEC3_ADD(cam->lower_left_corner, tmp[0], tmp[0]);
    VEC3_SUB(tmp[0], cam->origin, tmp[0]);

    RAY_SET(r, cam->origin, tmp[0]);
    return r;
}
