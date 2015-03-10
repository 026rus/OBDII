#include "connectobd.h"
#include <cstdio>
#include <csignal>
#include <stdio.h>
#include <ftdi.h>
#include <string.h>
#include <iostream>
#include "connectobd.h"


ConnectOBD::ConnectOBD()
{
    bits = BITS_8;
    sbit = STOP_BIT_1;
    parity = NONE;
    lineend = BREAK_OFF;
    modem_status = 0;

}

ConnectOBD::~ConnectOBD()
{
    if ((ret = ftdi_usb_close(&ftdic)) < 0)
    {
      fprintf(stderr, "unable to close ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
      return ;
    }

    ftdi_deinit(&ftdic);

    free(tx);
    free(rx);
}
int ConnectOBD::setftdi()
{
    if ((tx = (unsigned char*)malloc(128)) == NULL || (rx = (unsigned char*)malloc(128)) == NULL)
    {
      fprintf(stderr, "Error: could not allocate memory\n");
      return EXIT_FAILURE;
    }

    if (ftdi_init(&ftdic) < 0)
    {
      fprintf(stderr, "Error: could not initialize FTDI\n");
      return EXIT_FAILURE;
    }

    if ((ret = ftdi_usb_open(&ftdic, 0x0403, 0x6001)) < 0)
    {
      fprintf(stderr, "unable to open ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
      return EXIT_FAILURE;
    }

    if ((ret = ftdi_usb_reset(&ftdic)) < 0)
    {
      fprintf(stderr, "unable to reset ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
      return EXIT_FAILURE;
    }

    if ((ret = ftdi_usb_purge_buffers(&ftdic)) < 0)
    {
      fprintf(stderr, "unable to purge buffers on ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
      return EXIT_FAILURE;
    }

    if ((ret = ftdi_set_baudrate(&ftdic, 38400)) < 0)
    {
      fprintf(stderr, "unable to set ftdi device baud rate: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
      return EXIT_FAILURE;
    }

    if ((ret = ftdi_set_line_property2(&ftdic, bits, sbit, parity, lineend)) < 0)
    {
      fprintf(stderr, "unable to set ftdi device properties: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
      return EXIT_FAILURE;
    }


    ftdi_poll_modem_status(&ftdic, &modem_status);


    memcpy(tx, "AT E0\r", 6); /* turn echoing off */

    if (ftdi_write_data(&ftdic, tx, sizeof(unsigned char) * 6) < 0)
    {
      fprintf(stderr, "unable to send ftdi device data: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
      return EXIT_FAILURE;
    }

    return 0;
}
void ConnectOBD::promt()
{
    printf("R232 status: 0x%x \t=> ", modem_status);
    int tx_size = 0;
    while ( ( ret = getchar() ) != 0x0a && tx_size < 128) tx[tx_size++] = ret;
    tx[tx_size++] = 0x0d;
}

void ConnectOBD::writetdi()
{
    printf("R232 status: 0x%x \t=> ", modem_status);
    int tx_size = 0;
    while ( ( ret = getchar() ) != 0x0a && tx_size < 128) tx[tx_size++] = ret;
    tx[tx_size++] = 0x0d;

    ret = 0;

    ftdi_usb_purge_tx_buffer(&ftdic);
    ftdi_usb_purge_rx_buffer(&ftdic);


    for (int i = 0; i < tx_size; i++)
        ret += ftdi_write_data(&ftdic, tx+i, sizeof(unsigned char) * 1);
}

void ConnectOBD::set_tx(unsigned char * var, int x)
{
    tx_size = x;
    tx = var;
}

void ConnectOBD::readtdi()
{
    ret = 0;
    do
    {
      ret = ftdi_read_data(&ftdic, rx, 128);

    }while(ret == 0 );
}

void ConnectOBD::print()
{
    if (ret > 0)
    {

      ret -= 3; /* remove > prompt */

      printf("\t\t\t=> ");

      for (int i = 0; i < ret; i++) printf("%c",rx[i]);

      printf("\n");

    }
    else if (ret < 0)
    {
      fprintf(stderr, "unable to read from ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
      return;
    }
}
/*




























kjhghg

*/
