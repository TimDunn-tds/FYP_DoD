#include <stddef.h>
#include <math.h>
#include "givensQR.h"
#include "kalman.h"
#include "mpu6050.h"

static float _kf_mean[KF_N_STATE];
static float _kf_sqrtcov[KF_N_STATE][KF_N_STATE];

// Initial mean
static float _kf_mean_init[KF_N_STATE] =
{
    0.0f, 0.0f, 0.0f
};

// Initial square-root covariance
static const float _kf_sqrtcov_init[KF_N_STATE][KF_N_STATE] =
{
    // #include "kf_sqrtcov.def"
    #include "kf_sqrtcovnew.def"
    // #include "kf_sqrtcovtotal.def"
    // {0.0f, 0.0f, 0.0f},
    // {0.0f, 0.0f, 0.0f},
    // {0.0f, 0.0f, 0.0f},
};

static const float _kf_A[KF_N_STATE][KF_N_STATE] = 
{
    // #include "kf_A.def"
    #include "kf_Anew.def"
    // #include "kf_Atotal.def"
    // {0.0f, 0.0f, 0.0f},
    // {0.0f, 0.0f, 0.0f},
    // {0.0f, 0.0f, 0.0f},
};

static const float _kf_B[KF_N_STATE][KF_N_INPUT] = 
{
    // #include "kf_B.def"
    #include "kf_Bnew.def"
    // #include "kf_Btotal.def"

};

static const float _kf_SQ[KF_N_STATE][KF_N_STATE] = 
{
    // #include "kf_SQ.def"
    #include "kf_SQnew.def"
    // #include "kf_SQtotal.def"

    // {0.0f, 0.0f, 0.0f},
    // {0.0f, 0.0f, 0.0f},
    // {0.0f, 0.0f, 0.0f},
};

static const float _kf_C[KF_N_OUTPUT][KF_N_STATE] = 
{
    // #include "kf_C.def"
    #include "kf_Cnew.def"
    // #include "kf_Ctotal.def"

    // {0.0f, 1.0f, 0.0f},
    // {1.0f, 0.0f, 1.0f}
};

static const float _kf_D[KF_N_OUTPUT][KF_N_INPUT] = 
{
    // #include "kf_D.def"
    #include "kf_Dnew.def"
    // #include "kf_dtotal.def"
};

static const float _kf_SR[KF_N_OUTPUT][KF_N_OUTPUT] = 
{
    // #include "kf_SR.def"    
    #include "kf_SRnew.def"  
    // #include "kf_SRtotal.def"  
    // {0.0f, 0.0f},
    // {0.0f, 0.14861f}
};

void kf_init(void)
{
    for (size_t i = 0; i < KF_N_STATE; ++i)
    {
        _kf_mean[i] = _kf_mean_init[i];
        for (size_t j = 0; j < KF_N_STATE; ++j)
        {
            _kf_sqrtcov[i][j] = _kf_sqrtcov_init[i][j];
        }
    }
}

float * kf_get_mean(void)
{
    return _kf_mean;
}

float * kf_get_sqrtcovariance(void)
{
    return &_kf_sqrtcov[0][0];
}

float * kf_run(const float input[KF_N_INPUT], const float output[KF_N_OUTPUT])
{
    kf_measurement_update(input, output);
    kf_time_update(input);
    return _kf_mean;
}

