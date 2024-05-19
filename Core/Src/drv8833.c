#include "drv8833.h"

void DRV8833_init(DRV8833_CH_HandleTypeDef *hdrv)
{
    if(hdrv == NULL)
    {
        return;
    }

    HAL_TIM_PWM_Start(hdrv->tim_handle, hdrv->pwm_ch1);
    HAL_TIM_PWM_Start(hdrv->tim_handle, hdrv->pwm_ch2);
}

void DRV8833_set_raw(DRV8833_CH_HandleTypeDef *hdrv, int value)
{
    if(value >= 0)
    {
        __HAL_TIM_SET_COMPARE(hdrv->tim_handle, hdrv->pwm_ch1, (uint32_t)value);
        __HAL_TIM_SET_COMPARE(hdrv->tim_handle, hdrv->pwm_ch2, 0);
    }
    else
    {
        __HAL_TIM_SET_COMPARE(hdrv->tim_handle, hdrv->pwm_ch1, 0);
        __HAL_TIM_SET_COMPARE(hdrv->tim_handle, hdrv->pwm_ch2, (uint32_t)(-value));
    }
}

void DRV8833_set_percent(DRV8833_CH_HandleTypeDef *hdrv, int value)
{
    value = (value > 100) ? 100 : 
            (value < -100) ? -100 : 
            value;

    int ccr_val = (int)(hdrv->tim_handle->Init.Period) * value / 100;
    DRV8833_set_raw(hdrv, ccr_val);
}

void DRV8833_set_schar(DRV8833_CH_HandleTypeDef *hdrv, int value)
{
    const int max_value = 127;
    value = (value > max_value) ? max_value : 
            (value < -max_value) ? -max_value : 
            value;

    int ccr_val = (int)(hdrv->tim_handle->Init.Period) * value / max_value;
    DRV8833_set_raw(hdrv, ccr_val);
}
