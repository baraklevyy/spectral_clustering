#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "B.h"
#include "Common.h"
#include "vector.h"
#include "SparseMatrix.h"

/*use flag == 0 if the use is regular i.e to calculate f_i_g .Otherwise use flag == 1 to include Kornecker delta for Matrix_shifting*/
double f_i_g(const struct _spmat *A, const int *k, int M, const int *g, int g_size, int i) {
	double res = 0;
	const int *g_pointer = g;
	int g_i = 0;
	node **rows = A->private;
	node *current = rows[i];/*first element in the list[i]*/

	while (NULL != current && g_i < g_size) {
		if (current->col > *(g_pointer + g_i))
			g_i++;
		else if (current->col < *(g_pointer + g_i))
			current = current->next;
		else {
			res += (1.0 - ((double)k[i] * (double)k[current->col]) / (double)M);
			
			current = current->next;
			g_i++;
		}
	}

	return res;
}


double B_gag_i_j(int i, int j, const struct _spmat *A, const int *k, const int *g, int g_size, int M) {
	double first = 0.0;
	double second = 0.0;
	int g_i = 0;
	const int *g_pointer = g;
	node **rows = A->private;
	node *current = rows[i];/*first element in the list[i]*/
	
	// calculate first element
	while (current != NULL) {
		if (current->col == j) {
			first = 1.0 - ((double)k[i] * (double)k[j]) / (double)M;
			break;
		}
		else if (current->col > j || !current->next) {
			first = -((double)k[i] * (double)k[j]) / (double)M;
			break;
		}
		current = current->next;
	}

	second = (i == j ? f_i_g(A, k, M, g, g_size, i) : 0);
	
	return first - second;
}

double B_gag_L1_norm(const struct _spmat *A, const int *k, int M, const int *g, int g_size) {
	int gi, gj;
	int i, j;
	double Bgagij;
	double current_row_sum;
	double max_row_sum;

	for (gi = 0; gi < g_size; gi++) {
		i = g[gi];
		current_row_sum = 0;
		for (gj = 0; gj < g_size; gj++) {
			j = g[gj];
			Bgagij = B_gag_i_j(i, j, A, k, g, g_size, M);
			current_row_sum += fabs(Bgagij);
		}

		if (gi == 0)
			max_row_sum = current_row_sum;
		else
			max_row_sum = MAX(max_row_sum, current_row_sum);
	}

	return max_row_sum;
}

// helper function that multiplies Bgag with a given vector, and stores the result in res
void B_gag_vec_mult(const struct _spmat *A, const int *k, const int *g, int g_size, int M, double L1norm,
					const double *v, double *res) {
	int gi, gj;
	int i, j;
	double Bgagij;
	double row_sum;
	
	for (gi = 0; gi < g_size; gi++) {
		i = g[gi];
		row_sum = 0;

		for (gj = 0; gj < g_size; gj++) {
			j = g[gj];
			Bgagij = B_gag_i_j(i, j, A, k, g, g_size, M);

			// matrix shifting addition
			if (i == j)
				Bgagij += L1norm;

			row_sum += Bgagij * (*(v + gj));
		}

		*(res + gi) = row_sum;
	}
}

// int version of the helper function above
void B_gag_vec_mult_int(const struct _spmat *A, const int *k, const int *g, int g_size, int M, double L1norm,
	const int *v, double *res) {
	int gi, gj;
	int i, j;
	double Bgagij;
	double row_sum;

	for (gi = 0; gi < g_size; gi++) {
		i = g[gi];
		row_sum = 0;

		for (gj = 0; gj < g_size; gj++) {
			j = g[gj];
			Bgagij = B_gag_i_j(i, j, A, k, g, g_size, M);

			// matrix shifting addition
			if (i == j)
				Bgagij += L1norm;

			row_sum += Bgagij * (double)(*(v + gj));
		}

		*(res + gi) = row_sum;
	}
}

#define disp(v) printf("[%lf, %lf]\n", v[0], v[1])

Status power_iteration(const struct _spmat *A, const int *k, const int *g, int g_size, int M, double L1norm, double *res) {
	Status status = INVALID_STATUS_CODE;
	int i;
	//int is_res = 1; // if is_res==1 then the result of the multiplication is stored in res, otherwise in v

	//generate a random normalized vector
	double *v = (double*)malloc(g_size * sizeof(double));
	if (NULL == v) {
		status = MALLOC_FAILED_CODE;
		get_error_message(status);
		goto l_cleanup;
	}

	for (i = 0; i < g_size; i++)
		*(v + i) = (double)rand() / (double)RAND_MAX;
	vec_normalize(v, g_size);

	while (1) {
		B_gag_vec_mult(A, k, g, g_size, M, L1norm, v, res);

		vec_normalize(res, g_size);

		if (diff_below_epsilon(v, res, g_size))
			break;

		for (i = 0; i < g_size; i++)
			*(v + i) = *(res + i);
	}

	free(v);
	status = SUCCESS_STATUS_CODE;
	return status;
l_cleanup:
	exit(status);	
}

Status power_iteration_eigval(const struct _spmat *A, const int *k, const int *g, int g_size, int M,
							  double L1norm, const double *eig_vec, double *eig_val) {
	Status status = INVALID_STATUS_CODE;
	double *Bgag_mul_eig_vec;
	double denom;

	Bgag_mul_eig_vec = (double*)malloc(g_size * sizeof(double));

	B_gag_vec_mult(A, k, g, g_size, M, L1norm, eig_vec, Bgag_mul_eig_vec);

	denom = vec_dot(eig_vec, eig_vec, g_size);
	if (!IS_POSITIVE(denom)) {
		status = ZERO_DIVISION_CODE;
		get_error_message(status);
		goto l_cleanup;
	}

	*eig_val = vec_dot(eig_vec, Bgag_mul_eig_vec, g_size) / denom;
	*eig_val -= L1norm;

	free(Bgag_mul_eig_vec);
	status = SUCCESS_STATUS_CODE;
	return status;

l_cleanup:
	free(Bgag_mul_eig_vec);
	exit(status);
}