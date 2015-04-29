#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <cctype>
#include "qcustomplot.h"
#include "obd2client.h"
#include "ParseJson.h"
#include "DataOut.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->conn = new serial::PortReaderWriter();
    this->connected = false;

    ui->setupUi(this);
    setWindowTitle("Group 2 Software Engineering ODBII Reader");
    this->conn->timeoutMillis = ui->sbTimeout->value();

    /* disable these widgets when there is no connection present */
    ui->submitButton->setDisabled(true);
    ui->checkEngineButton->setDisabled(true);
    ui->monitorButton->setDisabled(true);

    speedCount = 1;
    rpmCount = 1;

    visibility = true;
    speedClicked = false;
    rpmClicked = false;
    loadClicked = false;

    // register the plot document object (only needed once, no matter how many plots will be in the QTextDocument):
    QCPDocumentObject *plotObjectHandler = new QCPDocumentObject(this);
    ui->textEdit->document()->documentLayout()->registerHandler(QCPDocumentObject::PlotTextFormat, plotObjectHandler);

    QFont font;
    font.setPointSize(12);
    ui->statusLabel->setFont(font);
    ui->statusLabel->setText("Connection Status: Disconnected");

    ui->connectStatus->setValue(0);

    QFont legendFont;  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    ui->customPlot->legend->setFont(legendFont);

    for (int i = 0; i < 3; i++){
        ui->customPlot->addGraph();
        ui->customPlot->graph(i)->removeFromLegend();
    }

    ui->distanceTraveledBox->setDisabled(true);
    ui->engineCoolantBox->setDisabled(true);
    ui->engineLoadBox->setDisabled(false);
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
    visibility = !visibility;

    speedClicked = !speedClicked;
    rpmClicked = !rpmClicked;
    on_rpmBox_clicked();
    on_speedBox_clicked();
    ui->customPlot->graph(0)->setName("RPM");
    ui->customPlot->graph(1)->setName("Speed");
    ui->customPlot->graph(2)->setName("Engine Load");
}

void MainWindow::on_submitButton_clicked() { sendRawData(); }

void MainWindow::on_barometricPressureBox_clicked(){

}

void MainWindow::on_distanceTraveledBox_clicked(){

}

void MainWindow::on_engineCoolantBox_clicked(){

}

void MainWindow::on_engineLoadBox_clicked(){
    QString name = "load";
    setupGraph(ui->customPlot, name, loadClicked);

}

void MainWindow::on_engineOilBox_clicked(){

}

void MainWindow::on_ethanolFuelBox_clicked(){

}

void MainWindow::on_fuelAirBox_clicked(){

}

void MainWindow::on_fuelLevelBox_clicked(){

}

void MainWindow::on_fuelPressureBox_clicked(){

}

void MainWindow::on_intakeAirBox_clicked(){

}

void MainWindow::on_intakeManifoldBox_clicked(){

}

void MainWindow::on_runTimeBox_clicked(){

}

