#ifndef HITABLEH
#define HITABLEH

enum { CRAY_SPHERE };

typedef struct cray_object cray_object;
typedef struct cray_material cray_material;

typedef struct {
    float t;
    vec3 p;
    vec3 normal;
    cray_material *mat;
} cray_hitable;

typedef int (* cray_func) (cray_object *, cray_ray *,CRAYFLT, CRAYFLT, cray_hitable*);

struct cray_object {
   cray_func f;
   unsigned int type;
   void *ud;
};


#endif
