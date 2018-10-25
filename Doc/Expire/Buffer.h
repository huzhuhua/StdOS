#ifndef _Buffer_H_
#define _Buffer_H_

#include "Type.h"

/*
���������ԭ��
1���̶���������װ Buffer	=> Object

��ֵ�����ԭ��
1����ֵ���㿽�����Ⱥ����ݣ���������Ϊ����һ��������
2����ֵ���ȴ��ڵ�����ֵʱ����ֵ���ȱ�С
3����ֵ����С����ֵʱ����ֵ����SetLength����
4���������ʧ�ܣ����԰����ʧ�ܣ����а���ֵ���ȱ��ֲ���
*/

// �ڴ�����������װָ��ͳ���
// �ο�C#��Byte[]����Ҫʵ�ֶ�����ָ����໥ת������ֵ�����������á���ȡ���ȽϵȲ�����
// �ڲ�ָ��ָ����ڴ�ͳ��ȣ������ⲿ���룬�ڲ������Զ����䡣
// ���еĽ���������������ڲ�ָ�����󳤶ȣ������Զ����ݣ���������̳���չSetLength��
// ������ԭ���Ǿ�����Ϊ���ж��ٿ��ÿռ�Ϳ������ٳ��ȡ�
class Buffer 
{
public:
	// ���һ��ָ��ͳ���ָ����������
	Buffer(void* ptr, int len);
	// ���ÿ������캯��
	Buffer(const Buffer& buf) = delete;
	// ����mov������ָ��ͳ��ȹ��ң���նԷ�
	Buffer(Buffer&& rval);

	// ����һ�����󿽱����ݺͳ��ȣ����Ȳ���������ʧ��ʱ����
	Buffer& operator = (const Buffer& rhs);
	// ��ָ�뿽����ʹ���ҵĳ���
	Buffer& operator = (const void* ptr);
	// ����mov������ָ��ͳ��ȹ��ң���նԷ�
	Buffer& operator = (Buffer&& rval);

	// �ó�ָ�빩�ⲿʹ��
	inline uint8_t* GetBuffer()				{ return (uint8_t*)_Arr; }
	inline const uint8_t* GetBuffer() const	{ return (uint8_t*)_Arr; }
	inline int Length() const				{ return _Length; }

	// �������鳤�ȡ�ֻ����С�����������������չ��ʵ���Զ�����
	virtual bool SetLength(int len);
	//virtual void SetBuffer(void* ptr, int len);

	// ����ָ��λ�õ�ֵ�����Ȳ���ʱ�Զ�����
	bool SetAt(int index, uint8_t value);

    // �������������[]������ָ��Ԫ�صĵ�һ���ֽ�
    uint8_t operator[](int i) const;
	// ֧�� buf[i] = 0x36 ���﷨
    uint8_t& operator[](int i);

	// ԭʼ���������㣬�����߽�
	static void Copy(void* dest, const void* src, int len);
	static void Zero(void* dest, int len);

	// �������ݣ�Ĭ��-1���ȱ�ʾ��ǰ����
	virtual int Copy(int destIndex, const void* src, int len);
	// �����ݸ��Ƶ�Ŀ�껺������Ĭ��-1���ȱ�ʾ��ǰ����
	virtual int CopyTo(int srcIndex, void* dest, int len) const;
	// �������ݣ�Ĭ��-1���ȱ�ʾ������С����
	virtual int Copy(int destIndex, const Buffer& src, int srcIndex, int len);
	// ����һ�����󿽱����ݺͳ��ȣ����Ȳ���������ʧ��ʱ����
	int Copy(const Buffer& src, int destIndex = 0);

	// ��ָ���ֽ����ó�ʼ��һ������
	int Set(uint8_t item, int index, int len);
	void Clear(uint8_t item = 0);

	// ��ȡһ���ӻ�������Ĭ��-1���ȱ�ʾʣ��ȫ��
	//### �����߼����Կ����޸�Ϊ����len�����ڲ�����ʱ��ֱ�����ڲ����ȶ�������������Ӧ�ò���ȥ�Ƚϳ��ȵĆ���
	Buffer Sub(int index, int len);
	const Buffer Sub(int index, int len) const;

	uint16_t	ToUInt16(int index = 0) const;
	uint32_t	ToUInt32(int index = 0) const;
	uint64_t	ToUInt64(int index = 0) const;
	void Write(uint16_t value, int index = 0);
	void Write(short value, int index = 0);
	void Write(uint32_t value, int index = 0);
	void Write(int value, int index = 0);
	void Write(uint64_t value, int index = 0);

    explicit operator bool() const { return _Length > 0; }
    bool operator !() const { return _Length == 0; }
	int CompareTo(const Buffer& bs) const;
	int CompareTo(const void* ptr, int len = -1) const;
	friend bool operator == (const Buffer& bs1, const Buffer& bs2);
	friend bool operator == (const Buffer& bs1, const void* ptr);
	friend bool operator != (const Buffer& bs1, const Buffer& bs2);
	friend bool operator != (const Buffer& bs1, const void* ptr);

protected:
    char*	_Arr;		// ����ָ�� string(4) string(2)
	int		_Length;	// ���� string(8)
	int		_LengthOrigin;//ԭʼ���ȣ��������ó��Ȳ�����ԭʼ����

	void move(Buffer& rval);
public:
	virtual void Show(bool newLine = false) const;
	virtual void ShowHex(bool newLine = false, char sep = 0X20) const;
};

#endif