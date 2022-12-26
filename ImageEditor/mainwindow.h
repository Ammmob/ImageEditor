#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stack>
#include <QLabel>

#include "Image.h"
#include "mosaicdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *);

    //QLabel *lay;
    bool isFileOpen;
    Image *initImg;
    Image *img;
    std::string filePath;
    std::stack<Image*> undoStack;

    QMenuBar *mBar;
    QToolBar *tBar;
    QStatusBar *sBar;

    QLabel *statusLabel;



    QAction *undoEdit;
    QAction *resetEdit;
    QAction *bigEdit;
    QAction *smallEdit;

    void wheelEvent(QWheelEvent*e);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
