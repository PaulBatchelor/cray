#ifndef SCENEH
#define SCENEH
typedef struct cray_scene {
    cray_camera cam;
    cray_hitablelist world;
    int nx;
    int ny;
    int ns;
    vec3 tint[2];
} cray_scene;

void cray_scene_init(cray_scene *scene, 
    int width, 
    int height, 
    int samp, 
    int nobjects);

void cray_scene_write_ppm(cray_scene *scene, 
    const char *filename, 
    CRAYFLT *buf);

void cray_scene_render(cray_scene *scene, 
    int offx, int offy,
    int w, int h,
    CRAYFLT *buf, int verbose);

void cray_scene_tint_top(cray_scene *scene, CRAYFLT r, CRAYFLT g, CRAYFLT b);
void cray_scene_tint_bottom(cray_scene *scene, CRAYFLT r, CRAYFLT g, CRAYFLT b);
void cray_demo();

#endif
