#include "unity_fixture.h"

#include <stdint.h>
#include "givensQR.h"

TEST_GROUP(GivensQR);

TEST_SETUP(GivensQR)
{
}

TEST_TEAR_DOWN(GivensQR)
{
}

TEST(GivensQR, UpperTriangular)
{
    enum {M = 4, N = 3};
    float A_expected[M][N] =
    {
        {1.0f, 2.0f, 3.0f},
        {0.0f, 4.0f, 5.0f},
        {0.0f, 0.0f, 6.0f},
        {0.0f, 0.0f, 0.0f}
    };

    float A[M][N] =
    {
        {1.0f, 2.0f, 3.0f},
        {0.0f, 4.0f, 5.0f},
        {0.0f, 0.0f, 6.0f},
        {0.0f, 0.0f, 0.0f}
    };

    // Replace A with qr(A)
    givensQR(M,N,A);
    
    for (size_t i = 0; i < M; ++i)
        for (size_t j = 0; j < N; ++j)
            if (i > j) TEST_ASSERT_EQUAL_FLOAT(0.0f, A[i][j]);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(A_expected, A, M*N);
}

// Since the matrix square root isn't unique,
// show that qr(A).'*qr(A) == A.'*A
TEST(GivensQR, Squared)
{
    enum {M = 4, N = 3};

    float AA_expected[N][N] =
    {
        {166.0f, 188.0f, 210.0f},
        {188.0f, 214.0f, 240.0f},
        {210.0f, 240.0f, 270.0f}
    };

    float A[M][N] =
    {
        {1.0f, 2.0f, 3.0f},
        {4.0f, 5.0f, 6.0f},
        {7.0f, 8.0f, 9.0f},
        {10.0f, 11.0f, 12.0f}
    };

    // Replace A with qr(A)
    givensQR(M,N,A);

    // AA = A.'*A
    float AA[N][N];
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            AA[i][j] = 0.0f;
            for (size_t k = 0; k < M; ++k)
            {
                AA[i][j] += A[k][i]*A[k][j];
            }
        }
    }

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(AA_expected, AA, N*N);
}



TEST_GROUP_RUNNER(GivensQR)
{
    RUN_TEST_CASE(GivensQR, UpperTriangular);
    RUN_TEST_CASE(GivensQR, Squared);
}