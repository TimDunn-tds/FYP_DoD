#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpu6050.h"
#include "mpu6050_cmd.h"
#define PI 3.141592653589793f

void MPU6050_cmd(int argc, char *argv[])
{
    if (argc <= 1)
    {
        float ax_max = 2064.5;
        float ay_max = 2063.6;
        float az_max = 2100.4;
        float x_ofst = -108.3667;
        float y_ofst = +48.3733;
        float z_ofst = +213.38;

        MPU6050_read();

        // float ax_print = (MPU6050_get_ax()+x_ofst)*(9.82/ax_max);
        // float ay_print = (MPU6050_get_ay()+y_ofst)*(9.82/ay_max);
        // float az_print = (MPU6050_get_az()+z_ofst)*(9.82/az_max);
        // float gx_print = (MPU6050_get_gx()/131)*(PI/180);
        // float gy_print = (MPU6050_get_gy()/131)*(PI/180);
        // float gz_print = (MPU6050_get_gz()/131)*(PI/180);

        printf("ax = %f m/s^2, ", MPU6050_get_ax());
        printf("ay = %f m/s^2, ", MPU6050_get_ay());
        printf("az = %f m/s^2, ", MPU6050_get_az());
        printf("gx = %f rad/s, ", MPU6050_get_gx());
        printf("gy = %f rad/s, ", MPU6050_get_gy());
        printf("gz = %f rad/s\n", MPU6050_get_gz());
    }
    else
    {
        printf("%s: expecting 0 arguments, syntax is: %s\n", argv[0], argv[0]);
    }
}