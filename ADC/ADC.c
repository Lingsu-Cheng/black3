#include "STC8H.h"
#include "adc.h"
#include"oled.h"
#include <intrins.h>  // 确保正确包含 intrins.h

void delay_us(unsigned int us) {
    while (us--) {
        // 每次循环大约1微秒（在12MHz下）
        _nop_(); _nop_(); _nop_(); _nop_();
        _nop_(); _nop_(); _nop_(); _nop_();
    }
}


// ADC初始化函数
void ADC_Init(void) {
    // 设置ADC控制寄存器
    ADC_CONTR = 0x80;  // ADC电源打开，选择速度Fosc/2
    P1M0 |= 0x01;      // 设置 P1.0 为高阻输入（模拟输入）
    P1M1 |= 0x01;
    delay_ms(2);
}

// ADC读取函数
unsigned int ADC_Read(unsigned char ch) {
    ADC_CONTR = 0x80 | (ch & 0x07); // 启用ADC电源，并选择通道
    delay_us(2);        // 等待稳定
    ADC_CONTR |= 0x08;  // 启动转换
    while (!(ADC_CONTR & 0x10));  // 等待转换完成
    ADC_CONTR &= ~0x10;           // 清除完成标志
    return (ADC_RES << 2) | (ADC_RESL & 0x03); // 返回10位ADC值
}
float ADC_ToVoltage(unsigned int adc_val) {
    return adc_val * 3.3f / 1023.0f;   // 若为3.3V系统则改为3.3f
}