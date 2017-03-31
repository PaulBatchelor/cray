#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <runt.h>

#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "hitablelist.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "scene.h"

runt_int runt_load_cray(runt_vm *vm);
/*
static void demo()
{
    int x, y;
    int bx, by;
    cray_sphere sphere[5];
    cray_object *pobj[5];
    cray_hitablelist *world;
    cray_camera *cam;
    cray_lambertian lam[2];
    cray_dielectric di[2];
    cray_metal met[2];
    CRAYFLT *buf;
    cray_scene scene;

    bx = 8;
    by = 8;

    cray_scene_init(&scene, bx * 32, by * 32, 200, 4);

    buf = calloc(sizeof(CRAYFLT), scene.nx * scene.ny * 3);
    cam = &scene.cam;
    world = &scene.world;

    cray_lambertian_init(&lam[0]);
    cray_lambertian_color(&lam[0], 0.8, 0.8, 0.0);
    cray_lambertian_init(&lam[1]);
    cray_lambertian_color(&lam[1], 1.0, 1.0, 1.0);

    cray_metal_init(&met[0]);
    cray_metal_color(&met[0], 0.3, 0.3, 0.8);
    cray_metal_fuzz(&met[0], 0.1);
    cray_metal_init(&met[1]);
    cray_metal_color(&met[1], 0.3, 0.3, 0.8);
    cray_metal_fuzz(&met[1], 0.3);

    cray_dielectric_init(&di[0]);
    cray_dielectric_refraction(&di[0], 2.3);
    cray_dielectric_color(&di[0], 0.8, 1.0, 0.8);
    cray_dielectric_init(&di[1]);
    cray_dielectric_refraction(&di[1], 0.7);

    cray_sphere_init(&sphere[0], &lam[0].mat);
    cray_sphere_pos(&sphere[0], 0, 0, -1);
    cray_sphere_radius(&sphere[0], 0.5);

    cray_sphere_init(&sphere[1], &lam[1].mat);
    cray_sphere_pos(&sphere[1], 0, -100.5, -1);
    cray_sphere_radius(&sphere[1], 100);
    
    cray_sphere_init(&sphere[2], &met[0].mat);
    cray_sphere_pos(&sphere[2], -1, 0, -1);
    cray_sphere_radius(&sphere[2], 0.5);

    cray_sphere_init(&sphere[3], &di[0].mat);
    cray_sphere_pos(&sphere[3], 1, 0, -1);
    cray_sphere_radius(&sphere[3], 0.5);

    cray_hitablelist_init(world, pobj, 1);
    cray_hitablelist_append(world, &sphere[0].obj);
    cray_hitablelist_append(world, &sphere[1].obj);
    cray_hitablelist_append(world, &sphere[2].obj);
    cray_hitablelist_append(world, &sphere[3].obj);

    cray_camera_aperture(cam, 0.8);
    cray_camera_lookfrom(cam, 3, 3, 3);
    cray_camera_lookat(cam, 0, 0, -1);
    cray_camera_vup(cam, 0, 1, 0);
    cray_camera_vfov(cam, 30);
    cray_camera_aspect(cam, (float)scene.nx / (float) scene.ny);
    cray_camera_focus_dist(cam, cray_camera_dist(cam));
    cray_camera_update(cam);

    for(y = 0; y < by; y++) {
        for(x = 0; x < bx; x++) {
            printf("rendering block %d of %d\n", y * bx + x, bx * by);
            cray_scene_render(&scene, x*32, y*32, 32, 32, buf, 1);
        }
    }

    cray_scene_write_ppm(&scene, "out.ppm", buf);

    free(buf);
}
*/
static runt_int loader(runt_vm *vm)
{
    runt_load_minimal(vm);
    runt_load_cray(vm);
    return RUNT_OK;
}

int main(int argc, char *argv[])
{
    return irunt_begin(argc, argv, loader);
}
