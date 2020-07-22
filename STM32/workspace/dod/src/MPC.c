#include <stdint.h>
#include <stdlib.h>

#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "potentiometer.h"
#include "encoder.h"
#include "dimmer_task.h"
#include "controller.h"
#include "math.h"
#include "motor.h"
#include "reference.h"
#include "observer.h"
#include "MPC.h"
#include "qpas_sub_noblas.h"

/*  Note: Matrix notation used in MPC.c and qpas_sub_noblas.c is _column major_
    As such, when declaring the matrices here they are denoted Matrix[columns][rows].
    */
static const float H[MPC_N_CONTROL][MPC_N_CONTROL] =
{
    #include "MPC_H.def"
};
static const float Gamma[MPC_N_STATE+MPC_N_INPUT+MPC_N_OUTPUT][MPC_N_CONTROL] =
{
    #include "MPC_Gamma.def"
};
static float Aineq[MPC_N_CONTROL][MPC_N_INEQ] =
{  
    #include "MPC_Aineq.def"
};
static const float bineq[MPC_N_INEQ] =
{  
    #include "MPC_bineq.def"
};
static float Aeq[MPC_N_CONTROL][MPC_N_EQ] =
{  
    // #include "MPC_Aeq.def"
};
static const float beq[MPC_N_EQ] =
{  
    // #include "MPC_beq.def"
};
static float l[MPC_N_LB] =
{  
    // #include "MPC_lb.def"
};
static float u[MPC_N_UB] =
{  
    // #include "MPC_ub.def"
};

/* Internal variables */
float *Ac, *bc, *lm, *pt1, *pt2;
float *y, *z, *tm, *gm, dtmp, *Hc;

varint me,mc,nl,nu,i,j,err,display,numits,numadd,numdrop;
varint ione  = 1;
varint izero = 0;
float *tmp;

static float u0 = 0.0f;
static float _phi = 0.0f;
static float kk[MPC_N_STATE+MPC_N_INPUT+MPC_N_OUTPUT] = {0};
static float g[MPC_N_CONTROL] = {0};
static float ustar[MPC_N_CONTROL] = {0};
static float u1;

uint8_t _is_init = 0;

void MPC_init(void)
{
    if (!_is_init)
    {
        /* Number of inequality constraints */
        mc = MPC_N_INEQ;
        /* Number of equality constraints */
        me = MPC_N_EQ;
        /* Number of lower limits */
        nl = MPC_N_LB;
        /* Number of upper limits */
        nu = MPC_N_UB;
        /* Print optimiser output (Y/N) */
        display = 0;

        Ac=(float *)malloc((me+mc)*MPC_N_CONTROL*sizeof(float));
        bc=(float *)malloc((me+mc)*sizeof(float));
        lm=(float *)malloc((me+mc+nl+nu)*sizeof(float));
        /* Copy the Hessian in case we modify it */
        Hc = (float *)malloc(MPC_N_CONTROL*MPC_N_CONTROL*sizeof(float));
        if (!(Ac && bc && lm && Hc))
        {
            printf("Couldn't allocate enough memory\n");
            return;
        }
        for(i=0;i<MPC_N_CONTROL*MPC_N_CONTROL;i++)
        {
            Hc[i]=*(*H+i);
        }
        /* Initialise matrices */
        for(i=0;i<me+mc+nl+nu;i++)
        {
            lm[i]=0.0;
        }
        pt1=*Aeq; 
        pt2=Ac;
        for(j=0;j<MPC_N_CONTROL;j++)
        {
            for(i=0;i<me;i++)
            {
                pt2[i]=pt1[i];
            }
            pt1+=me;
            pt2+=me+mc;
        }
        for(i=0;i<me;i++)
        {
            bc[i]=beq[i];
        }
        pt1=*Aineq;
        pt2=&Ac[me];
        for(j=0;j<MPC_N_CONTROL;j++)
        {
            for(i=0;i<mc;i++)
            {
                pt2[i]=pt1[i];
            }
            pt1+=mc;
            pt2+=me+mc;
        }
        for(i=0;i<mc;i++)
        {
            bc[me+i]=bineq[i];
        }

        _is_init = 1;
    }
}

float* MPC_getustar(void)
{
    return ustar;
}

float MPC_run(float x1[MPC_N_STATE], float u0, float ref)
{
    if (!_is_init)
    {
        printf("MPC is not initialised\n");
        return 0.0f;
    }
    // Compute f = Gamma*kk for current timestep
    /* kk = [x1; u0; ref], where
        x1 is the predicted state from the observer x[k+1],
        u0 is the currently applied control action  u[k],
        ref is the current output reference         ystar[k].   */
    for(i=0;i<MPC_N_STATE;i++)
    {
        kk[i]=x1[i];
    }
    /* TODO: Handle multiple plant inputs */
    kk[i] = u0;
    /* TODO: Handle reference for multiple outputs */
    kk[MPC_N_STATE+MPC_N_INPUT+MPC_N_OUTPUT-1] = ref;
    // g = Gamma*kk where Gamma is stored as column-major
    for (i = 0; i < MPC_N_CONTROL; ++i)
    {
        g[i] = 0.0f;
        for (size_t j = 0; j < (MPC_N_STATE+MPC_N_INPUT+MPC_N_OUTPUT); ++j)
            g[i] += Gamma[j][i]*kk[j];
    }

    // Solve ustar = argmin(u){1/2 u'Hu + g'u} s.t. Lu <= m
    err=qpas_sub_noblas(MPC_N_CONTROL,me,mc,nl,nu,Hc,g,Ac,bc,l,u,ustar,lm,display,&numits,&numadd,&numdrop);

    // Return ustar[k+1]
    /* TODO: Handle multiple plant inputs */
    u1 = ustar[0];
    // printvector (OBS_N_STATE, x1, "x1");
    // printvector ((MPC_N_STATE+MPC_N_INPUT+MPC_N_OUTPUT), kk, "kk");
    // printvector (MPC_N_CONTROL, g, "g");
    // printmatrix (MPC_N_INEQ, MPC_N_CONTROL, Aineq, MPC_N_INEQ, "Aineq");
    // printmatrix (MPC_N_INEQ, MPC_N_CONTROL, Ac, MPC_N_INEQ, "Ac");
    // printvector (MPC_N_CONTROL, ustar, "ustar");
    return u1;
}

void MPC_free(void)
{
    free(Ac);
    free(bc);
    free(lm);
    free(Hc);
    _is_init=0;
}