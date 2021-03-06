#ifndef _PWMSOLO_H
    #define _PWMSOLO_H
    #include "Type.h"
    #include "Port.h"

    class PwmSolo: public Port
    {
        public:
            PwmSolo(uint frq = 10000, uint duty = 50,bool invert=false);
			virtual Port& SetPin(Pin pin);	// 设置引脚
			void Open();
            void SetOut(int pwmValue); //设置输出值
            void SetOutPercent(float per); //设置输出比例，0-100		
		protected:
			void OnOpen2();//初始化
        private:
            uint _freq; //频率
            uint _duty; //输出值
			bool _invert;
    };
#endif
