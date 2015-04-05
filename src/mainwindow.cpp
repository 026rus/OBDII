#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->conn = new serial::PortReaderWriter();
    connect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString trobelCode = "";
    { // Try to get the Trouble Code
        conn->sendCommand("01 01");
        // TODO: Needs to be multiline aware
        QByteArray buff = conn->readLine();
        trobelCode = conn->decodeErr(buff);
        ui->textBrowser->setText(trobelCode);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
     int vehicleSpeed = 0;
    { // Try to get the RPM
        conn->sendCommand("01 0D");
        QByteArray buff = conn->readLine();
        vehicleSpeed = conn->decodeVehicleSpeed(buff);
    }
    ui->textBrowser->setText(  QString::number(vehicleSpeed)  );
}

void MainWindow::on_pushButton_3_clicked()
{
    int rpmVal = 0;
    { // Try to get the RPM
        conn->sendCommand("01 0C");
        QByteArray buff = conn->readLine();
        rpmVal = conn->decodeRPM(buff);
    }
    ui->textBrowser->setText( QString::number(rpmVal) );
}

void MainWindow::on_pushButton_4_clicked()
{
    QString tempstr="";
    ui->textBrowser->setText("");

    QString instr = ui->lineEdit->text();
    QByteArray qbin = instr.toUtf8();

    if(!conn->sendCommand( qbin ))
    {
        ui->textBrowser->setText( "Problem writing !!!!");
    }
    QByteArray buff = conn->readLine();
    tempstr = "Buff size 1: ";
    tempstr += QString::number(buff.size());
    ui->textBrowser->append( tempstr );

    buff.remove(0, instr.size()+1);

    tempstr = "Buff size 2: ";
    tempstr += QString::number( buff.size() );

    ui->textBrowser->append( tempstr );

    tempstr = "(";
    tempstr += buff;
    tempstr += ")" ;

    ui->textBrowser->append( tempstr );

    ui->lineEdit->setText("");
}

void MainWindow::connect()
{
    QString infoout="";

    if (!conn->serialConnect())
    {
        ui->textBrowser->setText("Could not connect!");
    }


    if (this->conn->isConnected())
    {
        infoout = "Connected to serial port!" +  this->conn->getConnectedPortName();
        ui->textBrowser->setText(infoout);
    }
    else
    {
        ui->textBrowser->setText("Not connected to a serial port!");
    }

}
