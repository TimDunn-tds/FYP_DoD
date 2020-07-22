#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "mpu6050.h"
#include "tm_stm32_mpu6050.h"

#define GRAVITY 9.82f
#define GYRO_SENS 131.0f
#define PI 3.141592653589793f

static TM_MPU6050_t _mpu6050;
static uint8_t _is_init = 0;

static const float ax_ofst = 
{
    #include "MPU_ax_ofst.def"
};

static const float ay_ofst =
{
    #include "MPU_ay_ofst.def"
};

static const float az_ofst =
{
    #include "MPU_az_ofst.def"
};

static const float gx_range =
{
    #include "MPU_gx_range.def"
};

static const float gy_range =
{
    #include "MPU_gy_range.def"
};

static const float gz_range =
{
    #include "MPU_gz_range.def"
};




void MPU6050_init(void)
{
    if (!_is_init)
    {
        TM_MPU6050_Result_t result = TM_MPU6050_Result_Error; //Initialise to error state. Result is overwritten in following line.
        // DONE: Uncomment and complete the initialisation
        if ((result = TM_MPU6050_Init(&_mpu6050, TM_MPU6050_Device_0, TM_MPU6050_Accelerometer_8G, TM_MPU6050_Gyroscope_250s)) == TM_MPU6050_Result_Ok) 
        {
            _is_init = 1;
        }
        else
        {
            printf("Error initialising MPU6050! Error code = %d\n", result);
        }
    }
}

void MPU6050_read(void)
{
    TM_MPU6050_ReadAll(&_mpu6050);
}

float MPU6050_get_ax(void)
{
    return (_mpu6050.Accelerometer_X - ax_ofst)*(GRAVITY/gx_range);    
}
float MPU6050_get_ay(void)
{
    return (_mpu6050.Accelerometer_Y - ay_ofst)*(GRAVITY/gy_range);    
}
float MPU6050_get_az(void)
{
    return (_mpu6050.Accelerometer_Z - az_ofst)*(GRAVITY/gz_range);    
}
float MPU6050_get_gx(void)
{
    return (_mpu6050.Gyroscope_X/GYRO_SENS)*(PI/180.0f);    
}
float MPU6050_get_gy(void)
{
    return (_mpu6050.Gyroscope_Y/GYRO_SENS)*(PI/180.0f);    
}
float MPU6050_get_gz(void)
{
    return (_mpu6050.Gyroscope_Z/GYRO_SENS)*(PI/180.0f);    
}