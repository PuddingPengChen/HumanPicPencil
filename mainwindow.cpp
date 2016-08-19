#include <QRgb>
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "imatrix.h"
#include "ETF.h"
#include "fdog.h"
#include "myvec.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnLoad_clicked()
{
    file = QFileDialog::getOpenFileName(this,tr("Open Image"),".",tr("Image (*.jpg *.jpeg *.png)"));
    QImage source(file);
    ui->labelSource->setPixmap(QPixmap::fromImage(source));
}

void MainWindow::on_btnFdog_clicked()
{
    QImage image(file);
    int depth = 4;
    imatrix img(image.height(),image.width());
    for (int ii = 0; ii < image.height(); ii++)
    {
        uchar* scan = image.scanLine(ii);
        for (int jj = 0; jj < image.width(); jj++)
        {

            QRgb* rgbpixel = reinterpret_cast<QRgb*>(scan + jj*depth);
            int gray = qGray(*rgbpixel);
            img[ii][jj]=gray;
        }
    }

    //We assume that you have loaded your input image into an imatrix named "img"

    int image_x = img.getRow();
    int image_y = img.getCol();

    //构建flow
    ETF e;
    e.init(image_x, image_y);
    e.set(img); // get flows from input image
    //    e.set2(img); // get flows from flow map(二次流)
    e.Smooth(4, 2);

    //基于flow，做DOG
    double tao = 0.99;
    double thres = 0.7;
    GetFDoG(img, e, 1.0, 3.0, tao);
    GrayThresholding(img, thres);

    QImage image2 = image;
    QRgb *pixels = (QRgb *)image2.scanLine(0);
    for (int i=0;i<image2.height();i++)
    {
        for (int j=0;j<image2.width();j++)
        {
            int color=img.p[i][j];
            QRgb threadw = qRgb(color,color,color);
            pixels[j+i*image2.width()]  = threadw;
        }
    }
    ui->labelDog->setPixmap(QPixmap::fromImage(image2));
    outfile = image2;

}

void MainWindow::on_btnOut_clicked()
{
    QString save = QFileDialog::getSaveFileName(this,tr("Save"),".",tr("Image (*.png)"));
    outfile.save(save,"PNG",100);
}
