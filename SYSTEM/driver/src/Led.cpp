#include "Led.h"

CLed::CLed(uint16_t pin, uint16_t period1, uint16_t onTime1):CPort(pin)
{    
    this->SetModeOut_PP();
    this->period = period1;
    this->onTime = onTime1;
}

CLed::~CLed(){

}
uint8_t CLed::Init(void)
{
    return 0;
}

void CLed::On(void)
{
    this->Reset();
}

void CLed::Off(void)
{
    this->Set();	
}
