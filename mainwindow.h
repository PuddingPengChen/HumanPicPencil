#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnLoad_clicked();

    void on_btnFdog_clicked();

    void on_btnOut_clicked();

private:
    Ui::MainWindow *ui;
    QString file;
    QImage outfile;
};

#endif // MAINWINDOW_H
