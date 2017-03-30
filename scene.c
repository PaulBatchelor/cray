#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "hitablelist.h"
#include "sphere.h"
#include "camera.h"
#include "rand.h"
#include "material.h"
#include "scene.h"

void cray_scene_init(cray_scene *scene, 
    int width, 
    int height, 
    int samp, 
    int nobjects)
{
    scene->nx = width;
    scene->ny = height;
    scene->ns = samp;

    
    cray_hitablelist_init(&scene->world, NULL, 0);
    cray_camera_init(&scene->cam);

}

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

void cray_scene_write_ppm(cray_scene *scene, 
    const char *filename, 
    CRAYFLT *buf)
{
    int x, y;
    int r, g, b;
    int pos;
    FILE *fp;
    fp = fopen("out.ppm", "w");
    fprintf(fp, "P3\n%d %d\n255\n", scene->nx, scene->ny);

    pos = 0;
    for(y = scene->ny - 1; y >= 0; y--) {
        for(x = 0; x < scene->nx ; x++) {
            pos = y * scene->nx * 3 + x * 3;
            r = (int)(255.99 * buf[pos]);
            g = (int)(255.99 * buf[pos + 1]);
            b = (int)(255.99 * buf[pos + 2]);
            fprintf(fp, "%d %d %d\n", r, g, b);
        }
    }

    fclose(fp);
}

void cray_scene_render(cray_scene *scene, 
    int offx, int offy,
    int w, int h,
    CRAYFLT *buf, int verbose)
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
    cray_camera *cam;
    cray_hitablelist *world;

    cam = &scene->cam;
    world = &scene->world;

    step = 0;
    size = w * h;


    xstart = offx;
    xend = offx + w;
    
    ystart = (scene->ny - 1) - offy;
    yend = (scene->ny - h) - offy;

    for(j = ystart; j >= yend; j--) {
        for(i = xstart; i < xend; i++) {
            VEC3_SET(col, 0, 0, 0);
            for(s = 0; s < scene->ns; s++) {
                u = (CRAYFLT) (i + cray_rand()) / (CRAYFLT)scene->nx; 
                v = (CRAYFLT) (j + cray_rand()) / (CRAYFLT)scene->ny;
                r = cray_camera_get_ray(cam, u, v);
                tmp = color(&r, world, 0);
                VEC3_ADD(col, tmp, col);
            }

            VEC3_DIVS(col, (CRAYFLT)scene->ns, col);
            VEC3_SQRT(col, col);

            pos = j * scene->nx * 3 + i * 3;
            buf[pos] = col.x;
            buf[pos + 1] = col.y;
            buf[pos + 2] = col.z;

            step++;

            if(verbose && step % 100 == 0) {
                fprintf(stderr, "%06d/%06d\r", step, size);
            }
        }
    }
}

