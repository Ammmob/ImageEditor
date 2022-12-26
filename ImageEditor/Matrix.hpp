#pragma once
#include "RGBA.h"
#include <vector>
#include <iomanip>
#include <random>
#include <time.h>
#include <vector>
#include <math.h>
#include <QDebug>
template<class T>
class Matrix {
public:
    //构造函数
    Matrix();
    Matrix(int h, int w, T val=0);
    Matrix(int h, int w, std::vector<std::vector<T>>&b);
    Matrix(const Matrix<T>& m);
    ~Matrix();

    //获取信息
    int getHeight()const;
    int getWidth()const;

    Matrix<RGBA>& Double2RGBA();
    Matrix<double>& RGBA2Double();

    //判断信息
    bool IsEmpty();//判断是否是空矩阵
    bool IsSquare();//判断矩阵是否为方阵
    //bool IsSingular();//判断是否是奇异矩阵 Todo
    bool IsSimilar(const Matrix<T>& b);//判断两矩阵是否可以进行加减运算
    bool IsMulAble(const Matrix<T>& b);//判断两矩阵是否可以进行乘法运算
    //bool IsdivAble(const Matrix<T>& b);//判断两矩阵是否可以进行除法运算 Todo



    //生成特殊矩阵
    static Matrix<double>& Zeros(int h, int w=0);//生成零矩阵，单参数为方阵
    static Matrix<double>& Ones(int h,  int w=0);//生成一矩阵，单参数为方阵 ---
    static Matrix<double>& Random(int h, int w=0); //生成随机矩阵，单参数为方阵
    static Matrix<double>& Identity(int n);//生成单位矩阵，单参数为方阵 ---- 应该如何赋值？？？

    static void Swap(Matrix<T>& a, Matrix<T>& b);//交换两矩阵

    //加工型操作
    void Transpose();
    void Set(int row, int col, const T& val);//设置单位元素为定值
    void Set(int lx, int ly, int rx, int ry,const T& val);//设置区域值为一定值
    void Clip(double minv, double maxv);



    //void Clip(RGBA minv, RGBA maxv);
    //void Reshape(int h, int w); 暂时弃用

    //数值处理
    T Max(int lx, int ly, int rx, int ry);
    T Min(int lx, int ly, int rx, int ry);
    T Mean(int lx, int ly, int rx, int ry);
    T Median(int lx, int ly, int rx, int ry);
    T StandardDeviation(int lx, int ly, int rx, int ry);
    Matrix<T>& GradientX();//前向-中心差分
    Matrix<T>& GradientY();//前向-中心差分
    Matrix<double> PowDouble(int n);
    Matrix<double> Sqrt();

    //运算符重载
    Matrix<T> operator+(const Matrix<T>& b);
    Matrix<T> operator-(const Matrix<T>& b);
    Matrix<T> operator*(const Matrix<T>& b);//矩阵点乘
    Matrix<T> operator/(const Matrix<T>& b);//矩阵点乘
    Matrix<T> operator+(const T& b);
    Matrix<T> operator-(const T& b);
    Matrix<T> operator*(const T& b);
    Matrix<T> operator/(const T& b);

    //调试输出
    void DateDebug() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                qDebug()<<data[i][j]<<" ";
            }
        }
    }

    void DatePrint() {
        std::cout << height << " " << width << std::endl;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                std::cout<<std::setw(3)<<data[i][j]<<" ";
                std::cout << ",";
            }
            std::cout << std::endl;
        }
    }
    std::vector <std::vector<T>> data;
protected:
    int height;
    int width;

};


//类模板实现

//构造函数
template<class T>
Matrix<T>::Matrix() :height(0), width(0) {}


template<class T>
Matrix<T>::Matrix(int h, int w, T val):height(h),width(w) {
    data = std::vector<std::vector<T>>(h, std::vector<T>(w, val));
}

template<class T>
Matrix<T>::Matrix(int h, int w, std::vector<std::vector<T>>& b) :height(h), width(w), data(b) {}

template<class T>
Matrix<T>::Matrix(const Matrix<T>& m) :  data(m.data), height(m.height), width(m.width) {}

template<class T>
Matrix<T>::~Matrix() {}

