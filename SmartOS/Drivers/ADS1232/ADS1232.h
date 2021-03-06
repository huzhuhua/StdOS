#ifndef _ADS1232_H
#define _ADS1232_H
    #include "Device\Port.h"

    class ADS1232
    {
        public:
            enum Speed  //AD转换速度
            {
                    SPEED10,  //10hZ
                    SPEED80  //80Hz
            };
            enum Gain  //放大倍数
            {
                    GAIN1, GAIN2, GAIN64, GAIN128
            };
			uint Status;//状态
			int CalCnt;//校准次数
			int readCnt; //读取数量
        public:
            void Init();
            void SetPin(Pin pdout, Pin psclk, Pin ppwdn);
            void SetPinSpeed(Pin pin);
            void SetPinGain(Pin gain0, Pin gain1);
            void SetSpeed(Speed speed = SPEED10);
            void SetGain(Gain gain = GAIN1);
            int Read(bool cal=false);
        private:
            Port dout;
            Port sclk;
            Port pwdn;
            Port pSpeed;
            Port pGain0;
            Port pGain1;
        private:            
            int oldval;
    };

#endif
