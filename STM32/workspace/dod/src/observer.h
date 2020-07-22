#ifndef OBSERVER_H
#define OBSERVER_H

enum {
    OBS_N_INPUT  = 1, // number of observer inputs (Type I: Motor voltage U) (Type II: wheel velocity dphi)
    OBS_N_STATE  = 2, // number of observer states
    OBS_N_OUTPUT = 2, // number of plant outputs/measurements (Type I: encoder, potentiometer) Type II: dtheta, theta
};

void    obs_init(void);
void    obs_set_state(const float state[OBS_N_STATE]);
float * obs_get_state(void);
float * obs_run(const float input[OBS_N_INPUT], const float output[OBS_N_INPUT]);
float * obs_states(float dtheta, float theta);

#endif