float * kf_measurement_update(const float input[KF_N_INPUT], const float output[KF_N_OUTPUT])
{
    // Form measurement and state joint square-root covariance
    //        ny      nx
    // SS = [ Sxx*C.' Sxx ] nx
    //      [ SR        0 ] ny
    //
    float SS[KF_N_STATE+KF_N_OUTPUT][KF_N_OUTPUT+KF_N_STATE];
    for (size_t i = 0; i < KF_N_STATE; ++i)
    {
        // SS(1:nx,1:ny) = Sxx*C.'
        for (size_t j = 0; j < KF_N_OUTPUT; ++j)
        {
            SS[i][j] = 0.0f;
            for (size_t k = 0; k < KF_N_STATE; ++k)
            {
                SS[i][j] += _kf_sqrtcov[i][k]*_kf_C[j][k];
            }
        }
        // SS(1:nx,ny+1:nx+ny) = Sxx
        for (size_t j = 0; j < KF_N_STATE; ++j)
        {
            if (i <= j) SS[i][KF_N_OUTPUT+j] = _kf_sqrtcov[i][j];
            else        SS[i][KF_N_OUTPUT+j] = 0.0f;
        }
    }
    for (size_t i = 0; i < KF_N_OUTPUT; ++i)
    {
        // SS(nx+1:nx+ny,1:ny) = SR
        for (size_t j = 0; j < KF_N_OUTPUT; ++j)
        {
            if (i <= j) SS[KF_N_STATE+i][j] = _kf_SR[i][j];
            else        SS[KF_N_STATE+i][j] = 0.0f;
        }
        // SS(nx+1:nx+ny,ny+1:nx+ny) = zeros(ny,nx)
        for (size_t j = 0; j < KF_N_STATE; ++j)
            SS[KF_N_STATE+i][KF_N_OUTPUT+j] = 0.0f;
    }

    // Upper triangularise joint covariance
    //        ny  nx
    // SS = [ S1  S2  ] ny
    //      [  0  Sxx ] nx
    // 
    givensQR(KF_N_OUTPUT+KF_N_STATE, KF_N_OUTPUT+KF_N_STATE, SS);

    // Corrected state covariance
    // Sxx = SS(ny+1:ny+nx,ny+1:ny+nx)
    for (size_t i = 0; i < KF_N_STATE; ++i)
    {
        for (size_t j = 0; j < KF_N_STATE; ++j)
        {
            if (i <= j) _kf_sqrtcov[i][j] = SS[KF_N_OUTPUT+i][KF_N_OUTPUT+j];
            else        _kf_sqrtcov[i][j] = 0.0f;
        }
    }

    //
    // Predict measurement
    //

    // muy = C*mux + D*u;
    float muy[KF_N_OUTPUT];
    for (size_t i = 0; i < KF_N_OUTPUT; ++i)
    {
        muy[i] = 0.0f;
        for (size_t k = 0; k < KF_N_STATE; ++k)
            muy[i] += _kf_C[i][k]*_kf_mean[k];
        for (size_t k = 0; k < KF_N_INPUT; ++k)
            muy[i] += _kf_D[i][k]*input[k];
    }

    // Prediction error
    // dy = y - muy
    float dy[KF_N_OUTPUT];
    for (size_t i = 0; i < KF_N_OUTPUT; ++i)
        dy[i] = output[i] - muy[i];

    // Triangular forward substitution
    // dy = S1.'\dy
    dy[0] /= SS[0][0];
    for (size_t i = 1; i < KF_N_OUTPUT; ++i)
    {
        for (size_t k = 0; k < i; ++k)
            dy[i] -= SS[k][i]*dy[k];
        dy[i] /= SS[i][i];
    }

    // S2 = SS(1:ny,ny+1:ny+nx)
    // mux = mux + S2.'*dy
    for (size_t i = 0; i < KF_N_STATE; ++i)
        for (size_t k = 0; k < KF_N_OUTPUT; ++k)
            _kf_mean[i] += SS[k][KF_N_OUTPUT+i]*dy[k];

    return _kf_mean;
}

float * kf_time_update(const float input[KF_N_INPUT])
{
    // Assemble square-root of A*P*A.' + Q
    //        nx
    // SS = [ Sxx*A.' ] nx
    //      [ SQ      ] nx
    //
    float SS[2*KF_N_STATE][KF_N_STATE];
    for (size_t i = 0; i < KF_N_STATE; ++i)
    {
        for (size_t j = 0; j < KF_N_STATE; ++j)
        {
            // SS(1:nx,1:nx) = Sxx*A.'
            SS[i][j] = 0.0f;
            for (size_t k = 0; k < KF_N_STATE; ++k)
            {
                SS[i][j] += _kf_sqrtcov[i][k]*_kf_A[j][k];
            }

            // SS(nx+1:2*nx,1:nx) = SQ
            if (i <= j) SS[KF_N_STATE+i][j] = _kf_SQ[i][j];
            else        SS[KF_N_STATE+i][j] = 0.0f;
        }
    }

    // Upper triangularise SS
    //        nx
    // SS = [ Sxx ] nx
    //      [  0  ] nx
    // 
    givensQR(2*KF_N_STATE, KF_N_STATE, SS);

    // Extract predicted state covariance
    // Sxx = SS(1:nx,1:nx)
    for (size_t i = 0; i < KF_N_STATE; ++i)
        for (size_t j = 0; j < KF_N_STATE; ++j)
            _kf_sqrtcov[i][j] = SS[i][j];

    // Predict state mean
    // mux = A*mux + B*u;
    float next_mean[KF_N_STATE];
    for (size_t i = 0; i < KF_N_STATE; ++i)
    {
        next_mean[i] = 0.0f;
        for (size_t k = 0; k < KF_N_STATE; ++k)
            next_mean[i] += _kf_A[i][k]*_kf_mean[k];
        for (size_t k = 0; k < KF_N_INPUT; ++k)
            next_mean[i] += _kf_B[i][k]*input[k];
    }
    for (size_t i = 0; i < KF_N_STATE; ++i)
        _kf_mean[i] = next_mean[i];

    return _kf_mean;
}


float * kf_obs_states(void)
{

    float _input[KF_N_INPUT];
    float _meas[3];
    float _output[KF_N_OUTPUT];
    MPU6050_read();

    // _input[0] = alpha;

    _meas[0] = MPU6050_get_az();
    _meas[1] = MPU6050_get_ay();
    _meas[2] = MPU6050_get_gx();

    _output[0] = atan2f(-_meas[0], _meas[1]);
    _output[1] = _meas[2];
    kf_measurement_update(_input,_output);
    kf_time_update(_input);
    float* _mux = kf_get_mean();
    
    return _mux;
}