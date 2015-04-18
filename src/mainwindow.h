#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "PortReaderWriter.h"
#include "qcustomplot.h"
#include <QTextDocument>
#include <QFileDialog>
#include "qcpdocumentobject.h"
#include "obd2client.h"


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

    void setupGraph(QCustomPlot *customPlot, QString dataName, bool &dataClicked);

private slots:

    void on_checkEngineButton_clicked();
    void on_monitorButton_clicked();
    void on_submitButton_clicked();
    void sendRawData();
    void on_connectButton_clicked();
    void on_speedBox_clicked();
    void on_rpmBox_clicked();
    void on_addGraphButton_clicked();
    void on_saveGraphButton_clicked();
    void on_uploadButton_clicked();

    // this is function for usinf enter in the line stop plise deleting it !!!
    void on_inputEdit_returnPressed();


private:
    Ui::MainWindow *ui;
    serial::PortReaderWriter *conn;
    int speedCount;
    int rpmCount;
    int pressCount;
    int distanceCount;
    int coolCount;
    int loadCount;
    int oilTempCount;
    int ethPercCount;
    int ratioCount;
    int fuelPercCount;
    int fuelPressCount;
    int airTempCount;
    int manPressCount;
    int runTimeCount;
    int throtlePercCount;

    QVector<double> vspeed;
    QVector<double> vrpm;
    QVector<double> vpressure;
    QVector<double> vdistance;
    QVector<double> vcooltemp;
    QVector<double> vload;
    QVector<double> voiltemp;
    QVector<double> vethanolpercent;
    QVector<double> vratio;
    QVector<double> vfuelpercent;
    QVector<double> vfuelpressure;
    QVector<double> vairtemp;
    QVector<double> vmanifoldpressure;
    QVector<double> vruntime;
    QVector<double> vthrotlepercent;
    OBD2Client clientsocet;

};

#endif // MAINWINDOW_H
