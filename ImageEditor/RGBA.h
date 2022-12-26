#pragma once
#include<iostream>
#include <math.h>
typedef unsigned char BYTE;
class RGBA {
public:
	//构造函数
	RGBA(BYTE val=0);//无参构造像素 -- 实现BYTE自动类型转换成RGBA
    RGBA(BYTE b, BYTE g, BYTE r, BYTE a = 255);
	RGBA(const RGBA& b);

	RGBA Set(const BYTE& val);//设置像素为定值

	static void Swap(RGBA& a, RGBA& b);
    static RGBA White();
    static RGBA Black();
    void Gray();//灰度化

	//运算符重载
	RGBA operator+(const RGBA& b);
	RGBA operator-(const RGBA& b);
	RGBA operator*(const RGBA& b);
	RGBA operator/(const RGBA& b);
	RGBA&operator=(const RGBA& b);
	RGBA& operator+=(const RGBA& b);
	RGBA& operator-=(const RGBA& b);
	bool operator==(const RGBA& b);
	bool operator!=(const RGBA& b);
	bool operator>(const RGBA& b);
	bool operator<(const RGBA& b);
    friend std::ostream& operator<<(std::ostream& out, const RGBA& a);


    BYTE B;
	BYTE G;
    BYTE R;
	BYTE A;
};
std::ostream& operator<<(std::ostream&out,const RGBA& a);
