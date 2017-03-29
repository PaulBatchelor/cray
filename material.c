#include <stdlib.h>
#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "material.h"

static int cray_nothing (cray_material *m, 
        cray_hitable *hit, 
        cray_ray *ray_in, 
        vec3 *vec, 
        cray_ray *scattered)
{
    return 0;
}

void cray_material_init(cray_material *m)
{
    m->type = CRAY_EMPTY;
    m->ud = NULL;
    m->scatter = cray_nothing;
}
