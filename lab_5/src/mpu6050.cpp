#include "mpu6050.h"

#include <Arduino.h>

#include "i2c.h"

#define SLA 0x68  // MPU_6050 address with PIN AD0 grounded
#define PWR_MGMT  0x6B
#define WAKEUP 0x00

#define SL_MEMA_XAX_HIGH  0x3B
#define SL_MEMA_XAX_LOW   0x3C
#define SL_MEMA_YAX_HIGH  0x3D
#define SL_MEMA_YAX_LOW   0x3E
#define SL_MEMA_ZAX_HIGH  0x3F
#define SL_MEMA_ZAX_LOW   0x40
#define SL_TEMP_HIGH      0x41
#define SL_TEMP_LOW       0x42

#define SL_GYRO_XAX_HIGH  0x43
#define SL_GYRO_XAX_LOW   0x44
#define SL_GYRO_YAX_HIGH  0x45
#define SL_GYRO_YAX_LOW   0x46
#define SL_GYRO_ZAX_HIGH  0x47
#define SL_GYRO_ZAX_LOW   0x48

mpu6050_data mpu_data;

void wake_mpu6050() {
    StartI2C_Trans(SLA);
  //status = TWSR & 0xF8;
  write(PWR_MGMT);// address on SLA for Power Management
  write(WAKEUP); // send data to Wake up from sleep mode

  StopI2C_Trans();
}


int mpu_read_word(char high_reg, char low_reg) {
    int val;

    Read_from(SLA, high_reg);
    val = Read_data();          // high byte

    Read_from(SLA, low_reg);
    val = (val << 8) | Read_data();   // combine with low byte

    return val;
}

mpu6050_data read_mpu6050_data(bool debug = false) {
    mpu_data.accel_x = mpu_read_word(SL_GYRO_XAX_HIGH, SL_GYRO_XAX_LOW);
    mpu_data.accel_y = mpu_read_word(SL_GYRO_YAX_HIGH, SL_GYRO_YAX_LOW);
    mpu_data.accel_z = mpu_read_word(SL_GYRO_ZAX_HIGH, SL_GYRO_ZAX_LOW);

    StopI2C_Trans();
    StopI2C_Trans();

    if (debug) {
        Serial.print("T_x, T_y, T_z: ");
        Serial.print(mpu_data.accel_x);
        Serial.print(",");
        Serial.print(mpu_data.accel_y);
        Serial.print(",");
        Serial.print(mpu_data.accel_z);
        Serial.println();
    }

    return mpu_data;
}



bool is_sensor_laying_down(mpu6050_data data) {
    // Check if the sensor is laying flat based on accelerometer data
    const int x_accel_setpoint = -300; // Define a threshold for "flat" position
    const int x_tolerance = 100;

    const int y_accel_setpoint = 140;
    const int y_tolerance = 100;

    const int z_accel_setpoint = 10; 
    const int z_tolerance = 100;

    bool x_flat = (data.accel_x >= (x_accel_setpoint - x_tolerance)) && (data.accel_x <= (x_accel_setpoint + x_tolerance));
    // Serial.print("X flat: ");
    // Serial.print(x_flat);
    bool y_flat = (data.accel_y >= (y_accel_setpoint - y_tolerance)) && (data.accel_y <= (y_accel_setpoint + y_tolerance));
    // Serial.print("  Y flat: ");
    // Serial.print(y_flat);
    // bool z_flat = (data.accel_z >= (z_accel_setpoint - z_tolerance)) && (data.accel_z <= (z_accel_setpoint + z_tolerance));
    // Serial.print("  Z flat: ");
    // Serial.println(z_flat);

    return x_flat && y_flat;
}