#include "Common.h"
#include "SparseMatrix.h"

#ifndef UTILITY_H_
#define UTILITY_H_

Status generate_graph();
Status graph_loading(int argc, char** argv, struct _spmat *A, int *k, int *M);
Status extract_matrix_size(int argc, char** argv, int *n);



#endif /* UTILITY_H_ */
