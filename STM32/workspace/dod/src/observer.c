#include <stddef.h>
#include "observer.h"

static float _obs_state[OBS_N_STATE];

static const float _obs_A[OBS_N_STATE][OBS_N_STATE] =
{
    #include "obsA.def"
    // #include "obsAstep.def"

};

static const float _obs_B[OBS_N_STATE][OBS_N_INPUT] =
{
    #include "obsB.def"
    // #include "obsBstep.def"
};

static const float _obs_L[OBS_N_STATE][OBS_N_OUTPUT] =
{
    #include "obsL.def"
    // #include "obsLstep.def"
};

void obs_init(void)
{
    for (size_t i = 0; i < OBS_N_STATE; ++i)
        _obs_state[i] = 0.0f;
}
    
void obs_set_state(const float state[OBS_N_STATE])
{
    for (size_t i = 0; i < OBS_N_STATE; ++i)
        _obs_state[i] = state[i];
}

float * obs_get_state(void)
{
    return _obs_state;
}

float * obs_run(const float input[OBS_N_INPUT], const float output[OBS_N_OUTPUT])
{
    // mu[k+1] = Aobs*mu[k] + Bobs*u[k] + Lobs*y[k]
    float next_state[OBS_N_STATE];
    for (size_t i = 0; i < OBS_N_STATE; ++i)
    {
        next_state[i] = 0.0f;
        for (size_t j = 0; j < OBS_N_STATE; ++j)
            next_state[i] += _obs_A[i][j]*_obs_state[j];
        for (size_t j = 0; j < OBS_N_INPUT; ++j)
            next_state[i] += _obs_B[i][j]*input[j];
        for (size_t j = 0; j < OBS_N_OUTPUT; ++j)
            next_state[i] += _obs_L[i][j]*output[j];
    }

    // Set next state
    for (size_t i = 0; i < OBS_N_STATE; ++i)
        _obs_state[i] = next_state[i];

    // add on the dphi to this
    // determine wheel rotational velocity
    


    return _obs_state;
}

float * obs_states(float dtheta, float theta)
{
    float _input[OBS_N_INPUT];
    float _output[OBS_N_OUTPUT];

    _output[0] = dtheta;
    _output[1] = theta;
    float* _out = obs_run(_input, _output);
    return _out;

}