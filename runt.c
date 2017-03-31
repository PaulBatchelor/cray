#include <stdio.h>
#include <stdlib.h>
#include <runt.h>

#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "hitablelist.h"
#include "sphere.h"
#include "camera.h"
#include "rand.h"
#include "material.h"
#include "scene.h"

static runt_int rproc_cray_init(runt_vm *vm, runt_ptr p)
{
    runt_int rc;
    runt_stacklet *s;
    runt_int w, h, samps;
    runt_int nobj;
    cray_scene *scene;
    cray_object **obj;

    scene = runt_to_cptr(p);
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    nobj = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    samps = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    h = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    w = s->f;

    cray_scene_init(scene, w, h, samps, 0);
    runt_malloc(vm, sizeof(cray_object *) * nobj, (void **)&obj);
    runt_mark_set(vm);

    cray_hitablelist_init(&scene->world, obj, nobj);
    return RUNT_OK;
}

static runt_int rproc_write_ppm(runt_vm *vm, runt_ptr p)
{
    const char *filename;
    cray_scene *scene;
    CRAYFLT *buf;
    runt_int rc;
    runt_stacklet *s;

    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    filename = runt_to_string(s->p);
    scene = runt_to_cptr(p);
    buf = calloc(sizeof(CRAYFLT), scene->nx * scene->ny * 3);

    cray_scene_render(scene, 0, 0, scene->nx, scene->ny, buf, 1);
    cray_scene_write_ppm(scene, filename, buf);
    free(buf);
    return RUNT_OK;
}

static runt_int rproc_add_sphere(runt_vm *vm, runt_ptr p)
{
    cray_sphere *sphere;
    cray_scene *scene;
    runt_float x, y, z;
    runt_float radius;
    runt_int rc;
    runt_stacklet *s;
    cray_material *mat;

    scene = runt_to_cptr(p);
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    mat = runt_to_cptr(s->p);

    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    radius = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    z = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    y = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    x = s->f;

    runt_malloc(vm, sizeof(cray_sphere), (void **)&sphere);


    cray_sphere_init(sphere, mat);
    cray_sphere_pos(sphere, x, y, z);
    cray_sphere_radius(sphere, radius);
    
    cray_hitablelist_append(&scene->world, &sphere->obj);

    runt_mark_set(vm);
    return RUNT_OK;
}

static runt_int rproc_lambertian(runt_vm *vm, runt_ptr p)
{
    runt_int rc;
    runt_stacklet *s;
    cray_lambertian *lam;
    runt_float r, g, b;

    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    b = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    g = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    r = s->f;

    runt_malloc(vm, sizeof(cray_lambertian), (void **)&lam);
    runt_mark_set(vm);
    cray_lambertian_init(lam);
    cray_lambertian_color(lam, r, g, b);
   
    rc = runt_ppush(vm, &s);
    RUNT_ERROR_CHECK(rc);

    s->p = runt_mk_cptr(vm, &lam->mat);

    return RUNT_OK;
}

static runt_int rproc_metallic(runt_vm *vm, runt_ptr p)
{
    runt_int rc;
    runt_stacklet *s;
    cray_metal *met;
    runt_float r, g, b;
    runt_float fuzz;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    fuzz = s->f;

    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    b = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    g = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    r = s->f;

    runt_malloc(vm, sizeof(cray_metal), (void **)&met);
    runt_mark_set(vm);
    cray_metal_init(met);
    cray_metal_color(met, r, g, b);
    cray_metal_fuzz(met, fuzz);
   
    rc = runt_ppush(vm, &s);
    RUNT_ERROR_CHECK(rc);

    s->p = runt_mk_cptr(vm, &met->mat);

    return RUNT_OK;
}

static runt_int rproc_dielectric(runt_vm *vm, runt_ptr p)
{
    runt_int rc;
    runt_stacklet *s;
    cray_dielectric *di;
    runt_float r, g, b;
    runt_float refract;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    refract = s->f;

    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    b = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    g = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    r = s->f;

    runt_malloc(vm, sizeof(cray_dielectric), (void **)&di);
    runt_mark_set(vm);
    cray_dielectric_init(di);
    cray_dielectric_color(di, r, g, b);
    cray_dielectric_refraction(di, refract);
   
    rc = runt_ppush(vm, &s);
    RUNT_ERROR_CHECK(rc);

    s->p = runt_mk_cptr(vm, &di->mat);

    return RUNT_OK;
}


static int rproc_cam_aperture(runt_vm *vm, runt_ptr p)
{
    runt_float aperture;
    cray_scene *scene;
    runt_int rc;
    runt_stacklet *s;

    scene = runt_to_cptr(p);
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    aperture = s->f;

    cray_camera_aperture(&scene->cam, aperture);
    return RUNT_OK;
}

