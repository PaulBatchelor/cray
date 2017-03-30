#include <stdio.h>
#include <math.h>
#include "vec3.h"
#include "ray.h"
#include "camera.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define VEC3_PRINT(V) printf("%g %g %g\n", (V).x, (V).y, (V).z);

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
    vec3 lookfrom,
    vec3 lookat, 
    vec3 vup,
    CRAYFLT vfov, 
    CRAYFLT aspect)
{
    CRAYFLT theta;
    CRAYFLT half_height;
    CRAYFLT half_width;
    vec3 u, v, w;
    vec3 tmp[2];
    CRAYFLT var;

    VEC3_PRINT(lookfrom);
    VEC3_PRINT(lookat);
    VEC3_PRINT(vup);
    cam->origin = lookfrom;
    theta = vfov * M_PI / 180.0;
    half_height = tan(theta / 2.0);
    half_width = aspect * half_height;

    VEC3_SUB(lookfrom, lookat, w);
    VEC3_PRINT(w);
    VEC3_UNIT_VECTOR(w, var, w);
    VEC3_PRINT(w);

    VEC3_CROSS(vup, w, u);
    VEC3_PRINT(u);
    VEC3_UNIT_VECTOR(u, var, u);
    VEC3_PRINT(u);

    VEC3_CROSS(w, u, v);

    VEC3_MULS(u, half_width, tmp[0]);
    VEC3_MULS(v, half_height, tmp[1]);

    VEC3_SUB(cam->origin, tmp[0], tmp[0]);
    VEC3_SUB(tmp[0], tmp[1], tmp[0]);
    VEC3_SUB(tmp[0], w, cam->lower_left_corner);

    var = 2 * half_width;
    VEC3_MULS(u, var, cam->horizontal);
    var = 2 * half_height;
    VEC3_MULS(v, var, cam->vertical);
}
