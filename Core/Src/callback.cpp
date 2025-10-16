//
// Created by FangJianRu on 2025/10/16.
//

#include "callback.h"
#include "main.h"
#include "tim.h"
#include "imu.h"
IMU imu;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == htim6.Instance){
        imu.acc_calculate();
        imu.gyro_calculate();
    }
}