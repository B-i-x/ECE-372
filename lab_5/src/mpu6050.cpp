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

mpu6050_data mpu_data;

void wake_mpu6050() {
    StartI2C_Trans(SLA);
  //status = TWSR & 0xF8;
  write(PWR_MGMT);// address on SLA for Power Management
  write(WAKEUP); // send data to Wake up from sleep mode

  StopI2C_Trans();
}

mpu6050_data read_mpu6050_data(bool debug = false) {
    int T_val;
    Read_from(SLA,SL_MEMA_XAX_HIGH); 
    // status = TWSR & 0xF8;
    T_val= Read_data(); // read upper value
    Read_from(SLA,SL_MEMA_XAX_LOW);
    T_val = (T_val << 8)| Read_data(); // append lower value
    mpu_data.accel_x = T_val;
    Read_from(SLA,SL_MEMA_YAX_HIGH);
    T_val= Read_data(); // read upper value
    Read_from(SLA,SL_MEMA_YAX_LOW);
    T_val = (T_val << 8 )| Read_data(); // append lower value
    mpu_data.accel_y = T_val;
    Read_from(SLA,SL_MEMA_ZAX_HIGH); 
    // status = TWSR & 0xF8;
    T_val= Read_data(); // read upper value
    Read_from(SLA,SL_MEMA_ZAX_LOW);
    T_val = (T_val << 8)| Read_data(); // append lower value
    mpu_data.accel_z = T_val;
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
    const int x_accel_setpoint = 1300; // Define a threshold for "flat" position
    const int x_tolerance = 1000;

    const int y_accel_setpoint = 0;
    const int y_tolerance = 1000;

    const int z_accel_setpoint = 16000; // Assuming 1g = 16000 in raw data
    const int z_tolerance = 5000;

    bool x_flat = (data.accel_x >= (x_accel_setpoint - x_tolerance)) && (data.accel_x <= (x_accel_setpoint + x_tolerance));
    // Serial.print("X flat: ");
    // Serial.print(x_flat);
    bool y_flat = (data.accel_y >= (y_accel_setpoint - y_tolerance)) && (data.accel_y <= (y_accel_setpoint + y_tolerance));
    // Serial.print("  Y flat: ");
    // Serial.print(y_flat);
    bool z_flat = (data.accel_z >= (z_accel_setpoint - z_tolerance)) && (data.accel_z <= (z_accel_setpoint + z_tolerance));
    // Serial.print("  Z flat: ");
    // Serial.println(z_flat);

    return x_flat && y_flat && z_flat;
}