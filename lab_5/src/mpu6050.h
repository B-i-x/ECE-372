#ifndef MPU6050_H
#define MPU6050_H

struct mpu6050_data {
    int accel_x;
    int accel_y;
    int accel_z;
};

void wake_mpu6050();

int mpu_read_word(char high_reg, char low_reg);

mpu6050_data read_mpu6050_data();

bool is_sensor_laying_down(mpu6050_data data);

void compute_accel_angles(mpu6050_data data, float *roll_deg, float *pitch_deg);

#endif