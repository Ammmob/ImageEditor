#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Image.h"
#include "imagecropperdialog.h"
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QString>
#include <string>
#include <QDebug>
#include <QPainter>
#include <QPaintDevice>
#include <QPaintEvent>
#include <QInputDialog>
#include <QLayout>
#include <QColorDialog>
#include <mosaicdialog.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //初始化
    ui->setupUi(this);
    resize(1000,750);
    setMinimumSize(QSize(900,700));
    //setFixedSize(900,600);
    initImg = NULL;
    img = NULL;
    statusLabel = new QLabel;
    isFileOpen=false;
    filePath = "";

    mBar = menuBar();
    setMenuBar(mBar);
    tBar = new QToolBar;
    addToolBar(tBar);
    //tBar->addWidget()
    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    tBar->addWidget(spacer);

    sBar = statusBar();
    setStatusBar(sBar);
    sBar->addWidget(statusLabel);



    //lay = new QLabel();

    //GUI设计
//     this->setStyleSheet("background-color:rgb(224,238, 224);"
//                    );

    setWindowIcon(QIcon(":/res/pic.png"));


    //工具栏
    undoEdit = tBar->addAction(QIcon(":/res/undo2.png"),"撤销");

    resetEdit = tBar->addAction(QIcon(":/res/reset1.png"),"重置");

    bigEdit = tBar->addAction(QIcon(":/res/biger.png"),"放大");

    smallEdit = tBar->addAction(QIcon(":/res/smaller.png"),"缩小");
    //菜单栏


    //文件菜单模块
    QMenu *fileMenu = mBar->addMenu("文件");

    QAction *openFile = fileMenu->addAction(QIcon(":/res/open.png"),"打开文件");
    //fileMenu->addSeparator();

    QAction *saveFile = fileMenu->addAction(QIcon(":/res/save.png"),"保存");

    QAction *saveAsFile = fileMenu->addAction(QIcon(":/res/saveas.png"),"另存为");

    QAction *closeFile = fileMenu->addAction(QIcon(":/res/close.png"),"关闭文件");

    //编辑菜单模块
    QMenu *editMenu = mBar->addMenu("编辑");

    editMenu->addAction(undoEdit);
    editMenu->addAction(resetEdit);

    editMenu->addSeparator();

    QMenu *flipmenu = editMenu->addMenu("翻转");

    QAction *fliplrEdit = flipmenu->addAction(QIcon(":/res/fliplr.png"),"水平翻转");

    QAction *flipudEdit = flipmenu->addAction(QIcon(":/res/flipud.png"),"垂直翻转");

    QMenu *rotatemenu = editMenu->addMenu("旋转");

    QAction *rotateEdit = rotatemenu->addAction(QIcon(":/res/rotateR.png"),"右旋转90度");

    QAction *anti_rotateEdit = rotatemenu->addAction(QIcon(":/res/rotateL.png"),"左旋转90度");

    QAction *cropEdit= editMenu->addAction(QIcon(":/res/crop.png"),"裁剪");
    editMenu->addSeparator();

    QAction *colorReversalEdit = editMenu->addAction(QIcon(":/res/colorRev.png"),"颜色反转");
    QAction *colorChangeEdit = editMenu->addAction(QIcon(":/res/colorCha.png"),"颜色转换");
    editMenu->addSeparator();

    QMenu *filtermenu = editMenu->addMenu(QIcon(":/res/wave.png"),"滤波");

    QAction *meanFilter = filtermenu->addAction("均值滤波");
    QAction *medianFilter = filtermenu->addAction("中值滤波");
    QAction *gaussFilter = filtermenu->addAction("高斯滤波");

    QAction *sharpenEdit = editMenu->addAction(QIcon(":/res/sharpen.png"),"锐化");

    //风格菜单模块
    QMenu *styleMenu = mBar->addMenu("风格");

    QAction *grayStyle = styleMenu->addAction(QIcon(":/res/hb1.png"),"灰度化");

    QAction *binarization = styleMenu->addAction(QIcon(":/res/hb2.png"),"二值化");

    QAction *mosaic = styleMenu->addAction(QIcon(":/res/mosaic.png"),"马赛克");


    QAction *handStyle = styleMenu->addAction(QIcon(":/res/pan.png"),"手绘风");

    QAction *retroStyle = styleMenu->addAction(QIcon(":/res/retro.png"),"复古风");

    QAction *BlueStyle = styleMenu->addAction(QIcon(":/res/blue.png"),"清冷风");



    //文件链接
    //打开文件的连接
    connect(openFile,&QAction::triggered,[=](){
       QString filePath = QFileDialog::getOpenFileName(this,"open file");
       this->filePath = filePath.toStdString();
       std::fstream bmp;
       if(Image::FileOpen(filePath.toStdString(),bmp)){
            while(!undoStack.empty()){
               Image*tmp = undoStack.top();
               delete tmp;
               undoStack.pop();
            }
            qDebug()<<"read success!";
            img = new Image;
            initImg = new Image;
            *img = Image::ReadBmp(bmp);//读入img
            *initImg = *img;//保存初始状态
            isFileOpen = true;//更改状态
            update();//更新绘图事件

            statusLabel->setText(QString::number(img->getW())+"×"+QString::number(img->getH()));

       }else{
           //弹出提示框 文件打开失败
          qDebug()<<"file open error!";
       }

    });

    //保存文件的连接
    connect(saveFile,&QAction::triggered,[=](){
        if(isFileOpen){
            img->WriteBmp(filePath);
        }else{
            //弹出提示框 文件未打开
            qDebug()<<"file is not open!";
        }

    });

    connect(saveAsFile,&QAction::triggered,[=](){
        if(isFileOpen){
            QString savePath = QFileDialog::getSaveFileName(this,"save file");
            img->WriteBmp(savePath.toStdString());
        }else{
            //弹出提示框 文件未打开
            qDebug()<<"file is not open!";
        }
    });

    //关闭文件的链接
    connect(closeFile,&QAction::triggered,[=](){
        if(isFileOpen){
            if(img!=NULL){
                delete img;
                img = NULL;
            }

            if(initImg!=NULL){
                delete initImg;
                initImg = NULL;
            }

            isFileOpen = false;//文件打开状态切换
            update();

            while(!undoStack.empty()){
                Image*tmp = undoStack.top();
                delete tmp;
                undoStack.pop();
            }
            statusLabel->setText("");
        }else{
            //弹出提示框 文件未打开
            qDebug()<<"file is not open!";
        }

    });

    //编辑链接
    connect(undoEdit,&QAction::triggered,[=](){
        if(!undoStack.empty()){
            delete img;
            img = undoStack.top();
            undoStack.pop();
            update();
            statusLabel->setText(QString::number(img->getW())+"×"+QString::number(img->getH()));

        }else{
            qDebug()<<"already init status\n";
            //弹出警告框
        }
    });

    connect(resetEdit,&QAction::triggered,[=](){
        if(isFileOpen){
            *img = *initImg;
            while(!undoStack.empty()){
                Image*tmp = undoStack.top();
                delete tmp;
                undoStack.pop();
            }
            update();
            statusLabel->setText(QString::number(img->getW())+"×"+QString::number(img->getH()));

        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框
        }
    });

    //缩放链接
    connect(bigEdit,&QAction::triggered,[=](){
        if(isFileOpen&&img->getH()<1000&&img->getW()<1000){
            Image *tmp = new Image;
            *tmp = *img;
            undoStack.push(tmp);

            img->Scaled(1.1);
            update();
            statusLabel->setText(QString::number(img->getW())+"×"+QString::number(img->getH()));
        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框
        }
    });

    connect(smallEdit,&QAction::triggered,[=](){
        if(isFileOpen&&img->getH()>50&&img->getW()>50){
            Image *tmp = new Image;
            *tmp = *img;
            undoStack.push(tmp);

            img->Scaled(1.0/1.1);
            statusLabel->setText(QString::number(img->getW())+"×"+QString::number(img->getH()));
            update();
        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框
        }
    });

    //功能链接
    connect(fliplrEdit,&QAction::triggered,[=](){
        if(isFileOpen){
            Image *tmp = new Image;
            *tmp = *img;
            undoStack.push(tmp);

            img->Flip();
            update();
        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框
        }
    });

    connect(flipudEdit,&QAction::triggered,[=](){
        if(isFileOpen){
            Image *tmp = new Image;
            *tmp = *img;
            undoStack.push(tmp);

            img->Flip(1);
            update();
        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框
        }
    });

    connect(rotateEdit,&QAction::triggered,[=](){
        if(isFileOpen){
            Image *tmp = new Image;
            *tmp = *img;
            undoStack.push(tmp);

            img->Rotate(90);
            update();
        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框
        }
    });

    connect(anti_rotateEdit,&QAction::triggered,[=](){
        if(isFileOpen){
            Image *tmp = new Image;
            *tmp = *img;
            undoStack.push(tmp);

            img->Rotate(270);
            update();

        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框
        }
    });


    connect(cropEdit,&QAction::triggered,[=](){
        if(isFileOpen){


            QImage inputImage = img->ToQImage();
            QPixmap inputPixmap = QPixmap::fromImage(inputImage);
            bool ok=false;
            QRect rect = ImageCropperDialog::getCroppedRect(ok,inputPixmap, 600, 400, CropperShape::SQUARE);
            if(ok){
                Image *tmp = new Image;
                *tmp = *img;
                undoStack.push(tmp);

                img->Crop(img->getH()-rect.y(),rect.x(),img->getH()-rect.y()-rect.height(),rect.x()+rect.width());

                update();

                statusLabel->setText(QString::number(img->getW())+"×"+QString::number(img->getH()));
            }

        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框
        }
    });

    connect(colorReversalEdit,&QAction::triggered,[=](){
        if(isFileOpen){
            Image *tmp = new Image;
            *tmp = *img;
            undoStack.push(tmp);

            img->ColorReversal();
            update();
        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框
        }
    });

    connect(colorChangeEdit,&QAction::triggered,[=](){
        if(isFileOpen){
            bool ok3;

            QColor a = QColorDialog::getColor(Qt::white,nullptr,"选择指定颜色");
            QColor b= QColorDialog::getColor(Qt::white,nullptr,"选择目标颜色");
            int r = QInputDialog::getInt(this,"色彩转换","请输入容差半径",0,0,255,1,&ok3);
            if(ok3){
                Image *tmp = new Image;
                *tmp = *img;
                undoStack.push(tmp);

                img->ColorChange(a,b,r);
                update();
            }
        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框
        }
    });

    //滤波
    connect(meanFilter,&QAction::triggered,[=](){
        if(isFileOpen){

            bool ok;
            int radius = QInputDialog::getInt(this,"均值滤波","请输入滤波器半径",3,2,15,1,&ok);
            if(ok){
                Image *tmp = new Image;
                *tmp = *img;
                undoStack.push(tmp);

                img->MeanFiltering(radius);
                update();
            }
        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框
        }
    });

    connect(medianFilter,&QAction::triggered,[=](){
        if(isFileOpen){

            bool ok;
            int radius = QInputDialog::getInt(this,"中值滤波","请输入滤波器半径",3,2,10,1,&ok);
            if(ok){
                Image *tmp = new Image;
                *tmp = *img;
                undoStack.push(tmp);

                img->MedianFiltering(radius);
                update();
            }
        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框
        }
    });

    connect(gaussFilter,&QAction::triggered,[=](){
        if(isFileOpen){
            Image *tmp = new Image;
            *tmp = *img;
            undoStack.push(tmp);

            img->GaussFiltering();
            update();

        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框
        }
    });

    connect(sharpenEdit,&QAction::triggered,[=](){
        if(isFileOpen){
            Image *tmp = new Image;
            *tmp = *img;
            undoStack.push(tmp);

            img->LaplaceSharpen();
            update();

        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框
        }
    });


    connect(binarization,&QAction::triggered,[=](){
        if(isFileOpen){

            bool ok;
            int threshold = QInputDialog::getInt(this,"二值化","请输入阈值",100,0,255,1,&ok);
            if(ok){
                Image *tmp = new Image;
                *tmp = *img;
                undoStack.push(tmp);

                img->Binarization(threshold);
                update();
            }
        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框

        }
    });

    connect(mosaic,&QAction::triggered,[=](){
        if(isFileOpen){
            int *r=new int(0);
            MosaicDialog * mosaicdialog = new MosaicDialog(img,r,this);
            mosaicdialog->setAttribute(Qt::WA_DeleteOnClose);
            mosaicdialog->show();
            connect(mosaicdialog,&MosaicDialog::OK,[=](){
                Image *tmp = new Image;
                *tmp = *img;
                undoStack.push(tmp);
                img->Mosaic(*r);
                delete r;
                update();
            });

        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框
        }
    });



    connect(grayStyle,&QAction::triggered,[=](){
        if(isFileOpen){
            Image *tmp = new Image;
            *tmp = *img;
            undoStack.push(tmp);

            img->Gray();
            update();
        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框

        }
    });

    connect(handStyle,&QAction::triggered,[=](){
        if(isFileOpen){
            Image *tmp = new Image;
            *tmp = *img;
            undoStack.push(tmp);

            img->HandStyle();
            update();
        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框

        }
    });

    connect(retroStyle,&QAction::triggered,[=](){
        if(isFileOpen){
            Image *tmp = new Image;
            *tmp = *img;
            undoStack.push(tmp);

            img->RetroStyle();
            update();
        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框

        }
    });

    connect(BlueStyle,&QAction::triggered,[=](){
        if(isFileOpen){
            Image *tmp = new Image;
            *tmp = *img;
            undoStack.push(tmp);

            img->BlueStyle();
            update();
        }else{
            qDebug()<<"file is not opened!\n";
            //弹出警告框

        }
    });

}

MainWindow::~MainWindow()
{
    delete ui;
    if(img!=NULL){
        delete img;
        img = NULL;
    }

    if(initImg!=NULL){
        delete initImg;
        initImg = NULL;
    }

    while(!undoStack.empty()){
        Image*tmp=undoStack.top();
        delete tmp;
        undoStack.pop();
    }
}

void MainWindow::paintEvent(QPaintEvent*){
    if(isFileOpen){

        QPainter painter(this);

        QRect target((width()-img->getW())/2,
                    (height()-img->getH())/2,
                     img->getW(),img->getH());//初设需要修改 -- 设置中心位置
        painter.drawImage(target,this->img->ToQImage());
    }
}

void MainWindow::wheelEvent(QWheelEvent *e){
    if(isFileOpen){
        if(e->delta()>0){
            bigEdit->trigger();
        }else{
            smallEdit->trigger();
        }
    }
}




