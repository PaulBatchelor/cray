#include <float.h>
#include <stdio.h>
#include <math.h>

#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "hitablelist.h"
#include "sphere.h"

static int g_i = 0;
static int g_j = 0;

static vec3 color(cray_ray *r, cray_hitablelist *world)
{
    vec3 unit_direction;
    vec3 tmp[2];
    float t;
    cray_hitable rec;

    if(cray_hitablelist_hit(world, r, 0.0, FLT_MAX, &rec)) {
        VEC3_ADDS(rec.normal, 1.0, tmp[0]);
        VEC3_MULS(tmp[0], 0.5, tmp[0]);
        return tmp[0];
    } else {

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
    cray_sphere sphere[2];
    cray_object obj[2];
    cray_object *pobj[2];
    cray_hitablelist world;

    nx = 200;
    ny = 100;


    fp = fopen("out.ppm", "w");

    VEC3_SET(lower_left_corner, -2.0, -1.0, -1.0);
    VEC3_SET(horizontal, 4.0, 0.0, 0.0);
    VEC3_SET(vertical, 0.0, 2.0, 0.0);
    VEC3_SET(origin, 0.0, 0.0, 0.0);

    VEC3_SET(tmp[0], 0, 0, -1);
    cray_sphere_init(&sphere[0], &tmp[0], 0.5);
    VEC3_SET(tmp[0], 0, -100.5, -1);
    cray_sphere_init(&sphere[1], &tmp[0], 100);

    cray_sphere_mk_obj(&sphere[0], &obj[0]);
    cray_sphere_mk_obj(&sphere[1], &obj[1]);

    pobj[0] = &obj[0];
    pobj[1] = &obj[1];
    cray_hitablelist_init(&world, pobj, 2);

    fprintf(fp, "P3\n%d %d\n255\n", nx, ny);

    for(j = ny -1; j >= 0; j--) {
        for(i = 0; i < nx; i++) {
            g_i = i;
            g_j = j;
            u = (CRAYFLT) i / (CRAYFLT)nx; 
            v = (CRAYFLT) j / (CRAYFLT)ny;


            /* lower_left_corner + u*horizontal + v*vertical */
            VEC3_MULS(horizontal, u, tmp[0]);
            VEC3_MULS(vertical, v, tmp[1]);
            VEC3_ADD(tmp[0], tmp[1], tmp[0]);
            VEC3_ADD(lower_left_corner, tmp[0], tmp[0]);

            RAY_SET(r, origin, tmp[0]);

            col = color(&r, &world);

            ir = (int)(255.99 * col.x);
            ig = (int)(255.99 * col.y);
            ib = (int)(255.99 * col.z);
            fprintf(fp, "%d %d %d\n", ir, ig, ib);
        }
    }

    fclose(fp);
    return 0;
}
