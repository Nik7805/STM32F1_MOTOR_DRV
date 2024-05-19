#include <string.h>
#include <stdio.h>
#include "i2c_dev.h"

#define I2C_DEV_BUFFER_SIZE 4
#define I2C_DEV_TRANSACTION_SIZE 2

static uint32_t is_listening = 0;
static I2C_Dev_HandleTypeDef* i2c_dev_hdev = NULL;
static uint8_t tx_buffer[I2C_DEV_BUFFER_SIZE];
static uint8_t rx_buffer[I2C_DEV_BUFFER_SIZE];
static uint8_t reg_map[I2CDEV_REGMAP_SIZE];

static void i2c_dev_start_listen();
static int byte_to_int(uint8_t value);

void i2c_dev_init(I2C_Dev_HandleTypeDef *hdev)
{
    if(hdev == NULL)
    {
        Error_Handler();
    }
    
    i2c_dev_hdev = hdev;
    memset(tx_buffer, I2C_DEV_BUFFER_SIZE, 0);
    memset(rx_buffer, I2C_DEV_BUFFER_SIZE, 0);
    is_listening = 0;
}

void i2c_dev_process()
{
    if(is_listening == 0)
        i2c_dev_start_listen();
}

void i2c_dev_start_listen()
{
    if(HAL_I2C_EnableListen_IT(i2c_dev_hdev->hi2c) != HAL_OK)
    {
        /* Transfer error in reception process */
        // Error_Handler();
    }
    is_listening = 1;
}

int byte_to_int(uint8_t value)
{
    int result = (int)(value & 0x7F);

    if(value & 0x80)
    {
        result = -result;
    }

    return result;
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
    is_listening = 0;

    tx_buffer[0]++;
    tx_buffer[1]++;
    tx_buffer[2]++;
    tx_buffer[3]++;
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{

    is_listening = 0;
    // process data here.
    // tx_buffer[0]=rx_buffer[0];
    // tx_buffer[1]=rx_buffer[1];
    // tx_buffer[2]=rx_buffer[2];
    // tx_buffer[3]=rx_buffer[3];
    uint8_t reg_addr = rx_buffer[0];
    uint8_t reg_value = rx_buffer[1];
    if(reg_addr < I2CDEV_REGMAP_SIZE)
    {
        switch (reg_addr)
        {
        case I2CDEV_DRVPWR_LF:
            DRV8833_set_schar(i2c_dev_hdev->left_front_drv, byte_to_int(reg_value));
            break;
        case I2CDEV_DRVPWR_LR:
            DRV8833_set_schar(i2c_dev_hdev->left_rear_drv, byte_to_int(reg_value));
            break;
        case I2CDEV_DRVPWR_RF:
            DRV8833_set_schar(i2c_dev_hdev->right_front_drv, byte_to_int(reg_value));
            break;
        case I2CDEV_DRVPWR_RR:
            DRV8833_set_schar(i2c_dev_hdev->right_rear_drv, byte_to_int(reg_value));
            break;
        
        default:
            break;
        }
        reg_map[reg_addr] = rx_buffer[1];
    }

}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
    if (TransferDirection == I2C_DIRECTION_RECEIVE)
    {
        if (HAL_I2C_Slave_Seq_Transmit_IT(i2c_dev_hdev->hi2c, tx_buffer, I2C_DEV_BUFFER_SIZE, I2C_FIRST_AND_LAST_FRAME) != HAL_OK)
        {
            /* Transfer error in transmission process */
            Error_Handler();
        }
    }
    else
    {
        if (HAL_I2C_Slave_Seq_Receive_IT(i2c_dev_hdev->hi2c, rx_buffer, I2C_DEV_TRANSACTION_SIZE, I2C_FIRST_AND_LAST_FRAME) != HAL_OK)
        {
            /* Transfer error in reception process */
            Error_Handler();
        }
    }
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
    is_listening = 0;
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
    /** Error_Handler() function is called when error occurs.
    * 1- When Slave doesn't acknowledge its address, Master restarts communication.
    * 2- When Master doesn't acknowledge the last data transferred, Slave doesn't care in this example.
    */
    if (HAL_I2C_GetError(I2cHandle) != HAL_I2C_ERROR_AF)
    {
        Error_Handler();
    }

    is_listening = 0;
}