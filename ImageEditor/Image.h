#pragma once
#include "Matrix.hpp"
#include <fstream>
#include <Windows.h>
#include <string>
#include <qimage.h>

class Image{
public:
	//���캯��
	Image();
    Image(int h, int w, RGBA val=RGBA(0,0,0));
	Image(BITMAPFILEHEADER bf, BITMAPINFOHEADER bi, int h,int w);
    Image(BITMAPFILEHEADER bf, BITMAPINFOHEADER bi,const Matrix<RGBA>& b);
    Image(const Image& b);
    ~Image();

    int getH()const;
    int getW()const;
    //����ת��
    QImage ToQImage();


    static bool FileOpen(std::string filePath,std::fstream& bmp);
	static void Swap(Image &a,Image &b);
    static Image& ReadBmp(std::fstream& bmp);
    void WriteBmp(std::string filePath);

	//��ͼ����
    void Flip(int mode=0);
    void Crop(int lx,int ly,int rx,int ry);
	void Transpose();
	void Rotate(int degree);
    void ColorReversal();//��ɫ��ת
    void ColorChange(QColor a,QColor b,int r);//��ɫת��
    void Binarization(int threshold);
    void Resize(int h,int w);
    void Scaled(double rate);
    void Mosaic(int r);

    //��ֵ����
    void MeanFiltering(int r=3);//��ֵ�˲�
    void MedianFiltering(int r=3);//��ֵ�˲�
    void GaussFiltering();
    void LaplaceSharpen();

    //���
    void Gray();//�ҶȻ�
    void HandStyle();//�ֻ��
    void RetroStyle();//���ŷ�
    void BlueStyle();
    //void Reshape(int h,int w);��ʱ����

    //���������
    Image& operator=(const Image& b);
	


protected:
	BITMAPFILEHEADER Bf;
	BITMAPINFOHEADER Bi;
    Matrix<RGBA>*m;
};
