#include <QtCore/QCoreApplication>

class CarCoreApp : public QCoreApplication {
    Q_OBJECT

public:
    CarCoreApp(int &argc, char ** argv);
     ~CarCoreApp();

    void run();

private slots:
    void cleanupProgramAtExit();
};
