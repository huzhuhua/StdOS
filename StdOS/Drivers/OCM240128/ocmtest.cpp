#include "OCM240128.h"
#include "Sys.h"
#include "ocmtest.h"

void ocmtest::SetPin(Pin ce, Pin wr, Pin rd, Pin fs, Pin led, Pin cd)
{
	this->ce.SetPin(ce);
	this->wr.SetPin(wr);
	this->rd.SetPin(rd);
	/*this->pinfs.SetPin(fs);*/
	//this->pinled.SetPin(led);
	this->cd.SetPin(cd);

	this->ce.pinMode(GPIO_Out_PP);
	this->wr.pinMode(GPIO_Out_PP);
	this->rd.pinMode(GPIO_Out_PP);
	/*this->pinfs.pinMode(GPIO_Out_PP);*/
	//this->pinled.pinMode(GPIO_Out_PP);
	this->cd.pinMode(GPIO_Out_PP);
}

void ocmtest::SetDataPin(Pin d0, Pin d1, Pin d2, Pin d3, Pin d4, Pin d5, Pin d6, Pin d7)
{
	this->data0.SetPin(d0);
	this->data1.SetPin(d1);
	this->data2.SetPin(d2);
	this->data3.SetPin(d3);
	this->data4.SetPin(d4);
	this->data5.SetPin(d5);
	this->data6.SetPin(d6);
	this->data7.SetPin(d7);

	this->data0.pinMode(GPIO_Out_PP);
	this->data1.pinMode(GPIO_Out_PP);
	this->data2.pinMode(GPIO_Out_PP);
	this->data3.pinMode(GPIO_Out_PP);
	this->data4.pinMode(GPIO_Out_PP);
	this->data5.pinMode(GPIO_Out_PP);
	this->data6.pinMode(GPIO_Out_PP);
	this->data7.pinMode(GPIO_Out_PP);
}
void ocmtest::writedata(uint8_t da)
{
	this->data0 = da & (1 << 0);
	this->data1 = da & (1 << 1);
	this->data2 = da & (1 << 2);
	this->data3 = da & (1 << 3);
	this->data4 = da & (1 << 4);
	this->data5 = da & (1 << 5);
	this->data6 = da & (1 << 6);
	this->data7 = da & (1 << 7);
}
#define width      30       //显示区宽度
#define addr_w     0x0000   //文本显示区首地址
#define addr_t     0x01e0   //图形显示区首地址
//#define data_ora   P0       //MCU P0<------> LCM

uint8_t const tab11[] = {
0x00,0x00,0x00,0x00,0x00,0x27,0x4f,0x4c,0x44,0x45,0x4e,0x00,0x30,0x41,0x4c,
0x4d,0x00,0x33,0x43,0x49,0x45,0x4e,0x43,0x45,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x34,0x45,0x43,0x48,0x4e,0x4f,0x4c,0x4f,0x47,0x59,
0x00,0x23,0x4f,0x0e,0x0c,0x2c,0x54,0x44,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x2e,0x4f,0x0e,0x13,0x11,0x00,0x2a,0x49,0x41,0x4e,0x53,0x48,0x45,0x00,0x32,
0x4f,0x41,0x44,0x13,0x00,0x3a,0x48,0x41,0x4f,0x51,0x49,0x4e,0x47,0x00,0x00,
0x30,0x0e,0x32,0x0e,0x23,0x48,0x49,0x4e,0x41,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x30,0x4f,0x53,0x54,0x43,0x4f,0x44,0x45,0x1a,0x00,0x15,0x12,0x16,0x10,0x14,
0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x34,0x25,0x2c,0x1a,0x00,0x18,0x16,0x0d,0x17,0x15,0x18,0x0d,0x12,0x12,0x12,
0x13,0x11,0x19,0x16,0x00,0x00,0x12,0x12,0x12,0x10,0x13,0x17,0x12,0x00,0x00,
0x48,0x54,0x54,0x50,0x1a,0x0f,0x0f,0x57,0x57,0x57,0x0e,0x47,0x50,0x54,0x0e,
0x43,0x4f,0x4d,0x0e,0x43,0x4e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x25,0x4d,0x41,0x49,0x4c,0x1a,0x4a,0x4d,0x47,0x50,0x43,0x20,0x50,0x55,0x42,
0x0e,0x5a,0x48,0x41,0x4f,0x51,0x49,0x4e,0x47,0x0e,0x47,0x44,0x0e,0x43,0x4e,
0x26,0x21,0x38,0x1a,0x00,0x12,0x12,0x18,0x15,0x11,0x11,0x10,0x00,0x00,0x28,
0x4f,0x54,0x4c,0x49,0x4e,0x45,0x1a,0x00,0x12,0x12,0x19,0x12,0x10,0x11,0x14 };

