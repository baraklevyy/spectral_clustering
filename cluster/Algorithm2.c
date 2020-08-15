#include <stdlib.h>
#include "Algorithm2.h"

Status algorithm2(const struct _spmat *A, const int *k, int M, const int *g, int g_size, int *s) {
	Status status = INVALID_STATUS_CODE;
	double L1norm = B_gag_L1_norm(A, k, M, g, g_size);
	double *normalized_eig_vec;
	double eig_val;
	int i;

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

	for (i = 0; i < g_size; i++)
		*(s + i) = 1 ? IS_POSITIVE(*(normalized_eig_vec + i)) : 0;

	// we now use normalized_eig_vec to store the result of B_gag[g] * s;
	B_gag_vec_mult_int(A, k, g, g_size, M, L1norm, s, normalized_eig_vec);
	
	if (!IS_POSITIVE(vec_dot_int(s, normalized_eig_vec, g_size))) {
		status = GROUP_NOT_DIVISIBLE_CODE;
		get_error_message(status);
		goto l_cleanup;
	}

	free(normalized_eig_vec);
	status = SUCCESS_STATUS_CODE;
	return status;

l_cleanup:
	free(normalized_eig_vec);
	exit(status);
}

