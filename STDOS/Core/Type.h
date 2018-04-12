#ifndef __Type_H__
#define __Type_H__
#include <stdint.h>
/* 类型定义 */
typedef const char*		cstring;

#define UInt64_Max 0xFFFFFFFFFFFFFFFFull

// 逐步使用char替代byte，在返回类型中使用char*替代void*
// 因为格式化输出时，可以用%c输出char，用%s输出char*

class String;

// 根对象
class Object
{
public:
	// 输出对象的字符串表示方式
	virtual String& ToStr(String& str) const;
	// 输出对象的字符串表示方式。支持RVO优化
	virtual String ToString() const;
	// 显示对象。默认显示ToString
	virtual void Show(bool newLine = false) const;

	//const Type GetType() const;
};

// 数组长度
#define ArrayLength(arr) (int)(sizeof(arr)/sizeof(arr[0]))

int StdPrintf(const char *format, ...); //代替 printf

// 弱函数
#if defined(_MSC_VER)
#define WEAK
#else
#define	WEAK	__attribute__((weak))
#endif

#endif