void MainWindow::on_throtlePositionBox_clicked(){

}

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

    // determine which set of data to use
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
    else if ("barometric" == dataName){
        data = vpressure;
        count = pressCount;
        graphNumb = 5;
    graphColor = QPen(Qt::blue);
    }
    else if ("distance" == dataName){
        data = vdistance;
        count = distanceCount;
        graphNumb = 3;
    graphColor = QPen(Qt::blue);
    }
    else if ("coolant" == dataName){
        data = vcooltemp;
        count = coolCount;
        graphNumb = 4;
    graphColor = QPen(Qt::blue);
    }
    else if ("load" == dataName){
        data = vload;
        count = rpmCount;
        graphNumb = 2;
    graphColor = QPen(Qt::green);
    }
    else if ("oil" == dataName){
        data = voiltemp;
        count = oilTempCount;
        graphNumb = 6;
    graphColor = QPen(Qt::blue);
    }
    else if ("fuelPercent" == dataName){
        data = vfuelpercent;
        count = fuelPercCount;
        graphNumb = 7;
    graphColor = QPen(Qt::blue);
    }
    else if ("fuelAir" == dataName){
        data = vratio;
        count = ratioCount;
        graphNumb = 8;
    graphColor = QPen(Qt::blue);
    }
    else if ("fuelLevel" == dataName){
        data = vlevel;
        count = levelCount;
        graphNumb = 9;
    graphColor = QPen(Qt::blue);
    }
    else if ("fuelPressure" == dataName){
        data = vfuelpressure;
        count = fuelPressCount;
        graphNumb = 10;
    graphColor = QPen(Qt::blue);
    }
    else if ("intakeTemp" == dataName){
        data = vairtemp;
        count = airTempCount;
        graphNumb = 11;
    graphColor = QPen(Qt::blue);
    }
    else if ("intakePress" == dataName){
        data = vmanifoldpressure;
        count = manPressCount;
        graphNumb = 12;
    graphColor = QPen(Qt::blue);
    }
    else if ("runTime" == dataName){
        data = vruntime;
        count = runTimeCount;
        graphNumb = 13;
    graphColor = QPen(Qt::blue);
    }
    else if ("throttlePos" == dataName){
        data = vthrotlepercent;
        count = throtlePercCount;
        graphNumb = 14;
    graphColor = QPen(Qt::blue);
    }

    QVector<double> c;

    // if the checkbox is clicked, graph the data
    if (dataClicked == false){
      for (int i=0; i<=count; i++)
      {
          c.append(i);
      }

      // create graph and assign data to it:
      customPlot->graph(graphNumb)->setVisible(true);
      customPlot->graph(graphNumb)->setData(c,data);

      // give the axes some labels:
      customPlot->xAxis->setLabel("Count");
      customPlot->yAxis->setLabel("Speed (KPH) and RPMx100");

      // set axes ranges, so we see all data:
      customPlot->xAxis->setRange(0, count - 1);
      customPlot->yAxis->setRange(0, 150);
      customPlot->graph(graphNumb)->setPen(graphColor); // line color blue for first graph
      customPlot->graph(graphNumb)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
      customPlot->graph(graphNumb)->addToLegend();
  }

  // else set the data to invisible
  else{
      ui->customPlot->graph(graphNumb)->setVisible(false);
      customPlot->graph(graphNumb)->setBrush(QBrush(QColor(0, 0, 0, 0)));
      customPlot->graph(graphNumb)->removeFromLegend();
  }
  dataClicked = !dataClicked;

  // Set graph names and colors
  customPlot->graph(0)->setName("RPM");
  customPlot->graph(1)->setName("Speed");
  customPlot->graph(2)->setName("Load Value");
  customPlot->graph(0)->setPen(QPen(Qt::red)); // line color blue for first graph
  customPlot->graph(1)->setPen(QPen(Qt::blue)); // line color blue for first graph
  customPlot->graph(2)->setPen(QPen(Qt::green)); // line color blue for first graph

  // setup legend for later development
  /*  customPlot->graph(5)->setName("Barometric Pressure");
  customPlot->graph(3)->setName("Distance Traveled");
  customPlot->graph(4)->setName("Coolant Temperature");
  customPlot->graph(6)->setName("Oil Temperature");
  customPlot->graph(7)->setName("Ethanol Percentage");
  customPlot->graph(8)->setName("Fuel/Air Ratio");
  customPlot->graph(9)->setName("Fuel Percentage");
  customPlot->graph(10)->setName("Fuel Pressure");
  customPlot->graph(11)->setName("Intake Temperature");
  customPlot->graph(12)->setName("Intake Pressure");
  customPlot->graph(13)->setName("Run Time");
  customPlot->graph(14)->setName("Throttle Position");*/

  // Set the legend invisible unless there is something being graphed
  if (ui->customPlot->legend->itemCount() == 0){
      customPlot->legend->setVisible(false);
  }
  else {
      customPlot->legend->setVisible(true);
  }

  //plot all changes
  customPlot->replot();
}

