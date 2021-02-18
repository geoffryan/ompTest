#include <math.h>
#include "prim.h"

void source_prim(double *vec, double x, double y, int nq)
{
    int q;

    vec[0] += sin(x+y);
    if(nq < 2)
        return;

    vec[1] += sqrt(x*x+y*y);
    if(nq < 3)
        return;

    vec[2] += (x*x + y*y) / (x+y);
    if(nq < 4)
        return;

    vec[3] += pow(x, y);
    if(nq < 5)
        return;

    vec[4] += (x + 4)*(y-5);
    if(nq < 6)
        return;

    for(q=5; q<nq; q++)
        vec[q] += 3.3*q;
}
