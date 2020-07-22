#ifndef MPC_H
#define MPC_H

#include <stdint.h>


/* TODO: Export these numbers directly from MATLAB */
enum {
    MPC_N_INPUT         = 1,  // number of MPC inputs  (actuated plant inputs)
    MPC_N_STATE         = 3,  // number of MPC states  (controller states)
    MPC_N_OUTPUT        = 1,  // number of MPC outputs (regulated plant outputs)
    MPC_N_CONTROL       = 35, // MPC control horizon
    MPC_N_INEQ          = 70,  // number of general linear inequality constraints
    MPC_N_EQ            = 0,  // number of general linear equality constraints
    MPC_N_LB            = 0,  // number of element-wise lower bound constraints
    MPC_N_UB            = 0,  // number of element-wise upper bound constraints


};

void    MPC_init(void);
float* 	MPC_getustar(void);
float 	MPC_run(float x1[MPC_N_STATE], float u0, float ref);
void 	MPC_free(void);

#endif