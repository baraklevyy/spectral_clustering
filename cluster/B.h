#include <stdio.h>
#include "Common.h"
#include "SparseMatrix.h"



#ifndef B_H_
#define B_H_


double f_i_g(const struct _spmat *A, const int *k, int M, const int *g, int g_size, int i);
double B_gag_i_j(int i, int j, const struct _spmat *A, const int *k, const int *g, int g_size, int M);
double B_gag_L1_norm(const struct _spmat *A, const int *k, int M, const int *g, int g_size);
Status power_iteration(const struct _spmat *A, const int *k, const int *g, int g_size, int M, double L1norm, double *res);
Status power_iteration_eigval(const struct _spmat *A, const int *k, const int *g, int g_size, int M,
							  double L1norm, const double *eig_vec, double *eig_val);
void B_gag_vec_mult_int(const struct _spmat *A, const int *k, const int *g, int g_size, int M, double L1norm,
	const int *v, double *res);

#endif /* B_H_ */
