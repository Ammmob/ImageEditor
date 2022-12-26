#include "mosaicdialog.h"
#include "ui_mosaicdialog.h"

MosaicDialog::MosaicDialog(Image *Img,int *r,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MosaicDialog)
{
    ui->setupUi(this);

    tmp = new Image;
    *tmp = *Img;
    setFixedSize(600,580);
    setMinimumSize(600,550);
    setWindowTitle("马赛克");
    ui->mosaic_label->resize(tmp->getW(),tmp->getH());
    ui->mosaic_label->setAlignment(Qt::AlignHCenter);
    ui->mosaic_label->setGeometry((width()-tmp->getW())/2,
                                 10,
                                 tmp->getW(),tmp->getH());

    ui->mosaic_spinBox->setGeometry(ui->mosaic_label->x(),
                                    ui->mosaic_label->y()+ui->mosaic_label->height()+10,
                                    50,20);
    ui->mosaic_slider->setGeometry(ui->mosaic_spinBox->x()+ui->mosaic_spinBox->width()+5,
                                   ui->mosaic_label->y()+ui->mosaic_label->height()+10,
                                   200,20);
    ui->mosaic_ok->setGeometry(ui->mosaic_label->x()+ui->mosaic_label->width()-ui->mosaic_ok->width()-ui->mosaic_cancel->width()-20,
                               ui->mosaic_spinBox->y()+ui->mosaic_spinBox->height(),
                               ui->mosaic_ok->width(),ui->mosaic_ok->height());
    ui->mosaic_cancel->setGeometry(ui->mosaic_ok->x()+ui->mosaic_ok->width()+10,
                               ui->mosaic_spinBox->y()+ui->mosaic_spinBox->height(),
                               ui->mosaic_cancel->width(),ui->mosaic_cancel->height());


    void(QSpinBox::*spSingal)(int)=&QSpinBox::valueChanged;
    connect(ui->mosaic_spinBox,spSingal,[=]()mutable{
        int value = ui->mosaic_spinBox->value();
        ui->mosaic_slider->setValue(value);
        *r = value;
        *tmp = *Img;
        tmp->Mosaic(value);
        ui->mosaic_label->update();
    });

    connect(ui->mosaic_slider,&QSlider::valueChanged,[=]()mutable{
        int value = ui->mosaic_slider->value();
        ui->mosaic_spinBox->setValue(value);
        *r = value;
        *tmp = *Img;
        tmp->Mosaic(value);
        ui->mosaic_label->update();
    });

    connect(ui->mosaic_ok,&QPushButton::clicked,[=]()mutable{

        *r = ui->mosaic_spinBox->value();
        emit OK();
        close();
    });
    connect(ui->mosaic_cancel,&QPushButton::clicked,[=]()mutable{

        close();
    });
}

MosaicDialog::~MosaicDialog()
{
    delete ui;
    if(tmp!=NULL){
        delete tmp;
    }
}

void MosaicDialog::paintEvent(QPaintEvent *)
{
     ui->mosaic_label->setPixmap(QPixmap::fromImage(tmp->ToQImage()));
}


