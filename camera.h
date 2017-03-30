#ifndef CAMERAH
#define CAMERAH
typedef struct {
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
    vec3 u, v, w;
    CRAYFLT lens_radius;
    CRAYFLT vfov;
    CRAYFLT focus_dist;
    CRAYFLT aspect;
    vec3 lookat;
    vec3 lookfrom;
    vec3 vup;
} cray_camera;

void cray_camera_init(cray_camera *cam);
cray_ray cray_camera_get_ray(cray_camera *cam, CRAYFLT u, CRAYFLT v);

void cray_camera_update(cray_camera *cam);
void cray_camera_aperture(cray_camera *cam, CRAYFLT aperture);
void cray_camera_vfov(cray_camera *cam, CRAYFLT vfov);
void cray_camera_aspect(cray_camera *cam, CRAYFLT aspect);
void cray_camera_lookfrom(cray_camera *cam, CRAYFLT x, CRAYFLT y, CRAYFLT z);
void cray_camera_lookat(cray_camera *cam, CRAYFLT x, CRAYFLT y, CRAYFLT z);
void cray_camera_vup(cray_camera *cam, CRAYFLT x, CRAYFLT y, CRAYFLT z);
void cray_camera_focus_dist(cray_camera *cam, CRAYFLT focus_dist);

CRAYFLT cray_camera_dist(cray_camera *cam);
#endif
