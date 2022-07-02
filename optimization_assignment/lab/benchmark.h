#pragma once
#include "polynomial.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /* Add a new version of the inner product function to benchmark.
        Also needs the base version to test for correctness since there is no point in getting a faster version that
       produces wrong results. It also takes in a description of the new version. This will be printed with the stats,
       so we know who the stats belongs to.
     */
    void add_polynomial_function(polynomial new_version, polynomial base_version, char* new_version_description);


#ifdef __cplusplus
} // extern "C"
#endif
