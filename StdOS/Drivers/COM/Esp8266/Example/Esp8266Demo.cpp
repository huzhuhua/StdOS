#include "../Esp8266.h"
#include "Sys.h"
#include "BspPlatform/Interrupt.h"
#include "Port.h"
#include "Task.h"
#include "Core/RingBuffer.h"

#define _ESP8266EST_CPP
#ifdef _ESP8266EST_CPP

/********************************** 用户需要设置的参数**********************************/
//要连接的热点的名称，即WIFI名称
#define      macUser_ESP8266_ApSsid           "@PHICOMM_18" 

//要连接的热点的密钥
#define      macUser_ESP8266_ApPwd            "wildfire" 

//要连接的服务器的 IP，即电脑的IP
#define      macUser_ESP8266_TcpServer_IP     "www.armku.com" 

//要连接的服务器的端口
#define      macUser_ESP8266_TcpServer_Port    "8081"         


USARTHAL usart333(COM3,115200);
Esp8266 esp;

#include "../HAL/STM32F1/ARCH_UART.h"
uint8_t   loop_bufcom3[64] = { 0 };                             //定义环形缓冲区
RingBuffer ringRcvcom3(loop_bufcom3, ArrayLength(loop_bufcom3));
bool FlagInFrame;//接收到完整一帧数据
//向环形缓冲区【写】一字节数据
static void write_loop_buf(uint8_t dat)
{
	ringRcvcom3.Put(dat);
}

void checkComRoutin(void* param)
{
	static int RxCnt = 0;
	static int RxCntOld = 0;
	static int FlagIdleCnt = 0;//空闲时间

	int readlen;//读取的缓冲区长度
	char buf[100];

	if (!esp.FlagRcvAuto)
		return;

	RxCnt = ringRcvcom3.length;
	if (RxCnt != RxCntOld)
	{
		RxCntOld = RxCnt;
		return;
	}
	if (RxCnt == 0)
		return;

	if (++FlagIdleCnt > 10)
	{
		FlagInFrame = 1;

		readlen = ringRcvcom3.Get(buf, ArrayLength(buf));
		UART1_send_data((uint8_t*)buf, readlen);
		esp.DealBufIn((char*)buf,0);

		switch (esp.step)
		{
		case 0:
			break;
		default:
			break;
		}
		FlagInFrame = 0;
		FlagIdleCnt = 0;
	}
}
//要求1000ms调用一次
void Esp8266Routin(void* param)
{
	/*char cStr[100] = { 0 };*/
	switch (esp.step)
	{
	case 0:
		//debug_printf("\r\n 正在配置 ESP8266 ......\r\n");
		esp.CH_PD = 1;
		esp.RST = 1;
		esp.step++;
		break;
	case 1:
		esp.AT();
		debug_printf("AT send step:%d\r\n", esp.step);
		break;
	case 2:
		esp.ESP8266_Net_Mode_Choose(STA);
		debug_printf("Choose(STA) send step:%d\r\n", esp.step);
		break;
	case 3:
		//esp.ESP8266_JoinAP(macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd);
		debug_printf("JoinAP send step:%d\r\n", esp.step);
		break;
	case 4:
		//esp.ESP8266_Enable_MultipleId(DISABLE);
		debug_printf("Enable_MultipleId(DISABLE) send step:%d\r\n", esp.step);
		break;
	case 5:
		//esp.ESP8266_Link_Server(enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0);
		debug_printf("Link_Server send step:%d\r\n", esp.step);
		break;
	case 6:
		//esp.ESP8266_UnvarnishSend();
		debug_printf("UnvarnishSend send step:%d\r\n", esp.step);
		//配置完成
		break;
	case 7:
		//esp.ESP8266_SendStr("Hello workd!");
		debug_printf("UnvarnishSend send step:%d\r\n", esp.step);
		break;
	default:
		break;
	}
}

void Esp8266TestInit()
{
	esp.SetPinCH_PD(PG13);
	esp.SetPinRST(PG14);
	esp.Init();
	UART_3_rcv_IRQ = write_loop_buf;
	esp.send_byte = UART_3_send_byte;         //串口发送一字节数据
	esp.send_buf = UART3_send_data;

	debug_printf("\r\n WF-ESP8266 WiFi模块测试例程\r\n"); //打印测试例程提示信息)

	FlagInFrame = false;
	Sys.AddTask(checkComRoutin, 0, 0, 10, "EspCOmCHK");
	Sys.AddTask(Esp8266Routin, 0, 0, 1000, "EspRoutin");
}
#endif
