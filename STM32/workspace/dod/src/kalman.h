#ifndef KALMAN_H
#define KALMAN_H

enum {
    KF_N_INPUT  = 0, // number of inputs was 0
    KF_N_STATE  = 3, // number of states
    KF_N_OUTPUT = 2, // number of measurements was 2
}; 

void    kf_init(void);
float * kf_get_mean(void);
float * kf_get_sqrtcovariance(void);
float * kf_run(const float input[KF_N_INPUT], const float output[KF_N_INPUT]);
float * kf_measurement_update(const float input[KF_N_INPUT], const float output[KF_N_OUTPUT]);
float * kf_time_update(const float input[KF_N_INPUT]);

float * kf_obs_states(void);

#endif