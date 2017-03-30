#ifndef HITABLELISTH
#define HITABLELISTH

typedef struct {
    cray_object **list;
    int list_size;
    int max_size;
} cray_hitablelist;

void cray_hitablelist_init(cray_hitablelist *hitlist, 
        cray_object **list, 
        int list_size);

int cray_hitablelist_hit(cray_hitablelist *hitlist,
        cray_ray *ray, 
        CRAYFLT t_min,
        CRAYFLT t_max,
        cray_hitable *hit);

int cray_hitablelist_add(cray_hitablelist *hitlist, cray_object *obj, int id);

int cray_hitablelist_append(cray_hitablelist *hitlist, cray_object *obj);
#endif
