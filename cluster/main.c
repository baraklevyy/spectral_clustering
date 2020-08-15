/*
 ============================================================================
 Name        : Clustering.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SparseMatrix.h"
#include "Common.h"
#include "Utility.h"
#include "B.h"
#include "Algorithm2.h"

int main(int argc, char *argv[]) {
	Status status = INVALID_STATUS_CODE;
	int g[] = {0,1,2,3,4};
	int g_size = 5;
	spmat *A;
	int M, *k;
	int n;
	/*status = generate_graph();*/
	// seed random
	srand((unsigned int)time(0));
	status = extract_matrix_size(argc, argv, &n);
	A = spmat_allocate_list(n); /*allocation check within the function*/
	if (NULL == A) {
		status = MALLOC_FAILED_CODE;
		get_error_message(status);
		goto l_cleanup;
	}
	k = (int*)malloc(n * sizeof(int));
	if (NULL == k) {
		status = MALLOC_FAILED_CODE;
		get_error_message(status);
		goto l_cleanup;
	}
	status = graph_loading(argc, argv, A, k, &M);

	printf("total number of vertices %d", A->n);
	printf("k: ");
	for (int i = 0; i < n; i++)
		printf("%d, ", k[i]);
	printf("\n");
	
	int s[] = { 0,0,0,0,0 };
	algorithm2(A, k, M, g, g_size, s);
	printf("s: ");
	for (int i = 0; i < g_size; i++)
		printf("%d, ", s[i]);
	printf("\n");

	status = SUCCESS_STATUS_CODE;
	return 0;

l_cleanup:
	exit(status);
}
