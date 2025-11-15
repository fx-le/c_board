//
// Created by FangJianRu on 2025/10/16.
//

#include "main.h"
#include "rc.h"
#include "usart.h"
extern uint8_t rx_buf[64];
extern uint8_t rx_data[64];
RC_Class RC;
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    if(huart->Instance == USART3){
        if(Size > 0 && Size < 64){
            RC.updateOnReceive(rx_data, Size);
            RC.handle(rx_data);
        }
    }
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rx_buf, Size);
}