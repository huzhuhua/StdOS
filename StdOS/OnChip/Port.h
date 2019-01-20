#ifndef _Port_H_
#define _Port_H_

#include "Core\Type.h"
#include "Platform\Pin.h"
#include "port/std_port_gpio.h"

// 端口基类
class Port:public mcuGpio
{
public:	
    bool Open();
};
// 输出口
class OutputPort : public Port
{
public:
    bool OpenDrain	= false;	// 是否开漏输出

    OutputPort();
    OutputPort(Pin pin, uint8_t invert, bool openDrain = false);
	
    void Write(bool value) ;

    static void Write(Pin pin, bool value);

    OutputPort& operator=(bool value) { Write(value); return *this; }
    OutputPort& operator=(OutputPort& port) { Write(port.Read()); return *this; }
    operator bool()  { return Read(); }

protected:
    virtual void OnOpen(void* param);
	virtual void OpenPin(void* param);
};

#endif //_Port_H_
