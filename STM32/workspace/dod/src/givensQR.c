#include <stddef.h>
#include <math.h>
#include "givensQR.h"

static void _givens(float *c, float *s, float a, float b);

void givensQR(size_t m, size_t n, float A[m][n]) // A is passed using C99 VLA
{
    // See Algorithm 5.2.4 from Golub and van Loan (2013)
    for (size_t j = 0; j < n; ++j)
    {
        for (size_t i = m-1; i > j; --i)
        {
            float c, s;
            _givens(&c, &s, A[i-1][j], A[i][j]);
            for (size_t k = j; k < n; ++k)
            {
                float tau1 = A[i-1][k];
                float tau2 = A[i][k];
                A[i-1][k]  = c*tau1 - s*tau2;
                A[i][k]    = s*tau1 + c*tau2;
            }
        }
    }
}

void _givens(float *c, float *s, float a, float b)
{
    // See Algorithm 5.1.3 from Golub and van Loan (2013)
    if (b == 0.0f)
    {
        *c = 1.0f;
        *s = 0.0f;
    }
    else
    {
        if (fabsf(b) > fabsf(a))
        {
            float tau = -a/b;
            *s = 1.0f/sqrtf(1.0f + tau*tau);
            *c = *s*tau;
        }
        else
        {
            float tau = -b/a;
            *c = 1.0f/sqrtf(1.0f + tau*tau);
            *s = *c*tau;
        }
    }
}