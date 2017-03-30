#include <math.h>
#include "vec3.h"
#include "ray.h"
#include "camera.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void cray_camera_init(cray_camera *cam)
{
}

cray_ray cray_camera_get_ray(cray_camera *cam, CRAYFLT s, CRAYFLT t)
{
    cray_ray r;
    vec3 tmp[2];

    VEC3_MULS(cam->horizontal, s, tmp[0]);
    VEC3_MULS(cam->vertical, t, tmp[1]);
    VEC3_ADD(tmp[0], tmp[1], tmp[0]);
    VEC3_ADD(cam->lower_left_corner, tmp[0], tmp[0]);
    VEC3_SUB(tmp[0], cam->origin, tmp[0]);

    RAY_SET(r, cam->origin, tmp[0]);
    return r;
}

void cray_camera_setup(cray_camera *cam, 
    CRAYFLT vfov, 
    CRAYFLT aspect)
{
    CRAYFLT theta;
    CRAYFLT half_height;
    CRAYFLT half_width;

    theta = vfov * M_PI / 180.0;
    half_height = tan(theta / 2.0);
    half_width = aspect * half_height;

    VEC3_SET(cam->lower_left_corner, -half_width, -half_height, -1.0);
    VEC3_SET(cam->horizontal, 2.0 * half_width, 0.0, 0.0);
    VEC3_SET(cam->vertical, 0.0,  2.0 * half_height, 0.0);
    VEC3_SET(cam->origin, 0.0, 0.0, 0.0);

}
