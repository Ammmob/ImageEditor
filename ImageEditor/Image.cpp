#include "Image.h"
#include <assert.h>
#include <QDebug>
#include <math.h>
#define pi atan2(0,-1)
typedef unsigned char BYTE;

Image::Image(): Bf(),Bi(){
    m=NULL;
}

Image::Image(int h, int w, RGBA val) {
	Bi.biWidth = w;
	Bi.biHeight = h;
	Bi.biBitCount = 32;
	Bi.biSize = 40;
	Bi.biPlanes = 1;
	Bi.biCompression = 0;
	Bi.biClrUsed = 0;
	Bi.biClrImportant = 0;
	Bi.biXPelsPerMeter = 0;
	Bi.biYPelsPerMeter = 0;
	Bi.biSizeImage = Bi.biWidth * 4 * Bi.biHeight;
	Bf.bfType = 0X4D42;
	Bf.bfSize = Bi.biSizeImage + 14;
	Bf.bfReserved1 = Bf.bfReserved2 = 0;
	Bf.bfOffBits = 14 + 40;
    m = new Matrix<RGBA>(h, w, val);
}

Image::Image(BITMAPFILEHEADER bf, BITMAPINFOHEADER bi, int h, int w): Bf(bf),Bi(bi){
    m = new Matrix<RGBA>(h,w);
}

Image::Image(BITMAPFILEHEADER bf, BITMAPINFOHEADER bi, const Matrix<RGBA>& b) {
    if (bi.biWidth != b.getWidth() || bi.biHeight != b.getWidth()) {
        qDebug()<<"Error:";
		std::cout << "Error" << std::endl;
	}
	Bf = bf;
	Bi = bi;
    m = new Matrix<RGBA>(b);
}

Image::Image(const Image& b){
    Bf = b.Bf;
    Bi = b.Bi;
    m = new Matrix<RGBA>(*b.m);
}

Image::~Image(){
    if (m != NULL){
        delete m;
    }
}

void Image::Swap(Image& a, Image& b){
	std::swap(a.Bf, b.Bf);
	std::swap(a.Bi, b.Bi);
    Matrix<RGBA>::Swap(*a.m, *b.m);
}

int Image::getH()const{
    return m->getHeight();
}

int Image::getW()const{
    return m->getWidth();
}

QImage Image::ToQImage(){

    int h=m->getHeight(),w=m->getWidth();
    QImage tmp(w,h,QImage::Format_RGB32);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            RGBA temp = m->data[h-i-1][j];
            tmp.setPixel(j, i, qRgba(temp.R, temp.G, temp.B, temp.A));
        }
    }

    return tmp;
}



bool Image::FileOpen(std::string filePath, std::fstream &bmp){
    std::locale::global(std::locale(""));
    bmp.open(filePath, std::ios::binary | std::ios::in);
    std::locale::global(std::locale("C"));
    if(bmp.is_open())
        return true;
    else
        return false;
}

Image& Image::ReadBmp(std::fstream& bmp){

	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;

	bmp.read((char*)&bf, sizeof(BITMAPFILEHEADER));
	bmp.read((char*)&bi, sizeof(BITMAPINFOHEADER));
    //std::cout << bi.biHeight << " " << bi.biWidth << std::endl;

    Image *tmp = new Image(bf,bi,bi.biHeight,bi.biWidth);
    int h=tmp->getH(),w=tmp->getW();
    for (int i = 0; i <h; ++i) {
        for(int j=0;j<w;j++){
            BYTE m[4];
            bmp.read((char*)m, 4);
            RGBA t(m[0], m[1], m[2], m[3]);
            tmp->m->Set(i, j,t);
        }
    }
    bmp.close();
    return *tmp;
}

