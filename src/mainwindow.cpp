#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "qcustomplot.h"
#include "obd2client.h"
#include "ParseJson.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->conn = new serial::PortReaderWriter();

    ui->setupUi(this);
    setWindowTitle("Group 2 Software Engineering ODBII Reader");

    /* disable these widgets when there is no connection present */
    ui->submitButton->setDisabled(true);
    ui->checkEngineButton->setDisabled(true);
    ui->monitorButton->setDisabled(true);

    speedCount = 1;
    rpmCount = 1;

    visibility = true;
    speedClicked = false;
    rpmClicked = false;

    // register the plot document object (only needed once, no matter how many plots will be in the QTextDocument):
    QCPDocumentObject *plotObjectHandler = new QCPDocumentObject(this);
    ui->textEdit->document()->documentLayout()->registerHandler(QCPDocumentObject::PlotTextFormat, plotObjectHandler);

    QFont font;
    font.setPointSize(12);
    ui->statusLabel->setFont(font);
    ui->statusLabel->setText("Connection Status: Disconnected");

    ui->connectStatus->setValue(0);

    ui->customPlot->addGraph();
    ui->customPlot->addGraph();
    ui->customPlot->legend->setVisible(true);
    QFont legendFont;  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    ui->customPlot->legend->setFont(legendFont);

    ui->distanceTraveledBox->setDisabled(true);
    ui->engineCoolantBox->setDisabled(true);
    ui->engineLoadBox->setDisabled(true);
    ui->engineOilBox->setDisabled(true);
    ui->ethanolFuelBox->setDisabled(true);
    ui->fuelAirBox->setDisabled(true);
    ui->fuelLevelBox->setDisabled(true);
    ui->fuelPressureBox->setDisabled(true);
    ui->intakeAirBox->setDisabled(true);
    ui->intakeManifoldBox->setDisabled(true);
    ui->runTimeBox->setDisabled(true);
    ui->throtlePositionBox->setDisabled(true);
    ui->barometricPressureBox->setDisabled(true);
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

void MainWindow::on_checkEngineButton_clicked() {
    QString trobelCode = this->conn->queryOBDErrorCodes();
    QString rawCodes = this->conn->decodeOBDErrors(trobelCode);

    /*Use Commented rawCodes if above code is not working. Hard codes literal codes if decode broken.
    Example decoded value = "B0001\r\nC0035";*/
    //Code below takes rawCodes, seperates them, and iterates them to find descriptions.
    QStringList deliminatedCodes = rawCodes.split("\n", QString::SkipEmptyParts);
    ParseJson parser = ParseJson();
    foreach(const QString &data, deliminatedCodes){
	    QString description = parser.getDesc(data);
	    if (description.contains("Code Unknown")) {
		description += " " + data;
	    }

            qDebug() << "code: " <<qPrintable(data);
            qDebug() << "Some thinfg: "<< description;
            ui->outputBrowser->append(description);
    }
}

void MainWindow::on_monitorButton_clicked() {
    int vehicleSpeed = conn->decodeVehicleSpeed(conn->queryVehicleSpeed());
    ui->outputBrowser->setText(  QString::number(vehicleSpeed)  );

    if(vehicleSpeed < 0) {
        vspeed.append(0);
    } else {
        vspeed.append(vehicleSpeed);
    }
    speedCount++;

    int rpmVal = conn->decodeRPM(conn->queryRPM());
    qDebug() << "Actual rpmVal: " << rpmVal;
    ui->outputBrowser->setText( QString::number(rpmVal) );

    if (rpmVal < 0) {
       vrpm.append(0);
    } else {
        vrpm.append(rpmVal/100);
    }
   rpmCount++;

    // not necessary now, but the code to set
    // the other buttons to disabled while
    // monitoring the RPM
    ui->checkEngineButton->setDisabled(visibility);
    ui->submitButton->setDisabled(visibility);
    ui->connectButton->setDisabled(visibility);
    visibility = !visibility;

    speedClicked = !speedClicked;
    rpmClicked = !rpmClicked;
    on_rpmBox_clicked();
    on_speedBox_clicked();
    ui->customPlot->graph(0)->setName("RPM");
    ui->customPlot->graph(1)->setName("Speed");
}

void MainWindow::on_submitButton_clicked() { sendRawData(); }

void MainWindow::on_speedBox_clicked() {
    QString name = "speed";
    setupGraph(ui->customPlot, name, speedClicked); }

void MainWindow::on_rpmBox_clicked() {
    QString name = "rpm";
    setupGraph(ui->customPlot, name, rpmClicked); }

