#ifndef ADC_H
#define ADC_H

// 确保没有重复声明
extern void delay_us(unsigned int us);
extern void ADC_Init(void);
extern unsigned int ADC_Read(unsigned char channel);
extern float ADC_ToVoltage(unsigned int adc_val);
#endif