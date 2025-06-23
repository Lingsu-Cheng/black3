#include<oled.h>
#include <STC8H.H>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int

sbit INPUT= P1^6  ;
sbit OUTPUT = P1^7	 ;
sbit testkey = P3^2;  
unsigned int num = 0;
unsigned char time_out = 0;
/* 宏定义 */
#define SOUND_VELOCITY 34300  // 声速 (cm/s)
 #define PERIOD_US 1.085e-6  // 系统时钟频率 (Hz)
#define CLOCK_PERIOD PERIOD_US
void Timer0_Init(void) {
    TMOD &= 0xF0;  // 清除定时器0模式位
    TMOD |= 0x01;  // 定时器0设置为模式1（16位）
    TR0 = 0;       // 停止定时器
    TF0 = 0; 
	ET0 = 0;      // 清除溢出标志
}
void Start_Timer() {
    TH0 = 0;            // 高8位清零
    TL0 = 0;            // 低8位清零
    TF0 = 0;            // 清中断标志
    TR0 = 1;            // 启动定时器
}
void Stop_Timer() {
    TR0 = 0;            // 关闭定时器
}
unsigned int Get_Timer_Value() {
    unsigned int t0;
    t0 = ((unsigned int)TH0 << 8) | TL0;  // 获取当前 Timer0 计数值（单位：us）
    return t0 ;   // 返回16位当前计数值，单位为微秒（us）
}

void start_project(void)
{	     OLED_ShowChinese(28,0,9,12);//显示”电"
		 OLED_ShowChinese(42,0,10,12);//显示”路“
		 OLED_ShowChinese(56,0,11,12);//显示”黑”
		 OLED_ShowChinese(68,0,12,12);//显示”箱 “
		 OLED_ShowChinese(80,0,13,12);//显示”测
		 OLED_ShowChinese(92,0,14,12);//显示”试”
		 OLED_ShowChinese(104,0,15,12);//显示”仪“	 
         OLED_ShowChinese(0,2,0,12);//显示”杨"
		 OLED_ShowChinese(14,2,1,12);//显示”宇“
		 OLED_ShowChinese(28,2,2,12);//显示”非”
		 OLED_ShowString(60,2,"23211336",8); 
		 OLED_ShowChinese(0,4,3,12);//显示”高 “
		 OLED_ShowChinese(14,4,4,12);//显示”若
		 OLED_ShowChinese(28,4,5,12);//显示”恒”
		 OLED_ShowString(60,4,"23211290",8);
		 OLED_ShowChinese(0,6,6,12);//显示”汪“
		 OLED_ShowChinese(14,6,7,12);//显示”超”
		 OLED_ShowChinese(28,6,8,12);//显示”然“
		 OLED_ShowString(60,6,"23211301",8);
		 delay_ms(2000);
		 OLED_Clear();		 
}
void Generate_Step_Signal()
{		   INPUT = 1 ;
           delay_ms(2000);
		    INPUT = 0;

}
void ADC_Init()
{
}
float ADC_Read()
{
}

 void system_init()
 {	  P1M0 = 0x00;  // 设置 P1 为准双向口（传统 51 模式）
    P1M1 = 0x00;
	P3M0 &= ~(1 << 2);  // M0.2 = 0
    P3M1 &= ~(1 << 2);  // M1.2 = 0
	Timer0_Init();
	  testkey= 1;
	  INPUT =0;
	  OUTPUT =0;
	  ADC_Init();  
 }


 unsigned int keypressed()			//检测按键状态
 {		 
 	unsigned int key_Flag=0;
   if (testkey == 0) {         // 检测按键按下（低电平）
        delay_ms(20);           // 简单消抖
        if (testkey == 0) {
            if (!key_Flag) {
                key_Flag = 1;   // 设置按键已按下标志
                return 1;       // 第一次检测到按下，返回1
            }
        }
    } else {
        key_Flag = 0;           // 松开按键后，复位标志位
    }
	return 0;
 }
 void box_sort()
 {
 }
void Start_BlackBox_Test()
{		   float   v0 =0;
           unsigned int t;
           Generate_Step_Signal();
           v0= ADC_Read();
		   delay_ms(10);
		   Start_Timer();

		   Stop_Timer();
		   t=  Get_Timer_Value();
		   blacksort();

}

void main() {
	
    system_init();
    OLED_Init();//?????OLED
	OLED_ColorTurn(0);//0?????????1 ??????
    OLED_DisplayTurn(0);//0??????? 1 ?????????
 	 start_project();
	  while (1) {
        if (keypressed()) {
//           OLED_ShowString(60,2,"hello",16);  // 进入黑箱测试流程
              Start_BlackBox_Test();
        }
    }
}		   
	
 

  
	
	