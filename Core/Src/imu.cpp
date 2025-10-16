//
// Created by FangJianRu on 2025/10/12.
//
#include <cstdint>
#include "imu.h"
#include "bmi088.h"

#define g 9.81f
extern uint8_t rx_acc_data[6];
extern uint8_t rx_gyro_data[6];
float acc_data[3];
float gyro_data[3];

void IMU::acc_calculate(){
    // 1. 设置/读取acc0x41寄存器中的量程range参数，并换算为量程系数
    uint8_t rx_acc_range_raw;
    bmi088_accel_read_reg(0x41, &rx_acc_range_raw, 1);
    float rx_acc_range=(float)(rx_acc_range_raw+1)*3.f*g;
    // 2. 读取acc0x12寄存器中的6位acc数据
    bmi088_accel_read_reg(0x12, rx_acc_data, 6);
    // 3. 用量程系数将原始数据转换为常用单位
    acc_data[0] = (float)(int16_t)((rx_acc_data[1] << 8) | rx_acc_data[0])/32768.f*rx_acc_range;  // X轴
    acc_data[1] = (float)(int16_t)((rx_acc_data[3] << 8) | rx_acc_data[2])/32768.f*rx_acc_range;  // Y轴
    acc_data[2] = (float)(int16_t)((rx_acc_data[5] << 8) | rx_acc_data[4])/32768.f*rx_acc_range;  // Z轴
}

void IMU::gyro_calculate(){
    // 1. 设置/读取gyro0x0F寄存器中的量程range参数，并换算为量程系数
    uint8_t rx_gyro_range_raw;
    bmi088_gyro_read_reg(0x0F, &rx_gyro_range_raw, 1);
    float rx_gyro_range=2000.f/(float)(rx_gyro_range_raw+1);
    // 2. 读取gyro0x02寄存器中的6位gyro数据
    bmi088_gyro_read_reg(0x02, rx_gyro_data, 6);
    // 3. 用量程系数将原始数据转换为常用单位
    gyro_data[0] = (float)(int16_t)((rx_gyro_data[1] << 8) | rx_gyro_data[0])/32768.f*rx_gyro_range;  // X轴
    gyro_data[1] = (float)(int16_t)((rx_gyro_data[3] << 8) | rx_gyro_data[2])/32768.f*rx_gyro_range;  // Y轴
    gyro_data[2] = (float)(int16_t)((rx_gyro_data[5] << 8) | rx_gyro_data[4])/32768.f*rx_gyro_range;  // Z轴
}
