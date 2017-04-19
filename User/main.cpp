#include <stdio.h>
#include "SerialPort.h"
#include "AT24CXX.h"
#include "Button.h"
#include "Port.h"
#include "WatchDog.h"
#include "SString.h"
#include "string.h"
#include "DateTime.h"
#include "List.h"
#include "Pwm.h"
#include "Modbus1.h"
#include "Spi.h"
#include "RTC.h"
#include "Flash.h"

typedef enum
{
    KEY_UP = 1,  // 模拟输入 
    KEY_DOWN = 1 << 1,  // 输入
    KEY_LEFT = 1 << 2,  // 浮空输入
    KEY_RIGHT = 1 << 3,  // 推挽式输入
    KEY_OK = 1 << 4,  // 开漏输出
    KEY_MENU = 1 << 5,  // 推挽式输出
    KEY_NULL = 0
} KEY_MODE;

OutputPort led1(PB0);
OutputPort led2(PF7);
OutputPort led(PF8);

//按键 PC13 PA0
//InputPort exti(PC13); //PA1 PB3     PA0 PC13
//InputPort exti1(PA0);
//void OnKeyPress(InputPort* pin, bool down, void *param)
//{
//    //led1.Write(onoff);
//    led2 = !led2;
////    printf("中断引脚：P%c%d 值%d \r\n", _PIN_NAME(pin), down);
//}

/*
ISO-V2:PB5控制485方向
ISO-V3:PC2控制485方向
 */
//ModbusSlave ModbusSlave;
//static uint OnUsartRead(ITransport *transport, Buffer &bs, void *para)
//{
//    ModbusSlave.Process(bs, para);
//    return 0;
//}
//static uint OnUsart1Read(ITransport *transport, Buffer &bs, void *para)
//{
//   bs.Show(true);
//    return 0;
//}
InputPort key0(PA0);
void LedTask(void *param)
{
    OutputPort *leds = (OutputPort*)param;
    *leds = ! * leds;
	led1=key0;
}

#define namee "StdOS"

void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	/* 使能串口1接收中断 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
}

int main(void)
{
    TSys &sys = (TSys &)(Sys);
    //    #if 0
    //        sys.Codec = codec;
    //	#endif

    sys.Name = (char*)namee;
    //Rtc提取时间
    //    HardRtc *Rtc = HardRtc::Instance();
    //    Rtc->LowPower = false;
    //    Rtc->External = false;
    //    Rtc->Init();
    //    Rtc->Start(false, false);
    sys.Init();
        #if DEBUG
            Sys.MessagePort = COM1;
			//USART1_Config();			
            Sys.ShowInfo();

    //        WatchDog::Start(20000, 10000);
    //    #else 
    //        WatchDog::Start();
        #endif 
    //    #if 0
    //        //flash 最后一块作为配置区
    //        Config::Current = &Config::CreateFlash();
    //    #endif 

    //    exti.Register(OnKeyPress);
    //    exti1.Register(OnKeyPress);
    //	SerialPort::GetMessagePort()->Register(OnUsart1Read);

    // 初始化为输出


    Sys.AddTask(LedTask, &led, 0, 500, "LedTask");

    Sys.Start();
}