void MainWindow::on_connectButton_clicked()
{
    if (this->connected) {
        // Disconnecting from the port.  We should clean up and get new buffers. */
        this->conn->port->close();
        delete this->conn;
        this->connected = false;
        this->conn = new serial::PortReaderWriter();
        this->conn->timeoutMillis = ui->sbTimeout->value();

        QFont font;
        font.setPointSize(12);
        ui->statusLabel->setText("Connection Status: Disconnected");
        ui->statusLabel->setFont(font);
        ui->connectStatus->setValue(0);
        ui->connectButton->setText(QApplication::translate("MainWindow", "Connect", 0));

        /* disable these widgets when there is no connection */
        ui->checkEngineButton->setDisabled(true);
        ui->submitButton->setDisabled(true);
        ui->monitorButton->setDisabled(true);
        return;
    }

    // Connecting to the port
    QString input_device = "";
    QVector<QSerialPortInfo> ports = serial::PortReaderWriter::getAvailPorts();

    // Dirty dirty HACK!!!  Needs to be moved to a GUI Dialog
    foreach (const QSerialPortInfo &info, ports) {
        if (info.manufacturer().contains("FTDI")) {
            input_device = info.portName();
        }
    }

    // We try using the port anyway.  If all is working we can enable the buttons
    if (0 < input_device.length()) {
        if (this->conn->setPort(input_device.toStdString())) {
            this->connected = true;
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
    }

    this->connected = false;
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

  QVector<QPair<QString,QVector<double> > > collectedData = CollectData();
  DataOut *testThread = new DataOut(collectedData, false);
  testThread->start();


}

QVector<QPair<QString,QVector<double> > > MainWindow::CollectData(){

    auto collectedData = QVector<QPair<QString, QVector<double> > >();
    if (vspeed.size() > 0) collectedData.append(qMakePair(QString("vspeed"), vspeed));
    if (vrpm.size() > 0) collectedData.append(qMakePair(QString("vrpm"), vrpm));
    if (vpressure.size() > 0) collectedData.append(qMakePair(QString("vpressure"), vpressure));
    if (vdistance.size() > 0) collectedData.append(qMakePair(QString("vdistance"), vdistance));
    if (vcooltemp.size() > 0) collectedData.append(qMakePair(QString("vcooltemp"), vcooltemp));
    if (vload.size() > 0) collectedData.append(qMakePair(QString("vload"), vload));
    if (voiltemp.size() > 0) collectedData.append(qMakePair(QString("voiltemp"), voiltemp));
    if (vethanolpercent.size() > 0) collectedData.append(qMakePair(QString("vethanolpercent"), vethanolpercent));
    if (vratio.size() > 0) collectedData.append(qMakePair(QString("vratio"), vratio));
    if (vfuelpercent.size() > 0) collectedData.append(qMakePair(QString("vfuelpercent"), vfuelpercent));
    if (vfuelpressure.size() > 0) collectedData.append(qMakePair(QString("vfuelpressure"), vfuelpressure));
    if (vairtemp.size() > 0) collectedData.append(qMakePair(QString("vairtemp"), vairtemp));
    if (vmanifoldpressure.size() > 0) collectedData.append(qMakePair(QString("vmanifoldpressure"), vmanifoldpressure));
    if (vruntime.size() > 0) collectedData.append(qMakePair(QString("vruntime"), vruntime));
    if (vthrotlepercent.size() > 0) collectedData.append(qMakePair(QString("vthrotlepercent"), vthrotlepercent));

    return collectedData;
}

void MainWindow::on_uploadButton_clicked(){

    QVector<QPair<QString,QVector<double> > > collectedData = CollectData();
    DataOut *testThread = new DataOut(collectedData, true);
    testThread->start();

}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this,"About","Authors:\nZac Slade, Zac Wisdom, Vitaly Borodin, Joseph Jenkins\n\nCreated:\nSpring 2015");
}

void MainWindow::on_sbTimeout_valueChanged(int newVal)
{
    this->conn->timeoutMillis = newVal;
}

void MainWindow::on_jsonSave_clicked(){
    qDebug() << "Clicked the save to file button.";
    QVector<QPair<QString,QVector<double> > > collectedData = CollectData();
    DataOut *testThread = new DataOut(collectedData, false);
    testThread->start();
}
