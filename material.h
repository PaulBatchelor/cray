#ifndef MATERIALH
#define MATERIALH


enum { CRAY_EMPTY, CRAY_LAMBERTIAN, CRAY_METAL, CRAY_DIELECTRIC, CRAY_SOLID };

typedef int (* cray_matfunc) (cray_material*, 
        cray_hitable *, 
        cray_ray *,
        vec3 *,
        cray_ray *);

struct cray_material {
    cray_matfunc scatter;
    unsigned int type;
    void *ud;
};

void cray_material_init(cray_material *m);
vec3 cray_reflect(vec3 *v, vec3 *n);

#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "solid.h"
#endif
