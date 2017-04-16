#ifndef MATERIALH
#define MATERIALH


enum { CRAY_EMPTY, CRAY_LAMBERTIAN, CRAY_METAL, CRAY_DIELECTRIC, CRAY_SOLID };

typedef int (* cray_matfunc) (cray_material*, 
        cray_hitable *, 
        cray_ray *,
        vec3 *,
        cray_ray *);

typedef void (* cray_tex) (cray_material*,
        CRAYFLT ,
        CRAYFLT ,
        vec3 *,
        vec3 *);

struct cray_material {
    cray_matfunc scatter;
    cray_tex value;
    unsigned int type;
    void *ud;
    vec3 albedo;
};

void cray_material_init(cray_material *m);
vec3 cray_reflect(vec3 *v, vec3 *n);
void cray_material_albedo(cray_material *m, CRAYFLT r, CRAYFLT g, CRAYFLT b);

#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "solid.h"
#endif
