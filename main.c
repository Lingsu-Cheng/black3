#include<oled.h>
#include <STC8H.H>
#include <intrins.h>
#include "adc.h"
#include <math.h>
#define uchar unsigned char
#define uint unsigned int

sbit INPUT= P1^6  ;
sbit OUTPUT = P1^7	 ;
sbit testkey = P3^2;  
unsigned int num = 0;
unsigned char time_out = 0;
/* �궨�� */
#define SOUND_VELOCITY 34300  // ���� (cm/s)
 #define PERIOD_US 1.085e-6  // ϵͳʱ��Ƶ�� (Hz)
#define CLOCK_PERIOD PERIOD_US
float Get_Time_ms() {
    unsigned int t = ((unsigned int)TH0 << 8) | TL0;
    return t / 1000.0f;  // �Ժ���Ϊ��λ����
}
void Timer0_Init(void) {
    TMOD &= 0xF0;  // �����ʱ��0ģʽλ
    TMOD |= 0x01;  // ��ʱ��0����Ϊģʽ1��16λ��
    TR0 = 0;       // ֹͣ��ʱ��
    TF0 = 0; 
	ET0 = 0;      // ��������־
}
void Start_Timer() {
    TH0 = 0;            // ��8λ����
    TL0 = 0;            // ��8λ����
    TF0 = 0;            // ���жϱ�־
    TR0 = 1;            // ������ʱ��
}
void Stop_Timer() {
    TR0 = 0;            // �رն�ʱ��
}
unsigned int Get_Timer_Value() {
    unsigned int t0;
    t0 = ((unsigned int)TH0 << 8) | TL0;  // ��ȡ��ǰ Timer0 ����ֵ����λ��us��
    return t0 ;   // ����16λ��ǰ����ֵ����λΪ΢�루us��
}

void start_project(void)
{	     OLED_ShowChinese(28,0,9,12);//��ʾ����"
		 OLED_ShowChinese(42,0,10,12);//��ʾ��·��
		 OLED_ShowChinese(56,0,11,12);//��ʾ���ڡ�
		 OLED_ShowChinese(68,0,12,12);//��ʾ���� ��
		 OLED_ShowChinese(80,0,13,12);//��ʾ����
		 OLED_ShowChinese(92,0,14,12);//��ʾ���ԡ�
		 OLED_ShowChinese(104,0,15,12);//��ʾ���ǡ�	 
         OLED_ShowChinese(0,2,0,12);//��ʾ����"
		 OLED_ShowChinese(14,2,1,12);//��ʾ���
		 OLED_ShowChinese(28,2,2,12);//��ʾ���ǡ�
		 OLED_ShowString(60,2,"23211336",8); 
		 OLED_ShowChinese(0,4,3,12);//��ʾ���� ��
		 OLED_ShowChinese(14,4,4,12);//��ʾ����
		 OLED_ShowChinese(28,4,5,12);//��ʾ���㡱
		 OLED_ShowString(60,4,"23211290",8);
		 OLED_ShowChinese(0,6,6,12);//��ʾ������
		 OLED_ShowChinese(14,6,7,12);//��ʾ������
		 OLED_ShowChinese(28,6,8,12);//��ʾ��Ȼ��
		 OLED_ShowString(60,6,"23211301",8);
		 delay_ms(2000);
		 OLED_Clear();		 
}
void Generate_Step_Signal()
{		   INPUT = 1 ;
           delay_ms(2000);
		    INPUT = 0;

}


 void system_init()
 {	  P1M0 = 0x00;  // ���� P1 Ϊ׼˫��ڣ���ͳ 51 ģʽ��
    P1M1 = 0x00;
	P3M0 &= ~(1 << 2);  // M0.2 = 0
    P3M1 &= ~(1 << 2);  // M1.2 = 0
	Timer0_Init();
	  testkey= 1;
	  INPUT =0;
	  OUTPUT =0;
	 ADC_Init();  
 }


 unsigned int keypressed()			//��ⰴ��״̬
 {		 
 	unsigned int key_Flag=0;
   if (testkey == 0) {         // ��ⰴ�����£��͵�ƽ��
        delay_ms(20);           // ������
        if (testkey == 0) {
            if (!key_Flag) {
                key_Flag = 1;   // ���ð����Ѱ��±�־
                return 1;       // ��һ�μ�⵽���£�����1
            }
        }
    } else {
        key_Flag = 0;           // �ɿ������󣬸�λ��־λ
    }
	return 0;
 }
 void box_sort(float tau,float v0,float v_inf)
 {
 }
void Start_BlackBox_Test()
{		   unsigned int adc_val;
           float v0, v_inf, vt, v_target;
           float tau = 0;
           float t_ms = 0;
		   Generate_Step_Signal() ;
		   delay_us(50);               // �ȴ��������ȶ�

    // === 2. ������ʼ��ѹ v0 ===
            adc_val = ADC_Read(0);
            v0 = ADC_ToVoltage(adc_val);
		    delay_ms(30);                   // �ȴ������ѹ�����ȶ�
            adc_val = ADC_Read(0);
             v_inf = ADC_ToVoltage(adc_val);
			   // === 4. ���� v_target = v_inf + (v0 - v_inf) * e^(-1) ===
               v_target = v_inf + (v0 - v_inf) * 0.368f;



    // === 5. �ٴ�ʩ�ӽ�Ծ�����¿�ʼ����׼����ʱ ===
    Generate_Step_Signal();
    delay_us(50);               // ����ʼ��ѹ�ȶ�ʱ��

    Start_Timer();              // ������ʱ��
    do {
        adc_val = ADC_Read(0);
        vt = ADC_ToVoltage(adc_val);
    } while (fabs(vt - v_target) > 0.02 && Get_Timer_Value() < 60000);
    Stop_Timer();

    t_ms = Get_Time_ms();      // ������ֵת��Ϊ ms
     tau = t_ms;   
	 box_sort()	 ;
}

void main() {
	
    system_init();
    OLED_Init();//?????OLED
	OLED_ColorTurn(0);//0?????????1 ??????
    OLED_DisplayTurn(0);//0??????? 1 ?????????
 	 start_project();
	  while (1) {
        if (keypressed()) {
          OLED_ShowString(60,2,"hello",16);  // ��������������
//              Start_BlackBox_Test();
        }
    }
}		   
	
 

  
	
	