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

    void on_checkEngineButton_clicked();

    void on_monitorButton_clicked();

    void on_submitButton_clicked();

    void connect();

    void sendcommand();

    void on_connectButton_clicked();

    void on_speedBox_clicked();

    void on_rpmBox_clicked();

    // this is function for usinf enter in the line stop plise deleting it !!!
    void on_inputEdit_returnPressed();


private:
    Ui::MainWindow *ui;
    serial::PortReaderWriter *conn;
    int speedCount;
    int rpmCount;
    QVector<double> vspeed;
    QVector<double> vrpm;
};

#endif // MAINWINDOW_H
