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

#include "temp_detc_process.h"
#include "M117_MTS01_I2C.h"

static MY_M117B_Process_State_Typedef MY_M117B_Process_State = {0x00};
   
void MY_M117B_Process(void)
{
    if(MY_M117B_Process_State.ctrl_flag == 0)
    {
        MY_ConvertTemp();
        
        MY_M117B_Process_State.ctrl_flag = 1;
        
        MY_M117B_Process_State.measure_start_tick = Systick_Get();
        
        return;
    }
    
    if(MY_M117B_Process_State.wait_flag == 0)
    {
        if(Systick_Get() - MY_M117B_Process_State.measure_start_tick < 15) 
            return;
        else
            MY_M117B_Process_State.wait_flag = 1;
    }
    
    if(MY_M117B_Process_State.read_flag == 0)
    {
        MY_ReadTempWaiting(&(MY_M117B_Process_State.temperature_raw));
        
        MY_M117B_Process_State.temperature_val = MY_OutputtoTemp((int16_t)MY_M117B_Process_State.temperature_raw);
        
        MY_M117B_Process_State.read_flag = 1;
        
        MY_M117B_Process_State.measure_finish_tick = Systick_Get();
        
        printf("M117 Temp: %.3f %2X \r\n", MY_M117B_Process_State.temperature_val, MY_M117B_Process_State.temperature_raw);
    }
    
    if(MY_M117B_Process_State.read_flag == 1)
    {
        if(Systick_Get() - MY_M117B_Process_State.measure_finish_tick < 100) 
        {
            return;
        }
        else
        {
            MY_M117B_Process_State.ctrl_flag = 0;
  
            MY_M117B_Process_State.wait_flag = 0;
  
            MY_M117B_Process_State.read_flag = 0;
            
            MY_M117B_Process_State.temperature_raw = 0;
        }
    }
}

float Get_M117B_Temperature_Val(void)
{
    return MY_M117B_Process_State.temperature_val;
}

