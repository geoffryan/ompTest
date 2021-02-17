#ifndef FUNCS_H
#define FUNCS_H

void source0(double **dom, int nx, int *ny, int nq);
void source1(double **dom, int nx, int *ny, int nq);
void source2(double **dom, int nx, int *ny, int nq);
void source3(double **dom, int nx, int *ny, int nq);
void source4(double **dom, int nx, int *ny, int nq);
void plm(double **dom, int nx, int *ny, int nq);
void flux(double **dom, int nx, int *ny, int nq);
double reduce(double **dom, int nx, int *ny, int nq);

#endif
