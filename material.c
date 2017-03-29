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

vec3 cray_reflect(vec3 *v, vec3 *n)
{
    vec3 tmp;
    CRAYFLT s;
    s = 2.0 * VEC3_DOT(*v, *n);
    VEC3_MULS(*n, s, tmp);
    VEC3_SUB(*v, tmp, tmp);
    return tmp;
}
