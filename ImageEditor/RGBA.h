#pragma once
#include<iostream>
#include <math.h>
typedef unsigned char BYTE;
class RGBA {
public:
	//���캯��
	RGBA(BYTE val=0);//�޲ι������� -- ʵ��BYTE�Զ�����ת����RGBA
    RGBA(BYTE b, BYTE g, BYTE r, BYTE a = 255);
	RGBA(const RGBA& b);

	RGBA Set(const BYTE& val);//��������Ϊ��ֵ

	static void Swap(RGBA& a, RGBA& b);
    static RGBA White();
    static RGBA Black();
    void Gray();//�ҶȻ�

	//���������
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
