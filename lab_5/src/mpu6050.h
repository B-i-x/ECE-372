#ifndef MPU6050_H
#define MPU6050_H

struct mpu6050_data {
    int accel_x;
    int accel_y;
    int accel_z;
};

void wake_mpu6050();

mpu6050_data read_mpu6050_data(bool debug);

bool is_sensor_laying_down(mpu6050_data data);

#endif