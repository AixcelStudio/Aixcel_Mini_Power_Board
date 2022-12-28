/*
******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2022 Aixcel Co.,Ltd</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of Aixcel nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#ifndef POWER_CTRL_PROCESS_H
#define POWER_CTRL_PROCESS_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "common.h"

#define  BATTERY_CHG_VOLTAGE_MAX_DEFAULT               25600    /* 1mV/bit */

#define  BATTERY_CHG_VOLTAGE_MIN_DEFAULT               22200    /* 1mV/bit */

#define  BATTERY_CHG_CURRENT_MAX_DEFAULT               10000    /* 1mA/bit */

#define  BATTERY_CHG_CURRENT_MIN_DEFAULT               10000    /* 1mA/bit */

#define  BATTERY_SOC_100_VOLTAGE_DEFAULT               25200    /* 1mV/bit */

#define  BATTERY_SOC_000_VOLTAGE_DEFAULT               18000    /* 1mV/bit */

#define  BATTERY_TEMPERATURE_MAX_DEFAULT                8000    /* 0.01¡æ/bit */

#define  BATTERY_TEMPERATURE_MIN_DEFAULT               -1000    /* 0.01¡æ/bit */

#define  PWR_SW_KEY_FILTER_TICKS_DEFAULT                2000    /* 1ms/bit */

#define  PWR_SHUTDOWN_WAIT_TICKS_DEFAULT                3000    /* 1ms/bit */

typedef enum
{
    ADC_Convertion_U_PWR_I,
    ADC_Convertion_M_PWR_I,
    ADC_Convertion_U_PWR_V,
    ADC_Convertion_M_PWR_V,
    ADC_Convertion_BAT_OUT_V,
    ADC_Convertion_BAT_OUT_I,
    ADC_Convertion_BAT_CHG_V,
    ADC_Convertion_BAT_CHG_I,
    ADC_Convertion_INPUT_CHG_V,
    ADC_Convertion_VCC_5V_I,
    ADC_Convertion_CNT,
}ADC_Convertion_Rank_Typedef;

extern uint16_t ADC_Convertion_Value[ADC_Convertion_CNT];

typedef enum
{
    BAT_IS_UNCHARG,
    BAT_IS_DOCKCHG,
    BAT_IS_WIRECHG,
} CHG_State_Typedef;

typedef enum
{
    PWR_IS_OFF,
    PWR_IS_TURNING_ON,
    PWR_IS_ON,
    PWR_IS_TURNING_OFF,
} PWR_State_Typedef;

int32_t Get_U_PWR_I_Val(void);

int32_t Get_M_PWR_I_Val(void);

int32_t Get_U_PWR_V_Val(void);

int32_t Get_M_PWR_V_Val(void);

int32_t Get_BAT_OUT_V_Val(void);

int32_t Get_BAT_OUT_I_Val(void);

int32_t Get_BAT_CHG_V_Val(void);

int32_t Get_BAT_CHG_I_Val(void);

int32_t Get_INPUT_CHG_V_Val(void);

int32_t Get_VCC_5V_I_Val(void);

void Power_Ctrl_Process(void);

#ifdef __cplusplus
}
#endif

#endif /* POWER_CTRL_PROCESS_H */
