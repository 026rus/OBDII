#include "serialcomms.h"

#include <QtCore/QCoreApplication>


class CarCoreApp : public QCoreApplication {
    Q_OBJECT

public:
    CarCoreApp(int &argc, char ** argv[]);
     ~CarCoreApp();

    void run();

private:
    serial::SerialComms conn;

signals:
    void done();

private slots:
    void cleanupProgramAtExit();
};