uint8_t const tab12[] = {
	/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
	/*--  文字:  肇  --*/
	0x10,0x40,0x08,0x40,0x3E,0xFE,0x23,0x48,0x3E,0x30,0x40,0xC8,0x41,0x06,0x9F,0xF0,
	0x01,0x10,0x7F,0xFC,0x01,0x10,0x3F,0xF0,0x01,0x00,0xFF,0xFE,0x01,0x00,0x01,0x00,
	/*--  文字:  庆  --*/
	0x01,0x00,0x00,0x80,0x3F,0xFE,0x20,0x80,0x20,0x80,0x20,0x80,0x3F,0xFE,0x21,0x40,
	0x21,0x40,0x21,0x20,0x22,0x20,0x22,0x10,0x44,0x18,0x48,0x0E,0x90,0x04,0x00,0x00,
	/*--  文字:  金  --*/
	0x01,0x00,0x01,0x00,0x02,0x80,0x04,0x40,0x08,0x30,0x30,0x0E,0xCF,0xF4,0x01,0x00,
	0x01,0x00,0x3F,0xF8,0x01,0x00,0x09,0x20,0x05,0x20,0x05,0x40,0xFF,0xFE,0x00,0x00,
	/*--  文字:  鹏  --*/
	0x00,0x10,0x77,0x20,0x55,0x7C,0x55,0x64,0x77,0x54,0x55,0x54,0x55,0x4C,0x55,0x40,
	0x77,0x7E,0x55,0x02,0x55,0x02,0x55,0xFA,0x55,0x02,0xB9,0x14,0x13,0x08,0x00,0x00,
	/*--  文字:  科  --*/
	0x06,0x08,0x78,0x88,0x08,0x48,0x08,0x48,0xFE,0x08,0x18,0x88,0x1C,0x48,0x2A,0x48,
	0x28,0x0E,0x48,0x78,0x8B,0x88,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
	/*--  文字:  技  --*/
	0x10,0x20,0x10,0x20,0x10,0x20,0xFD,0xFE,0x10,0x20,0x14,0x20,0x19,0xFC,0x31,0x08,
	0xD0,0x88,0x10,0x90,0x10,0x60,0x10,0x60,0x10,0x90,0x11,0x0E,0x56,0x04,0x20,0x00,
	/*--  文字:  有  --*/
	0x01,0x00,0x01,0x00,0x7F,0xFE,0x02,0x00,0x04,0x00,0x0F,0xF0,0x18,0x10,0x28,0x10,
	0x4F,0xF0,0x88,0x10,0x08,0x10,0x0F,0xF0,0x08,0x10,0x08,0x90,0x08,0x70,0x08,0x20,
	/*--  文字:  限  --*/
	0x00,0x00,0xFB,0xF8,0x92,0x08,0x93,0xF8,0xA2,0x08,0xA2,0x08,0x93,0xF8,0x8A,0x80,
	0x8A,0x48,0xAA,0x50,0x92,0x20,0x82,0x20,0x82,0x10,0x82,0x8E,0x83,0x04,0x82,0x00,
	/*--  文字:  公  --*/
	0x00,0x00,0x04,0x40,0x06,0x40,0x04,0x40,0x08,0x20,0x08,0x10,0x10,0x18,0x22,0x0E,
	0x43,0x04,0x02,0x00,0x04,0x20,0x08,0x10,0x11,0xF8,0x3F,0x18,0x10,0x10,0x00,0x00,
	/*--  文字:  司  --*/
	0x00,0x00,0x3F,0xFC,0x00,0x04,0x00,0x04,0x7F,0xE4,0x00,0x04,0x00,0x04,0x3F,0xC4,
	0x20,0x44,0x20,0x44,0x3F,0xC4,0x20,0x44,0x20,0x04,0x00,0x14,0x00,0x08,0x00,0x00,
	/*--  文字:  肇  --*/
	0x10,0x40,0x08,0x40,0x3E,0xFE,0x23,0x48,0x3E,0x30,0x40,0xC8,0x41,0x06,0x9F,0xF0,
	0x01,0x10,0x7F,0xFC,0x01,0x10,0x3F,0xF0,0x01,0x00,0xFF,0xFE,0x01,0x00,0x01,0x00,
	/*--  文字:  庆  --*/
	0x01,0x00,0x00,0x80,0x3F,0xFE,0x20,0x80,0x20,0x80,0x20,0x80,0x3F,0xFE,0x21,0x40,
	0x21,0x40,0x21,0x20,0x22,0x20,0x22,0x10,0x44,0x18,0x48,0x0E,0x90,0x04,0x00,0x00,
	/*--  文字:  市  --*/
	0x02,0x00,0x01,0x00,0xFF,0xFE,0x01,0x00,0x01,0x00,0x1F,0xF8,0x11,0x08,0x11,0x08,
	0x11,0x08,0x11,0x08,0x11,0x08,0x11,0x08,0x11,0x28,0x11,0x10,0x01,0x00,0x01,0x00,
	/*--  文字:  建  --*/
	0x00,0x40,0x78,0x40,0x0B,0xF8,0x10,0x48,0x17,0xFE,0x20,0x48,0x7B,0xF8,0x08,0x40,
	0x4B,0xFC,0x48,0x40,0x28,0x40,0x17,0xFC,0x28,0x40,0x46,0x40,0x81,0xFE,0x00,0x00,
	/*--  文字:  设  --*/
	0x40,0x00,0x21,0xF0,0x31,0x10,0x21,0x10,0x01,0x10,0x01,0x10,0xE2,0x0E,0x25,0xF8,
	0x21,0x08,0x21,0x08,0x20,0x90,0x20,0x90,0x28,0x60,0x30,0x90,0x23,0x0E,0x0C,0x04,
	/*--  文字:  三  --*/
	0x00,0x00,0x00,0x00,0x7F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xF8,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFE,0x00,0x00,0x00,0x00,
	/*--  文字:  路  --*/
	0x00,0x80,0x78,0x80,0x48,0xFC,0x49,0x88,0x4A,0x48,0x7C,0x50,0x50,0x20,0x10,0x58,
	0x5D,0x86,0x53,0xFC,0x51,0x08,0x51,0x08,0x5D,0x08,0xE1,0xF8,0x01,0x08,0x00,0x00,
	/*--  文字:  三  --*/
	0x00,0x00,0x00,0x00,0x7F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xF8,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFE,0x00,0x00,0x00,0x00,
	/*--  文字:  一  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x7F,0xFE,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/*--  文字:  号  --*/
	0x1F,0xF0,0x10,0x10,0x10,0x10,0x10,0x10,0x1F,0xF0,0x00,0x00,0xFF,0xFE,0x08,0x00,
	0x08,0x00,0x1F,0xF0,0x08,0x10,0x00,0x10,0x00,0x10,0x01,0x10,0x00,0xA0,0x00,0x40 };

