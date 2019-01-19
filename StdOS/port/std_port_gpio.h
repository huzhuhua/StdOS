#ifndef _STD_PORT_GPIO_H
#define _STD_PORT_GPIO_H

#include "std_gpio.h"
#include "stm32f10x.h"

class mcuGpio : public Gpio
{
public:
	mcuGpio() {}
	mcuGpio(GPIO_TypeDef *port, uint16_t pin);
	virtual void SetPin(Pin pin);//设置管脚
	virtual void mode(PIN_MODE mode);
	virtual void mode(PIN_MODE mode, uint8_t af_configration);
	virtual void set();
	virtual void reset();
	virtual void write(uint8_t val);
	virtual void toggle();
	virtual void read(uint8_t *val);
	virtual uint8_t read(void);

	operator int();
	int operator =(mcuGpio &);
	mcuGpio operator = (int value);

public:
	GPIO_TypeDef *_port; /**< 引脚的端口 */
	uint16_t      _pin; /**< 引脚的序号 */
};


#endif // !_STD_PORT_GPIO_H
