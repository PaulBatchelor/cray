#ifndef HITABLELISTH
#define HITABLELISTH

typedef struct {
    cray_object **list;
    int list_size;
} cray_hitablelist;

int cray_hitablelist_hit(cray_hitablelist *hitlist,
        cray_ray *ray, 
        CRAYFLT t_min,
        CRAYFLT t_max,
        cray_hitable *hit);

#endif
