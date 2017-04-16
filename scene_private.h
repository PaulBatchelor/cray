#ifndef SCENE_PRIVATE
#define SCENE_PRIVATE

struct cray_scene {
    cray_camera cam;
    cray_hitablelist world;
    int nx;
    int ny;
    int ns;
    vec3 tint[2];
    void (*bgcolor)(cray_scene *, cray_ray *, int, int, CRAYFLT *);
    void (*shadow)(cray_scene *, CRAYFLT *, int, int);
    int maxdepth;
};

#endif
