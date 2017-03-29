#ifndef VEC3H
#define VEC3H
#include "sqrtf.h"

#ifndef CRAYFLT
#define CRAYFLT float
#endif

typedef struct {
    CRAYFLT x;
    CRAYFLT y;
    CRAYFLT z;
} vec3;

#define VEC3_SET(V, X, Y, Z) (V).x = (X); (V).y = (Y); (V).z = (Z);
#define VEC3_ADD(V1, V2, OUT) \
    (OUT).x = (V1).x + (V2).x;\
    (OUT).y = (V1).y + (V2).y;\
    (OUT).z = (V1).z + (V2).z;
#define VEC3_SUB(V2, V1, OUT) \
    (OUT).x = (V2).x - (V1).x;\
    (OUT).y = (V2).y - (V1).y;\
    (OUT).z = (V2).z - (V1).z;
#define VEC3_DIVS(V, S, OUT)\
    (OUT).x = (V).x / (S);\
    (OUT).y = (V).y / (S);\
    (OUT).z = (V).z / (S); 
#define VEC3_MULS(V, S, OUT) \
    (OUT).x = (V).x * (S);\
    (OUT).y = (V).y * (S);\
    (OUT).z = (V).z * (S);
#define VEC3_ADDS(V, S, OUT) \
    (OUT).x = (V).x + S;\
    (OUT).y = (V).y + S;\
    (OUT).z = (V).z + S;
#define VEC3_LENGTH(V, OUT)\
    (OUT) = cray_sqrtf((V).x*(V).x + (V).y*(V).y + (V).z*(V).z);
#define VEC3_SQUARED_LENGTH(V)\
    ((V).x*(V).x + (V).y*(V).y + (V).z*(V).z)
#define VEC3_UNIT_VECTOR(V, TMP, OUT)\
    VEC3_LENGTH(V, TMP);\
    VEC3_DIVS(V, TMP, OUT);
#define VEC3_DOT(A, B) ((A).x*(B).x + (A).y*(B).y + (A).z*(B).z)
#define VEC3_SQRT(V, OUT)\
    (OUT).x = cray_sqrtf((V).x);\
    (OUT).y = cray_sqrtf((V).y);\
    (OUT).z = cray_sqrtf((V).z);
#define VEC3_MUL(V1, V2, OUT)\
    (OUT).x = (V1).x * (V2).x;\
    (OUT).y = (V1).y * (V2).y;\
    (OUT).z = (V1).z * (V2).z;
#endif
