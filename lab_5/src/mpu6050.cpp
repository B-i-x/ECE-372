#include "mpu6050.h"

#include <Arduino.h>

#include "i2c.h"
#include "timer.h"

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

#define ACCEL_SCALE 16384.0f


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

mpu6050_data read_mpu6050_data() {
    // Read accelerometer registers, not gyro
    mpu_data.accel_x = mpu_read_word(SL_MEMA_XAX_HIGH, SL_MEMA_XAX_LOW);
    mpu_data.accel_y = mpu_read_word(SL_MEMA_YAX_HIGH, SL_MEMA_YAX_LOW);
    mpu_data.accel_z = mpu_read_word(SL_MEMA_ZAX_HIGH, SL_MEMA_ZAX_LOW);

    StopI2C_Trans();

    return mpu_data;
}

// Compute roll and pitch (in degrees) from accelerometer data
// roll  = rotation around X axis
// pitch = rotation around Y axis
void compute_accel_angles(mpu6050_data data, float *roll_deg, float *pitch_deg) {
    // Convert raw counts to g
    float ax = data.accel_x / ACCEL_SCALE;
    float ay = data.accel_y / ACCEL_SCALE;
    float az = data.accel_z / ACCEL_SCALE;

    // Protect against divide by zero
    if (az == 0.0f && ay == 0.0f) {
        *roll_deg = 0.0f;
    } else {
        *roll_deg = atan2f(ay, az) * 180.0f / M_PI;
    }

    float denom = sqrtf(ay * ay + az * az);
    if (denom == 0.0f) {
        *pitch_deg = 0.0f;
    } else {
        *pitch_deg = atan2f(-ax, denom) * 180.0f / M_PI;
    }
}

bool is_sensor_laying_down(mpu6050_data data) {
    // Use angle based check instead of raw counts

    float roll_deg = 0.0f;
    float pitch_deg = 0.0f;
    compute_accel_angles(data, &roll_deg, &pitch_deg);

    // Define what "laying down" means in terms of angles.
    // Example: both roll and pitch within +-10 degrees of 0.
    const float roll_setpoint   = 0.0f;
    const float roll_tolerance  = 30.0f;

    const float pitch_setpoint  = 0.0f;
    const float pitch_tolerance = 30.0f;

    bool roll_ok  = (roll_deg  >= (roll_setpoint  - roll_tolerance)) &&
                    (roll_deg  <= (roll_setpoint  + roll_tolerance));

    bool pitch_ok = (pitch_deg >= (pitch_setpoint - pitch_tolerance)) &&
                    (pitch_deg <= (pitch_setpoint + pitch_tolerance));

#ifdef DEBUG
    // Serial.print("accel: ");
    // Serial.print(data.accel_x);
    // Serial.print(", ");
    // Serial.print(data.accel_y);
    // Serial.print(", ");
    // Serial.println(data.accel_z);

    Serial.print("angles (deg) roll, pitch: ");
    Serial.print(roll_deg);
    Serial.print(", ");
    Serial.println(pitch_deg);
#endif

    delayMs(10);

    return roll_ok && pitch_ok;
}


// bool is_sensor_laying_down(mpu6050_data data) {
//     // Check if the sensor is laying flat based on accelerometer data
//     const int x_accel_setpoint = -300; // Define a threshold for "flat" position
//     const int x_tolerance = 500;

//     const int y_accel_setpoint = 140;
//     const int y_tolerance = 500;

//     // const int z_accel_setpoint = 10; 
//     // const int z_tolerance = 200;

//     bool x_flat = (data.accel_x >= (x_accel_setpoint - x_tolerance)) && (data.accel_x <= (x_accel_setpoint + x_tolerance));
//     // Serial.print("X flat: ");
//     // Serial.print(x_flat);
//     bool y_flat = (data.accel_y >= (y_accel_setpoint - y_tolerance)) && (data.accel_y <= (y_accel_setpoint + y_tolerance));
//     // Serial.print("  Y flat: ");
//     // Serial.print(y_flat);
//     // bool z_flat = (data.accel_z >= (z_accel_setpoint - z_tolerance)) && (data.accel_z <= (z_accel_setpoint + z_tolerance));
//     // Serial.print("  Z flat: ");
//     // Serial.println(z_flat);

//     #ifdef DEBUG
//         Serial.print("T_x, T_y: ");
//         Serial.print(mpu_data.accel_x);
//         Serial.print(",");
//         Serial.println(mpu_data.accel_y);
//     #endif

//     delayMs(10);
    

//     return x_flat && y_flat;
// }