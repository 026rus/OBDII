#include "serialselector.h"
#include "ui_serialselector.h"

SerialSelector::SerialSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialSelector)
{
    ui->setupUi(this);
}

SerialSelector::~SerialSelector()
{
    delete ui;
}
