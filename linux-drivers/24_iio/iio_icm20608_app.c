#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/input.h>
#include <string.h>
// 浮点小数字符串转换为浮点数数值
#define SENSOR_FLOAT_DATA_GET(ret, index, str, member) \
    ret = file_data_read(file_path[index], str);       \
    icm20608->member = atof(str);                      \
// 整数字符串转换为整数数值
#define SENSOR_INT_DATA_GET(ret, index, str, member) \
    ret = file_data_read(file_path[index], str);     \
    icm20608->member = atoi(str);                    \
// icm20608 iio框架对应的文件路径
static char *file_path[] = {
    "/sys/bus/iio/devices/iio:device0/in_accel_scale",
    "/sys/bus/iio/devices/iio:device0/in_accel_x_calibbias",
    "/sys/bus/iio/devices/iio:device0/in_accel_x_raw",
    "/sys/bus/iio/devices/iio:device0/in_accel_y_calibbias",
    "/sys/bus/iio/devices/iio:device0/in_accel_y_raw",
    "/sys/bus/iio/devices/iio:device0/in_accel_z_calibbias",
    "/sys/bus/iio/devices/iio:device0/in_accel_z_raw",
    "/sys/bus/iio/devices/iio:device0/in_anglvel_scale",
    "/sys/bus/iio/devices/iio:device0/in_anglvel_x_calibbias",
    "/sys/bus/iio/devices/iio:device0/in_anglvel_x_raw",
    "/sys/bus/iio/devices/iio:device0/in_anglvel_y_calibbias",
    "/sys/bus/iio/devices/iio:device0/in_anglvel_y_raw",
    "/sys/bus/iio/devices/iio:device0/in_anglvel_z_calibbias",
    "/sys/bus/iio/devices/iio:device0/in_anglvel_z_raw",
    "/sys/bus/iio/devices/iio:device0/in_temp_offset",
    "/sys/bus/iio/devices/iio:device0/in_temp_raw",
    "/sys/bus/iio/devices/iio:device0/in_temp_scale",
};
enum path_index
{
    IN_ACCEL_SCALE = 0,
    IN_ACCEL_X_CALIBBIAS,
    IN_ACCEL_X_RAW,
    IN_ACCEL_Y_CALIBBIAS,
    IN_ACCEL_Y_RAW,
    IN_ACCEL_Z_CALIBBIAS,
    IN_ACCEL_Z_RAW,
    IN_ANGLVEL_SCALE,
    IN_ANGLVEL_X_CALIBBIAS,
    IN_ANGLVEL_X_RAW,
    IN_ANGLVEL_Y_CALIBBIAS,
    IN_ANGLVEL_Y_RAW,
    IN_ANGLVEL_Z_CALIBBIAS,
    IN_ANGLVEL_Z_RAW,
    IN_TEMP_OFFSET,
    IN_TEMP_RAW,
    IN_TEMP_SCALE,
};
struct ICM20608
{
    int accel_x_calibbias, accel_y_calibbias, accel_z_calibbias;
    int accel_x_raw, accel_y_raw, accel_z_raw;

    int gyro_x_calibbias, gyro_y_calibbias, gyro_z_calibbias;
    int gyro_x_raw, gyro_y_raw, gyro_z_raw;

    int temp_offset, temp_raw;

    float accel_scale, gyro_scale, temp_scale;

    float gyro_x_act, gyro_y_act, gyro_z_act;
    float accel_x_act, accel_y_act, accel_z_act;
    float temp_act;
};

int file_data_read(char *filename, char *str)
{
    int ret = 0;
    FILE *data_stream;

    data_stream = fopen(filename, "r");
    if (data_stream == NULL)
    {
        printf("can't open file %s\r\n", filename);
        return -1;
    }

    ret = fscanf(data_stream, "%s", str);
    if (!ret)
    {
        printf("file read error!\r\n");
    }
    else if (ret == EOF)
    {
        // 读到文件末尾将文件指针重新调整到文件头
        fseek(data_stream, 0, SEEK_SET);
    }
    fclose(data_stream);
    return 0;
}
static int sensor_read(struct ICM20608 *icm20608)
{
    int ret = 0;
    char str[50];

    // 获取陀螺仪原始数据
    SENSOR_FLOAT_DATA_GET(ret, IN_ANGLVEL_SCALE, str, gyro_scale);
    SENSOR_INT_DATA_GET(ret, IN_ANGLVEL_X_RAW, str, gyro_x_raw);
    SENSOR_INT_DATA_GET(ret, IN_ANGLVEL_Y_RAW, str, gyro_y_raw);
    SENSOR_INT_DATA_GET(ret, IN_ANGLVEL_Z_RAW, str, gyro_z_raw);

    // 获取加速度计原始数据
    SENSOR_FLOAT_DATA_GET(ret, IN_ACCEL_SCALE, str, accel_scale);
    SENSOR_INT_DATA_GET(ret, IN_ACCEL_X_RAW, str, accel_x_raw);
    SENSOR_INT_DATA_GET(ret, IN_ACCEL_Y_RAW, str, accel_y_raw);
    SENSOR_INT_DATA_GET(ret, IN_ACCEL_Z_RAW, str, accel_z_raw);

    // 获取温度值
    SENSOR_FLOAT_DATA_GET(ret, IN_TEMP_SCALE, str, temp_scale);
    SENSOR_INT_DATA_GET(ret, IN_TEMP_OFFSET, str, temp_offset);
    SENSOR_INT_DATA_GET(ret, IN_TEMP_RAW, str, temp_raw);

    // 转换为实际数值
    icm20608->accel_x_act = icm20608->accel_x_raw * icm20608->accel_scale;
    icm20608->accel_y_act = icm20608->accel_y_raw * icm20608->accel_scale;
    icm20608->accel_z_act = icm20608->accel_z_raw * icm20608->accel_scale;

    icm20608->gyro_x_act = icm20608->gyro_x_raw * icm20608->gyro_scale;
    icm20608->gyro_y_act = icm20608->gyro_y_raw * icm20608->gyro_scale;
    icm20608->gyro_z_act = icm20608->gyro_z_raw * icm20608->gyro_scale;

    icm20608->temp_act = ((icm20608->temp_raw - icm20608->temp_offset) / icm20608->temp_scale) + 25;
    return ret;
}

int main(int argc, char *argv[])
{
    struct ICM20608 icm20608;
    int ret = 0;

    if (argc != 1)
    {
        printf("Error Usage!\r\n");
        return -1;
    }

    while (1)
    {
        ret = sensor_read(&icm20608);
        if (ret == 0)
        {
            printf("\r\n 原始值:\r\n");
            printf("gx = %d, gy = %d, gz = %d\r\n",
                   icm20608.gyro_x_raw,
                   icm20608.gyro_y_raw,
                   icm20608.gyro_z_raw);
            printf("ax = %d, ay = %d, az = %d\r\n",
                   icm20608.accel_x_raw,
                   icm20608.accel_y_raw,
                   icm20608.accel_z_raw);
            printf("temp = %d\r\n", icm20608.temp_raw);
            printf("实际值:");
            printf("act gx = %.2f°/S, act gy = %.2f°/S,act gz = %.2f°/S\n", icm20608.gyro_x_act,
                   icm20608.gyro_y_act, icm20608.gyro_z_act);
            printf("act ax = %.2fg, act ay = %.2fg,act az = %.2fg\n", icm20608.accel_x_act,
                   icm20608.accel_y_act, icm20608.accel_z_act);
            printf("act temp = %.2f°C\r\n", icm20608.temp_act);
        }
        usleep(500000); // 100ms
    }

    return 0;
}