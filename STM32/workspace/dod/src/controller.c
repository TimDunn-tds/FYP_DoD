#include <stddef.h>
#include "controller.h"

static float _ctrl_state[CTRL_N_STATE];
static float _ctrl_output[CTRL_N_OUTPUT];

static const float _ctrl_A[CTRL_N_STATE][CTRL_N_STATE] =
{
    // #include "ctrlA.def"
    // #include "ctrlAstep.def"
    {2.25317f, 1.26665f, 0.0196422f, 5.86388f, 0.0274621f},
    {-2.50255f, -1.53248f, -0.850065f, -13.7825f, -0.0546918f},
    {0.0155307f, 0.00559188f, 0.994722f, 0.0241661f, 0.000373021f},
    {-0.0110501f, -0.00118899f, -0.0373699f, 0.726182f, -0.000744898f},
    {5.69487f, 5.74831f, -0.255573f, 27.0430f, -0.485043f}
};

static const float _ctrl_B[CTRL_N_STATE][CTRL_N_INPUT] =
{
    // #include "ctrlB.def"
    // #include "ctrlBstep.def"
    {-1.25319f, -0.000785690f, -0.000184694f},
    {2.50258f, 0.0340026f, 2.26893f},
    {-0.00553094f, 0.000211135f, 0.00164254f},
    {0.0110505f, 0.00149480f, 0.223234f},
    {-5.69402f, 0.0102229f, 0.0502858f}
};

static const float _ctrl_C[CTRL_N_OUTPUT][CTRL_N_STATE] =
{
    // #include "ctrlC.def"
    // #include "ctrlCstep.def"
    {34.9629f, 17.6462f, -0.00000f, 82.0111f, -0.0458034f}
};

static const float _ctrl_D[CTRL_N_OUTPUT][CTRL_N_INPUT] =
{
    // #include "ctrlD.def"
    // #include "ctrlDstep.def"
    {-17.4625f, 0.00000f, 0.00000f}
    
};


void ctrl_init(void)
{
    for (size_t i = 0; i < CTRL_N_STATE; ++i)
        _ctrl_state[i] = 0.0f;
}
    
void ctrl_set_state(const float state[CTRL_N_STATE])
{
    for (size_t i = 0; i < CTRL_N_STATE; ++i)
        _ctrl_state[i] = state[i];
}

float * ctrl_get_state(void)
{
    return _ctrl_state;
}

float * ctrl_run(const float input[CTRL_N_INPUT])
{
    // y[k] = C*x[k] + D*u[k]
    for (size_t i = 0; i < CTRL_N_OUTPUT; ++i)
    {
        _ctrl_output[i] = 0.0f;
        for (size_t j = 0; j < CTRL_N_STATE; ++j)
            _ctrl_output[i] += _ctrl_C[i][j]*_ctrl_state[j];
        for (size_t j = 0; j < CTRL_N_INPUT; ++j)
            _ctrl_output[i] += _ctrl_D[i][j]*input[j];
    }

    // x[k+1] = A*x[k] + B*u[k]
    float next_state[CTRL_N_STATE];
    for (size_t i = 0; i < CTRL_N_STATE; ++i)
    {
        next_state[i] = 0.0f;
        for (size_t j = 0; j < CTRL_N_STATE; ++j)
            next_state[i] += _ctrl_A[i][j]*_ctrl_state[j];
        for (size_t j = 0; j < CTRL_N_INPUT; ++j)
            next_state[i] += _ctrl_B[i][j]*input[j];
    }

    // Set next state
    for (size_t i = 0; i < CTRL_N_STATE; ++i)
        _ctrl_state[i] = next_state[i];
    return _ctrl_output;
}