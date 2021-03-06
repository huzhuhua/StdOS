#ifndef _AT24CXX_2_H_
#define _AT24CXX_2_H_

#include "..\I2CSoft_ARMKU.h"

typedef enum
{
	AT24C01 = 127,  //
	AT24C02 = 255,  //
	AT24C04 = 511,  //
	AT24C08 = 1023,  //
	AT24C16 = 2047,  //
	AT24C32 = 4095,  //
	AT24C64 = 8191,  //
	AT24C128 = 16383,  //
	AT24C256 = 32767,  //
	AT24C512 = (32767 * 2),  //
	AT24C1024 = (32767 * 4),  // 
	AT24C2048 = (32767 * 8) //
} EW24XXType;
// EEPROM
class AT24CXXX
{
public:
	I2CSoft_ARMKU* IIC; // I2C通信口
	byte Address; // 设备地址

	bool Write(ushort addr, byte data);
	byte Read(ushort addr);

	virtual bool Write(uint addr, void * buf, int len, int bufpos = 0);
	virtual bool Read(uint addr, void * buf, int len, int bufpos = 0);
public:
	AT24CXXX(EW24XXType devtype, byte devaddr = 0xA0, uint wnms = 5); //写延时时间
	void SetPin(Pin pinscl, Pin pinsda, Pin pinwriteprotect = P0);
	byte CheckOk();

	OutputPort pinWP; //保护引脚
private:
	byte checkDevice();
	ushort jsPageSize(uint type); //计算存储页大小
private:
	int PageRead(ushort addr, void * buf, int len);//页内读，最多一页
	int PageWrite(ushort addr, void * buf, int len);//页内写，最多一页
	
	EW24XXType deviceType; //器件类型
	uint Block; //存储页大小
	uint writedelaynms; //写延时	  
};

#endif