void MainWindow::sendRawData() {
    QString instr = ui->inputEdit->text();

    if(!conn->sendCommand( instr.toUtf8() )) {
        ui->outputBrowser->append( "Problem writing !!!!");
    }
    QString buff = conn->readAll();
    QString tempstr = "Buff size 1: ";
    tempstr += QString::number(buff.size());
    ui->outputBrowser->append( tempstr );

    buff.remove(0, instr.size()+1);

    tempstr = "Buff size 2: ";
    tempstr += QString::number( buff.size() );
    ui->outputBrowser->append( tempstr );

    tempstr = "(";
    tempstr += buff;
    tempstr += ")" ;
    ui->outputBrowser->append( tempstr );
    ui->inputEdit->selectAll();
}

void MainWindow::setupGraph(QCustomPlot *customPlot, QString dataName, bool &dataClicked)
{
    QVector<double> data;
    int count;
    int graphNumb;
    QPen graphColor = QPen(Qt::red);
    if("rpm" == dataName){
        data = vrpm;
        count = rpmCount;
        graphNumb = 0;
    }
    else if ("speed" == dataName){
        data = vspeed;
        count = speedCount;
        graphNumb = 1;
	graphColor = QPen(Qt::blue);
    }

    QVector<double> c;
  if (dataClicked == false){
      for (int i=0; i<=count; i++)
      {
          c.append(i);
      }

      // create graph and assign data to it:
      //customPlot->addGraph();
      customPlot->graph(graphNumb)->setData(c,data);

      // give the axes some labels:
      customPlot->xAxis->setLabel("Count");
      customPlot->yAxis->setLabel("Speed (MPH) and RPMx100");

      // set axes ranges, so we see all data:
      customPlot->xAxis->setRange(0, count - 1);
      customPlot->yAxis->setRange(0, 100);
      customPlot->graph(graphNumb)->setPen(graphColor); // line color blue for first graph
      customPlot->graph(graphNumb)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
//      customPlot->graph(graphNumb)->setBrush(QBrush(Qt::lightGray)); // first graph will be filled with translucent blue
      customPlot->graph(graphNumb)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 10));
  }
  else{
      ui->customPlot->removeGraph(graphNumb);
      ui->customPlot->addGraph();
  }
  dataClicked = !dataClicked;
  customPlot->graph(0)->setName("RPM");
  customPlot->graph(1)->setName("Speed");
  customPlot->replot();
}

void MainWindow::on_connectButton_clicked()
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
        QString text = "Connected: " + input_device;
        ui->statusLabel->setText(text);
        ui->statusLabel->setFont(font);
        ui->connectStatus->setValue(100);
        ui->connectButton->setText(QApplication::translate("MainWindow", "Disconnect", 0));

        /* enable these widgets when there is a connection present */
        ui->checkEngineButton->setEnabled(true);
        ui->submitButton->setEnabled(true);
        ui->monitorButton->setEnabled(true);
        return;
    }

    QFont font;
    font.setPointSize(12);
    ui->statusLabel->setText("Connection Status: Disconnected");
    ui->statusLabel->setFont(font);
    ui->connectStatus->setValue(0);
    ui->connectButton->setText(QApplication::translate("MainWindow", "Connect", 0));

    /* We disconnected.  We should clean up the PortReaderWriter and get new buffers. */
    delete this->conn;
    this->conn = new serial::PortReaderWriter();

    /* disable these widgets when there is no connection */
    ui->checkEngineButton->setDisabled(true);
    ui->submitButton->setDisabled(true);
    ui->monitorButton->setDisabled(true);

    return;
}

void MainWindow::on_inputEdit_returnPressed() { sendRawData(); }

void MainWindow::on_addGraphButton_clicked()
{
  ui->textEdit->setText("");
  QTextCursor cursor = ui->textEdit->textCursor();

  // insert the current plot at the cursor position. QCPDocumentObject::generatePlotFormat creates a
  // vectorized snapshot of the passed plot (with the specified width and height) which gets inserted
  // into the text document.
  double width = 480;
  double height = 340;
  cursor.insertText(QString(QChar::ObjectReplacementCharacter), QCPDocumentObject::generatePlotFormat(ui->customPlot, width, height));
  ui->textEdit->setTextCursor(cursor);

}

void MainWindow::on_saveGraphButton_clicked()
{
  QString fileName = QFileDialog::getSaveFileName(this, "Save document...", qApp->applicationDirPath(), "*.pdf");
  if ((!fileName.endsWith(".pdf")) && (!fileName.isEmpty())){
      fileName.append(".pdf");
  }
  if (!fileName.isEmpty())
  {
    QPrinter printer(QPrinter::HighResolution);
    printer.setFullPage(true);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    ui->textEdit->document()->print(&printer);
  }
}

void MainWindow::on_uploadButton_clicked(){
}
