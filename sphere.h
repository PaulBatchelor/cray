#ifndef SPHERE

typedef struct {
    cray_object obj;
    vec3 center;
    CRAYFLT radius;
    cray_material *mat;
} cray_sphere;

cray_sphere * cray_to_sphere(cray_object *obj);

void cray_sphere_init(cray_sphere *sphere, 
    vec3 *center, 
    CRAYFLT r, 
    cray_material *mat);

int cray_sphere_hit(cray_object *obj, 
    cray_ray *r,
    CRAYFLT t_min,
    CRAYFLT t_max,
    cray_hitable *rec);

#endif
