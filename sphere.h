#ifndef SPHERE

typedef struct {
    vec3 center;
    CRAYFLT radius;
} cray_sphere;

cray_sphere * cray_to_sphere(cray_object *obj);

void cray_sphere_init(cray_sphere *sphere, vec3 *center, CRAYFLT r);

void cray_sphere_mk_obj(cray_sphere *sphere, cray_object *obj);

int cray_sphere_hit(cray_object *obj, 
    cray_ray *r,
    CRAYFLT t_min,
    CRAYFLT t_max,
    cray_hitable *rec);

#endif
