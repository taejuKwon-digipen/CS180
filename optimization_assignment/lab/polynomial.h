#pragma once

#include "vec.h"

#ifdef __cplusplus
extern "C"
{
#endif
    /* Declaration of a polynomial routine */
    typedef void (*polynomial)(const vec_ptr, const data_t*, data_t*);

    /* Called by to register all the various implementations of polynomial for benchmarking */
    void register_polynomial_functions(void);

#ifdef __cplusplus
} // extern "C"
#endif