uint8_t const tab3[] = {
	/*--  宋体18;  此字体下对应的点阵为：宽x高=24x24   --*/
	/*--  文字:  本  --*/
	0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,
	0x18,0x00,0x00,0x18,0x08,0x1F,0xFF,0xF0,0x00,0x3C,0x00,0x00,0x7A,0x00,0x00,0x7A,
	0x00,0x00,0xD9,0x00,0x00,0x99,0x80,0x01,0x98,0x80,0x03,0x18,0x40,0x06,0x18,0x70,
	0x04,0x18,0xB8,0x1B,0xFF,0xDE,0x20,0x18,0x00,0x40,0x18,0x00,0x00,0x18,0x00,0x00,
	0x18,0x00,0x00,0x18,0x00,0x00,0x00,0x00,
	/*--  文字:  模  --*/
	0x00,0x00,0x00,0x00,0x08,0x00,0x06,0x0C,0x40,0x04,0x0C,0x44,0x04,0x0C,0x4E,0x04,
	0x7F,0xF0,0x04,0x8C,0x40,0x3F,0x10,0x10,0x0C,0x1F,0xF8,0x0C,0x10,0x10,0x0E,0x10,
	0x10,0x0D,0x9F,0xF0,0x14,0x90,0x10,0x14,0x90,0x10,0x24,0x1F,0xF0,0x24,0x02,0x00,
	0x44,0x02,0x0C,0x04,0xFF,0xF0,0x04,0x06,0x80,0x04,0x04,0x40,0x04,0x08,0x60,0x04,
	0x10,0x3C,0x04,0x60,0x0C,0x01,0x80,0x00,
	/*--  文字:  块  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x06,0x00,0x04,0x04,0x00,0x04,0x04,0x00,0x04,
	0x04,0x00,0x04,0x04,0x30,0x04,0x3F,0xF0,0x04,0xC4,0x30,0x3F,0x04,0x30,0x04,0x04,
	0x30,0x04,0x04,0x30,0x04,0x04,0x34,0x04,0x04,0x3E,0x04,0xFF,0xC0,0x04,0x0D,0x00,
	0x04,0xC9,0x00,0x0F,0x08,0x80,0x78,0x18,0xC0,0x20,0x30,0x60,0x00,0x60,0x30,0x00,
	0x80,0x1C,0x03,0x00,0x08,0x00,0x00,0x00,
	/*--  文字:  是  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0xC0,0x01,0xFF,0xC0,0x01,0x00,0xC0,0x01,
	0x00,0xC0,0x01,0xFF,0xC0,0x01,0x00,0xC0,0x01,0x00,0xC0,0x01,0x00,0xC0,0x01,0xFF,
	0xC0,0x00,0x00,0x08,0x00,0x00,0x1C,0x1F,0xFF,0xE0,0x01,0x08,0x00,0x03,0x08,0x10,
	0x02,0x0F,0xF8,0x02,0x08,0x00,0x07,0x08,0x00,0x04,0xC8,0x00,0x08,0x38,0x00,0x10,
	0x0F,0xFC,0x20,0x00,0xF8,0x00,0x00,0x00,
	/*--  文字:  一  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x0C,0x3F,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/*--  文字:  种  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0x03,0xC0,0x80,0x3E,0x00,0x80,0x02,
	0x00,0x80,0x02,0x00,0x80,0x02,0x5F,0xFC,0x7F,0xE8,0x88,0x02,0x08,0x88,0x06,0x08,
	0x88,0x07,0x08,0x88,0x0E,0xC8,0x8C,0x0A,0x4F,0xFC,0x12,0x58,0x8C,0x12,0x10,0x80,
	0x22,0x00,0x80,0x42,0x00,0x80,0x02,0x00,0x80,0x02,0x00,0x80,0x02,0x00,0x80,0x02,
	0x00,0x80,0x02,0x00,0x80,0x00,0x00,0x00,
	/*--  文字:  图  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x08,0x1F,0xFF,0xF8,0x18,0x60,0x08,0x18,
	0x41,0x08,0x18,0xFF,0x88,0x18,0xC2,0x08,0x19,0x24,0x08,0x1A,0x28,0x08,0x18,0x10,
	0x08,0x18,0x2C,0x08,0x18,0x47,0x08,0x19,0xA1,0xE8,0x1E,0x1C,0x48,0x18,0x04,0x08,
	0x18,0x00,0x08,0x18,0x78,0x08,0x18,0x0E,0x08,0x18,0x02,0x08,0x18,0x00,0x08,0x1F,
	0xFF,0xF8,0x18,0x00,0x08,0x00,0x00,0x00,
	/*--  文字:  形  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x06,0x18,0x1F,0xF8,0x30,0x02,
	0x30,0x60,0x02,0x30,0x40,0x02,0x30,0x80,0x02,0x33,0x00,0x02,0x34,0x08,0x02,0x36,
	0x18,0x1F,0xF8,0x30,0x06,0x30,0x40,0x06,0x30,0x80,0x04,0x31,0x00,0x04,0x36,0x04,
	0x04,0x30,0x0C,0x0C,0x30,0x18,0x08,0x30,0x30,0x10,0x30,0x40,0x10,0x31,0x80,0x20,
	0x32,0x00,0x40,0x0C,0x00,0x00,0x00,0x00,
	/*--  文字:  点  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x10,0x00,0x00,0x10,0x10,0x00,
	0x1F,0xF8,0x00,0x10,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x02,0x10,0x60,0x03,0xEF,
	0xE0,0x02,0x00,0x60,0x02,0x00,0x60,0x02,0x00,0x60,0x02,0x00,0x60,0x03,0xFF,0xE0,
	0x02,0x00,0x60,0x00,0x00,0x00,0x00,0x84,0x20,0x08,0x42,0x18,0x08,0x63,0x08,0x18,
	0x21,0x0C,0x30,0x00,0x00,0x00,0x00,0x00,
	/*--  文字:  阵  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x1F,0x86,0x00,0x11,0x84,0x08,0x11,
	0x7F,0xFC,0x11,0x0C,0x00,0x12,0x08,0x00,0x12,0x09,0x80,0x14,0x19,0x00,0x12,0x11,
	0x00,0x11,0x11,0x08,0x10,0xBF,0xF0,0x10,0x81,0x00,0x10,0x81,0x00,0x11,0x81,0x00,
	0x1F,0x81,0x0C,0x12,0x7F,0xF0,0x10,0x01,0x80,0x10,0x01,0x80,0x10,0x01,0x80,0x10,
	0x01,0x80,0x10,0x01,0x80,0x00,0x00,0x00,
	/*--  文字:  液  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x08,0x01,0x00,0x0C,0x01,0x0C,0x06,
	0x7E,0xF0,0x04,0x11,0x00,0x00,0x11,0x00,0x41,0x31,0x00,0x32,0x23,0xF8,0x12,0x42,
	0x10,0x1A,0x67,0x10,0x04,0xE4,0xD0,0x04,0xEC,0x70,0x05,0x6A,0x20,0x06,0x72,0x20,
	0x08,0x61,0x60,0x38,0x61,0xC0,0x18,0x60,0xC0,0x18,0x61,0x60,0x18,0x63,0x30,0x18,
	0x64,0x1E,0x18,0x68,0x08,0x00,0x10,0x00,
	/*--  文字:  晶  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x40,0x01,0xFF,0xC0,0x01,0x00,0x40,0x01,
	0x00,0x40,0x01,0xFF,0xC0,0x01,0x00,0x40,0x01,0x00,0x40,0x01,0xFF,0xC0,0x01,0x00,
	0x40,0x00,0x00,0x00,0x10,0x64,0x08,0x1F,0xE7,0xFC,0x10,0x46,0x08,0x10,0x46,0x08,
	0x10,0x46,0x08,0x1F,0xC7,0xF8,0x10,0x46,0x08,0x10,0x46,0x08,0x10,0x46,0x08,0x1F,
	0xC7,0xF8,0x10,0x46,0x08,0x00,0x00,0x00,
	/*--  文字:  显  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x03,0xFF,0xF0,0x02,0x00,0x60,0x02,
	0x00,0x60,0x02,0x00,0x60,0x03,0xFF,0xE0,0x02,0x00,0x60,0x02,0x00,0x60,0x03,0xFF,
	0xE0,0x02,0x00,0x60,0x00,0x46,0x00,0x00,0x46,0x10,0x08,0x46,0x18,0x04,0x46,0x30,
	0x06,0x46,0x20,0x02,0x46,0x40,0x03,0x46,0x80,0x02,0x47,0x00,0x00,0x46,0x08,0x3F,
	0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,
	/*--  文字:  示  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x07,0xFF,0xC0,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x3F,0xFF,
	0xF0,0x00,0x18,0x00,0x01,0x99,0x00,0x03,0x18,0x80,0x03,0x18,0x40,0x06,0x18,0x20,
	0x04,0x18,0x10,0x08,0x18,0x18,0x10,0x18,0x0C,0x20,0x18,0x0C,0x40,0x18,0x00,0x00,
	0xF8,0x00,0x00,0x30,0x00,0x00,0x00,0x00,
	/*--  文字:  模  --*/
	0x00,0x00,0x00,0x00,0x08,0x00,0x06,0x0C,0x40,0x04,0x0C,0x44,0x04,0x0C,0x4E,0x04,
	0x7F,0xF0,0x04,0x8C,0x40,0x3F,0x10,0x10,0x0C,0x1F,0xF8,0x0C,0x10,0x10,0x0E,0x10,
	0x10,0x0D,0x9F,0xF0,0x14,0x90,0x10,0x14,0x90,0x10,0x24,0x1F,0xF0,0x24,0x02,0x00,
	0x44,0x02,0x0C,0x04,0xFF,0xF0,0x04,0x06,0x80,0x04,0x04,0x40,0x04,0x08,0x60,0x04,
	0x10,0x3C,0x04,0x60,0x0C,0x01,0x80,0x00,
	/*--  文字:  块  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x06,0x00,0x04,0x04,0x00,0x04,0x04,0x00,0x04,
	0x04,0x00,0x04,0x04,0x30,0x04,0x3F,0xF0,0x04,0xC4,0x30,0x3F,0x04,0x30,0x04,0x04,
	0x30,0x04,0x04,0x30,0x04,0x04,0x34,0x04,0x04,0x3E,0x04,0xFF,0xC0,0x04,0x0D,0x00,
	0x04,0xC9,0x00,0x0F,0x08,0x80,0x78,0x18,0xC0,0x20,0x30,0x60,0x00,0x60,0x30,0x00,
	0x80,0x1C,0x03,0x00,0x08,0x00,0x00,0x00,
	/*--  文字:  可  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x3F,0xFF,0xF0,0x00,
	0x00,0x40,0x00,0x00,0x40,0x00,0x00,0x40,0x07,0xF8,0x40,0x04,0x18,0x40,0x04,0x18,
	0x40,0x04,0x18,0x40,0x04,0x18,0x40,0x04,0x18,0x40,0x04,0x18,0x40,0x07,0xF8,0x40,
	0x04,0x18,0x40,0x04,0x10,0x40,0x08,0x00,0x40,0x00,0x00,0x40,0x00,0x06,0xC0,0x00,
	0x01,0xC0,0x00,0x00,0x80,0x00,0x00,0x00,
	/*--  文字:  用  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x10,0x0F,0xFF,0xF8,0x0C,0x08,0x10,0x0C,
	0x08,0x10,0x0C,0x08,0x10,0x0C,0x08,0x10,0x0C,0x08,0x10,0x0F,0xFF,0xF0,0x08,0x08,
	0x10,0x08,0x08,0x10,0x08,0x08,0x10,0x08,0x08,0x10,0x0F,0xFF,0xF0,0x08,0x08,0x10,
	0x08,0x08,0x10,0x08,0x08,0x10,0x10,0x18,0x10,0x10,0x18,0x10,0x20,0x18,0x10,0x20,
	0x18,0xF0,0x40,0x00,0x30,0x00,0x00,0x00,
	/*--  文字:  于  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x70,0x07,0xFF,0x80,0x00,
	0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,
	0x0C,0x3F,0xFF,0xF0,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,
	0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x01,0x98,0x00,0x00,
	0x70,0x00,0x00,0x30,0x00,0x00,0x00,0x00,
	/*--  文字:  显  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x03,0xFF,0xF0,0x02,0x00,0x60,0x02,
	0x00,0x60,0x02,0x00,0x60,0x03,0xFF,0xE0,0x02,0x00,0x60,0x02,0x00,0x60,0x03,0xFF,
	0xE0,0x02,0x00,0x60,0x00,0x46,0x00,0x00,0x46,0x10,0x08,0x46,0x18,0x04,0x46,0x30,
	0x06,0x46,0x20,0x02,0x46,0x40,0x03,0x46,0x80,0x02,0x47,0x00,0x00,0x46,0x08,0x3F,
	0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,
	/*--  文字:  示  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x07,0xFF,0xC0,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x3F,0xFF,
	0xF0,0x00,0x18,0x00,0x01,0x99,0x00,0x03,0x18,0x80,0x03,0x18,0x40,0x06,0x18,0x20,
	0x04,0x18,0x10,0x08,0x18,0x18,0x10,0x18,0x0C,0x20,0x18,0x0C,0x40,0x18,0x00,0x00,
	0xF8,0x00,0x00,0x30,0x00,0x00,0x00,0x00,
	/*--  文字:  字  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x08,0x00,0x08,0x08,0x18,0x0F,
	0xF7,0xF8,0x18,0x00,0x10,0x30,0x00,0x20,0x00,0x00,0xC0,0x07,0xFF,0xE0,0x00,0x03,
	0x00,0x00,0x04,0x00,0x00,0x18,0x00,0x00,0x18,0x0C,0x7F,0xFF,0xFE,0x00,0x18,0x00,
	0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,
	0xF8,0x00,0x00,0x30,0x00,0x00,0x00,0x00,
	/*--  文字:  符  --*/
	0x00,0x00,0x00,0x00,0x01,0x00,0x03,0x01,0x80,0x06,0x01,0x00,0x04,0x12,0x0C,0x0B,
	0xE3,0xF0,0x18,0xC4,0x20,0x10,0x48,0x20,0x61,0x10,0x00,0x01,0x80,0x60,0x03,0x00,
	0x44,0x02,0x00,0x4E,0x04,0x7F,0xF0,0x0E,0x20,0x40,0x16,0x10,0x40,0x26,0x08,0x40,
	0x46,0x0C,0x40,0x06,0x08,0x40,0x06,0x00,0x40,0x06,0x00,0x40,0x06,0x00,0x40,0x06,
	0x07,0xC0,0x06,0x00,0xC0,0x00,0x00,0x00,
	/*--  文字:  ，  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x1E,0x00,0x00,0x1E,0x00,0x00,0x0C,0x00,0x00,0x04,0x00,0x00,0x08,0x00,0x00,0x10,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/*--  文字:  图  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x08,0x1F,0xFF,0xF8,0x18,0x60,0x08,0x18,
	0x41,0x08,0x18,0xFF,0x88,0x18,0xC2,0x08,0x19,0x24,0x08,0x1A,0x28,0x08,0x18,0x10,
	0x08,0x18,0x2C,0x08,0x18,0x47,0x08,0x19,0xA1,0xE8,0x1E,0x1C,0x48,0x18,0x04,0x08,
	0x18,0x00,0x08,0x18,0x78,0x08,0x18,0x0E,0x08,0x18,0x02,0x08,0x18,0x00,0x08,0x1F,
	0xFF,0xF8,0x18,0x00,0x08,0x00,0x00,0x00,
	/*--  文字:  形  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x06,0x18,0x1F,0xF8,0x30,0x02,
	0x30,0x60,0x02,0x30,0x40,0x02,0x30,0x80,0x02,0x33,0x00,0x02,0x34,0x08,0x02,0x36,
	0x18,0x1F,0xF8,0x30,0x06,0x30,0x40,0x06,0x30,0x80,0x04,0x31,0x00,0x04,0x36,0x04,
	0x04,0x30,0x0C,0x0C,0x30,0x18,0x08,0x30,0x30,0x10,0x30,0x40,0x10,0x31,0x80,0x20,
	0x32,0x00,0x40,0x0C,0x00,0x00,0x00,0x00,
	/*--  文字:  和  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0xF0,0x00,0x1F,0x00,0x08,0x01,
	0x03,0xFC,0x01,0x02,0x08,0x01,0x02,0x08,0x01,0x1A,0x08,0x1F,0xE2,0x08,0x01,0x02,
	0x08,0x03,0x82,0x08,0x03,0x62,0x08,0x07,0x32,0x08,0x05,0x1A,0x08,0x09,0x02,0x08,
	0x11,0x02,0x08,0x21,0x03,0xF8,0x41,0x02,0x08,0x01,0x02,0x08,0x01,0x00,0x00,0x01,
	0x80,0x00,0x01,0x00,0x00,0x00,0x00,0x00,
	/*--  文字:  字  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x08,0x00,0x08,0x08,0x18,0x0F,
	0xF7,0xF8,0x18,0x00,0x10,0x30,0x00,0x20,0x00,0x00,0xC0,0x07,0xFF,0xE0,0x00,0x03,
	0x00,0x00,0x04,0x00,0x00,0x18,0x00,0x00,0x18,0x0C,0x7F,0xFF,0xFE,0x00,0x18,0x00,
	0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,
	0xF8,0x00,0x00,0x30,0x00,0x00,0x00,0x00,
	/*--  文字:  符  --*/
	0x00,0x00,0x00,0x00,0x01,0x00,0x03,0x01,0x80,0x06,0x01,0x00,0x04,0x12,0x0C,0x0B,
	0xE3,0xF0,0x18,0xC4,0x20,0x10,0x48,0x20,0x61,0x10,0x00,0x01,0x80,0x60,0x03,0x00,
	0x44,0x02,0x00,0x4E,0x04,0x7F,0xF0,0x0E,0x20,0x40,0x16,0x10,0x40,0x26,0x08,0x40,
	0x46,0x0C,0x40,0x06,0x08,0x40,0x06,0x00,0x40,0x06,0x00,0x40,0x06,0x00,0x40,0x06,
	0x07,0xC0,0x06,0x00,0xC0,0x00,0x00,0x00,
	/*--  文字:  －  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x1F,0xFF,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/*--  文字:  图  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x08,0x1F,0xFF,0xF8,0x18,0x60,0x08,0x18,
	0x41,0x08,0x18,0xFF,0x88,0x18,0xC2,0x08,0x19,0x24,0x08,0x1A,0x28,0x08,0x18,0x10,
	0x08,0x18,0x2C,0x08,0x18,0x47,0x08,0x19,0xA1,0xE8,0x1E,0x1C,0x48,0x18,0x04,0x08,
	0x18,0x00,0x08,0x18,0x78,0x08,0x18,0x0E,0x08,0x18,0x02,0x08,0x18,0x00,0x08,0x1F,
	0xFF,0xF8,0x18,0x00,0x08,0x00,0x00,0x00,
	/*--  文字:  形  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x06,0x18,0x1F,0xF8,0x30,0x02,
	0x30,0x60,0x02,0x30,0x40,0x02,0x30,0x80,0x02,0x33,0x00,0x02,0x34,0x08,0x02,0x36,
	0x18,0x1F,0xF8,0x30,0x06,0x30,0x40,0x06,0x30,0x80,0x04,0x31,0x00,0x04,0x36,0x04,
	0x04,0x30,0x0C,0x0C,0x30,0x18,0x08,0x30,0x30,0x10,0x30,0x40,0x10,0x31,0x80,0x20,
	0x32,0x00,0x40,0x0C,0x00,0x00,0x00,0x00,
	/*--  文字:  包  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x01,0x80,0x00,0x03,0x00,0x00,0x03,
	0xFF,0xF0,0x06,0x00,0x20,0x04,0x00,0x20,0x08,0x00,0x20,0x17,0xFE,0x20,0x16,0x04,
	0x20,0x22,0x04,0x20,0x42,0x04,0x20,0x02,0x04,0x20,0x03,0xFC,0x20,0x02,0x04,0x20,
	0x02,0x03,0xE0,0x02,0x00,0xC8,0x02,0x00,0x04,0x02,0x00,0x04,0x02,0x00,0x0C,0x03,
	0xFF,0xFC,0x00,0x7F,0xF0,0x00,0x00,0x00,
	/*--  文字:  含  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x38,0x00,0x00,0x64,0x00,0x00,
	0x42,0x00,0x01,0x81,0x00,0x03,0x10,0xE0,0x06,0x08,0x3E,0x08,0x08,0x08,0x30,0x00,
	0x80,0x03,0xFF,0x80,0x00,0x01,0x00,0x00,0x02,0x00,0x00,0x06,0x00,0x02,0x00,0x40,
	0x03,0xFF,0xC0,0x02,0x00,0xC0,0x02,0x00,0xC0,0x02,0x00,0xC0,0x02,0x00,0xC0,0x03,
	0xFF,0xC0,0x02,0x00,0xC0,0x00,0x00,0x00,
	/*--  文字:  了  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xFF,0xF0,0x00,0x00,0x70,0x00,
	0x00,0x80,0x00,0x03,0x00,0x00,0x14,0x00,0x00,0x1C,0x00,0x00,0x18,0x00,0x00,0x18,
	0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,
	0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x01,0x18,0x00,0x00,
	0xF0,0x00,0x00,0x30,0x00,0x00,0x00,0x00,
	/*--  文字:  多  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x30,0x00,0x00,0x60,0x30,0x00,
	0xDF,0xF0,0x01,0x40,0x60,0x06,0x20,0xC0,0x18,0x33,0x00,0x00,0x26,0x00,0x00,0x18,
	0x00,0x00,0xEC,0x00,0x07,0x1C,0x00,0x38,0x30,0x08,0x00,0x6F,0xFC,0x00,0xC0,0x30,
	0x03,0x20,0x60,0x0C,0x30,0xC0,0x00,0x11,0x80,0x00,0x16,0x00,0x00,0x18,0x00,0x00,
	0x60,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,
	/*--  文字:  种  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0x03,0xC0,0x80,0x3E,0x00,0x80,0x02,
	0x00,0x80,0x02,0x00,0x80,0x02,0x5F,0xFC,0x7F,0xE8,0x88,0x02,0x08,0x88,0x06,0x08,
	0x88,0x07,0x08,0x88,0x0E,0xC8,0x8C,0x0A,0x4F,0xFC,0x12,0x58,0x8C,0x12,0x10,0x80,
	0x22,0x00,0x80,0x42,0x00,0x80,0x02,0x00,0x80,0x02,0x00,0x80,0x02,0x00,0x80,0x02,
	0x00,0x80,0x02,0x00,0x80,0x00,0x00,0x00,
	/*--  文字:  功  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x01,0x00,0x00,0x21,0x00,0x00,
	0x21,0x00,0x1F,0xC1,0x00,0x02,0x01,0x00,0x02,0x1F,0xFC,0x02,0x01,0x0C,0x02,0x01,
	0x0C,0x02,0x01,0x0C,0x02,0x01,0x08,0x02,0x03,0x08,0x02,0x02,0x08,0x02,0x32,0x08,
	0x03,0xC6,0x08,0x1C,0x04,0x08,0x30,0x08,0x08,0x00,0x18,0x08,0x00,0x20,0x18,0x00,
	0x41,0xF0,0x01,0x80,0x30,0x00,0x00,0x00,
	/*--  文字:  能  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x02,0x00,0x02,0x02,0x00,0x04,0x42,0x10,0x08,
	0x22,0x30,0x10,0x32,0xC0,0x3F,0xD3,0x00,0x20,0x12,0x08,0x20,0x02,0x04,0x1F,0xF3,
	0xFC,0x18,0x20,0x00,0x18,0x20,0x00,0x1F,0xE3,0x10,0x18,0x22,0x18,0x18,0x22,0x60,
	0x1F,0xE2,0x80,0x18,0x23,0x00,0x18,0x22,0x04,0x18,0x22,0x04,0x18,0x22,0x04,0x19,
	0xE3,0xFE,0x18,0x41,0xF8,0x00,0x00,0x00,
	/*--  文字:  。  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x0F,0x00,0x00,0x11,0x00,0x00,0x11,0x00,0x00,0x11,0x00,0x00,0x0F,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

uint8_t const tab5[] = {
	/*--  调入了一幅图像：F:\梁\画图\cock128128.bmp  --*/
	/*--  宽度x高度=128x128  --*/
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,
	0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x01,
	0x80,0x00,0x00,0x10,0xC0,0x40,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFE,0x00,0x00,0x01,
	0x80,0x00,0x00,0x31,0xC1,0xC0,0x1E,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xE0,0x00,0x01,
	0x80,0x00,0x00,0x61,0xC3,0xC0,0x3E,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xE0,0x00,0x01,
	0x80,0x00,0x00,0xC7,0xFF,0xC0,0x7E,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x01,
	0x80,0x00,0x01,0xC7,0xFF,0x80,0xFC,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x01,
	0x80,0x00,0x23,0xFF,0xFE,0x01,0xF0,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x01,
	0x80,0x00,0x23,0xFF,0xFE,0x03,0xF0,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x01,
	0x80,0x00,0x33,0xFF,0xFF,0xFF,0xE0,0x00,0x0F,0xFF,0xFC,0x00,0x3F,0xFF,0xC0,0x01,
	0x80,0x00,0x33,0xFF,0xFF,0xFF,0xC0,0x00,0x1F,0xFF,0xF8,0x00,0x1F,0xFF,0xC0,0x01,
	0x80,0x00,0x3F,0xFF,0xFF,0xFF,0x00,0x00,0x7F,0xFE,0x00,0x00,0x00,0xFF,0xE0,0x01,
	0x80,0x00,0x3F,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFE,0x00,0x00,0x00,0x7F,0xE0,0x01,
	0x80,0x00,0x3F,0xFF,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0xFF,0xF8,0x00,0x1F,0xF0,0x01,
	0x80,0x00,0x3F,0xFF,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0xFF,0xFC,0x00,0x0F,0xF0,0x01,
	0x80,0x00,0x3F,0xFF,0xFF,0xFC,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xE0,0x03,0xF0,0x01,
	0x80,0x00,0x3D,0xFF,0xFF,0xF8,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xF0,0x01,0xF0,0x01,
	0x80,0x00,0x7D,0xFF,0xFF,0xF0,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0xF8,0x01,
	0x80,0x00,0x7F,0xFF,0xFF,0xE0,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x78,0x01,
	0x80,0x00,0xFF,0xFF,0xFF,0xC0,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x3C,0x01,
	0x80,0x00,0xFF,0xFF,0xFF,0x80,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x3C,0x01,
	0x80,0x01,0xFF,0xFF,0xF8,0x00,0x00,0x7F,0xFF,0xFF,0xE0,0x00,0x3F,0xC0,0x3C,0x01,
	0x80,0x01,0xFF,0xFF,0xF0,0x00,0x00,0x7F,0xFF,0xFF,0xE0,0x00,0x1F,0xE0,0x3C,0x01,
	0x80,0x01,0xFF,0xFF,0xE0,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xC0,0x0F,0xF0,0x1C,0x01,
	0x80,0x01,0xFF,0xFF,0xE0,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xE0,0x07,0xF0,0x1C,0x01,
	0x80,0x01,0x8F,0xFF,0xF0,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFC,0x01,0xF8,0x0C,0x01,
	0x80,0x01,0x0F,0xFF,0xF0,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0xF8,0x0C,0x01,
	0x80,0x00,0x3F,0xFF,0xFC,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x7C,0x0C,0x01,
	0x80,0x00,0x3F,0xFF,0xFE,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x7E,0x0C,0x01,
	0x80,0x00,0xFF,0xFF,0xFE,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x3E,0x0C,0x01,
	0x80,0x00,0xFF,0xFF,0xFE,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x1E,0x0C,0x01,
	0x80,0x03,0xFF,0xFF,0xFF,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x0E,0x0C,0x01,
	0x80,0x03,0xFF,0xFF,0xFF,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x0F,0x0C,0x01,
	0x80,0x0F,0xFF,0xFF,0xFF,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x07,0x0C,0x01,
	0x80,0x0F,0xFF,0xFF,0xFF,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x07,0x0C,0x01,
	0x80,0x1F,0xFF,0xFF,0xFF,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x07,0x0C,0x01,
	0x80,0x1F,0xFF,0xFF,0xFF,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x07,0x0C,0x01,
	0x80,0x3F,0xFF,0xFF,0xFF,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x07,0x0C,0x01,
	0x80,0x3F,0xFF,0xFF,0xFF,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x07,0x0C,0x01,
	0x80,0x7F,0xFF,0xFF,0xFF,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0x0C,0x01,
	0x80,0x7F,0xFF,0xFF,0xFF,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0x0C,0x01,
	0x80,0xFF,0xFF,0xFF,0xFF,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0x86,0x0C,0x01,
	0x80,0xFF,0xFF,0xFF,0xFF,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0x86,0x0C,0x01,
	0x81,0xFF,0xFF,0xFF,0xFF,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xFE,0x7F,0x8E,0x0C,0x01,
	0x81,0xFF,0xFF,0xFF,0xFF,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xFE,0x7F,0x8C,0x08,0x01,
	0x81,0xFF,0xFF,0xFF,0xFF,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0x88,0x10,0x01,
	0x81,0xFF,0xFF,0xFF,0xFF,0x80,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0x88,0x10,0x01,
	0x81,0xFF,0xFF,0xFF,0xFF,0xE1,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDF,0x80,0x20,0x01,
	0x81,0xFF,0xFF,0xFF,0xFF,0xF3,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDF,0x80,0x20,0x01,
	0x81,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEF,0x80,0x00,0x01,
	0x81,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEF,0x80,0x00,0x01,
	0x81,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0xFF,0xE7,0x80,0x00,0x01,
	0x81,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0xFF,0xE7,0x80,0x00,0x01,
	0x81,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0xFF,0xE3,0x80,0x00,0x01,
	0x81,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0xFF,0xE3,0x80,0x00,0x01,
	0x81,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x7F,0xE3,0x80,0x00,0x01,
	0x81,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x7F,0xE3,0x80,0x00,0x01,
	0x81,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x3F,0xE3,0x00,0x00,0x01,
	0x81,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x3F,0xE3,0x00,0x00,0x01,
	0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x3F,0xE2,0x00,0x00,0x01,
	0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x3F,0xE2,0x00,0x00,0x01,
	0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF3,0xFC,0x1F,0xE2,0x00,0x00,0x01,
	0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF3,0xFC,0x1F,0xE2,0x00,0x00,0x01,
	0x80,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF3,0xFC,0x1F,0xC2,0x00,0x00,0x01,
	0x80,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF3,0xFC,0x1F,0xC2,0x00,0x00,0x01,
	0x80,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF3,0xFC,0x1F,0xCC,0x00,0x00,0x01,
	0x80,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF3,0xFC,0x1F,0x8C,0x00,0x00,0x01,
	0x80,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF1,0xF3,0xFC,0x1F,0x0C,0x00,0x00,0x01,
	0x80,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF1,0xF3,0xFC,0x1F,0x0C,0x00,0x00,0x01,
	0x80,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF1,0xF3,0xFC,0x1E,0x0C,0x00,0x00,0x01,
	0x80,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF1,0xF3,0xFC,0x1E,0x0C,0x00,0x00,0x01,
	0x80,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE1,0xF3,0xFC,0x1E,0x0C,0x00,0x00,0x01,
	0x80,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE1,0xF3,0xFC,0x1C,0x0C,0x00,0x00,0x01,
	0x80,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC1,0xF3,0xFC,0x1C,0x0C,0x00,0x00,0x01,
	0x80,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC1,0xF3,0xFC,0x18,0x0C,0x00,0x00,0x01,
	0x80,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xC3,0xF3,0xFC,0x18,0x0C,0x00,0x00,0x01,
	0x80,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0x87,0xF3,0xF8,0x10,0x0C,0x00,0x00,0x01,
	0x80,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0xF3,0xF0,0x00,0x0C,0x00,0x00,0x01,
	0x80,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0xF3,0xF0,0x00,0x0C,0x00,0x00,0x01,
	0x80,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFE,0x07,0xE7,0xE0,0x00,0x0C,0x00,0x00,0x01,
	0x80,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFE,0x07,0xE7,0xE0,0x00,0x08,0x00,0x00,0x01,
	0x80,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFE,0x07,0xEF,0xC0,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFE,0x07,0xCF,0x80,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0x32,0x0F,0x8F,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0x32,0x0F,0x0E,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xE2,0x0E,0x0C,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xE2,0x0C,0x08,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xE1,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xE1,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xFF,0x1F,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xFF,0x0F,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xFE,0x07,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xFE,0x03,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xFC,0x03,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xFC,0x01,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xF9,0x00,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xF9,0x00,0x7F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xF0,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xF0,0x00,0x01,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xE0,0x00,0x00,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xE0,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xC0,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xC0,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xC0,0x00,0x1E,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xC0,0x00,0x1E,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xE0,0x00,0x03,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xE0,0x00,0x03,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xE0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x01,0xE0,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x02,0xD0,0x00,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x02,0xD0,0x00,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x04,0x8E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x04,0x8E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x08,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x08,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x30,0xCD,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0x30,0xCD,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x00,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x0A,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x0A,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x03,0xF2,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x03,0xF2,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x80,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};