template<class T>
int Matrix<T>::getHeight() const {
    return height;
}

template<class T>
int Matrix<T>::getWidth() const {
    return width;
}

template<class T>
Matrix<RGBA>& Matrix<T>::Double2RGBA() {
    Matrix<RGBA>* tmp =new  Matrix<RGBA>(getHeight(), getWidth());
    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            tmp->data[i][j] = RGBA((BYTE)data[i][j]);
        }
    }
    return *tmp;
}

template<class T>
Matrix<double>& Matrix<T>::RGBA2Double() {
    //mean double
    Matrix<double>* tmp = new Matrix<double>(getHeight(), getWidth());
    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            tmp->data[i][j] = (data[i][j].R + data[i][j].G + data[i][j].B) / 3.0;
        }
    }
    return *tmp;
}

template<class T>
//判断信息
bool Matrix<T>::IsEmpty() {
    return data.empty();
}

template<class T>
bool Matrix<T>::IsSquare() {
    return height == width;
}

//template<class T>
//bool Matrix<T>::IsSingular() {
//	//Todo
//	return false;
//}

template<class T>
bool Matrix<T>::IsSimilar(const Matrix<T>& b) {
    return (height == b.getHeight() && width == b.getWidth());
}

template<class T>
bool Matrix<T>::IsMulAble(const Matrix<T>& b) {
    return width == b.getHeight();
}

//template<class T>
//bool Matrix<T>::IsdivAble(const Matrix<T>& b) {
//	//Todo
//	return false;
//}


template<class T>
void Matrix<T>::Swap(Matrix<T>& a, Matrix<T>& b) {
    std::swap(a.height, b.height);
    std::swap(a.width, b.width);
    a.data.swap(b.data);
}

//生成特殊矩阵
template<class T>
Matrix<double>& Matrix<T>::Zeros(int h, int w) {
    if (w == 0)w = h;
    Matrix<double>* temp = new Matrix<double>(h, w, 0.0);
    return *temp;
}

template<class T>
Matrix<double>& Matrix<T>::Ones(int h, int w) {
    if (w == 0)w = h;
    Matrix<double>* temp = new Matrix<double>(h, w, 1.0);
    return *temp;
}

template<class T>
Matrix<double>& Matrix<T>::Random(int h, int w) {
    if (w == 0)w = h;
    Matrix<double>* temp = new Matrix<double>(h, w);
    srand((unsigned)time(NULL));
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            temp->data[i][j] = rand() % 1001 / 1000.0;
        }
    }
    return *temp;
}

template<class T>
Matrix<double>& Matrix<T>::Identity(int n) {
    Matrix<double>* tmp = new Matrix<double>(n, n);
    for (int i = 0; i < n; i++) {
        tmp->data[i][i] = 1.0;
    }
    return *tmp;
}

template<class T>
void Matrix<T>::Transpose() {
    Matrix<T> tmp = Matrix<T>(width, height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            tmp.data[j][i] = data[i][j];
        }
    }
    std::swap(height, width);
    data = tmp.data;
}



template<class T>
void Matrix<T>::Set(int row, int col, const T& val) {//检查边界
    if (row >= height || row < 0 || col >= width || col < 0) {
        //qDeubg()<<"Error:";
        std::cout << "Error:" << std::endl;
        return;
    }
    data[row][col] = val;
}

template<class T>
void Matrix<T>::Set(int lx, int ly, int rx, int ry, const T& val) {
    if (lx > rx)std::swap(lx, rx);
    if (ly > ry)std::swap(ly, ry);
    lx = std::max(0, lx);
    ly = std::max(0, ly);
    rx = std::min(getHeight(), rx);
    ry = std::min(getWidth(), ry);

    for (int i = lx; i < rx; i++) {
        for (int j = ly; j < ry; j++) {
            data[i][j] = val;
        }
    }
}

template<class T>
void Matrix<T>::Clip(double minv, double maxv){
    for (int i = 0; i < this->getHeight(); i++) {
        for (int j = 0; j < this->getWidth(); j++) {
            if (this->data[i][j] > maxv)this->data[i][j] = maxv;
            if (this->data[i][j] < minv)this->data[i][j] = minv;
        }
    }
}