void Image::WriteBmp(std::string filePath){
    std::locale::global(std::locale(""));
    std::fstream bmp(filePath, std::ios::binary | std::ios::out);
    std::locale::global(std::locale("C"));

    bmp.write((char*)&Bf, sizeof(BITMAPFILEHEADER));
    bmp.write((char*)&Bi, sizeof(BITMAPINFOHEADER));
    for (int i = 0; i < Bi.biWidth * Bi.biHeight; ++i) {
        bmp.write((char*)&(this->m->data[i/Bi.biWidth][i%Bi.biWidth]), 4);
    }
    bmp.close();
}




void Image::Flip(int mode){//mode: 0 left-right flip;mode:1 up-down flip
	if (mode) {
        for (int i = 0; i < m->getHeight() / 2; i++) {
            std::swap(m->data[i], m->data[m->getHeight() - i - 1]);
		}
	}
	else {
        for (int i = 0; i < m->getHeight(); i++) {
            for (int j = 0; j < m->getWidth() / 2; j++) {
                RGBA::Swap(m->data[i][j], m->data[i][m->getWidth() - j - 1]);
			}
		}
	}
}
void Image::Crop(int lx, int ly, int rx, int ry){
    if (lx > rx)std::swap(lx, rx);
    if (ly > ry)std::swap(ly, ry);
    int h = rx - lx, w = ry - ly;
    Matrix<RGBA>*tmp = new Matrix<RGBA>(h,w);
    for (int i = lx; i < rx; i++) {
        for (int j = ly; j < ry; j++) {
            tmp->data[i - lx][j - ly] = m->data[i][j];
		}
	}
    delete m;
    m = tmp;
    Bi.biHeight = h;
    Bi.biWidth = w;
}
void Image::Transpose(){
	std::swap(Bi.biHeight, Bi.biWidth);
    m->Transpose();
}
void Image::Rotate(int degree){
	assert(degree % 90 == 0);
	Transpose();
	Flip(1);
	if (degree > 90)Rotate(degree - 90);

}

void Image::ColorReversal(){
    *m = Matrix<RGBA>(getH(), getW(),255) - *m;
}

void Image::ColorChange(QColor a, QColor b,int R){
    RGBA x = RGBA(a.blue(),a.green(),a.red(),a.alpha());
    RGBA y = RGBA(b.blue(),b.green(),b.red(),b.alpha());
    for(int i=0;i<getH();i++){
        for(int j=0;j<getW();j++){
            int c1,c2,c3;
            c1 = std::abs((int)m->data[i][j].R-(int)x.R);
            c2 = std::abs((int)m->data[i][j].G-(int)x.G);
            c3 = std::abs((int)m->data[i][j].B-(int)x.B);

           if(c1<=R&&c2<=R&&c3<=R){
              m->data[i][j]=y;
           }
        }
    }
}

void Image::Binarization(int threshold){
    for(int i=0;i<getH();i++){
        for(int j=0;j<getW();j++){
            if(m->data[i][j]<RGBA(threshold)){
                m->data[i][j]=RGBA::Black();
            }else{
                m->data[i][j]=RGBA::White();
            }
        }
    }
}


