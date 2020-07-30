#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>
#include "tm_stm32_mpu6050.h"

void MPU6050_init(void);
void MPU6050_read(void);
float MPU6050_get_ax(void);
float MPU6050_get_ay(void);
float MPU6050_get_az(void);
float MPU6050_get_gx(void);
float MPU6050_get_gy(void);
float MPU6050_get_gz(void);

#endif