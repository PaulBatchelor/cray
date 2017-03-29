#include <stdio.h>

int main()
{
    int nx;
    int ny;
    int j;
    int i;
    float r, g, b;
    int ir, ig, ib;
    FILE *fp;

    nx = 200;
    ny = 100;

    fp = fopen("out.ppm", "w");

    fprintf(fp, "P3\n %d %d\n255\n", nx, ny);

    for(j = ny -1; j >= 0; j--) {
        for(i = 0; i < nx; i++) {
            r = (float)i / (float)nx;
            g = (float)j / (float)ny;
            b = 0.2;
            ir = (int)(255.99 * r);
            ig = (int)(255.99 * g);
            ib = (int)(255.99 * b);
            fprintf(fp, "%d %d %d\n", ir, ig, ib);
        }
    }

    fclose(fp);
    return 0;
}
