//
// Created by FangJianRu on 2025/10/16.
//

#include "callback.h"
#include "main.h"
#include "tim.h"
extern uint8_t rx_buf[RX_BUF_SIZE];
extern uint8_t rx_data[RX_DATA_SIZE];
void HAL_UARTEx_RxEventCallback(UART_Handle_TypeDef *huart, uint16_t Size){
    if(huart->Instance == USART3){
        if(Size > 0 && Size < RX_DATA_SIZE){

        }
    }
    HAL_UARTEx_ReceiveToIdle_DMA(*huart3, *rx_buf, Size);
}