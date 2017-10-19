#include "Modbus.h"
#include "Master.h"
#include "Slave.h"

/*
测试数组 
地址 功能码 地址Hi 地址Lo 个数Hi 个数Lo CrcLo Crc Hi
01 03 00 00 00 01 84 0A
*/
bool Modbus::Read(Stream &ms)
{
	#if 0
	if(ms.Length()<4)
	{
		return false;
	}
	this->Address=ms[0];
	this->Code=ms[1];
	this->Length=ms.Length()-4;
	for(int i=0;i<this->Length;i++)
	{
		this->Data[i]=ms[i+2];
	}
	this->Crc=ms[ms.Length()-1];
	this->Crc<<=8;
	this->Crc|=ms[ms.Length()-2];
	this->Crc2=GetCRC(this->Data,this->Length);
	#endif
	if(this->Crc==this->Crc2)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

void Modbus::Write(Stream &ms)
{
	#if 0
	ms[0]=this->Address;
	ms[1]=this->Code;
	this->Crc2=GetCRC(this->Data,this->Length);
	ms[this->Length+2]=this->Crc2&0xff;
	ms[this->Length+3]=(this->Crc2>>8)&0xff;
	for(int i=0;i<this->Length;i++)
	{
		ms[2+i]=this->Data[i];
	}
	#endif
}

void Modbus::SetError(ModbusErrors::Errors error){}
