#ifndef SCENEH
#define SCENEH
typedef struct cray_scene {
    cray_camera cam;
    cray_hitablelist world;
    int verbose;
    int nx;
    int ny;
    int ns;
} cray_scene;
#endif