//暂时弃用
// template<class T>
//void Matrix::Reshape(int h, int w){
//	if (h * w != height * width) {//抛出异常：矩阵维度不匹配
//		//qDebug()<<"Error:";
//		std::cout<<"Error:"<<std::endl;
//		return;
//	}
//	Matrix tmp = Matrix(h, w);
//	for (int i = 0; i < height * width; i++) {
//		tmp.data[i/w][i%w]=data[i/width][i%width];
//	}
//	height = h;
//	width = w;
//	data = tmp.data;
//}


template<class T>
T Matrix<T>::Max(int lx, int ly, int rx, int ry) {
    if (lx > rx)std::swap(lx, rx);
    if (ly > ry)std::swap(ly, ry);
    lx = std::max(0, lx);
    ly = std::max(0, ly);
    rx = std::min(getHeight(), rx);
    ry = std::min(getWidth(), ry);

    RGBA tmp(0, 0, 0, 0);
    for (int i = lx; i < rx; i++) {
        for (int j = ly; j < ry; j++) {
            if (data[i][j] > tmp)RGBA::Swap(data[i][j], tmp);
        }
    }
    return tmp;
}

template<class T>
T Matrix<T>::Min(int lx, int ly, int rx, int ry) {
    if (lx > rx)std::swap(lx, rx);
    if (ly > ry)std::swap(ly, ry);
    lx = std::max(0, lx);
    ly = std::max(0, ly);
    rx = std::min(getHeight(), rx);
    ry = std::min(getWidth(), ry);

    RGBA tmp(255, 255, 255, 255);
    for (int i = lx; i < rx; i++) {
        for (int j = ly; j < ry; j++) {
            if (data[i][j] < tmp)RGBA::Swap(data[i][j], tmp);
        }
    }
    return tmp;
}

template<class T>
inline T Matrix<T>::Mean(int lx, int ly, int rx, int ry) {
    if (lx > rx)std::swap(lx, rx);
    if (ly > ry)std::swap(ly, ry);
    lx = std::max(0, lx);
    ly = std::max(0, ly);
    rx = std::min(getHeight(), rx);
    ry = std::min(getWidth(), ry);

    int sum[4] = { 0 };
    for (int i = lx; i < rx; i++) {
        for (int j = ly; j < ry; j++) {
            sum[0]++;
            sum[1] += data[i][j].R;
            sum[2] += data[i][j].G;
            sum[3] += data[i][j].B;
        }
    }
    return RGBA(sum[3] / sum[0], sum[2] / sum[0], sum[1] / sum[0]);
}

template<class T>
inline T Matrix<T>::Median(int lx, int ly, int rx, int ry){
    if (lx > rx)std::swap(lx, rx);
    if (ly > ry)std::swap(ly, ry);
    lx = std::max(0, lx);
    ly = std::max(0, ly);
    rx = std::min(getHeight(), rx);
    ry = std::min(getWidth(), ry);

    std::vector<T>tmp;
    for (int i = lx; i < rx; i++) {
        for (int j = ly; j < ry; j++) {
            tmp.push_back(data[i][j]);
        }
    }
    std::sort(tmp.begin(), tmp.end());
    int s = tmp.size();
    if (s & 1) {
        return tmp[s / 2];
    }
    else {
        return (tmp[s / 2]/2 + tmp[s / 2 - 1]/2);
    }
}


template<class T>
T Matrix<T>::StandardDeviation(int lx, int ly, int rx, int ry) {
    if (lx > rx)std::swap(lx, rx);
    if (ly > ry)std::swap(ly, ry);
    lx = std::max(0, lx);
    ly = std::max(0, ly);
    rx = std::min(getHeight(), rx);
    ry = std::min(getWidth(), ry);

    long long tmp[4] = { 0 };
    RGBA mean = Mean(lx, ly, rx, ry);

    for (int i = lx; i < rx; i++) {
        for (int j = ly; j < ry; j++) {
            tmp[0]++;
            tmp[1] += (data[i][j].R - mean.R) * (data[i][j].R - mean.R);
            tmp[2] += (data[i][j].G - mean.G) * (data[i][j].G - mean.G);
            tmp[3] += (data[i][j].B - mean.B) * (data[i][j].B - mean.B);
        }
    }
    tmp[1] /= tmp[0];
    tmp[2] /= tmp[0];
    tmp[2] /= tmp[0];
    tmp[1] = std::sqrt(tmp[1]);
    tmp[2] = std::sqrt(tmp[2]);
    tmp[3] = std::sqrt(tmp[3]);

    return RGBA(tmp[1], tmp[2], tmp[3]);
}

