#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "PortReaderWriter.h"
#include "qcustomplot.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupQuadraticDemo(QCustomPlot *customPlot);

    void setupSpeedGraph(QCustomPlot *customPlot);

    void setupRPMGraph(QCustomPlot *customPlot);

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void connect();

    void sendcommand();

    //void on_pushButton_5_clicked();

    //void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    // this is function for usinf enter in the line stop plise deleting it !!!
    void on_lineEdit_returnPressed();


private:
    Ui::MainWindow *ui;
    serial::PortReaderWriter *conn;
    int speedCount;
    int rpmCount;
    QVector<double> vspeed;
    QVector<double> vrpm;
};

#endif // MAINWINDOW_H
