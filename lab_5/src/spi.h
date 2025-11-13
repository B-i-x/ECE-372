#ifndef SPI_H
#define SPI_H

void spi_init();
void write_execute(unsigned char CMD, unsigned char data);
void write_happy_face();
void write_sad_face();
void screen_init();
#endif