static int rproc_cam_lookfrom(runt_vm *vm, runt_ptr p)
{
    runt_float x, y, z;
    cray_scene *scene;
    runt_int rc;
    runt_stacklet *s;

    scene = runt_to_cptr(p);
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    z = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    y = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    x = s->f;

    cray_camera_lookfrom(&scene->cam, x, y, z);
    return RUNT_OK;
}

static int rproc_cam_lookat(runt_vm *vm, runt_ptr p)
{
    runt_float x, y, z;
    cray_scene *scene;
    runt_int rc;
    runt_stacklet *s;

    scene = runt_to_cptr(p);
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    z = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    y = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    x = s->f;

    cray_camera_lookat(&scene->cam, x, y, z);
    return RUNT_OK;
}

static int rproc_cam_vup(runt_vm *vm, runt_ptr p)
{
    runt_float x, y, z;
    cray_scene *scene;
    runt_int rc;
    runt_stacklet *s;

    scene = runt_to_cptr(p);
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    z = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    y = s->f;
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    x = s->f;

    cray_camera_vup(&scene->cam, x, y, z);
    return RUNT_OK;
}

static int rproc_vfov(runt_vm *vm, runt_ptr p)
{
    runt_float vfov;
    cray_scene *scene;
    runt_int rc;
    runt_stacklet *s;

    scene = runt_to_cptr(p);
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    vfov = s->f;

    cray_camera_vfov(&scene->cam, vfov);
    return RUNT_OK;
}

static int rproc_aspect(runt_vm *vm, runt_ptr p)
{
    cray_scene *scene;
    runt_int rc;
    runt_stacklet *s;

    scene = runt_to_cptr(p);
    
    rc = runt_ppush(vm, &s);
    RUNT_ERROR_CHECK(rc);

    s->f = scene->nx / scene->ny;

    return RUNT_OK;
}

static int rproc_cam_aspect(runt_vm *vm, runt_ptr p)
{
    cray_scene *scene;
    runt_int rc;
    runt_stacklet *s;
    runt_float aspect;

    scene = runt_to_cptr(p);
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    aspect = s->f;

    cray_camera_aspect(&scene->cam, aspect);

    return RUNT_OK;
}

static void cray_define(runt_vm *vm,
    const char *word,
    runt_uint size,
    runt_proc proc,
    runt_ptr p)
{
    runt_uint word_id;
    word_id = runt_word_define(vm, word, size, proc);
    runt_word_bind_ptr(vm, word_id, p);
}

static int rproc_cam_dist(runt_vm *vm, runt_ptr p)
{
    cray_scene *scene;
    runt_int rc;
    runt_stacklet *s;

    scene = runt_to_cptr(p);
    
    rc = runt_ppush(vm, &s);
    RUNT_ERROR_CHECK(rc);

    s->f = cray_camera_dist(&scene->cam);

    return RUNT_OK;
}

static int rproc_cam_focus_dist(runt_vm *vm, runt_ptr p)
{
    cray_scene *scene;
    runt_int rc;
    runt_stacklet *s;
    runt_float dist;

    scene = runt_to_cptr(p);
    
    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    dist = s->f;

    cray_camera_focus_dist(&scene->cam, dist);

    return RUNT_OK;
}

static int rproc_cam_update(runt_vm *vm, runt_ptr p)
{
    cray_scene *scene;

    scene = runt_to_cptr(p);
    cray_camera_update(&scene->cam);

    return RUNT_OK;
}

runt_int runt_load_cray(runt_vm *vm)
{
    cray_scene *scene;
    runt_ptr p;

    runt_malloc(vm, sizeof(cray_scene), (void **)&scene);

    p = runt_mk_cptr(vm, scene);

    cray_define(vm, "cray_init", 9, rproc_cray_init, p);
    cray_define(vm, "write_ppm", 9, rproc_write_ppm, p);
    cray_define(vm, "add_sphere", 10, rproc_add_sphere, p);
    cray_define(vm, "lambertian", 10, rproc_lambertian, p);
    cray_define(vm, "metallic", 8, rproc_metallic, p);
    cray_define(vm, "dielectric", 10, rproc_dielectric, p);
    cray_define(vm, "cam_aperture", 12, rproc_cam_aperture, p);
    cray_define(vm, "cam_lookfrom", 12, rproc_cam_lookfrom, p);
    cray_define(vm, "cam_lookat", 10, rproc_cam_lookat, p);
    cray_define(vm, "cam_vup", 7, rproc_cam_vup, p);
    cray_define(vm, "vfov", 4, rproc_vfov, p);
    cray_define(vm, "aspect", 6, rproc_aspect, p);
    cray_define(vm, "cam_aspect", 10, rproc_cam_aspect, p);
    cray_define(vm, "cam_dist", 8, rproc_cam_dist, p);
    cray_define(vm, "cam_focus_dist", 14, rproc_cam_focus_dist, p);
    cray_define(vm, "cam_update", 10, rproc_cam_update, p);

    return RUNT_OK;
}
