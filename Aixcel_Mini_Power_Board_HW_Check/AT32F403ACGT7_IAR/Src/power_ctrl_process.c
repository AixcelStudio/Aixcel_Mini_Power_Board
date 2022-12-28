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

#include "power_ctrl_process.h"
#include "hwconf.h"
#include "systick.h"
#include "temp_detc_process.h"

uint16_t ADC_Convertion_Value[ADC_Convertion_CNT] = {0x00};

/****************************BAT_CHG_Ctrl******************************/
static FunctionalState BAT_CHG_State = DISABLE;

void BAT_CHG_Ctrl(FunctionalState new_state)
{
    if(BAT_CHG_State == new_state)
        return;
    
    if(new_state == ENABLE)
    {
        CHG_EN_PULL_UP() ;
        
        BAT_CHG_State = ENABLE;
    }
    else if(new_state == DISABLE)
    {
        CHG_EN_PULL_DOWN() ;
        
        BAT_CHG_State = DISABLE;
    }
}

FunctionalState Get_BAT_CHG_State(void)
{
    return BAT_CHG_State;
}

/*****************************BAT_OUT_Ctrl*****************************/
static FunctionalState BAT_OUT_State = DISABLE;

void BAT_OUT_Ctrl(FunctionalState new_state)
{
    if(BAT_OUT_State == new_state)
        return;
    
    if(new_state == ENABLE)
    {
        VBAT_EN_PULL_UP() ;
        
        BAT_OUT_State = ENABLE;
    }
    else if(new_state == DISABLE)
    {
        VBAT_EN_PULL_DOWN() ;
        
        BAT_OUT_State = DISABLE;
    }
}

FunctionalState Get_BAT_OUT_State(void)
{
    return BAT_OUT_State;
}

/******************************U_PWR_Ctrl******************************/
static FunctionalState U_PWR_State = DISABLE;

void U_PWR_Ctrl(FunctionalState new_state)
{
    if(new_state == ENABLE)
    {
        U_PWR_EN_PULL_UP();
        
        U_PWR_State = ENABLE;
    }
    else if(new_state == DISABLE)
    {
        U_PWR_EN_PULL_DOWN();
        
        U_PWR_State = DISABLE;
    }
}

FunctionalState Get_U_Power_State(void)
{
    return U_PWR_State;
}

/******************************M_PWR_Ctrl******************************/
static FunctionalState M_PWR_State = DISABLE;

void M_PWR_Ctrl(FunctionalState new_state)
{
    if(new_state == ENABLE)
    {
        M_PWR_EN_PULL_UP();
        
        M_PWR_State = ENABLE;
    }
    else if(new_state == DISABLE)
    {
        M_PWR_EN_PULL_DOWN();
        
        M_PWR_State = DISABLE;
    }
}

FunctionalState Get_M_PWR_State(void)
{
    return M_PWR_State;
}

/*******************************U_PWR_I********************************/
static int32_t U_PWR_I_Val = 0x00;    /* 1mA/bit */

static void Calc_U_PWR_I_Val(void)
{
    U_PWR_I_Val = ADC_Convertion_Value[ADC_Convertion_U_PWR_I]*660*20/4096;
}

int32_t Get_U_PWR_I_Val(void)
{
    return U_PWR_I_Val;
}

/*******************************M_PWR_I********************************/
static int32_t M_PWR_I_Val = 0x00;    /* 1mA/bit */

static void Calc_M_PWR_I_Val(void)
{
    M_PWR_I_Val = ADC_Convertion_Value[ADC_Convertion_M_PWR_I]*660*20/4096;
}

int32_t Get_M_PWR_I_Val(void)
{
    return M_PWR_I_Val;
}

/*******************************U_PWR_V********************************/
static int32_t U_PWR_V_Val = 0x00;    /* 1mV/bit */

static void Calc_U_PWR_V_Val(void)
{
    U_PWR_V_Val = ADC_Convertion_Value[ADC_Convertion_U_PWR_V]*3300*16/4096;
}

int32_t Get_U_PWR_V_Val(void)
{
    return U_PWR_V_Val;
}

/*******************************M_PWR_V********************************/
static int32_t M_PWR_V_Val = 0x00;    /* 1mV/bit */

static void Calc_M_PWR_V_Val(void)
{
    M_PWR_V_Val = ADC_Convertion_Value[ADC_Convertion_M_PWR_V]*3300*16/4096;
}

int32_t Get_M_PWR_V_Val(void)
{
    return M_PWR_V_Val;
}

/******************************BAT_OUT_V********************************/
static int32_t BAT_OUT_V_Val = 0x00;    /* 1mV/bit */

