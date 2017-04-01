#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <runt.h>

#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "hitablelist.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "scene.h"

runt_int runt_load_cray(runt_vm *vm);
static runt_int loader(runt_vm *vm)
{
    runt_load_minimal(vm);
    runt_load_cray(vm);
    return RUNT_OK;
}

int main(int argc, char *argv[])
{
    return irunt_begin(argc, argv, loader);
}
