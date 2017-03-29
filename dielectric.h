#ifndef DIELECTRICH
#define DIELECTRICH

typedef struct {
    cray_material mat;
    CRAYFLT ref_idx;
} cray_dielectric;

void cray_dielectric_init(cray_dielectric *d);
void cray_dielectric_refraction(cray_dielectric *d, CRAYFLT ref_idx);

#endif
