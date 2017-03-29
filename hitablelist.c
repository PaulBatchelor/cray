#include <stdio.h>
#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "hitablelist.h"

int cray_hitablelist_hit(cray_hitablelist *hitlist,
        cray_ray *ray, 
        CRAYFLT t_min,
        CRAYFLT t_max,
        cray_hitable *hit)
{
    cray_hitable temp_rec;
    int hit_anything;
    double closest_so_far;
    int i;
    cray_object *obj;

    hit_anything = 0;
    closest_so_far = t_max;

    for(i = 0; i < hitlist->list_size; i++) {
        obj = hitlist->list[i];
        if(obj->f(obj, ray, t_min, closest_so_far, &temp_rec)) {
            hit_anything = 1;
            closest_so_far = temp_rec.t;
            *hit = temp_rec;
        }
    }


    return hit_anything;
}

void cray_hitablelist_init(cray_hitablelist *hitlist, 
        cray_object **list, 
        int list_size)
{
    hitlist->list = list;
    hitlist->list_size = list_size;
}
