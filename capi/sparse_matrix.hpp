#ifndef SKYLARK_SPARSE_MATRIX_HPP
#define SKYLARK_SPARSE_MATRIX_HPP

#include "matrix_types.hpp"
#include "../base/sparse_dist_matrix.hpp"

struct sparse_matrix_wrapper;

extern "C" {

SKYLARK_EXTERN_API int sl_create_sparse_matrix(char *type_, int N, 
    sparse_matrix_wrapper **sparse_matrix);

}

#endif // SKYLARK_SPARSE_MATRIX_HPP