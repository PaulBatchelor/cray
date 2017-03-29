#include <stdio.h>
#include <math.h>

#include "vec3.h"
#include "ray.h"

static float hit_sphere(const vec3 *center, float radius, cray_ray *r)
{
    vec3 oc;
    float a, b, c;
    float discriminant;

    VEC3_SUB(cray_ray_origin(r), *center, oc);
    a = VEC3_DOT(cray_ray_direction(r), cray_ray_direction(r));
    b = 2.0 * VEC3_DOT(oc, cray_ray_direction(r));
    c = VEC3_DOT(oc, oc) - radius * radius;
    discriminant = b*b - 4*a*c;
    if(discriminant < 0) {
        return -1;
    } else {
        return (-b - sqrt(discriminant)) / (2.0*a);
    }
}

static vec3 color(cray_ray *r)
{
    vec3 unit_direction;
    vec3 tmp[2];
    float t;

    VEC3_SET(tmp[0], 0, 0, -1);

    t = hit_sphere(&tmp[0], 0.5, r);

    if(t > 0.0) {
        /* N = unit_vector(r.point_at_parameter(t) - vec3(0, 0, -1)); */
        VEC3_SUB(cray_ray_point_at_param(r, t), tmp[0], tmp[0]);
        /* 't' is not longer needed, so use it as tmp variable  */
        VEC3_UNIT_VECTOR(tmp[0], t, tmp[0]);

        /* N = (N + 1) * 0.5 */
        VEC3_ADDS(tmp[0], 1, tmp[0]);
        VEC3_MULS(tmp[0], 0.5, tmp[0]);
        return tmp[0];
    }

    /* use t as tmp variable before it is used */
    VEC3_UNIT_VECTOR(cray_ray_direction(r), t, unit_direction);

    t = 0.5*(unit_direction.y + 1.0);

    /* v1 = (1.0 - t) * vec3(1.0, 1.0, 1.0) */
    VEC3_SET(tmp[0], 1.0, 1.0, 1.0);
    VEC3_MULS(tmp[0], (1.0 - t), tmp[0]);

    /* v2 = t * vec3(0.5, 0.7, 1.0) */
    VEC3_SET(tmp[1], 0.5, 0.7, 1.0);
    VEC3_MULS(tmp[1], t, tmp[1]);

    /* reuse unit_direction variable */
    /* unit_direction = v1 + v2 */

    VEC3_ADD(tmp[0], tmp[1], unit_direction);

    return unit_direction;
}

int main()
{
    int nx;
    int ny;
    int j;
    int i;
    int ir, ig, ib;
    FILE *fp;
    vec3 col;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
    vec3 tmp[2];
    cray_ray r;
    CRAYFLT u, v;

    nx = 200;
    ny = 100;

    fp = fopen("out.ppm", "w");

    VEC3_SET(lower_left_corner, -2.0, -1.0, -1.0);
    VEC3_SET(horizontal, 4.0, 0.0, 0.0);
    VEC3_SET(vertical, 0.0, 2.0, 0.0);
    VEC3_SET(origin, 0.0, 0.0, 0.0);

    fprintf(fp, "P3\n%d %d\n255\n", nx, ny);

    for(j = ny -1; j >= 0; j--) {
        for(i = 0; i < nx; i++) {
            u = (CRAYFLT) i / (CRAYFLT)nx; 
            v = (CRAYFLT) j / (CRAYFLT)ny;


            /* lower_left_corner + u*horizontal + v*vertical */
            VEC3_MULS(horizontal, u, tmp[0]);
            VEC3_MULS(vertical, v, tmp[1]);
            VEC3_ADD(tmp[0], tmp[1], tmp[0]);
            VEC3_ADD(lower_left_corner, tmp[0], tmp[0]);

            RAY_SET(r, origin, tmp[0]);

            col = color(&r);

            ir = (int)(255.99 * col.x);
            ig = (int)(255.99 * col.y);
            ib = (int)(255.99 * col.z);
            fprintf(fp, "%d %d %d\n", ir, ig, ib);
        }
    }

    fclose(fp);
    return 0;
}
