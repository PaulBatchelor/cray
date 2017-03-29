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
#define VEC3_ADD(V1, V2, OUT) \
    (OUT).x = (V1).x + (V2).x;\
    (OUT).y = (V1).y + (V2).y;\
    (OUT).z = (V1).z + (V2).z;
#define VEC3_DIVS(V, S, OUT)\
    (OUT).x = (V).x / (S);\
    (OUT).y = (V).y / (S);\
    (OUT).z = (V).z / (S); 
#define VEC3_MULS(V, S, OUT) \
    (OUT).x = (V).x * (S);\
    (OUT).y = (V).y * (S);\
    (OUT).z = (V).z * (S);
#define VEC3_LENGTH(V, OUT)\
    (OUT) = sqrt((V).x*(V).x + (V).y*(V).y + (V).z*(V).z);
#define VEC3_UNIT_VECTOR(V, TMP, OUT)\
    VEC3_LENGTH(V, TMP);\
    VEC3_DIVS(V, TMP, OUT);


#endif
