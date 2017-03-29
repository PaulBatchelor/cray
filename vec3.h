#ifndef VEC3H
#define VEC3H

#ifndef CRAYFLT
#define CRAYFLT float
#endif

typedef struct {
    CRAYFLT x;
    CRAYFLT y;
    CRAYFLT z;
} vec3;

#define VEC3_SET(V, X, Y, Z) (V).x = (X); (V).y = (Y); (V).z = (Z);

#endif
