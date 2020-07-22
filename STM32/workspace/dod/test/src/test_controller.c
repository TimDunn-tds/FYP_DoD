#include "unity_fixture.h"

#include <stdint.h>
#include "controller.h"

TEST_GROUP(Ctrl);

TEST_SETUP(Ctrl)
{
    ctrl_init();
}

TEST_TEAR_DOWN(Ctrl)
{
}

TEST(Ctrl, ZeroInitialState)
{
    TEST_ASSERT_EACH_EQUAL_FLOAT(0.0f, ctrl_get_state(), CTRL_N_STATE);
}

TEST(Ctrl, SetState)
{
    float state_expected[CTRL_N_STATE];
    for (size_t i = 0; i < CTRL_N_STATE; ++i)
        state_expected[i] = 1.2f*i;

    ctrl_set_state(state_expected);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(state_expected, ctrl_get_state(), CTRL_N_STATE);
}

TEST(Ctrl, OneStepAx)
{
    float x0[CTRL_N_STATE] =
    {
        0.1f, 0.1f, 0.1f, 0.1f, 0.1f
    };
    float u[CTRL_N_INPUT] = {0};    // Zero initialisation for all elements (C99)

    ctrl_set_state(x0);
    ctrl_run(u);
    float * x_actual = ctrl_get_state();

    float x_expected[CTRL_N_STATE] =
    {
        #include "OneStepAx.def"
        // 0.943081f, -1.87223f, 0.104038f, 0.0675829f, 3.77455f
    };
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(x_expected, x_actual, CTRL_N_STATE);
}

TEST(Ctrl, OneStepBu)
{
    float x0[CTRL_N_STATE] = {0};   // Zero initialisation for all elements (C99)
    float u[CTRL_N_INPUT] =
    {
        1, -2, 3
    };

    ctrl_set_state(x0);
    ctrl_run(u);
    float * x_actual = ctrl_get_state();

    float x_expected[CTRL_N_STATE] =
    {
        #include "OneStepBu.def"
        // -1.25217f, 9.24138f, -0.00102558f, 0.677763f, -5.56361f
    };
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(x_expected, x_actual, CTRL_N_STATE);
}

TEST(Ctrl, OutputCx)
{
    float x0[CTRL_N_STATE] =
    {
        0.1f, 0.1f, 0.1f, 0.1f, 0.1f
    };
    float u[CTRL_N_INPUT] = {0};    // Zero initialisation for all elements (C99)

    ctrl_set_state(x0);

    float y_expected[CTRL_N_OUTPUT] =
    {
        #include "OutputCx.def"
        // 13.4574f
    };
    float * y_actual = ctrl_run(u);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(y_expected, y_actual, CTRL_N_OUTPUT);
}

TEST(Ctrl, OutputDu)
{
    float x0[CTRL_N_STATE] = {0};   // Zero initialisation for all elements (C99)
    float u[CTRL_N_INPUT] =
    {
        1, -2, 3
    };

    ctrl_set_state(x0);

    float y_expected[CTRL_N_OUTPUT] =
    {
    #include "OutputDu.def"
                // -17.4625f
    };
    float * y_actual = ctrl_run(u);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(y_expected, y_actual, CTRL_N_OUTPUT);
}

TEST(Ctrl, RunSteps)
{
    float x0[CTRL_N_STATE] =
    {
        0.1f, 0.1f, 0.1f, 0.1f, 0.1f
    };

    float ref[] = // Reference input
    {
        1.000f, 1.001f, 1.002f, 1.003f, 1.004f
    };

    float phi[] = // Wheel angle
    {
        -2.000f, -1.998f, -1.996f, -1.994f, -1.992f
    };

    float theta[] = // Pendulum angle
    {
        0.0000f, 0.0001f, 0.0002f, 0.0003f, 0.0004f
    };

    enum {N_DATA = sizeof(ref)/sizeof(ref[0])};

    float y_expected[][CTRL_N_OUTPUT] =
    {
        #include "y_expected.def"
        // {-4.00507f},
        // {-12.0516f},
        // {-18.2877f},
        // {-23.4185f},
        // {-27.8955f}
    };

    float u[CTRL_N_INPUT];

    ctrl_set_state(x0);

    for (size_t i = 0; i < N_DATA; ++i)
    {
        u[0] = ref[i];
        u[1] = phi[i];
        u[2] = theta[i];
        float * y_actual = ctrl_run(u);
        TEST_ASSERT_EQUAL_FLOAT_ARRAY(&y_expected[i][0], y_actual, CTRL_N_OUTPUT);
    }
}

TEST_GROUP_RUNNER(Ctrl)
{
    RUN_TEST_CASE(Ctrl, ZeroInitialState);
    RUN_TEST_CASE(Ctrl, SetState);
    RUN_TEST_CASE(Ctrl, OneStepAx);
    RUN_TEST_CASE(Ctrl, OneStepBu);
    RUN_TEST_CASE(Ctrl, OutputCx);
    RUN_TEST_CASE(Ctrl, OutputDu);
    RUN_TEST_CASE(Ctrl, RunSteps);
}