#include <stdio.h>

#include "vec3.h"

int main()
{
    int nx;
    int ny;
    int j;
    int i;
    int ir, ig, ib;
    FILE *fp;
    vec3 col;

    nx = 200;
    ny = 100;

    fp = fopen("out.ppm", "w");

    fprintf(fp, "P3\n %d %d\n255\n", nx, ny);

    for(j = ny -1; j >= 0; j--) {
        for(i = 0; i < nx; i++) {
            VEC3_SET(col, 
                (CRAYFLT) i / (CRAYFLT)nx, 
                (CRAYFLT) j / (CRAYFLT)ny,
                0.2);
            ir = (int)(255.99 * col.x);
            ig = (int)(255.99 * col.y);
            ib = (int)(255.99 * col.z);
            fprintf(fp, "%d %d %d\n", ir, ig, ib);
        }
    }

    fclose(fp);
    return 0;
}
