#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "qcustomplot.h"

QVector<double> a(101), b(101);
QVector<double> c(101), d(101);
bool visibility;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->conn = new serial::PortReaderWriter();

    setWindowTitle("Group 2 Software Engineering ODBII Reader");
    speedCount = 1;
    rpmCount = 1;
    a[0] = 0;
    b[0] = 0;
    c[0] = 0;
    d[0] = 0;
    visibility = true;

    QFont font;
    font.setPointSize(12);
    ui->label->setText("Connection Status: ");
    ui->label->setFont(font);
    ui->progressBar->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// sample graph for testing demonstration
void MainWindow::setupQuadraticDemo(QCustomPlot *customPlot)
{
  // generate some data:
  QVector<double> x(101), y(101); // initialize with entries 0..100

  for (int i=0; i<10; i++)
  {
      x[i] = i;
      switch (i)
      {
      case 0: y[i] = 0; break;
      case 1: y[i] = 0; break;
      case 2: y[i] = 1; break;
      case 3: y[i] = 2; break;
      case 4: y[i] = 4; break;
      case 5: y[i] = 6; break;
      case 6: y[i] = 5; break;
      case 7: y[i] = 4; break;
      case 8: y[i] = 1; break;
      case 9: y[i] = 0; break;
      }
  }
  // create graph and assign data to it:
  customPlot->addGraph();
  customPlot->graph(0)->setData(x, y);
  // give the axes some labels:
  customPlot->xAxis->setLabel("Time");
  customPlot->yAxis->setLabel("Value");
  // set axes ranges, so we see all data:
  customPlot->xAxis->setRange(0, 10);
  customPlot->yAxis->setRange(0, 7);
  customPlot->graph(0)->setPen(QPen(Qt::red)); // line color blue for first graph
  customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
  customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 10));
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
    if(vehicleSpeed < 0)
    {
        vspeed.append(0);
        speedCount++;
    }
    else
    {
        vspeed.append(vehicleSpeed);
        speedCount++;
    }
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
    if (rpmVal < 0)
    {
       vrpm.append(rpmCount);
       rpmCount++;
    }
    else
    {
        vrpm.append(rpmVal/1000);
        rpmCount++;
    }

/*
    // not necessary now, but the code to set
    // the other buttons to disabled while
    // monitoring the RPM
    ui->pushButton->setDisabled(visibility);
    ui->pushButton_2->setDisabled(visibility);
    ui->pushButton_4->setDisabled(visibility);
    ui->pushButton_7->setDisabled(visibility);
    visibility = !visibility;
*/

}

void MainWindow::on_pushButton_4_clicked()
{
    sendcommand();
}

void MainWindow::on_radioButton_clicked()
{
    setupSpeedGraph(ui->customPlot);
}

void MainWindow::on_radioButton_2_clicked()
{
    setupRPMGraph(ui->customPlot);
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
    connect();


}

void MainWindow::sendcommand()
{

    QString tempstr="";

    QString instr = ui->lineEdit->text();
    QByteArray qbin = instr.toUtf8();

    if(!conn->sendCommand( qbin ))
    {
        ui->textBrowser->append( "Problem writing !!!!");
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

    ui->lineEdit->selectAll();
}

void MainWindow::setupSpeedGraph(QCustomPlot *customPlot)
{
  for (int i=0; i<=speedCount; i++)
  {
      c[i] = i;
  }

  // create graph and assign data to it:
  customPlot->addGraph();
  customPlot->graph(0)->setData(c,vspeed);

  // give the axes some labels:
  customPlot->xAxis->setLabel("Count");
  customPlot->yAxis->setLabel("Speed (MPH)");

  // set axes ranges, so we see all data:
  customPlot->xAxis->setRange(0, speedCount - 1);
  customPlot->yAxis->setRange(0, 80);
  customPlot->graph(0)->setPen(QPen(Qt::red)); // line color blue for first graph
  customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
  customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 10));
  ui->customPlot->replot();
}

void MainWindow::setupRPMGraph(QCustomPlot *customPlot)
{
  for (int i=0; i<=rpmCount; i++)
  {
      a[i] = i;
  }
  // create graph and assign data to it:
  customPlot->addGraph();
  customPlot->graph(0)->setData(a, vrpm);
  // give the axes some labels:
  customPlot->xAxis->setLabel("Count");
  customPlot->yAxis->setLabel("RPM (x1000)");
  // set axes ranges, so we see all data:
  customPlot->xAxis->setRange(0, rpmCount - 1);
  customPlot->yAxis->setRange(0, 10);
  customPlot->graph(0)->setPen(QPen(Qt::red));
  // first graph will be filled with translucent blue
  customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 10));
  ui->customPlot->replot();
}

void MainWindow::on_pushButton_7_clicked()
{
    QString input_device = "";
    QVector<QSerialPortInfo> ports = serial::PortReaderWriter::getAvailPorts();

    foreach (const QSerialPortInfo &info, ports) {
	if (info.manufacturer().contains("FTDI")) {
	    input_device = info.portName();
	}
    }

    if (this->conn->setPort(input_device.toStdString())) {
	QFont font;
	font.setPointSize(12);
	QString text = "Connection Status: " + input_device;
	ui->label->setText(text);
	ui->label->setFont(font);
	ui->progressBar->setValue(100);
    ui->pushButton_7->setText(QApplication::translate("MainWindow", "Disconnect", 0));
	return;
    }

    QFont font;
    font.setPointSize(12);
    ui->label->setText("Connection Status: Disconnected");
    ui->label->setFont(font);
    ui->progressBar->setValue(0);
    ui->pushButton_7->setText(QApplication::translate("MainWindow", "Connect", 0));
    return;
}

void MainWindow::on_lineEdit_returnPressed()
{
   sendcommand();
}
