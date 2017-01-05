#include <stdio.h>
#include "Type.h"
#include "stm32f10x.h"
#include "SerialPortOld.h"
#include "W24xxx.h"
#include "W25qxx.h"
#include "Data.h"
#include "multi_button.h"
#include "InputPort.h"
#include "Sys.h"
#include "WatchDog.h"
#include "Port.h"

CFIFORing com1buf; //串口1接收缓冲区
uint com1timeidle; //串口1空闲时间
//sequence 序列号 cur_seq max_seq step

void ledflash();
void eepread();
//1ms软件定时器
void softTimers();
OutputPort led1(PB0,true);
OutputPort led2(PF7,true);
OutputPort led3(PF8,true);
CW24xxx  w24c02(PB6,PB7,AT24C02);
//按键 PC13 PA0

InputPort exti(PC13);//PA1 PB3
 
WatchDog dog(3000);
void feeddog()
{
	dog.Feed();
}

void OnKeyPress(Pin pin, bool onoff)
{
	if(onoff)
	{
		led1.Write(true);
	}
	else		
	{
		led1.Write(false);
	}	
	led2=!led2;
	printf("中断引脚：%d 值%d \n",pin,onoff);
}
SerialPortOld sp1(COM1);
SerialPortOld sp2(COM2);
SerialPortOld sp3(COM3);
/*
KEY PA0 
*/
//系统初始化
void uart_init(uint32_t bound=115200);
void STDInit() 
{    	
	uart_init();
	Sys.Clock = 72000000;
	Sys.MessagePort=COM1;
	Sys.Init();
    
	exti.Init();
	exti.On();	
	exti.Register(OnKeyPress);
	
	Sys.ShowInfo();	
	//w24c02.Test();
		
	Sys.AddTask(softTimers,0,1,1,"1毫秒软件定时器");//1毫秒周期循环
	Sys.AddTask(feeddog, 0, 0, 10, "看门狗"); //看门狗-喂狗
	Sys.AddTask(ledflash,0,5,50,"状态指示灯");
	Sys.AddTask(eepread,0,10,1000,"测试任务");
	Sys.Start();
}

void ledflash()
{
	//led1=!led1;
	//led2=!led2;
	led3=!led3;	
}

void eepread()
{
	sp1.SendData('A');
	sp2.SendData('B');
	sp3.SendData('C');
}
byte USART_RX_BUF[100]; //接收缓冲,最大USART_REC_LEN个字节.
//1ms软件定时器
void softTimers()
{
    com1timeidle++;

    if (com1timeidle > 3)
    {
        com1timeidle = 0;
        ushort len = com1buf.GetLength();
        if (len >= 3)
        {
            com1buf.Pop(USART_RX_BUF, 0, len - 2);
        }
        com1buf.Reset();
    }
}
