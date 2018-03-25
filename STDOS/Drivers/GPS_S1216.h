#ifndef _GPS_H
#define _GPS_H

#include "Buffer.h"
//GPS NMEA-0183Э����Ҫ�����ṹ�嶨�� 
//������Ϣ
__packed typedef struct
{
	byte num;		//���Ǳ��
	byte eledeg;	//��������
	uint16_t azideg;	//���Ƿ�λ��
	byte sn;		//�����		   
}nmea_slmsg;
//���� NMEA-0183Э����Ҫ�����ṹ�嶨�� 
//������Ϣ
__packed typedef struct
{
	byte beidou_num;		//���Ǳ��
	byte beidou_eledeg;	//��������
	uint16_t beidou_azideg;	//���Ƿ�λ��
	byte beidou_sn;		//�����		   
}beidou_nmea_slmsg;

//UTCʱ����Ϣ
__packed typedef struct
{
	uint16_t year;	//���
	byte month;	//�·�
	byte date;	//����
	byte hour; 	//Сʱ
	byte min; 	//����
	byte sec; 	//����
}nmea_utc_time;
//NMEA 0183 Э����������ݴ�Žṹ��
__packed typedef struct
{
	byte svnum;					//�ɼ�GPS������
	byte beidou_svnum;					//�ɼ�GPS������
	nmea_slmsg slmsg[12];		//���12��GPS����
	beidou_nmea_slmsg beidou_slmsg[12];		//���������12�ű�������
	nmea_utc_time utc;			//UTCʱ��
	uint32_t latitude;				//γ�� ������100000��,ʵ��Ҫ����100000
	byte nshemi;					//��γ/��γ,N:��γ;S:��γ				  
	uint32_t longitude;			    //���� ������100000��,ʵ��Ҫ����100000
	byte ewhemi;					//����/����,E:����;W:����
	byte gpssta;					//GPS״̬:0,δ��λ;1,�ǲ�ֶ�λ;2,��ֶ�λ;6,���ڹ���.				  
	byte posslnum;				//���ڶ�λ��GPS������,0~12.
	byte possl[12];				//���ڶ�λ�����Ǳ��
	byte fixmode;					//��λ����:1,û�ж�λ;2,2D��λ;3,3D��λ
	uint16_t pdop;					//λ�þ������� 0~500,��Ӧʵ��ֵ0~50.0
	uint16_t hdop;					//ˮƽ�������� 0~500,��Ӧʵ��ֵ0~50.0
	uint16_t vdop;					//��ֱ�������� 0~500,��Ӧʵ��ֵ0~50.0 

	int altitude;			 	//���θ߶�,�Ŵ���10��,ʵ�ʳ���10.��λ:0.1m	 
	uint16_t speed;					//��������,�Ŵ���1000��,ʵ�ʳ���10.��λ:0.001����/Сʱ	 
}nmea_msg;
////////////////////////////////////////////////////////////////////////////////////////////////////
//SkyTra S1216F8 ���ò����ʽṹ��
__packed typedef struct
{
	uint16_t sos;            //�������У��̶�Ϊ0XA0A1
	uint16_t PL;             //��Ч���ݳ���0X0004�� 
	byte id;             //ID���̶�Ϊ0X05
	byte com_port;       //COM�ڣ��̶�Ϊ0X00����COM1   
	byte Baud_id;       //�����ʣ�0~8,4800,9600,19200,38400,57600,115200,230400,460800,921600��
	byte Attributes;     //�������ݱ���λ�� ,0���浽SRAM��1���浽SRAM&FLASH��2��ʱ����
	byte CS;             //У��ֵ
	uint16_t end;            //������:0X0D0A  
}SkyTra_baudrate;

//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 ����λ�ø����ʽṹ��
__packed typedef struct
{
	uint16_t sos;            //�������У��̶�Ϊ0XA0A1
	uint16_t PL;             //��Ч���ݳ���0X0003�� 
	byte id;             //ID���̶�Ϊ0X0E
	byte rate;           //ȡֵ��Χ:1, 2, 4, 5, 8, 10, 20, 25, 40, 50
	byte Attributes;     //�������ݱ���λ�� ,0���浽SRAM��1���浽SRAM&FLASH��2��ʱ����
	byte CS;             //У��ֵ
	uint16_t end;            //������:0X0D0A  
}SkyTra_PosRate;
//////////////////////////////////////////////////////////////////////////////////////////////////// 	
//SkyTra S1216F8 �����������(PPS)���Ƚṹ��
__packed typedef struct
{
	uint16_t sos;            //�������У��̶�Ϊ0XA0A1
	uint16_t PL;             //��Ч���ݳ���0X0007�� 
	byte id;             //ID���̶�Ϊ0X65
	byte Sub_ID;         //0X01
	uint32_t width;        //1~100000(us)
	byte Attributes;     //�������ݱ���λ�� ,0���浽SRAM��1���浽SRAM&FLASH��2��ʱ����
	byte CS;             //У��ֵ
	uint16_t end;            //������:0X0D0A 
}SkyTra_pps_width;

class GPS_S1216
{
public:
	GPS_S1216();
	void Analysis(byte *buf);
	void Show();
	nmea_msg gpsx;	//GPS��Ϣ
private:
	byte NMEA_Comma_Pos(byte *buf, byte cx);
	uint32_t NMEA_Pow(byte m, byte n);
	int NMEA_Str2num(byte *buf, byte*dx);
	byte SkyTra_Cfg_Ack_Check();
	void NMEA_GPGSV_Analysis(byte *buf);
	void NMEA_BDGSV_Analysis(byte *buf);
	void NMEA_GNGGA_Analysis(byte *buf);
	void NMEA_GNGSA_Analysis(byte *buf);
	void NMEA_GNRMC_Analysis(byte *buf);
	void NMEA_GNVTG_Analysis(byte *buf);
public:
	byte SkyTra_Cfg_Prt(uint32_t baud_id, byte* buf, int& len);
	byte SkyTra_Cfg_Tp(uint32_t width, byte* buf, int& len);
	byte SkyTra_Cfg_Rate(byte Frep, byte* buf, int& len);
};

#endif // !_GPS_H