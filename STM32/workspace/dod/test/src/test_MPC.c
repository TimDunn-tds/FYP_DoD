#include "unity_fixture.h"

#include <stdint.h>
#include "MPC.h"

TEST_GROUP(MPC);

TEST_SETUP(MPC)
{
    MPC_init();
}

TEST_TEAR_DOWN(MPC)
{
    MPC_free();
}

TEST(MPC, ZeroInitialState)
{
    TEST_ASSERT_EACH_EQUAL_FLOAT(0.0f, MPC_getustar(), MPC_N_CONTROL);
}

TEST(MPC, OneTrajectoryMPC)
{
    float x1[MPC_N_STATE] =
    {
        #include "test_mpc_x1impl.def"
    };
    float u0[MPC_N_INPUT] = 
    {
        #include "test_mpc_u0impl.def"
    };
    float ref[MPC_N_OUTPUT] = 
    {
        #include "test_mpc_refimpl.def"
    };
    
    float y0[MPC_N_OUTPUT] = {0};   // Zero initialisation for all elements (C99)

    MPC_run(x1, u0[0], ref[0]);

    float * ustar_actual = MPC_getustar();

    float ustar_expected[MPC_N_CONTROL] =
    {
        #include "test_mpc_ustarimpl.def"
    };
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(ustar_expected, ustar_actual, MPC_N_CONTROL);
}

TEST_GROUP_RUNNER(MPC)
{
    RUN_TEST_CASE(MPC, ZeroInitialState);
    RUN_TEST_CASE(MPC, OneTrajectoryMPC);
}