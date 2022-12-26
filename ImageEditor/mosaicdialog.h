#ifndef MOSAICDIALOG_H
#define MOSAICDIALOG_H

#include <QDialog>
#include <Image.h>

namespace Ui {
class MosaicDialog;
}

class MosaicDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MosaicDialog(Image *Img,int *r,QWidget *parent = nullptr);
    ~MosaicDialog();

    void paintEvent(QPaintEvent *);
    Image *tmp;
signals:
    void OK();
private:
    Ui::MosaicDialog *ui;
};
#endif // MOSAICDIALOG_H