static void Calc_BAT_OUT_V_Val(void)
{
    BAT_OUT_V_Val = ADC_Convertion_Value[ADC_Convertion_BAT_OUT_V]*3300*16/4096;
}

int32_t Get_BAT_OUT_V_Val(void)
{
    return BAT_OUT_V_Val;
}

/******************************BAT_OUT_I********************************/
static int32_t BAT_OUT_I_Val = 0x00;    /* 1mA/bit */

static void Calc_BAT_OUT_I_Val(void)
{
    BAT_OUT_I_Val = ADC_Convertion_Value[ADC_Convertion_BAT_OUT_I]*660*20/4096;
}

int32_t Get_BAT_OUT_I_Val(void)
{
    return BAT_OUT_I_Val;
}

/******************************BAT_CHG_V********************************/
static int32_t BAT_CHG_V_Val = 0x00;    /* 1mV/bit */

static void Calc_BAT_CHG_V_Val(void)
{
    BAT_CHG_V_Val = ADC_Convertion_Value[ADC_Convertion_BAT_CHG_V]*3300*16/4096;
}

int32_t Get_BAT_CHG_V_Val(void)
{
    return BAT_CHG_V_Val;
}

/******************************BAT_CHG_I********************************/
static int32_t BAT_CHG_I_Val = 0x00;    /* 1mA/bit */

static void Calc_BAT_CHG_I_Val(void)
{
    BAT_CHG_I_Val = ADC_Convertion_Value[ADC_Convertion_BAT_CHG_I]*660*20/4096;
}

int32_t Get_BAT_CHG_I_Val(void)
{
    return BAT_CHG_I_Val;
}

/******************************INPUT_CHG_V******************************/
static int32_t INPUT_CHG_V_Val = 0x00;    /* 1mV/bit */

static void Calc_INPUT_CHG_V_Val(void)
{
    INPUT_CHG_V_Val = ADC_Convertion_Value[ADC_Convertion_INPUT_CHG_V]*3300*16/4096;
}

int32_t Get_INPUT_CHG_V_Val(void)
{
    return INPUT_CHG_V_Val;
}

/*******************************VCC_5V_I********************************/
static int32_t VCC_5V_I_Val = 0x00;    /* 1mA/bit */

static void Calc_VCC_5V_I_Val(void)
{
    VCC_5V_I_Val = ADC_Convertion_Value[ADC_Convertion_VCC_5V_I]*165*20/4096;
}

int32_t Get_VCC_5V_I_Val(void)
{
    return VCC_5V_I_Val;
}

/**************************Charge_Ctrl_Process**************************/
static CHG_State_Typedef CHG_State = BAT_IS_UNCHARG;

static void Charge_Ctrl_Process(void)
{
    if(CHG_State == BAT_IS_UNCHARG)
    {
        if((INPUT_CHG_V_Val >= BATTERY_CHG_VOLTAGE_MIN_DEFAULT) && (INPUT_CHG_V_Val <= BATTERY_CHG_VOLTAGE_MAX_DEFAULT) && (BAT_OUT_V_Val < BATTERY_SOC_100_VOLTAGE_DEFAULT) && ((INPUT_CHG_V_Val-BAT_CHG_V_Val) > 200))
        {
            BAT_CHG_Ctrl(ENABLE);
            
            if(!GPIO_ReadInputDataBit(WIRE_SIG_GPIO_Port, WIRE_SIG_Pin))
            {
                CHG_State = BAT_IS_WIRECHG;
            }
            else
            {
                CHG_State = BAT_IS_DOCKCHG;
            }
        }
    }
    
    if(CHG_State != BAT_IS_UNCHARG)
    {
        if((INPUT_CHG_V_Val < BATTERY_CHG_VOLTAGE_MIN_DEFAULT) || (BAT_CHG_I_Val > BATTERY_CHG_CURRENT_MAX_DEFAULT) || (BAT_CHG_V_Val > BATTERY_SOC_100_VOLTAGE_DEFAULT) || (abs(INPUT_CHG_V_Val-BAT_CHG_V_Val) < 150))
        {
            BAT_CHG_Ctrl(DISABLE);
            
            CHG_State = BAT_IS_UNCHARG;
        }
    }
}

/*************************Power_Switch_Process**************************/
static PWR_State_Typedef PWR_State = PWR_IS_OFF;

static Key_State_Typedef PWR_SW_Key_State = Key_IS_IDLE;
    
static uint32_t PWR_SW_Key_Pressed_Tick_Record  = 0x00;
static uint32_t PWR_SW_Key_Released_Tick_Record = 0x00;

