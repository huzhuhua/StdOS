#ifndef _STD_PORT_GPIO_H
#define _STD_PORT_GPIO_H

#include "Core/Port/Gpio.h"
#include "stm32f10x.h"

class mcuGpio : public Gpio
{
public:
	mcuGpio();
	mcuGpio(GPIO_TypeDef *port, uint16_t pin);
	virtual void SetPin(Pin pin);//���ùܽ�
	virtual void mode(PIN_MODE mode);//���ùܽ�ģʽ
	virtual void mode(PIN_MODE mode, uint8_t af_configration);//���ùܽ�ģʽ������
	virtual void set();//��λ
	virtual void reset();//��λ
	virtual void write(uint8_t val);//дֵ
	virtual void toggle();//�˿ڷ�ת
	virtual void read(uint8_t *val);//��ȡ�˿�
	virtual uint8_t read(void);//��

	operator int();
	int operator =(mcuGpio &);
	/*mcuGpio operator = (int value);*/
	// Read/ReadInput ���������ڣ�ǰ�߶�������߶����룬�ڿ�©�����ʱ���кܴ�����
	virtual bool Read();
	//virtual uint8_t ReadInput()=0;
	void Write(bool value);

	static void Write(Pin pin, bool value);
	operator bool() { return Read(); }
	bool Open();
	mcuGpio(Pin pin, uint8_t invert, bool openDrain = false);
	virtual mcuGpio& operator=(bool value) { Write(value); return *this; }
	//virtual mcuGpio& operator=(mcuGpio& port) { Write(port.Read()); return *this; }
public:
	GPIO_TypeDef *_port; /**< ���ŵĶ˿� */
	uint16_t      _pin; /**< ���ŵ���� */	
};

#endif // !_STD_PORT_GPIO_H