template<class T>
Matrix<T>& Matrix<T>::GradientX() {
    Matrix<double>* tmp = new Matrix<double>(*this);
    int h = getHeight(), w = getWidth();
    for (int j = 0; j < w; j++) {
        tmp->data[0][j] = (data[0][j] - data[1][j]);
    }
    for (int j = 0; j < w; j++) {
        tmp->data[h - 1][j] = (data[h - 2][j] - data[h - 1][j]);
    }
    for (int i = 1; i < h - 1; i++) {
        for (int j = 0; j < w; j++) {
            tmp->data[i][j] = (data[i - 1][j] - data[i + 1][j]) / 2;
        }
    }
    return *tmp;
}

template<class T>
Matrix<T>& Matrix<T>::GradientY() {
    Matrix* tmp = new Matrix(*this);
    int h = height, w = width;
    for (int i = 0; i < h; i++) {
        tmp->data[i][0] = (data[i][0] - data[i][1]);
    }
    for (int i = 0; i < h; i++) {
        tmp->data[i][w - 1] = data[i][w - 2] - data[i][w - 1];
    }
    for (int i = 0; i < h; i++) {
        for (int j = 1; j < w - 1; j++) {
            tmp->data[i][j] = (data[i][j - 1] - data[i][j + 1]) / 2;
        }
    }
    return *tmp;
}

template<class T>
Matrix<double> Matrix<T>::PowDouble(int n){
    Matrix<double>* tmp = new Matrix<double>(getHeight(), getWidth());
    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            tmp->data[i][j] = pow(this->data[i][j], n);
        }
    }
    return *tmp;
}

template<class T>
Matrix<double> Matrix<T>::Sqrt() {
    Matrix<double>* tmp =new  Matrix<double>(getHeight(), getWidth());
    for (int i = 0; i < this->getHeight(); i++) {
        for (int j = 0; j < this->getWidth(); j++) {
            tmp->data[i][j] = sqrt(this->data[i][j]);
        }
    }
    return *tmp;
}

template<class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& b) {
    if (!IsSimilar(b)) {
        //qDebug()<<"Error:";
        //return;
    }
    Matrix tmp = *this;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            tmp.data[i][j] += b.data[i][j];
        }
    }
    return tmp;
}

template<class T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& b) {
    if (!IsSimilar(b)) {
        //qDebug()<<"Error:";
        //return;
    }
    Matrix<T> tmp = *this;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            tmp.data[i][j] -= b.data[i][j];
        }
    }
    return tmp;
}

template<class T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& b) {
    if (!IsSimilar(b)) {
        //qDebug()<<"Error:";
        //return;
    }
    Matrix<T> tmp = *this;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            tmp.data[i][j] = tmp.data[i][j] * b.data[i][j];
        }
    }
    return tmp;
}

template<class T>
inline Matrix<T> Matrix<T>::operator/(const Matrix<T>& b){
    if (!IsSimilar(b)) {
        //qDebug()<<"Error:";
        //std::cout<<"Error:";
        //return;
    }
    Matrix<T> tmp = *this;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            tmp.data[i][j] = tmp.data[i][j] / b.data[i][j];
        }
    }
    return tmp;

}


template<class T>
Matrix<T> Matrix<T>::operator+(const T& b) {
    Matrix tmp = *this;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            tmp.data[i][j] += b;
        }
    }
    return tmp;
}

template<class T>
Matrix<T> Matrix<T>::operator-(const T& b) {
    Matrix tmp = *this;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            tmp.data[i][j] -= b;
        }
    }
    return tmp;
}

template<class T>
Matrix<T> Matrix<T>::operator*(const T& b) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            data[i][j] = data[i][j] * b;
        }
    }
    return *this;
}

template<class T>
Matrix<T> Matrix<T>::operator/(const T& b) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            data[i][j] = data[i][j] / b;
        }
    }
    return *this;
}