static void Power_Switch_Process(void)
{
    if(!GPIO_ReadInputDataBit(SW_STATE_GPIO_Port, SW_STATE_Pin))
    {
        if(PWR_SW_Key_State == Key_IS_IDLE)
        {
            PWR_SW_Key_State = Key_IS_PRESSED;
        
            PWR_SW_Key_Pressed_Tick_Record = Systick_Get();
        }
    }
    else
    {
        if(PWR_SW_Key_State == Key_IS_PRESSED)
        {
            PWR_SW_Key_State = Key_IS_RELEASED;
        
            PWR_SW_Key_Released_Tick_Record = Systick_Get();
        }
    }
    
    if(PWR_SW_Key_State == Key_IS_PRESSED)
    {
        if(Systick_Diff_Get(Systick_Get(), PWR_SW_Key_Pressed_Tick_Record) > PWR_SW_KEY_FILTER_TICKS_DEFAULT)
        {
            if(!GPIO_ReadInputDataBit(VBAT_EN_GPIO_Port, VBAT_EN_Pin))
            {
                BAT_OUT_Ctrl(ENABLE);
            }
        }
    }
    
    if(PWR_SW_Key_State == Key_IS_RELEASED)
    {
        if(Systick_Diff_Get(PWR_SW_Key_Released_Tick_Record, PWR_SW_Key_Pressed_Tick_Record) > PWR_SW_KEY_FILTER_TICKS_DEFAULT)
        {
            if(PWR_State == PWR_IS_OFF)
            {
                PWR_State = PWR_IS_TURNING_ON;
            }
            else if(PWR_State == PWR_IS_ON)
            {
                PWR_State = PWR_IS_TURNING_OFF;
            }
        }
        
        PWR_SW_Key_State = Key_IS_IDLE;
    } 
}

void Power_Ctrl_Process(void)
{
    static bool PWR_Prepare_Shutdown_Flag = false;
    static bool PWR_Execute_Shutdown_Flag = false;
    
    static uint32_t PWR_Prepare_Shutdown_Tick_Record = 0x00;
    static uint32_t PWR_Execute_Shutdown_Tick_Record = 0x00;
    
    Calc_U_PWR_I_Val();
    Calc_M_PWR_I_Val();
    Calc_U_PWR_V_Val();
    Calc_M_PWR_V_Val();
    
    Calc_BAT_OUT_V_Val();
    Calc_BAT_OUT_I_Val();
    Calc_BAT_CHG_V_Val();
    Calc_BAT_CHG_I_Val();
    
    Calc_INPUT_CHG_V_Val();
    Calc_VCC_5V_I_Val();
    
    Power_Switch_Process();
    
    Charge_Ctrl_Process();
        
    if((PWR_State == PWR_IS_TURNING_ON) && (Get_M117B_Temperature_Val() < BATTERY_TEMPERATURE_MAX_DEFAULT) && (Get_M117B_Temperature_Val() > BATTERY_TEMPERATURE_MIN_DEFAULT))
    {
        BAT_OUT_Ctrl(ENABLE);
        PWR_State = PWR_IS_ON;
        
        U_PWR_Ctrl(ENABLE);
        M_PWR_Ctrl(ENABLE);
    }
    
    if((PWR_State == PWR_IS_ON) && ((Get_M117B_Temperature_Val() >= BATTERY_TEMPERATURE_MAX_DEFAULT) || (Get_M117B_Temperature_Val() <= BATTERY_TEMPERATURE_MIN_DEFAULT)))
    {
        PWR_State = PWR_IS_TURNING_OFF;
    }
    
    if(PWR_State == PWR_IS_TURNING_OFF)
    {
        if(PWR_Prepare_Shutdown_Flag == false)
        {
            PWR_Prepare_Shutdown_Flag = true;
            PWR_Prepare_Shutdown_Tick_Record = Systick_Get();
        }
    
        if(Systick_Diff_Get(Systick_Get(), PWR_Prepare_Shutdown_Tick_Record) > PWR_SHUTDOWN_WAIT_TICKS_DEFAULT)
        {
            PWR_Execute_Shutdown_Tick_Record = Systick_Get();
            PWR_Execute_Shutdown_Flag = true;
            PWR_State = PWR_IS_OFF;
            
            BAT_OUT_Ctrl(DISABLE);
            BAT_CHG_Ctrl(DISABLE);
            
            U_PWR_Ctrl(DISABLE);
            M_PWR_Ctrl(DISABLE);
            
            PWR_Prepare_Shutdown_Flag = false;
            PWR_Execute_Shutdown_Flag = false;
        }
    }
}



