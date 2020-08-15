#include "Common.h"
#include "SparseMatrix.h"

#ifndef UTILITY_H_
#define UTILITY_H_

Status load_spmat_from_cmd();
void subtract_mat(const struct _spmat *A, int *k, int M, int *g, int row, int *result);/*may be missing a row variable*/
Status graph_loading(int argc, char** argv, struct _spmat *A, int *k, int *M);
Status extract_matrix_size(int argc, char** argv, int *n);
Status generate_graph();


#endif /* UTILITY_H_ */
