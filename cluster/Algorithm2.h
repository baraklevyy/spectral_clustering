#ifndef ALGORITHM2_H_
#define ALGORITHM2_H_

#include "Common.h"
#include "B.h"
#include "SparseMatrix.h"
#include "vector.h"

Status algorithm2(const struct _spmat *A, const int *k, int M, const int *g, int g_size, int *s, int g1_size, int g2_size);
void extract_vectors(int *g, int g_size, int *s, int *v1, int *v2);
#endif