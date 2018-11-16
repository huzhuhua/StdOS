#include "EspDemoLink.h"
#include "Sys.h"
#include <string.h>  
#include "OnChip/USART.h"
#include "BspPlatform/Interrupt.h"
#include "BspPlatform\BspPlatform.h"
#include "OnChip\Port.h"
#include "OnChip\Configuration.h"
#include "Buffer.h"
#include <stdio.h>  
#include <string.h>  
#include "Sys.h"

USART usart333(USART3, 115200);
EspDemoLink espdemo(usart333);

void EspDemoLinkRoutin(void* param)
{
	if (espdemo.CheckFrame())
	{
		debug_printf("rcv one frame\n");
	}
}
void EspDemoLinkSendRoutin(void* param)
{
	usart333 << "AT\r\n";
}

void EspDemoLinkTestInit()
{
	espdemo.pinch.Set(PG13);
	espdemo.pinch.Invert = false;
	espdemo.pinch.OpenDrain = false;
	espdemo.pinch.Open();
	espdemo.pinch = 0;

	espdemo.pinrst.Set(PG14);
	espdemo.pinrst.Invert = false;
	espdemo.pinrst.OpenDrain = false;
	espdemo.pinrst.Open();
	espdemo.pinrst = 0;

	espdemo.SetPin(PG13,PG14);

	Sys.Delay(500);
	espdemo.pinch = 1;
	espdemo.pinrst = 1;

	Sys.AddTask(EspDemoLinkRoutin, 0, 1000, 1, "EspDemoLinkRoutin");
	Sys.AddTask(EspDemoLinkSendRoutin, 0, 1000, 1000, "EspDemoLinkSendRoutin");
}


EspDemoLink::EspDemoLink(USART &uart) :com(uart)
{

}

bool EspDemoLink::CheckFrame()
{

	return com.CheckFrame(rxFrame);
}

bool EspDemoLink::Send()
{
	if (!txFrame.isUpdated) //no new frame data, no need to send
		return false;
	if (!com.SendByte(txFrame.header))  //send frame header
		return false;
	if (txFrame.fnCode > MAX_FN_CODE || !com.SendByte(txFrame.fnCode))  //send function code
		return false;
	txFrame.dataLength = DATA_LENGTH[txFrame.fnCode][DIRECTION_SEND];
	if (!com.SendByte(txFrame.dataLength))  //send data length
		return false;
	if (!com.SendBytes(txFrame.data, txFrame.dataLength)) //send data;
		return false;
	txFrame.CreateCheckCode();
	if (!com.SendByte(txFrame.checkSum))    //send check code
		return false;
	txFrame.isUpdated = false;
	return true;
}
void EspDemoLink::SetPin(Pin pch, Pin prst)
{

}










#if 0
#include "DriversCom/esp8266/Socket_esp8266.h"
USART com3(USART3, 115200);
Socket_esp8266 mWifi(com3);

typedef struct SetData {
	char USEWIFIORGPRS;
	char IPADDR[20];
	char IPPORT[10];
	char WIFISSID[30];
	char WIFIKEY[30];
	char PEOPLENUM;
	char TRAINNUM;
	char TAGNUM;
	//u16
} SetDataStruct;

SetDataStruct mSetData;
u8 count = 0;

OutputPort pinch;
OutputPort pinrst;

void mWifiRoutin(void * param)
{
	static char buf[20];
	static int cnt = 0;
	static int step = 0;
	//int ret= mWifi.Kick();
	//debug_printf("检查连接：%d\n", ret);
	if (!mWifi.GetConnectStatus())
	{
		switch (step)
		{
		case 0:
			if (mWifi.Kick())//检查连接
			{
				step++;
			}
			break;
		case 1:
			mWifi.SetEcho(false);//关闭回响
			mWifi.SetMode(esp8266::esp8266_MODE_STATION, esp8266::esp8266_PATTERN_NULL);//设置station+ap模式
			mWifi.SetMUX(false);//单链接模式
			mWifi.QuitAP();
			if (mWifi.JoinAP(mSetData.WIFISSID, mSetData.WIFIKEY, esp8266::esp8266_PATTERN_NULL) && (count++ < 4));//加入AP
			{
				step++;
			}
			break;
		case 2:
			if (mWifi.Connect(mSetData.IPADDR, atoi(mSetData.IPPORT), Socket_Type_Stream, Socket_Protocol_IPV4) && (count++ < 4))
			{
				step++;
			}
			break;
		case 3:
			sprintf(buf, "%d hello world!\n", ++cnt);
			mWifi.Write(buf, strlen(buf));
			break;
		default:
			step = 0;
			break;
		}
		//	/*mMonitor.SetGPRSDataLogo(false);*/
			//while (!mWifi.Kick());//检查连接
		//	mWifi.SetEcho(false);//关闭回响
		//	mWifi.SetMode(esp8266::esp8266_MODE_STATION, esp8266::esp8266_PATTERN_NULL);//设置station+ap模式
		//	mWifi.SetMUX(false);//单链接模式
		//	mWifi.QuitAP();
		//	while (!mWifi.JoinAP(mSetData.WIFISSID, mSetData.WIFIKEY, esp8266::esp8266_PATTERN_NULL) && (count++ < 4));//加入AP
		//	count = 0;
		//	mWifi.Close();
		//	while (!mWifi.Connect(mSetData.IPADDR, atoi(mSetData.IPPORT), Socket_Type_Stream, Socket_Protocol_IPV4) && (count++ < 4));
		//	count = 0;
	}
	else
	{
		//	/*mMonitor.SetGPRSDataLogo(true);*/
	}
}
#include <stdio.h>  
#include <string.h>
char* ssid = "NETGEAR77";
char* key = "18353217097";
char * server = "www.braintics.net";
char *port = "8888";
void mWifiinit()
{
	pinch.Set(PG13);
	pinch.Invert = false;
	pinch.OpenDrain = false;
	pinch.Open();
	pinch = 0;

	pinrst.Set(PG14);
	pinrst.Invert = false;
	pinrst.OpenDrain = false;
	pinrst.Open();
	pinrst = 0;

	Sys.Delay(500);
	pinch = 1;
	pinrst = 1;

	strcpy(mSetData.WIFISSID, ssid);
	strcpy(mSetData.WIFIKEY, key);
	strcpy(mSetData.IPADDR, server);
	strcpy(mSetData.IPPORT, port);

	Sys.AddTask(mWifiRoutin, 0, 0, 1000, "mWifiRoutin");
}
#endif
