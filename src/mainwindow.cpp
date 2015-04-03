#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_pushButton_clicked()
{
    ui->textBrowser->setText("Not connected to device!");
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->textBrowser->setText("Not connected to device!");
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->textBrowser->setText("Not connected to device!");
}

void MainWindow::on_pushButton_4_clicked()
{
    QString input = ui->lineEdit->text();
    ui->textBrowser->setText("Not connected to device!\nThe entered text was: " + input);
    ui->lineEdit->setText("");
}

//void MainWindow::on_pushButton_5_clicked()
//{
//    ui->textBrowser->setText("The 'Check Engine Temp' button has been pressed");
//}
