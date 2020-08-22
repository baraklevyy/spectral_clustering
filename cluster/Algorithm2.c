#include <stdlib.h>
#include "Algorithm2.h"

Status algorithm2(const struct _spmat *A, const int *k, int M, const int *g, int g_size, int *s) {
	Status status = INVALID_STATUS_CODE;
	double L1norm = B_gag_L1_norm(A, k, M, g, g_size);
	double *normalized_eig_vec;
	double eig_val;
	int i;
/*
	g1_size = 0;
	g2_size = 0;
*/

	normalized_eig_vec = (double*)malloc(g_size * sizeof(double));
	if (NULL == normalized_eig_vec) {
		status = MALLOC_FAILED_CODE;
		get_error_message(status);
		goto l_cleanup;
	}

	// calculate eigenvector of maximal eigenvalue
	power_iteration(A, k, g, g_size, M, L1norm, normalized_eig_vec);

	// get corresponding eigenvalue
	power_iteration_eigval(A, k, g, g_size, M, L1norm, normalized_eig_vec, &eig_val);

	if (!IS_POSITIVE(eig_val)) {
		status = GROUP_NOT_DIVISIBLE_CODE;
		get_error_message(status);
		goto l_cleanup;
	}
	/*
	for (i = 0; i < g_size; i++)
		*(s + i) = 1 ? IS_POSITIVE(*(normalized_eig_vec + i)) : -1;
	
	*/
	for (i = 0; i < g_size; i++) {
		if (IS_POSITIVE(*(normalized_eig_vec + i)))
			*(s + i) = 1;
		else
			*(s + i) = -1;

	}
	// we now use normalized_eig_vec to store the result of B_gag[g] * s;
	B_gag_vec_mult_int(A, k, g, g_size, M, L1norm, s, normalized_eig_vec);
	
	if (!IS_POSITIVE(vec_dot_int(s, normalized_eig_vec, g_size))) {
		status = GROUP_NOT_DIVISIBLE_CODE;
		get_error_message(status);
		goto l_cleanup;
	}
	/*Implementing sizes into g1_size and g2_size*/
/*	for (i = 0; i < g_size; i++) {
		if (*(s + i) == 1) {
			*(g1_size) = *(g1_size) + 1;
		}
		else *(g2_size) = *(g2_size)+1;
	}
*/
	free(normalized_eig_vec);
	status = SUCCESS_STATUS_CODE;
	return status;

l_cleanup:
	free(normalized_eig_vec);
	for (i = 0; i < g_size; i++) *(s + i) = 1;
	return status;
}
/*Extracting the 2 groups indices(from output of algorithm 2) */
void extract_vectors(int *g, int g_size, int *s, int *v1, int *v2, int *g1_size, int *g2_size) {
	int i;
	for  (i = 0; i < g_size; i++){
		if (*(s + i) == 1) {
			*(v1++) = *(g + i);
			*(g1_size) = *(g1_size) + 1;
		}
		else {
			*(v2++) = *(g + i);
			*(g2_size) = *(g2_size)+1;
		}
	}
}
