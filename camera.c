#include <math.h>
#include "vec3.h"
#include "ray.h"
#include "camera.h"
#include "rand.h"

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
    vec3 rd;
    vec3 offset;
    CRAYFLT var;

    /* tmp[0] = s*horizontal + t*vertical - origin */
    VEC3_MULS(cam->horizontal, s, tmp[0]);
    VEC3_MULS(cam->vertical, t, tmp[1]);
    VEC3_ADD(tmp[0], tmp[1], tmp[0]);
    VEC3_ADD(cam->lower_left_corner, tmp[0], tmp[0]);
    VEC3_SUB(tmp[0], cam->origin, tmp[0]);

    rd = random_in_unit_disk();
    VEC3_MULS(rd, cam->lens_radius, rd);

    /* offset = u*rd.x + v*rd.y */
    var = rd.x;
    VEC3_MULS(cam->u, var, tmp[1]);
    var = rd.y;
    VEC3_MULS(cam->v, var, rd);
    VEC3_ADD(tmp[1], rd, offset);

    /* tmp[1] = origin + offset */
    VEC3_ADD(cam->origin, offset, tmp[1]);

    /* tmp[0] = tmp[0] - offset */
    VEC3_SUB(tmp[0], offset, tmp[0]);
    RAY_SET(r, tmp[1], tmp[0]);
    return r;
}

void cray_camera_setup(cray_camera *cam, 
    vec3 lookfrom,
    vec3 lookat, 
    vec3 vup,
    CRAYFLT vfov, 
    CRAYFLT aspect,
    CRAYFLT aperture,
    CRAYFLT focus_dist)
{
    CRAYFLT theta;
    CRAYFLT half_height;
    CRAYFLT half_width;
    vec3 tmp[2];
    CRAYFLT var;

    cam->lens_radius = aperture / 2.0;

    cam->origin = lookfrom;
    theta = vfov * M_PI / 180.0;
    half_height = tan(theta / 2.0);
    half_width = aspect * half_height;

    /* w = unit_vector(lookfrom - lookat) */
    VEC3_SUB(lookfrom, lookat, cam->w);
    VEC3_UNIT_VECTOR(cam->w, var, cam->w);

    /* u = unit_vector(cross(vup, w)) */
    VEC3_CROSS(vup, cam->w, cam->u);
    VEC3_UNIT_VECTOR(cam->u, var, cam->u);

    /* v = cross(w, u) */
    VEC3_CROSS(cam->w, cam->u, cam->v);

    /* tmp[0] = hw * fd * u */
    var = half_width * focus_dist;
    VEC3_MULS(cam->u, var, tmp[0]);
    
    /* tmp[1] = hh * fd * v */
    var = half_height * focus_dist;
    VEC3_MULS(cam->v, var, tmp[1]);

    /* tmp[0] = origin - tmp[0]  */
    VEC3_SUB(cam->origin, tmp[0], tmp[0]);
    /* tmp[0] = tmp[0] - tmp[1]  */
    VEC3_SUB(tmp[0], tmp[1], tmp[0]);
    /* tmp[1] = w * fd  */
    VEC3_MULS(cam->w, focus_dist, tmp[1]);
    /* lower_left_corner = tmp[0] - tmp[1]  */
    VEC3_SUB(tmp[0], tmp[1], cam->lower_left_corner);

    /* horiz = 2 * hw * fd * u */
    var = 2 * half_width * focus_dist;
    VEC3_MULS(cam->u, var, cam->horizontal);

    /* vert = 2 * hh * fd * v */
    var = 2 * half_height * focus_dist;
    VEC3_MULS(cam->v, var, cam->vertical);
}
