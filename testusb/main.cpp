#include <QCoreApplication>
#include <cstdio>
#include <csignal>
#include <stdio.h>
#include <ftdi.h>
#include <string.h>
#include <iostream>
#include "connectobd.h"


int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);

    ConnectOBD obd;
    obd.setftdi();

//    unsigned char x[]="ATI";
//    obd.set_tx(x,3);

//    obd.promt();
    obd.writetdi();

    obd.readtdi();
    obd.print();
    return 0;
//    return a.exec();
}
