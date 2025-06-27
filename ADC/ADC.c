#include "STC8H.h"
#include "adc.h"
#include"oled.h"
#include <intrins.h>  // ȷ����ȷ���� intrins.h

void delay_us(unsigned int us) {
    while (us--) {
        // ÿ��ѭ����Լ1΢�루��12MHz�£�
        _nop_(); _nop_(); _nop_(); _nop_();
        _nop_(); _nop_(); _nop_(); _nop_();
    }
}


// ADC��ʼ������
void ADC_Init(void) {
    // ����ADC���ƼĴ���
    ADC_CONTR = 0x80;  // ADC��Դ�򿪣�ѡ���ٶ�Fosc/2
    P1M0 |= 0x01;      // ���� P1.0 Ϊ�������루ģ�����룩
    P1M1 |= 0x01;
    delay_ms(2);
}

// ADC��ȡ����
unsigned int ADC_Read(unsigned char ch) {
    ADC_CONTR = 0x80 | (ch & 0x07); // ����ADC��Դ����ѡ��ͨ��
    delay_us(2);        // �ȴ��ȶ�
    ADC_CONTR |= 0x08;  // ����ת��
    while (!(ADC_CONTR & 0x10));  // �ȴ�ת�����
    ADC_CONTR &= ~0x10;           // �����ɱ�־
    return (ADC_RES << 2) | (ADC_RESL & 0x03); // ����10λADCֵ
}
float ADC_ToVoltage(unsigned int adc_val) {
    return adc_val * 3.3f / 1023.0f;   // ��Ϊ3.3Vϵͳ���Ϊ3.3f
}