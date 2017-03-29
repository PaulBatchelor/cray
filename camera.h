#ifndef CAMERAH
#define CAMERAH
typedef struct {
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
} cray_camera;

void cray_camera_init(cray_camera *cam);
cray_ray cray_camera_get_ray(cray_camera *cam, CRAYFLT u, CRAYFLT v);
#endif
