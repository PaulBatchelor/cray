#ifndef RAYH
#define RAYH

typedef struct {
    vec3 A;
    vec3 B;
} cray_ray;

vec3 cray_ray_origin(cray_ray *ray);
vec3 cray_ray_direction(cray_ray *ray);
vec3 cray_ray_point_at_param(cray_ray *ray, CRAYFLT t);

#define RAY_SET(RAY, ORIGIN, DIRECTION) (RAY).A = ORIGIN; (RAY).B = DIRECTION;


#endif