/*---------------延时子程序----------------*/
void ocmtest::delay(uint32_t us)
{
	while (us--);
}
void ocmtest::delay1(uint32_t ms)
{
	uint32_t i, j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 1000; j++)
			;
}
/*------------写命令或数据到LCD--------------*/
void ocmtest::wr_od(uint8_t dat, uint8_t comm)       //写一个数据和一个命令
{
	wr_data(dat);
	wr_comm(comm);
}
void ocmtest::wr_td(uint8_t datl, uint8_t dath, uint8_t comm)  //写两个数据和一个命令
{
	wr_data(datl);
	wr_data(dath);
	wr_comm(comm);
}
void ocmtest::wr_xd(uint32_t dat, uint8_t comm)       //写一个16进制数据和一个命令
{
	uint8_t datl, dath;
	datl = dat;
	dath = dat >> 8;
	wr_data(datl);
	wr_data(dath);
	wr_comm(comm);
}
void ocmtest::wr_auto(uint8_t dat)               //自动写数据
{
	chk_busy(1);
	cd = 0;
	rd = 1;
	this->writedata(dat);
	//data_ora = dat;
	wr = 0;
	wr = 1;
}

void ocmtest::wr_comm(uint8_t comm)       //写命令
{
	chk_busy(0);
	cd = 1;
	rd = 1;
	this->writedata(comm);
	//data_ora = comm;
	wr = 0;
	wr = 1;
}
void ocmtest::wr_data(uint8_t dat)       //写数据
{
	chk_busy(0);
	cd = 0;
	rd = 1;
	this->writedata(dat);
	//data_ora = dat;
	wr = 0;
	wr = 1;
}
void ocmtest::chk_busy(uint8_t autowr)    //测状态
{
	this->writedata(0xff);
	//data_ora = 0xff;
	cd = 1;
	wr = 1;
	rd = 0;
	if (autowr)
	{
		while (bf3 == 0)
			;
	}
	else
	{
		while ((bf0 == 0) || (bf1 == 0))
			;
	}
	rd = 1;
}
/*------------------初始化-----------------*/
void ocmtest::init_lcd(void)
{
	rst = 0;
	;
	rst = 1;
	fs = 0;
	ce = 0;
	wr = 1;
	rd = 1;
	wr_xd(addr_w, 0x40);                   //文本显示区首地址
	wr_xd(addr_t, 0x42);                   //图形显示区首地址
	wr_td(width, 0x00, 0x41);               //文本显示区宽度
	wr_td(width, 0x00, 0x43);               //图形显示区宽度
	wr_comm(0x81);                        //逻辑"异或"
	wr_td(0x02, 0x00, 0x22);                //CGRAM偏置地址设置
	wr_comm(0x9c);                        //启用文本显示,启用图形显示
}
/*--------------清RAM------------------*/
void ocmtest::clrram(void)
{
	uint8_t i, j;
	wr_xd(addr_w, 0x24);
	wr_comm(0xb0);
	for (j = 0; j < 144; j++)
	{
		for (i = 0; i < width; i++)
			wr_auto(0x00);
	}
	wr_comm(0xb2);
}
/*--------------显示点阵------------------*/
void ocmtest::disp_dz(uint8_t data1, uint8_t data2)
{
	uint8_t i, j;
	wr_xd(addr_t, 0x24);
	wr_comm(0xb0);
	for (j = 0; j < 32; j++)
	{
		for (i = 0; i < width * 2; i++)
			wr_auto(data1);
		for (i = 0; i < width * 2; i++)
			wr_auto(data2);
	}
	wr_comm(0xb2);
}
/*--------------在addr处显示8xl*yl的图形--------------*/
void ocmtest::disp_img(uint32_t addr, uint8_t xl, uint8_t yl, uint8_t const * img)
{
	uint8_t i, j;
	for (j = 0; j < yl; j++)
	{
		for (i = 0; i < xl; i++)
		{
			wr_xd(addr + j * width + i, 0x24);
			wr_od(img[j * xl + i], 0xc0);
		}
	}
}
/*----------在addr处显示row_yl行(每行row_xl个)8xl*yl的文字----------*/
void ocmtest::disp_chn(uint32_t addr, uint8_t xl, uint8_t yl, uint8_t row_xl, uint8_t row_yl, uint8_t const * chn)
{
	uint8_t i, j, k, m;
	for (m = 0; m < row_yl; m++)
	{
		for (k = 0; k < row_xl; k++)
		{
			for (j = 0; j < yl; j++)
			{
				for (i = 0; i < xl; i++)
				{
					wr_xd(addr + m * yl * width + k * xl + j * width + i, 0x24);
					wr_od(chn[(m * row_xl * xl * yl) + (k * xl * yl) + (j * xl) + i], 0xc0);
				}
			}
		}
	}
}
/*--------------显示字符------------------*/
void ocmtest::disp_eng(uint8_t const * eng)
{
	uint8_t i, j;
	wr_xd(addr_w, 0x24);
	wr_comm(0xb0);
	for (j = 0; j < 9; j++)
	{
		for (i = 0; i < width; i++)
			wr_auto(eng[j * width + i]);
	}
	wr_comm(0xb2);
}
/*------------------主程序--------------------*/
void ocmtest::lcdtest()
{
	//SP = 0x5f;
	init_lcd();
	while (1)
	{
		clrram();
		disp_eng(tab11);
		disp_chn(0x0c35, 2, 16, 10, 2, tab12);
		delay1(800);
		clrram();
		disp_dz(0xcc, 0xcc);
		delay1(800);
		clrram();
		disp_chn(0x025b, 3, 24, 8, 5, tab3);
		delay1(800);
		clrram();
		disp_dz(0xcc, 0x33);
		delay1(800);
		clrram();
		disp_img(0x01e7, 16, 128, tab5);
		delay1(800);
	}
}
