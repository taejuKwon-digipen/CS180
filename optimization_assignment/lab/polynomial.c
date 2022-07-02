//Name : Taeju Kwon
//Assignment : Optimization
//Course Name : CS180
//Term&Year : Fall&2021

#include "polynomial.h"
#include "benchmark.h"
#include <math.h>

/* calculate a0 + a1*x + a2*x^2 + ... + an*x^n */
void poly0(const vec_ptr coefficients, const data_t* x_input, data_t* output)
{
    *output = 0;
    for (long n = 0; n < vec_length(coefficients); n++)
    {
        data_t an;
        get_vec_element(coefficients, n, &an);
        *output += an * (data_t)pow((double)*x_input, (double)n);
    }
}

/* TODO: Add alternative versions of poly1 */
void poly1(const vec_ptr coefficients, const data_t* x_input, data_t* output) // optimization
{
    long    length            = vec_length(coefficients);
    data_t* coefficients_data = get_vec_start(coefficients);
    data_t  sum               = (data_t)0;
    double  xinput            = (double)*x_input;
    int n = 0;
    double i = 1;

    while( n < length)
    {
        sum += coefficients_data[n] * i;
        n++;
        i *= xinput;
    }
    *output = sum;
}

void poly2x1(const vec_ptr coefficients, const data_t* x_input, data_t* output)
{
    *output                   = 0;
    long    length            = vec_length(coefficients);
    data_t* coefficients_data = get_vec_start(coefficients);
    data_t  sum               = (data_t)0;
    double  xinput            = (double)*x_input;
    long    limit             = length - 1;
    int i = 0;
    //double xinput2 = xinput * xinput;

    for (i = 0; i < limit; i += 2)
    {
        sum += (coefficients_data[i + 0] * 1 ) * (coefficients_data[i + 1]  * xinput);
    }
    while (i < length)
    {
        sum += coefficients_data[i] * xinput;
        i++;
    }
    *output = sum;
}

void poly3x1(const vec_ptr coefficients, const data_t* x_input, data_t* output)
{
    *output                   = 0;
    long    length            = vec_length(coefficients);
    data_t* coefficients_data = get_vec_start(coefficients);
    data_t  sum               = (data_t)0;
    double  xinput            = (double)*x_input;
    long    limit             = length - 2;

    double xinput2 = xinput * xinput;
    int i = 0;

    for (i = 0; i < limit; i += 2)
    {
        sum += (coefficients_data[i + 0] * 1) * (coefficients_data[i + 1] * xinput) * 
        (coefficients_data[i + 2] * xinput2);
    }
    double j = 1;
    while ( i < length)
    {
        sum += coefficients_data[i] * (xinput * j);
        i++;
        j *= xinput;
    }
    *output = sum;
}

void poly4x1(const vec_ptr coefficients, const data_t* x_input, data_t* output)
{
    *output                   = 0;
    long    length            = vec_length(coefficients);
    data_t* coefficients_data = get_vec_start(coefficients);
    data_t  sum               = (data_t)0;
    double  xinput            = (double)*x_input;
    long    limit             = length - 3;

    double xinput2 = xinput * xinput;
    double xinput3 = xinput2 * xinput;
    
    int i = 0;

    for ( i = 0; i < limit; i += 2)
    {
        sum += (coefficients_data[i + 0] * 1) * (coefficients_data[i + 1] * xinput) * 
        (coefficients_data[i + 2] *xinput2) * (coefficients_data[i + 3 ] * xinput3);
    }
    double j =1;
    while ( i < length)
    {
        sum += coefficients_data[i] * (xinput * j);
        i++;
        j*= xinput;
    }
    *output = sum;
}