void Image::Resize(int h,int w){
    int width = getW(),height = getH();
    double hi = height*1.0/h;
    double wi = width*1.0/w;
    Matrix<RGBA>*temp=new Matrix<RGBA>(h,w);

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            double yp=i*hi , xp= j*wi;
            int x1,y1,x2,y2;
            double r1=0,g1=0,b1=0,r2=0,g2=0,b2=0;

            x1 = std::floor(xp);
            x2 = std::ceil(xp);
            y1 = std::floor(yp);
            y2 = std::ceil(yp);
            if((x2<width&&y2<height)&&x1!=x2&&y1!=y2){
                r1 = (x2-xp)*m->data[y1][x1].R+(xp-x1)*m->data[y1][x2].R;
                r2 = (x2-xp)*m->data[y2][x1].R+(xp-x1)*m->data[y2][x2].R;
                temp->data[i][j].R = (y2-yp)*r1+(yp-y1)*r2;

                g1 = (x2-xp)*m->data[y1][x1].G+(xp-x1)*m->data[y1][x2].G;
                g2 = (x2-xp)*m->data[y2][x1].G+(xp-x1)*m->data[y2][x2].G;
                temp->data[i][j].G = (y2-yp)*g1+(yp-y1)*g2;

                b1 = (x2-xp)*m->data[y1][x1].B+(xp-x1)*m->data[y1][x2].B;
                b2 = (x2-xp)*m->data[y2][x1].B+(xp-x1)*m->data[y2][x2].B;
                temp->data[i][j].B = (y2-yp)*b1+(yp-y1)*b2;
            }else{
                temp->data[i][j] = m->data[y1][x1];
            }
        }
    }
    delete m;
    m = temp;
    Bi.biWidth = w;
    Bi.biHeight = h;

}

void Image::Scaled(double rate){
    int h=rate*getH(),w=rate*getW();
    Resize(h,w);
}

void Image::Mosaic(int r){
    int w = getW(), h = getH();
    Matrix<RGBA>* temp = new Matrix<RGBA>(h, w);
    for (int i = 0; i < h; i += r) {
        for (int j = 0; j < w; j += r) {
            srand((unsigned int)time(NULL));
            int x = i + rand() % r;
            int y = j + rand() % r;
            x = std::min(h-1,x);
            y = std::min(w-1,y);
            temp->Set(i, j, i + r, j + r, m->data[x][y]);
        }
    }
    delete m;
    m = temp;
}

void Image::MeanFiltering(int r){
    if(r==0)return;

    Matrix<RGBA>* tmp = new Matrix<RGBA>(getH(), getW());
    for (int i = 0; i < getH(); i++) {
        for (int j = 0; j < getW(); j++) {
            tmp->data[i][j] = m->Mean(i - r, j - r, i + r + 1, j + r + 1);
        }
    }
    delete m;
    m = tmp;
}

void Image::MedianFiltering(int r){
    if(r==0)return;

    Matrix<RGBA>* tmp = new Matrix<RGBA>(getH(), getW());
    for (int i = 0; i < getH(); i++) {
        for (int j = 0; j < getW(); j++) {
            tmp->data[i][j] = m->Median(i - r, j - r, i + r + 1, j + r + 1);
        }
    }
    delete m;
    m = tmp;
}

void Image::GaussFiltering(){
    int height=getH(),width=getW();
    Matrix<RGBA>*tmp = new Matrix<RGBA>(height,width);
    int kernel[5][5] = {
        {0,0,1,0,0},
        {0,1,3,1,0},
        {1,3,7,3,1},
        {0,1,3,1,0},
        {0,0,1,0,0}
    };
    int kernelSize = 5;
    int sumKernel=27;
    int r,g,b;
    for(int x=kernelSize/2; x<height-kernelSize/2; x++)
    {
        for (int y=kernelSize/2; y<width-kernelSize/2; y++)
        {
            r = g = b = 0;
            for (int i=-kernelSize/2; i<=kernelSize/2; i++)
            {
                for (int j=-kernelSize/2; j<=kernelSize/2; j++)
                {

                    r += (int)m->data[x+i][y+j].R*kernel[kernelSize/2+i][kernelSize/2+j];
                    g += (int)m->data[x+i][y+j].G*kernel[kernelSize/2+i][kernelSize/2+j];
                    b += (int)m->data[x+i][y+j].B*kernel[kernelSize/2+i][kernelSize/2+j];
                }
            }
            tmp->data[x][y].R = qBound(0, r/sumKernel, 255);
            tmp->data[x][y].G = qBound(0, g/sumKernel, 255);
            tmp->data[x][y].B = qBound(0, b/sumKernel, 255);
        }
    }
    delete m;
    m = tmp;
}


