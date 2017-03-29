#ifndef MATERIALH
#define MATERIALH

enum { CRAY_LAMBERTIAN };

typedef int (* cray_matfunc) (cray_material*, cray_ray *,CRAYFLT, CRAYFLT, cray_hitable*);

struct cray_material {
    cray_matfunc scatter;
    unsigned int type;
    void *ud;
};

#endif
