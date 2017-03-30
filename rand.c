#include <stdlib.h>
#include "vec3.h"

CRAYFLT cray_rand()
{
    return (CRAYFLT) rand() / RAND_MAX;
}

vec3 random_in_unit_sphere() {
    vec3 p;
    vec3 tmp[2];
    do {
        VEC3_SET(tmp[0], cray_rand(), cray_rand(), cray_rand());
        VEC3_MULS(tmp[0], 2.0, tmp[0]);

        VEC3_SET(tmp[1], 1, 1, 1);
        VEC3_SUB(tmp[0], tmp[1], p);
    } while(VEC3_SQUARED_LENGTH(p) >= 1.0);

    return p;
}

vec3 random_in_unit_disk()
{
    vec3 p;
    vec3 tmp[2];
    do {
        VEC3_SET(tmp[0], cray_rand(), cray_rand(), 0);
        VEC3_MULS(tmp[0], 2, tmp[0]);

        VEC3_SET(tmp[1], 1, 1, 0);
        VEC3_SUB(tmp[0], tmp[1], p);
    } while ( VEC3_DOT(p, p) >= 1.0);

    return p;
}
