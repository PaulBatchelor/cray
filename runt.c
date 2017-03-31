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

    return RUNT_OK;
}
