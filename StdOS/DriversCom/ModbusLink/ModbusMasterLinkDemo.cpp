#include "ModbusLink.h"
#include "Sys.h"
#include "BspPlatform/Interrupt.h"
#include "Port.h"
#include "Task.h"

#ifndef _MODBUSMASTERLINKEST_CPP
#define _MODBUSMASTERLINKEST_CPP

USART usart222(USART2, 9600);
OutputPort u485dr;
ModbusMasterLink modMaster(usart222);
void ModbusMasterLinkTestInit()
{
	u485dr.Set(PC2);
	u485dr.OpenDrain = false;
	u485dr.Invert = false;
	u485dr.Open();
	u485dr = 0;//�������ģʽ
	usart222.RS485 = &u485dr;
}

#endif // !_MODBUSMASTERLINKEST_CPP