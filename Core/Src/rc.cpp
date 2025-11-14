//
// Created by FangJianRu on 2025/11/5.
//
#include "rc.h"
#include <cstring>
#include "main.h"

RC_Class::RC_Class()
        : data_length(0),
          last_receive_tick(0),
          connection_status(false),
          timeout_ms(100) {  // 100ms超时

    memset(rx_buf, 0, sizeof(rx_buf));
    memset(rx_data, 0, sizeof(rx_data));
    memset(&rc_data, 0, sizeof(rc_data));
}

// 初始化
void RC_Class::init(UART_HandleTypeDef *huart3) {
    if (HAL_UARTEx_ReceiveToIdle_DMA(huart3, rx_buf, sizeof(rx_buf)) == HAL_OK) {
        last_receive_tick = HAL_GetTick();
    }
    rc_data.rc.ch0 = 0;
    rc_data.rc.ch1 = 0;
    rc_data.rc.ch2 = 0;
    rc_data.rc.ch3 = 0;
    rc_data.rc.s1 = 1;
    rc_data.rc.s2 = 1;
    rc_data.mouse.x = 0;
    rc_data.mouse.y = 0;
    rc_data.mouse.z = 0;
    rc_data.mouse.press_l = 0;
    rc_data.mouse.press_r = 0;
}

// 处理函数 - 数据解包
void RC_Class::handle(uint8_t *pdata) {
    // 刷新连接状态
    updateConnectionStatus();
    // 如果有新数据，进行解析
    if (data_length > 0) {
        rc_data.rc.ch0 = ((static_cast<uint16_t>(pdata[0]) | (static_cast<uint16_t>(pdata[1]) << 8)) & 0x07FF);
        rc_data.rc.ch1 = ((static_cast<uint16_t>(pdata[1]) >> 3) | (static_cast<uint16_t>(pdata[2]) << 5)) & 0x07FF;
        rc_data.rc.ch2 = ((static_cast<uint16_t>(pdata[2]) >> 6) | (static_cast<uint16_t>(pdata[3]) << 2) | ((uint16_t)pdata[4] << 10)) & 0x07FF;
        rc_data.rc.ch3 = ((static_cast<uint16_t>(pdata[4]) >> 1) | (static_cast<uint16_t>(pdata[5]) << 7)) & 0x07FF;
        rc_data.rc.s1 = ((pdata[5]>>4) & 0x000C) >> 2;
        rc_data.rc.s2 = ((pdata[5]>>4) & 0x0003);
        rc_data.mouse.x = (static_cast<uint16_t>(pdata[6])) | (static_cast<uint16_t>(pdata[7]) << 8);
        rc_data.mouse.y = (static_cast<uint16_t>(pdata[8])) | (static_cast<uint16_t>(pdata[9]) << 8);
        rc_data.mouse.z = (static_cast<uint16_t>(pdata[10])) | (static_cast<uint16_t>(pdata[11]) << 8);
        rc_data.mouse.press_l = pdata[12];
        rc_data.mouse.press_r = pdata[13];
        rc_data.key.v = (static_cast<uint16_t>(pdata[14]));// | (static_cast<uint16_t>(pdata[15]) << 8);
        data_length = 0;  // 处理完成后重置数据长度
    }
}

// 获取数据接口
RC_Clt_t* RC_Class::getData() {
    return &rc_data;
}

// 在回调函数中调用此方法来更新接收状态
void RC_Class::updateOnReceive(uint8_t* data, uint16_t size) {
    last_receive_tick = HAL_GetTick(); // 记录接收时间
    // 拷贝数据到rx_data
    if (size <= sizeof(rx_data)) {
        memcpy(rx_data, data, size);
        data_length = size;
    }
    connection_status = true; //立即刷新连接状态
}

// 刷新连接状态
void RC_Class::updateConnectionStatus() {
    uint32_t current_tick = HAL_GetTick();
    if ((current_tick - last_receive_tick) > timeout_ms) {
        connection_status = false; //连接断开时，设置安全值
        rc_data.rc.ch0 = 0;
        rc_data.rc.ch1 = 0;
        rc_data.rc.ch2 = 0;
        rc_data.rc.ch3 = 0;
        rc_data.rc.s1 = 1;
        rc_data.rc.s2 = 1;
        rc_data.mouse.x = 0;
        rc_data.mouse.y = 0;
        rc_data.mouse.z = 0;
        rc_data.mouse.press_l = 0;
        rc_data.mouse.press_r = 0;
    }
    else connection_status = true;
}