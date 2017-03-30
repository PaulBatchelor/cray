#ifndef DIELECTRICH
#define DIELECTRICH

typedef struct {
    cray_material mat;
    CRAYFLT ref_idx;
    vec3 albedo;
} cray_dielectric;

void cray_dielectric_init(cray_dielectric *d);
void cray_dielectric_refraction(cray_dielectric *d, CRAYFLT ref_idx);
void cray_dielectric_color(cray_dielectric *d, CRAYFLT r, CRAYFLT g, CRAYFLT b);

#endif
