#ifndef IMAGECROPPER_H
#define IMAGECROPPER_H

#include <QWidget>
#include <QDialog>
#include <QPainter>
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include "imagecropperlabel.h"

class ImageCropperDialogPrivate : public QDialog {
    Q_OBJECT
public:
    ImageCropperDialogPrivate(const QPixmap& imageIn,
                              int windowWidth, int windowHeight,
                              CropperShape shape, QSize cropperSize = QSize()
                              ) :
        QDialog(nullptr)
    {
        this->setAttribute(Qt::WA_DeleteOnClose, true);
        this->setWindowIcon(QIcon(":/res/pic.png"));
        this->setWindowTitle("图像裁剪");
        this->setMouseTracking(true);
        this->setModal(true);
        this->setFixedSize(626,500);
        imageLabel = new ImageCropperLabel(windowWidth, windowHeight, this);
        imageLabel->setCropper(shape, cropperSize);
        imageLabel->setOutputShape(OutputShape::RECT);
        imageLabel->setOriginalImage(imageIn);
        imageLabel->enableOpacity(true);

        QHBoxLayout* btnLayout = new QHBoxLayout();
        btnOk = new QPushButton("OK", this);
        btnCancel = new QPushButton("Cancel", this);
        btnLayout->addStretch();
        btnLayout->addWidget(btnOk);
        btnLayout->addWidget(btnCancel);

        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(imageLabel);
        mainLayout->addLayout(btnLayout);

        connect(btnOk, &QPushButton::clicked, this, [=](){

            x = this->imageLabel->getCroppedRect().x();
            y = this->imageLabel->getCroppedRect().y();
            w = this->imageLabel->getCroppedRect().width();
            h = this->imageLabel->getCroppedRect().height();
            this->close();
            ok=true;

        });
        connect(btnCancel, &QPushButton::clicked, this, [this](){
            this->close();
        });
    }
    int x,y,w,h;
    bool ok=false;

private:
    ImageCropperLabel* imageLabel;
    QPushButton* btnOk;
    QPushButton* btnCancel;
};



class ImageCropperDialog : QObject {
public:
    static QRect getCroppedRect(bool &ok,QPixmap inputImage,int windowWidth, int windowHeight,
                                   CropperShape cropperShape, QSize crooperSize = QSize())
    {

        ImageCropperDialogPrivate* imageCropperDo =
            new ImageCropperDialogPrivate(inputImage,
                                          windowWidth, windowHeight,
                                          cropperShape, crooperSize);

        imageCropperDo->exec();
        ok = imageCropperDo->ok;
        QRect a(imageCropperDo->x,imageCropperDo->y,imageCropperDo->w,imageCropperDo->h);
        return a;
    }

};



#endif // IMAGECROPPER_H
