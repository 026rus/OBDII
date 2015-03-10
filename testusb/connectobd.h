#ifndef CONNECTOBD_H
#define CONNECTOBD_H
#include <cstdio>
#include <csignal>
#include <stdio.h>
#include <ftdi.h>
#include <string.h>
#include <iostream>

class ConnectOBD
{
private:
    struct ftdi_context ftdic;
    int ret;
    unsigned short modem_status;
    unsigned char * tx;
    unsigned char * rx;
    int tx_size;

    enum ftdi_bits_type bits;
    enum ftdi_stopbits_type sbit;
    enum ftdi_parity_type parity;
    enum ftdi_break_type lineend;


public:
    ConnectOBD();
    ~ConnectOBD();

    int setftdi();
    void writetdi();
    void set_tx(unsigned char*, int);
    void readtdi();
    void print();
    void promt();


};

#endif // CONNECTOBD_H
