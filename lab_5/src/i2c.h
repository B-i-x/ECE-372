#ifndef I2C_H
#define I2C_H

void initI2C();
void start_i2c_trans(unsigned char SLA);
void stop_i2c_trans();
void write(unsigned char data);
void read_from(unsigned char SLA, unsigned char MEMADDRESS);
unsigned char read_data();
#endif
