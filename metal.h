#ifndef METALH
#define METALH

typedef struct {
    cray_material mat;
    vec3 albedo;
    CRAYFLT fuzz;
} cray_metal;

void cray_metal_init(cray_metal *m);
void cray_metal_color(cray_metal *m, CRAYFLT r, CRAYFLT g, CRAYFLT b);
void cray_metal_fuzz(cray_metal *m, CRAYFLT fuzz);

#endif
