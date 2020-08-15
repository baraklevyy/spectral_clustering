#ifndef VECTOR_H_
#define VECTOR_H_

void vec_normalize(double *v, int size);
int diff_below_epsilon(double *v1, double *v2, int size);
double vec_dot(const double *v1, const double *v2, int size);
double vec_dot_int(const int *v1, const double *v2, int size);

#endif