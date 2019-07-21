#ifndef _Port_H_
#define _Port_H_

#include "Kernel\Sys.h"

/******** 端口打开关闭流程 ********/
/*
Port::Open
	#Port::Opening
		OutputPort::OnOpen
			#Port::OnOpen
		#OutputPort::OpenPin

Port::Close
	#Port::OnClose
*/


/******************************** Port ********************************/

// 端口基类
// 用于管理一个端口，通过PinBit标识该组的哪些引脚。
// 子类初始化时先通过SetPort设置端口，备份引脚状态，然后Config通过gpio结构体配置端口，端口销毁时恢复引脚状态
typedef enum
{
	GPIO_AIN = 0x0,//模拟输入模式
	GPIO_IN_FLOATING = 0x04,//浮空输入模式
	GPIO_IPD = 0x28,//下拉输入模式
	GPIO_IPU = 0x48, //上拉输入模式
	GPIO_Out_OD = 0x14,//开漏输出模式
	GPIO_Out_PP = 0x10,//通用推挽输出模式
	GPIO_AF_OD = 0x1C,//复用功能开漏输出
	GPIO_AF_PP = 0x18//复用功能推挽输出
}GPIOMode_T;

// 端口类
class Port
{
public:
	Port();
	virtual Port& SetPin(Pin pin);	// 设置引脚
	void pinMode(GPIOMode_T mode);//设置管脚模式

	bool ReadInput() const;// Read/ReadInput 的区别在于，前者读输出后者读输入，在开漏输出的时候有很大区别
	void Write(bool value) const;
	Port& operator=(bool value) { Write(value); return *this; }
	Port& operator=(Port& port) { Write(port.Read()); return *this; }
	
	bool Empty() const;

	void Close();

	virtual bool Read() const;
	operator bool() const { return Read(); }
public:
	int Invert;//是否反向
protected:
	Pin		_Pin;		// 引脚  	
private:
	bool	Opened;		// 是否已经打开
};

#endif //_Port_H_
