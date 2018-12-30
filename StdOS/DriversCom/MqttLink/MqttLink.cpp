#include <string.h>
#include "MqttLink.h"
#include "Sys.h"
#include "Buffer.h"

MqttLink::MqttLink(USART &uart) :com(uart)
{
	com.ClearRxBuf();
	com.ClearTxBuf();
}

bool MqttLink::CheckFrame()
{
	int rxlen = com.RxSize();

	if (com.GetBytes(&rxFrame.data[rxFrame.dataLength], rxlen))
	{
		rxFrame.dataLength += rxlen;
	}
}
bool MqttLink::Get(uint8_t & da)
{
	return true;
}
bool MqttLink::Gets(uint8_t * pData, uint16_t num)
{
	return true;
}

bool MqttLink::Send()
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
bool MqttLink::Connect()
{
	this->txFrame.data[0] = this->FixHead;
	this->txFrame.data[1] = 0x15;
	this->txFrame.data[2] = 0x00;
	this->txFrame.data[3] = 0x04;
	this->txFrame.data[4] = 'M';
	this->txFrame.data[5] = 'Q';
	this->txFrame.data[6] = 'T';
	this->txFrame.data[7] = 'T';
	this->txFrame.data[8] = 0x04;
	this->txFrame.data[9] = 0x02;
	this->txFrame.data[10] = 0x00;
	this->txFrame.data[11] = 0x3C;
	this->txFrame.data[12] = 0x00;
	this->txFrame.data[13] = strlen(ClientID);
	for (int i = 0; i < strlen(ClientID); i++)
	{
		this->txFrame.data[14 + i] = this->ClientID[i];
	}

	this->com.SendBytes(this->txFrame.data, 14 + strlen(ClientID));
	Sys.Sleep(500);
	
	if (com.RxSize() > 0)
	{
		debug_printf("rxlen:%d\n",com.RxSize());		
	}

	if ((com.RxSize() > 0) && CheckFrame())
	{
		debug_printf("rcv ok rxlen:%d-%d\n", com.RxSize(),rxFrame.dataLength);
		Buffer bs(rxFrame.data, rxFrame.dataLength);
		bs.ShowHex();
		this->com.ClearRxBuf();
		
		return true;
	}
	else
	{
		debug_printf("rcv error\n");
		return false;
	}
}
