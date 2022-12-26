#pragma once
#include "Matrix.hpp"
#include <fstream>
#include <Windows.h>
#include <string>
#include <qimage.h>

class Image{
public:
	//构造函数
	Image();
    Image(int h, int w, RGBA val=RGBA(0,0,0));
	Image(BITMAPFILEHEADER bf, BITMAPINFOHEADER bi, int h,int w);
    Image(BITMAPFILEHEADER bf, BITMAPINFOHEADER bi,const Matrix<RGBA>& b);
    Image(const Image& b);
    ~Image();

    int getH()const;
    int getW()const;
    //类型转换
    QImage ToQImage();


    static bool FileOpen(std::string filePath,std::fstream& bmp);
	static void Swap(Image &a,Image &b);
    static Image& ReadBmp(std::fstream& bmp);
    void WriteBmp(std::string filePath);

	//简单图像处理
    void Flip(int mode=0);
    void Crop(int lx,int ly,int rx,int ry);
	void Transpose();
	void Rotate(int degree);
    void ColorReversal();//颜色反转
    void ColorChange(QColor a,QColor b,int r);//颜色转换
    void Binarization(int threshold);
    void Resize(int h,int w);
    void Scaled(double rate);
    void Mosaic(int r);

    //数值处理
    void MeanFiltering(int r=3);//均值滤波
    void MedianFiltering(int r=3);//中值滤波
    void GaussFiltering();
    void LaplaceSharpen();

    //风格
    void Gray();//灰度化
    void HandStyle();//手绘风
    void RetroStyle();//复古风
    void BlueStyle();
    //void Reshape(int h,int w);暂时弃用

    //运算符重载
    Image& operator=(const Image& b);
	


protected:
	BITMAPFILEHEADER Bf;
	BITMAPINFOHEADER Bi;
    Matrix<RGBA>*m;
};