void poly3x3(const vec_ptr coefficients, const data_t* x_input, data_t* output)
{
     *output                   = 0;
    long    length            = vec_length(coefficients);
    data_t* coefficients_data = get_vec_start(coefficients);
    double  xinput            = (double)*x_input;
    long    limit             = length - 2;
    data_t  sum0   = (data_t)0;
    data_t  sum1   = (data_t)0;
    data_t  sum2   = (data_t)0;
    double xinput2 = xinput * xinput;
    int i = 0;

    /* Do 6 elements at a time */
    for (i = 0; i < limit; i += 3)
    {
        sum0 += coefficients_data[i + 0] * 1;
        sum1 += coefficients_data[i + 1] * xinput;
        sum2 += coefficients_data[i + 2] * xinput2;
    }
    /* Finish off any remaining elements */
    double j = 1;
    while (i < length)
    {
        sum0+= coefficients_data[i] * j;
        i++;
        j*= xinput;
    }
    *output = (sum0 + sum1 + sum2);
}

void poly4x4(const vec_ptr coefficients, const data_t* x_input, data_t* output)
{
     *output                   = 0;
    long    length            = vec_length(coefficients);
    data_t* coefficients_data = get_vec_start(coefficients);
    double  xinput            = (double)*x_input;
    long    limit             = length - 3;
    data_t  sum0   = (data_t)0;
    data_t  sum1   = (data_t)0;
    data_t  sum2   = (data_t)0;
    data_t  sum3   = (data_t)0;
    int i = 0;

    double xinput2 = xinput * xinput;
    double xinput3 = xinput2 * xinput;
   
    /* Do 6 elements at a time */
    for ( i = 0; i < limit; i += 3)
    {
        sum0 += coefficients_data[i + 0] * 1;
        sum1 += coefficients_data[i + 1] * xinput;
        sum2 += coefficients_data[i + 2] * xinput2;
        sum3 += coefficients_data[i + 3] * xinput3;
    }
    /* Finish off any remaining elements */
    double j = 1;

    while (i < length)
    {
        sum0+= coefficients_data[i] * j;
        i++;
        j*= xinput;
    }
    *output = (sum0 + sum1) + (sum2 + sum3);
}

void poly5x5(const vec_ptr coefficients, const data_t* x_input, data_t* output)
{
     *output                   = 0;
    long    length            = vec_length(coefficients);
    data_t* coefficients_data = get_vec_start(coefficients);
    double  xinput            = (double)*x_input;
    long    limit             = length - 5;
    data_t  sum0   = (data_t)0;
    data_t  sum1   = (data_t)0;
    data_t  sum2   = (data_t)0;
    data_t  sum3   = (data_t)0;
    data_t  sum4   = (data_t)0;

    double xinput2 = xinput * xinput;
    double xinput3 = xinput2 * xinput;
    double xinput4 = xinput3 * xinput;
    int i = 0;
   
    /* Do 6 elements at a time */
    for ( i = 0; i < limit; i += 3)
    {
        sum0 += coefficients_data[i + 0] * 1;
        sum1 += coefficients_data[i + 1] * xinput;
        sum2 += coefficients_data[i + 2] * xinput2;
        sum3 += coefficients_data[i + 3] * xinput3;
        sum4 += coefficients_data[i + 4] * xinput4;
    }
    /* Finish off any remaining elements */
    double j =1;

    while (i < length)
    {
        sum0 += coefficients_data[i] * j;
        i++;
        j *= xinput;
    }
    *output = (sum0 + sum1) + (sum2 + sum3 + sum4);
}


void register_polynomial_functions(void)
{
    /* add base case to be measured */
    add_polynomial_function(poly0, poly0, "poly0: naive version.");
    add_polynomial_function(poly1, poly0, "poly1:");
    add_polynomial_function(poly2x1, poly0, "poly2x1: 2 X 1 unrolling"); 
    add_polynomial_function(poly3x1, poly0, "poly3x1: 3 X 1 unrolling"); 
    add_polynomial_function(poly4x1, poly0, "poly4x1: 4 X 1 unrolling"); 
    add_polynomial_function(poly3x3, poly0, "poly3x3: 3 X 3 unrolling"); 
    add_polynomial_function(poly4x4, poly0, "poly4x4: 4 X 4 unrolling"); 
    add_polynomial_function(poly5x5, poly0, "poly4x5: 5 X 5 unrolling"); 
}