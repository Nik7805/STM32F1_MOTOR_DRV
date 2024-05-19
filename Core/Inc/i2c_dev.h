#ifndef I2C_DEV_H
#define I2C_DEV_H
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "drv8833.h"
#include "common_structures.h"

typedef struct
{
    DRV8833_CH_HandleTypeDef* left_front_drv;
    DRV8833_CH_HandleTypeDef* left_rear_drv;
    DRV8833_CH_HandleTypeDef* right_front_drv;
    DRV8833_CH_HandleTypeDef* right_rear_drv;
    I2C_HandleTypeDef* hi2c;
}I2C_Dev_HandleTypeDef;

enum DevRegMap
{
    I2CDEV_DRVPWR_LF = 0,
    I2CDEV_DRVPWR_LR,
    I2CDEV_DRVPWR_RF,
    I2CDEV_DRVPWR_RR,
    I2CDEV_REGMAP_SIZE
};

void i2c_dev_init(I2C_Dev_HandleTypeDef* hdev);
void i2c_dev_process();

#ifdef __cplusplus
}
#endif
#endif