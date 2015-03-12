#include "CarCoreApp.h"

CarCoreApp::CarCoreApp(int &argc, char** argv) : QCoreApplication(argc, argv) {
    connect(this, SIGNAL( aboutToQuit() ), SLOT( cleanupProgramAtExit() ) );
}

void CarCoreApp::run() {
    QCoreApplication::exec();
}

void CarCoreApp::cleanupProgramAtExit(){
    // Nothing for now
}

CarCoreApp::~CarCoreApp(){
    // No resources to clean yet
}
