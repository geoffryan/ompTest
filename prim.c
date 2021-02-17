#include <math.h>
#include "prim.h"

void source_prim(double *vec, double x, double y, int nq)
{
    int q;
    for(q=0; q<nq; q++)
        vec[q] += sin(x+y*q);
}
