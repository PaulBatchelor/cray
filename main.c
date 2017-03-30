#include <float.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "hitablelist.h"
#include "sphere.h"
#include "camera.h"
#include "rand.h"
#include "material.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static vec3 color(cray_ray *r, cray_hitablelist *world, int depth)
{
    vec3 unit_direction;
    vec3 tmp[2];
    float t;
    cray_hitable rec;
    cray_ray scattered;

    if(cray_hitablelist_hit(world, r, 0.001, FLT_MAX, &rec)) {
        if(depth < 50 && rec.mat->scatter(rec.mat, &rec, r, &tmp[0], &scattered)) {
            tmp[1] = color(&scattered, world, depth + 1);
            VEC3_MUL(tmp[1], tmp[0], tmp[0]);
            return tmp[0];
        } else {
            VEC3_SET(tmp[0], 0.0, 0.0, 0.0);
            return tmp[0];
        }
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
    /* this shouldn't happen */

    VEC3_SET(tmp[0], 0.0, 0.0, 0.0);
    return tmp[0];
}

static void write_ppm(const char *filename, int width, int height, CRAYFLT *buf)
{
    int x, y;
    int r, g, b;
    int pos;
    FILE *fp;
    fp = fopen("out.ppm", "w");
    fprintf(fp, "P3\n%d %d\n255\n", width, height);

    pos = 0;
    for(y = height - 1; y >= 0; y--) {
        for(x = 0; x < width; x++) {
            pos = y * width * 3 + x * 3;
            r = (int)(255.99 * buf[pos]);
            g = (int)(255.99 * buf[pos + 1]);
            b = (int)(255.99 * buf[pos + 2]);
            fprintf(fp, "%d %d %d\n", r, g, b);
        }
    }

    fclose(fp);
}

static void render(cray_hitablelist *world, 
    cray_camera *cam, 
    int nx, int ny, int ns, 
    int offx, int offy,
    int w, int h,
    CRAYFLT *buf)
{
    int i, j, s;
    vec3 col;
    vec3 tmp;
    cray_ray r;
    CRAYFLT u, v;
    int step;
    int size;
    unsigned int pos;
    int xstart, ystart;
    int xend, yend;

    step = 0;
    size = nx * ny;


    xstart = offx;
    xend = offx + w;
    
    ystart = (ny - 1) - offy;
    yend = (ny - h) - offy;

    for(j = ystart; j >= yend; j--) {
        for(i = xstart; i < xend; i++) {
            VEC3_SET(col, 0, 0, 0);
            for(s = 0; s < ns; s++) {
                u = (CRAYFLT) (i + cray_rand()) / (CRAYFLT)nx; 
                v = (CRAYFLT) (j + cray_rand()) / (CRAYFLT)ny;
                r = cray_camera_get_ray(cam, u, v);
                tmp = color(&r, world, 0);
                VEC3_ADD(col, tmp, col);
            }

            VEC3_DIVS(col, (CRAYFLT)ns, col);
            VEC3_SQRT(col, col);

            pos = j * nx * 3 + i * 3;
            buf[pos] = col.x;
            buf[pos + 1] = col.y;
            buf[pos + 2] = col.z;

            step++;

            if(step % 100 == 0) {
                fprintf(stderr, "%06d/%06d\r", step, size);
            }
        }
    }
}

int main()
{
    int nx;
    int ny;
    int ns;
    vec3 tmp[2];
    cray_sphere sphere[5];
    cray_object *pobj[5];
    cray_hitablelist world;
    cray_camera cam;
    cray_lambertian lam[2];
    cray_dielectric di[2];
    cray_metal met[2];
    CRAYFLT *buf;

    nx = 200;
    ny = 100;
    ns = 100;

    buf = calloc(sizeof(CRAYFLT), nx * ny * 3);

    cray_lambertian_init(&lam[0]);
    cray_lambertian_color(&lam[0], 0.8, 0.3, 0.3);
    cray_lambertian_init(&lam[1]);
    cray_lambertian_color(&lam[1], 0.8, 0.8, 0.0);

    cray_metal_init(&met[0]);
    cray_metal_color(&met[0], 0.8, 0.6, 0.2);
    cray_metal_fuzz(&met[0], 0.1);
    cray_metal_init(&met[1]);
    cray_metal_color(&met[1], 0.8, 0.8, 0.8);
    cray_metal_fuzz(&met[1], 0.3);

    cray_dielectric_init(&di[0]);
    cray_dielectric_refraction(&di[0], 1.5);
    cray_dielectric_init(&di[1]);
    cray_dielectric_refraction(&di[1], 1.5);

    VEC3_SET(tmp[0], 0, 0, -1);
    cray_sphere_init(&sphere[0], &tmp[0], 0.5, &lam[0].mat);
    VEC3_SET(tmp[0], 0, -100.5, -1);
    cray_sphere_init(&sphere[1], &tmp[0], 100, &lam[1].mat);
    
    VEC3_SET(tmp[0], 1, 0, -1);
    cray_sphere_init(&sphere[2], &tmp[0], 0.5, &met[0].mat);
    VEC3_SET(tmp[0], -1, 0, -1);
    cray_sphere_init(&sphere[3], &tmp[0], 0.5, &di[0].mat);
    
    VEC3_SET(tmp[0], -1, 0, -1);
    cray_sphere_init(&sphere[4], &tmp[0], -0.45, &di[1].mat);

    cray_hitablelist_init(&world, pobj, 5);
    cray_hitablelist_append(&world, &sphere[0].obj);
    cray_hitablelist_append(&world, &sphere[1].obj);
    cray_hitablelist_append(&world, &sphere[2].obj);
    cray_hitablelist_append(&world, &sphere[3].obj);
    cray_hitablelist_append(&world, &sphere[4].obj);

    cray_camera_init(&cam);

    cray_camera_aperture(&cam, 1.3);
    cray_camera_lookfrom(&cam, 3, 3, 2);
    cray_camera_lookat(&cam, 0, 0, -1);
    cray_camera_vup(&cam, 0, 1, 0);
    cray_camera_vfov(&cam, 20);
    cray_camera_aspect(&cam, (float)nx / (float) ny);
    cray_camera_focus_dist(&cam, cray_camera_dist(&cam));
    cray_camera_update(&cam);

    render(&world, &cam, nx, ny, ns, 0, 0, nx, ny, buf);
    write_ppm("out.ppm", nx, ny, buf);

    free(buf);
    return 0;
}
