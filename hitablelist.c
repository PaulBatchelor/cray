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
    hitlist->list_size = 0;
    hitlist->max_size = list_size;
}

int cray_hitablelist_add(cray_hitablelist *hitlist, cray_object *obj, int id)
{
   if(id > hitlist->max_size - 1) return 0;
   hitlist->list[id] = obj;
   return 1;
}

int cray_hitablelist_append(cray_hitablelist *hitlist, cray_object *obj)
{
    int id;
    if(hitlist->list_size >= hitlist->max_size) return 0;
    id = hitlist->list_size;
    hitlist->list_size++;
    return cray_hitablelist_add(hitlist, obj, id);
}
