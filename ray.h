#ifndef RAYH
#define RAYH

#ifndef CRAY_RAY_STRUCT
#define CRAY_RAY_STRUCT
typedef struct cray_ray cray_ray;
#endif

struct cray_ray {
    vec3 A;
    vec3 B;
};

vec3 cray_ray_origin(cray_ray *ray);
vec3 cray_ray_direction(cray_ray *ray);
vec3 cray_ray_point_at_param(cray_ray *ray, CRAYFLT t);

#define RAY_SET(RAY, ORIGIN, DIRECTION) (RAY).A = ORIGIN; (RAY).B = DIRECTION;


#endif
