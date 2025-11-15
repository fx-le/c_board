//
// Created by FangJianRu on 2025/11/5.
//

#ifndef SPI_RC_H
#define SPI_RC_H

#endif //SPI_RC_H
#ifndef RC_CLASS_H
#define RC_CLASS_H

#include "main.h"

#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)

typedef __packed struct
{
    struct {
        int16_t ch0;  // 右摇杆水平
        int16_t ch1;  // 右摇杆垂直
        int16_t ch2;  // 左摇杆水平
        int16_t ch3;  // 左摇杆垂直
        uint8_t s1;   // 左侧拨杆
        uint8_t s2;   // 右侧拨杆
    }rc;
    struct {
        int16_t x;
        int16_t y;
        int16_t z;
        uint8_t press_l;
        uint8_t press_r;
    }mouse;
    struct {
        int16_t v;
    }key;
}RC_Clt_t;

class RC_Class {
private:

    uint32_t last_receive_tick;  // 最后接收时间戳
    bool connection_status;      // 连接状态
    uint32_t timeout_ms;         // 连接超时时间
    RC_Clt_t rc_data;            // 解析后的数据

public:
    uint8_t rx_buf[64];      // DMA接收缓冲区
    uint8_t rx_data[64];     // 数据缓存
    uint16_t data_length;    // 实际数据长度
    RC_Class();
    void init(UART_HandleTypeDef *huart); //初始化
    void handle(uint8_t *pdata); //数据处理(解包)
    bool isConnected() {return connection_status;} // 连接状态接口
    void updateOnReceive(uint8_t* data, uint16_t size); // 在回调函数中调用此方法来更新接收状态

private:
    void updateConnectionStatus(); // 刷新连接状态
};

#endif