#ifndef DRV8833_H
#define DRV8833_H
#ifdef __cplusplus
extern "C" {
#endif

#include "tim.h"
#include <stdint.h>

typedef struct 
{
    TIM_HandleTypeDef* tim_handle;
    uint32_t pwm_ch1;
    uint32_t pwm_ch2;
} DRV8833_CH_HandleTypeDef;

void DRV8833_init(DRV8833_CH_HandleTypeDef* hdrv);
void DRV8833_set_raw(DRV8833_CH_HandleTypeDef* hdrv, int value);
void DRV8833_set_percent(DRV8833_CH_HandleTypeDef* hdrv, int value);
void DRV8833_set_schar(DRV8833_CH_HandleTypeDef* hdrv, int value);


#ifdef __cplusplus
}
#endif

#endif