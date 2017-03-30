#ifndef SCENEH
#define SCENEH
typedef struct cray_scene {
    cray_camera cam;
    cray_hitablelist world;
    int nx;
    int ny;
    int ns;
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

#endif
