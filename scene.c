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
#include "scene_private.h"

static void background(cray_scene *scene, cray_ray *ray, int x, int y, CRAYFLT *color)
{
    vec3 unit_direction;
    vec3 tmp[2];
    CRAYFLT t;

    VEC3_UNIT_VECTOR(cray_ray_direction(ray), t, unit_direction);

    t = 0.5*(unit_direction.y + 1.0);

    /* v1 = (1.0 - t) * vec3(1.0, 1.0, 1.0) */
    VEC3_MULS(scene->tint[0], (1.0 - t), tmp[0]);

    /* v2 = t * vec3(0.5, 0.7, 1.0) */
    /* VEC3_SET(tmp[1], 0.5, 0.7, 1.0); */
    VEC3_MULS(scene->tint[1], t, tmp[1]);

    /* reuse unit_direction variable */
    /* unit_direction = v1 + v2 */

    VEC3_ADD(tmp[0], tmp[1], unit_direction);

    color[0] = unit_direction.x;
    color[1] = unit_direction.y;
    color[2] = unit_direction.z;
}

static void shadow(cray_scene *scene, CRAYFLT *tmp, int x, int y)
{
    tmp[0] = 0.0;
    tmp[1] = 0.0;
    tmp[2] = 0.0;
}

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

    VEC3_SET(scene->tint[0], 1.0, 1.0, 1.0);
    VEC3_SET(scene->tint[1], 1.0, 1.0, 1.0);
    cray_scene_default_bgcolor(scene);
    cray_scene_maxdepth(scene, 50);
    cray_scene_default_shadow(scene);
}

static vec3 color(cray_scene *scene, cray_ray *r, int depth, int x, int y)
{
    vec3 tmp[2];
    cray_hitable rec;
    cray_ray scattered;
    CRAYFLT clr[3];

    if(cray_hitablelist_hit(&scene->world, r, 0.001, FLT_MAX, &rec)) {
        if(depth < scene->maxdepth  && rec.mat->scatter(rec.mat, &rec, r, &tmp[0], &scattered)) {
            tmp[1] = color(scene, &scattered, depth + 1, x, y);
            VEC3_MUL(tmp[1], tmp[0], tmp[0]);
            return tmp[0];
        } else {
            scene->shadow(scene, clr, x, y);
            VEC3_SET(tmp[0], clr[0], clr[1], clr[2]);
            return tmp[0];
        }
    } else {
        scene->bgcolor(scene, r, x, y, clr);
        VEC3_SET(tmp[0], clr[0], clr[1], clr[2]);
        return tmp[0];
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

    cam = &scene->cam;

    step = 0;
    size = w * h;


    xstart = offx;
    xend = offx + w;
    
    ystart = (scene->ny - 1) - offy;
    yend = (scene->ny - h) - offy;
    pos = 0;
    for(j = ystart; j >= yend; j--) {
        for(i = xstart; i < xend; i++) {
            VEC3_SET(col, 0, 0, 0);
            for(s = 0; s < scene->ns; s++) {
                u = (CRAYFLT) (i + cray_rand()) / (CRAYFLT)scene->nx; 
                v = (CRAYFLT) (j + cray_rand()) / (CRAYFLT)scene->ny;
                r = cray_camera_get_ray(cam, u, v);
                tmp = color(scene, &r, 0, i, j);
                VEC3_ADD(col, tmp, col);
            }

            VEC3_DIVS(col, (CRAYFLT)scene->ns, col);
            VEC3_SQRT(col, col);

            if(verbose & CRAY_SEGMENT) {
                buf[pos] = col.x;
                buf[pos + 1] = col.y;
                buf[pos + 2] = col.z;
                pos += 3;
            } else {
                pos = j * scene->nx * 3 + i * 3;
                buf[pos] = col.x;
                buf[pos + 1] = col.y;
                buf[pos + 2] = col.z;
            }


            step++;

            if((verbose & CRAY_VERBOSE) && step % 100 == 0) {
                fprintf(stderr, "%06d/%06d\r", step, size);
            }
        }
    }
}

void cray_scene_tint_top(cray_scene *scene, CRAYFLT r, CRAYFLT g, CRAYFLT b)
{
    VEC3_SET(scene->tint[0], r, g, b);
}

void cray_scene_tint_bottom(cray_scene *scene, CRAYFLT r, CRAYFLT g, CRAYFLT b)
{
    VEC3_SET(scene->tint[1], r, g, b);
}

void cray_scene_cb_bgcolor(cray_scene *scene, 
    void (*bgcolor)(cray_scene *, cray_ray *, int, int, CRAYFLT *))
{
    scene->bgcolor = bgcolor;
}

void cray_scene_default_bgcolor(cray_scene *scene)
{
    cray_scene_cb_bgcolor(scene, background);
}

void cray_scene_maxdepth(cray_scene *scene, int maxdepth)
{
    scene->maxdepth = maxdepth;
}

void cray_scene_cb_shadow(cray_scene *scene, 
        void (* shadow)(cray_scene *, CRAYFLT *, int, int))
{
    scene->shadow = shadow;
}

void cray_scene_default_shadow(cray_scene *scene)
{
    cray_scene_cb_shadow(scene, shadow);
}
