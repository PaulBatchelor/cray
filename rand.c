#include <stdlib.h>
#include "vec3.h"

CRAYFLT cray_rand()
{
    return (CRAYFLT) rand() / RAND_MAX;
}
