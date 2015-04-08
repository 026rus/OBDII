#ifndef SERIALSELECTOR_H
#define SERIALSELECTOR_H

#include <QDialog>

namespace Ui {
class SerialSelector;
}

class SerialSelector : public QDialog
{
    Q_OBJECT

public:
    explicit SerialSelector(QWidget *parent = 0);
    ~SerialSelector();

private:
    Ui::SerialSelector *ui;
};

#endif // SERIALSELECTOR_H
