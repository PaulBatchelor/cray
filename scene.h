#ifndef SCENEH
#define SCENEH

#ifndef CRAYFLT
#define CRAYFLT float
#endif


#define CRAY_VERBOSE 1
#define CRAY_SEGMENT 2

typedef struct cray_scene cray_scene;

#ifndef CRAY_RAY_STRUCT
#define CRAY_RAY_STRUCT
typedef struct cray_ray cray_ray;
#endif

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

/* used with runt to get global data struct */
cray_scene *cray_get();

void cray_scene_cb_bgcolor(cray_scene *scene, 
    void (*bgcolor)(cray_scene *, cray_ray *, int, int, CRAYFLT *));
void cray_scene_default_bgcolor(cray_scene *scene);
void cray_scene_maxdepth(cray_scene *scene, int maxdepth);
void cray_scene_cb_shadow(cray_scene *scene, 
        void (* shadow)(cray_scene *, CRAYFLT *, int, int));
void cray_scene_default_shadow(cray_scene *scene);
#endif
