#include "unity_fixture.h"

#include <stdint.h>
#include "observer.h"

TEST_GROUP(Observer);

TEST_SETUP(Observer)
{
    obs_init();
}

TEST_TEAR_DOWN(Observer)
{
}

TEST(Observer, ZeroInitialState)
{
    TEST_ASSERT_EACH_EQUAL_FLOAT(0.0f, obs_get_state(), OBS_N_STATE);
}

TEST(Observer, SetState)
{
    float state_expected[OBS_N_STATE];
    for (size_t i = 0; i < OBS_N_STATE; ++i)
        state_expected[i] = 1.2f*i;

    obs_set_state(state_expected);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(state_expected, obs_get_state(), OBS_N_STATE);
}

TEST(Observer, OneStepAx)
{
    float x0[OBS_N_STATE] =
    {
        #include "test_obs_ximpl.def"
        // 0.1f, 0.1f, 0.1f, 0.1f, 0.1f
    };
    float u0[OBS_N_INPUT] = {0};    // Zero initialisation for all elements (C99)
    float y0[OBS_N_OUTPUT] = {0};   // Zero initialisation for all elements (C99)

    obs_set_state(x0);
    obs_run(u0, y0);
    float * x1_actual = obs_get_state();

    float x1_expected[OBS_N_STATE] =
    {
        #include "test_obs_OneStepAx_x1impl.def"
        // 0.943081f, -1.87223f, 0.104038f, 0.0675829f, 3.77455f
    };
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(x1_expected, x1_actual, OBS_N_STATE);
}

TEST(Observer, OneStepBu)
{
    float x0[OBS_N_STATE] = {0};   // Zero initialisation for all elements (C99)
    float u0[OBS_N_INPUT] =
    {
        #include "test_obs_uimpl.def"
        // 1, -2, 3
    };
    float y0[OBS_N_OUTPUT] = {0};   // Zero initialisation for all elements (C99)

    obs_set_state(x0);
    obs_run(u0, y0);
    float * x1_actual = obs_get_state();

    float x1_expected[OBS_N_STATE] =
    {
        #include "test_obs_OneStepBu_x1impl.def"
        // -1.25217f, 9.24138f, -0.00102558f, 0.677763f, -5.56361f
    };
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(x1_expected, x1_actual, OBS_N_STATE);
}

TEST(Observer, OutputLy)
{
    float x0[OBS_N_STATE] = {0};   // Zero initialisation for all elements (C99)
    float u0[OBS_N_INPUT] = {0};    // Zero initialisation for all elements (C99)
    float y0[OBS_N_OUTPUT] =
    {
        #include "test_obs_yimpl.def"
        // 0.1f
    };

    obs_set_state(x0);

    float x1_expected[OBS_N_STATE] =
    {
        #include "test_obs_OutputLy_x1impl.def"
        // 13.4574f
    };
    float * x1_actual = obs_run(u0, y0);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(x1_expected, x1_actual, OBS_N_OUTPUT);
}

TEST(Observer, RunSteps)
{
    float x0[OBS_N_STATE] =
    {
        #include "test_obs_ximpl.def"
        // 0.1f, 0.1f, 0.1f, 0.1f, 0.1f
    };

    float u[][OBS_N_INPUT] =
    {
        #include "test_obs_RunSteps_u.def"
        // 3.000f, 3.001f, 3.002f, 3.003f, 3.004f
    };

    float phi[] = // Wheel angle
    {
        #include "test_obs_RunSteps_phi.def"
        // -2.000f, -1.998f, -1.996f, -1.994f, -1.992f
    };

    float theta[] = // Pendulum angle
    {
        #include "test_obs_RunSteps_theta.def"
        // 0.0000f, 0.0001f, 0.0002f, 0.0003f, 0.0004f
    };

    enum {N_DATA = sizeof(theta)/sizeof(theta[0])};

    float x1_expected[OBS_N_STATE][N_DATA] =
    {
        // Note: This data has to be exported as column-major
        #include "test_obs_RunSteps_x1impl.def"
        // {-4.00507f},
        // {-12.0516f},
        // {-18.2877f},
        // {-23.4185f},
        // {-27.8955f}
    };

    float u0[OBS_N_INPUT];
    float y0[OBS_N_OUTPUT];

    obs_set_state(x0);

    for (size_t i = 0; i < N_DATA; ++i)
    {
        for (size_t j = 0; j < OBS_N_INPUT; ++j)
        {
            u0[0] = u[i][j];
        }
        y0[0] = phi[i];
        y0[1] = theta[i];
        float * x1 = obs_run(u0, y0);
        TEST_ASSERT_EQUAL_FLOAT_ARRAY(x1_expected[i], x1, OBS_N_OUTPUT);
    }
}

TEST_GROUP_RUNNER(Observer)
{
    RUN_TEST_CASE(Observer, ZeroInitialState);
    RUN_TEST_CASE(Observer, SetState);
    RUN_TEST_CASE(Observer, OneStepAx);
    RUN_TEST_CASE(Observer, OneStepBu);
    RUN_TEST_CASE(Observer, OutputLy);
    RUN_TEST_CASE(Observer, RunSteps);
}