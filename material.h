#ifndef MATERIALH
#define MATERIALH


enum { CRAY_EMPTY, CRAY_LAMBERTIAN, CRAY_METAL };

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

#include "lambertian.h"
#include "metal.h"
#endif
