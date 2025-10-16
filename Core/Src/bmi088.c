#include "main.h"
#include "spi.h"

//
// Created by FangJianRu on 2025/10/12.
//

// Accelerometer Chip Select - LOW (active)
void BMI088_ACCEL_NS_L(void) {
  HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_RESET);
}
// Accelerometer Chip Select - HIGH (inactive)
void BMI088_ACCEL_NS_H(void) {
  HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_SET);
}
// Gyroscope Chip Select - LOW (active)
void BMI088_GYRO_NS_L(void) {
  HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_RESET);
}
// Gyroscope Chip Select - HIGH (inactive)
void BMI088_GYRO_NS_H(void) {
  HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_SET);
}

void bmi088_write_byte(uint8_t tx_data) {
  HAL_SPI_Transmit(&hspi1, &tx_data, 1, 1000);
  while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX);
}

/*void bmi088_read_byte(uint8_t *rx_data, uint8_t length) {
  uint8_t dummy = 0xFF;  // 发送空数据以产生时钟信号
  for (uint8_t i = 0; i < length; i++) {
    HAL_SPI_TransmitReceive(&hspi1, &dummy, &rx_data[i], 1, 1000);
    while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_RX);
  }
}*/

void bmi088_read_byte(uint8_t *rx_data, uint8_t length){
  HAL_SPI_Receive(&hspi1, rx_data, length, 1000);
  while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_RX);
}
// 向寄存器中写入数据，可参考5.3，5.4寄存器写入方法，先写首位判别符+地址，再写data
void bmi088_write_reg(uint8_t reg, uint8_t data) {
  bmi088_write_byte(reg & 0x7F);// 最高位清零表示写操作。第一个字节：写标志(0) + 寄存器地址
  bmi088_write_byte(data);// 第二个字节：要写入的数据
}

void bmi088_accel_write_single_reg(uint8_t reg, uint8_t data) {
  BMI088_GYRO_NS_H();
  BMI088_ACCEL_NS_L();

  bmi088_write_byte(reg & 0x7F);
  bmi088_write_byte(data);

  BMI088_ACCEL_NS_H();
}

void bmi088_accel_read_reg(uint8_t reg, uint8_t *rx_data, uint8_t length) {
  uint8_t dummy_byte;

  BMI088_GYRO_NS_H();
  BMI088_ACCEL_NS_L();

  bmi088_write_byte(reg | 0x80);
  bmi088_read_byte(&dummy_byte,1);
  bmi088_read_byte(rx_data,length);

  BMI088_ACCEL_NS_H();
}

void bmi088_gyro_write_single_reg(uint8_t reg, uint8_t tx_data) {
  BMI088_ACCEL_NS_H();
  BMI088_GYRO_NS_L();

  bmi088_write_byte(reg & 0x7F);
  bmi088_write_byte(tx_data);

  BMI088_GYRO_NS_H();
}

void bmi088_gyro_read_reg(uint8_t reg, uint8_t *rx_data, uint8_t length) {
  BMI088_ACCEL_NS_H();
  BMI088_GYRO_NS_L();

  bmi088_write_byte(reg | 0x80);
  bmi088_read_byte(rx_data,length);

  BMI088_GYRO_NS_H();
}