void Image::LaplaceSharpen(){
    int height=getH(),width=getW();
    Matrix<RGBA>*tmp = new Matrix<RGBA>(height,width);
    int kernel[3][3] = {
            {0,-1,0},
            {-1,5,-1},
            {0,-1,0}
   };
   int kernelSize = 3;
   int r,g,b;
   for(int x=kernelSize/2; x<height-kernelSize/2; x++)
   {
       for (int y=kernelSize/2; y<width-kernelSize/2; y++)
       {
           r = g = b = 0;
           for (int i=-kernelSize/2; i<=kernelSize/2; i++)
           {
               for (int j=-kernelSize/2; j<=kernelSize/2; j++)
               {
                   r += (int)m->data[x+i][y+j].R*kernel[kernelSize/2+i][kernelSize/2+j];
                   g += (int)m->data[x+i][y+j].G*kernel[kernelSize/2+i][kernelSize/2+j];
                   b += (int)m->data[x+i][y+j].B*kernel[kernelSize/2+i][kernelSize/2+j];
               }
           }
           tmp->data[x][y].R = qBound(0, r, 255);
           tmp->data[x][y].G = qBound(0, g, 255);
           tmp->data[x][y].B = qBound(0, b, 255);
       }
   }
   delete m;
   m = tmp;
}


void Image::Gray(){
    for (int i = 0; i < m->getHeight(); i++) {
        for (int j = 0; j < m->getWidth(); j++) {
            this->m->data[i][j].Gray();
        }
    }
}

void Image::HandStyle(){
    Gray();
    Matrix<double>gradx = this->m->RGBA2Double().GradientX();
    Matrix<double>grady = this->m->RGBA2Double().GradientY();
    gradx = gradx / 10;
    grady = grady / 10;
    Matrix<double>A = gradx.PowDouble(2) + grady.PowDouble(2)+ 1;
    A = A.Sqrt();

    gradx = gradx / A;
    grady = grady / A;
    Matrix<double>gradz = Matrix<double>(m->getHeight(), m->getWidth(), 1.0) / A;
    double vec_el = pi / 2.2;
    double vec_az = pi / 4;

    double dx = cos(vec_el) * cos(vec_az);
    double dy = cos(vec_el) * sin(vec_az);
    double dz = sin(vec_el);

    Matrix<double>ans = (gradx * dx + grady * dy + gradz * dz) * 255;
    ans.Clip(0.0,255.0);
    m->data = ans.Double2RGBA().data;

}

void Image::RetroStyle() {
    for (int i = 0; i < getH(); ++i) {
        for (int j = 0; j < getW(); j++) {
            double R, G, B;
            B = 0.272 * m->data[i][j].R + 0.534 * m->data[i][j].G + 0.131 * m->data[i][j].B;
            G = 0.349 * m->data[i][j].R + 0.686 * m->data[i][j].G + 0.168 * m->data[i][j].B;
            R = 0.393 * m->data[i][j].R + 0.769 * m->data[i][j].G + 0.189 * m->data[i][j].B;
            R = (R > 255) ? 255 : (R < 0 ? 0 : R);
            G = (G > 255) ? 255 : (G < 0 ? 0 : G);
            B = (B > 255) ? 255 : (B < 0 ? 0 : B);
            m->data[i][j].R = (BYTE)R;
            m->data[i][j].G = (BYTE)G;
            m->data[i][j].B = (BYTE)B;
        }
    }
}

void Image::BlueStyle(){
    ColorReversal();
    RetroStyle();
    ColorReversal();
}

Image& Image:: operator=(const Image& b){
    delete m;
    Bf = b.Bf;
    Bi = b.Bi;
    m = new Matrix<RGBA>(*b.m);
    return *this;
}
//��ʱ����
//void Image::Reshape(int h, int w){
//	Matrix::Reshape(h, w);
//	Bi.biHeight = h;
//	Bi.biWidth = w;
//}
