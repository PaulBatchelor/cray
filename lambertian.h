#ifndef LAMBERTIANH
#define LAMBERTIANH

typedef struct {
    cray_material mat;
} cray_lambertian;

void cray_lambertian_init(cray_lambertian *l);
void cray_lambertian_color(cray_lambertian *l, CRAYFLT r, CRAYFLT g, CRAYFLT b);

#endif
