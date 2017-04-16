#ifndef SOLIDH
#define SOLIDH

typedef struct {
    cray_material mat;
    vec3 albedo;
} cray_solid;

void cray_solid_init(cray_solid *s);
void cray_solid_color(cray_solid *s, CRAYFLT r, CRAYFLT g, CRAYFLT b);

#endif
