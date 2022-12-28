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

#include "at32f4xx.h"
#include <stdio.h>
#include "main.h"
#include "systick.h"
#include "hwconf.h"
#include "common.h"
#include "endian_test.h"
#include "uart_drv.h"
#include "can_drv.h"
#include "spi_flash_process.h"
#include "MY_I2C_DRV.h"
#include "temp_detc_process.h"
#include "power_ctrl_process.h"
#include "m485_ctrl_process.h"
#include "sendwave.h"


/*!
    \brief      AT32F403ACGT7 set SRAM to 224K
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Extend_SRAM(void)
{
   /* check if RAM has been set to 224K, if not, change EOPB0 */
   if(((UOPTB->EOPB0) & 0xFF) != 0xFE)
   {
       /* Unlock Option Bytes Program Erase controller */
       FLASH_Unlock();
       /* Erase Option Bytes */
       FLASH_EraseUserOptionBytes();
 
       /* Change SRAM size to 224KB */
       FLASH_ProgramUserOptionByteData((uint32_t)&UOPTB->EOPB0,0xFE);
       NVIC_SystemReset();
   }
}

static void LED_Twinkle_Task(void);

static void M485_Packet_Process(void);

static void UART_Packet_Process(void);

static void Transfer_Data_Wave(bool en_flag);

static Hardware_SN_Typedef Hardware_SN = 
{
    .MCU_MFR = {'A', 'T'},
    
    .DEV_TYP = 'P',
    
    .PCB_VER = 0x01,
    
    .Unique_ID_L = 0x00,
  
    .Unique_ID_M = 0x00,
  
    .Unique_ID_H = 0x00,
};

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

int main(void)
{
    /* configure systick */
    Systick_Config();
    
    /* initilize the periph */
    System_GPIO_Init();
    
    Hardware_SN.Unique_ID_L = *(uint32_t*)UNIQUE_ID_L_ADDR;
    Hardware_SN.Unique_ID_M = *(uint32_t*)UNIQUE_ID_M_ADDR;
    Hardware_SN.Unique_ID_H = *(uint32_t*)UNIQUE_ID_H_ADDR;
    
    System_NVIC_Init();
    
    System_EXTI_Init();
    
    System_DMA_Init();
    
    System_RTC_Init();
    
    System_USART1_Init(UART_BAUDRATE_DEFAULR);
    
    System_USART3_Init(M485_BAUDRATE_DEFAULR);
    
    System_ADC1_Init();
    
    System_SPI2_Init(3000000);
    
  #ifndef USE_SIMULATE_I2C1
    System_I2C1_Init(400000);
  #else
    GPIOI2C_Bus_Init();
  #endif
 
  #ifdef CAN1_ENABLE
    System_CAN1_Init(CAN1_BAUDRATE_DEFAULR);
  #endif
    
    /* print endian test result */
  #ifdef __DEBUG
    System_Endian_Test();
  #endif
    
    /* initilize user function */
  #ifdef USART1_ENABLE
    USART1_Object_Init();
    Start_UART_IT_CONT_Rx_Task(&USART1_OBJ, 1);
  #endif

  #ifdef USART3_ENABLE
    USART3_Object_Init();
    Start_UART_IT_CONT_Rx_Task(&USART3_OBJ, 1);
    
    USART3_OBJ.EN_Max485_Rx();
  #endif
    
    /* For GD25Q127C check, this dunction may cost beyong 60s */
    //SPI_Flash_Dev_Init();
    
  #ifdef CAN1_ENABLE
    CAN1_Object_Init();
    CAN_Rx_Ring_Buffer_Init(&CAN1_OBJ);
    CAN_Tx_Ring_Buffer_Init(&CAN1_OBJ);
    CAN_Rx_ID_Filter_All(&CAN1_OBJ);
    System_CAN_Start(CAN1);
  #endif
    
    /* must delay some time at here!!! */
    Delay_ms(100);
     
    System_WWDG_Init();
     
    while(1)
    {
        MY_M117B_Process();
        
        Power_Ctrl_Process();
        
      #ifdef CAN1_ENABLE
        CAN_Packet_Loopback(&CAN1_OBJ);
      #endif
        
        M485_Packet_Process();
        
        UART_Packet_Process();
        
        LED_Twinkle_Task();
        
        WWDG_SetCounter(127);
    }
}

static void LED_Twinkle_Task(void)
{
    static uint32_t Systick_Count = 0;
    
    if(Systick_Diff_Get(Systick_Get(), Systick_Count) >= 500)
    {
        Systick_Count = Systick_Get();
        SYS_LED_TOGGLE();
    }
}

static void M485_Packet_Process(void)
{
    static uint8_t M485_Sync_1 = 0xA5, M485_Sync_2 = 0x5A;
    
    static bool M485_Sync_1_Flag = false, M485_Sync_2_Flag = false;
    
    uint8_t sync = 0x00;
    
    if(Get_M485_Ready_Byte_Size(&USART3_OBJ))
    {
        Recv_Chars_From_M485(&USART3_OBJ, &sync, 1);
        
        if(sync == M485_Sync_1)
        {
            M485_Sync_1_Flag = true;
        }
        else if(sync == M485_Sync_2)
        {
            M485_Sync_2_Flag = true;
        }
    }
    
    if((M485_Sync_1_Flag == true) && (M485_Sync_2_Flag == true))
    {
        Send_Chars_From_M485(&USART3_OBJ, (uint8_t*)&Hardware_SN, sizeof(Hardware_SN));
        
        M485_Sync_1_Flag = false;
        M485_Sync_2_Flag = false;
    }
}

static void UART_Packet_Process(void)
{
    static uint8_t UART_Sync_1 = 0xAA, UART_Sync_2 = 0x55;
    
    static bool Wave_Transfer_En_Flag = false, UART_Sync_1_Flag = false, UART_Sync_2_Flag = false;
    
    uint8_t sync = 0x00;
    
    if(Get_UART_Ready_Byte_Size(&USART1_OBJ))
    {
        Recv_Chars_From_UART(&USART1_OBJ, &sync, 1);
        
        if(sync == UART_Sync_1)
        {
            UART_Sync_1_Flag = true;
        }
        else if(sync == UART_Sync_2)
        {
            UART_Sync_2_Flag = true;
        }
    }
    
    if((UART_Sync_1_Flag == true) && (UART_Sync_2_Flag == true))
    {
        if(Wave_Transfer_En_Flag == false)
        {
            Wave_Transfer_En_Flag = true;
        }
        else
        {
            Wave_Transfer_En_Flag = false;
        }
        
        UART_Sync_1_Flag = false;
        UART_Sync_2_Flag = false;
    }
    
    Transfer_Data_Wave(Wave_Transfer_En_Flag);
}

static void Transfer_Data_Wave(bool en_flag)
{
    static uint32_t Speed_Tick_Count = 0;
    
    if(en_flag == false)
        return;
    
    if(Systick_Diff_Get(Systick_Get(), Speed_Tick_Count) >= 10)
    {
        Speed_Tick_Count = Systick_Get();
        
        char send_buffer[80], len;
        
      #if 1
        // 点发送模式 int32类型（32bit有符号整数） 
        len = ws_point_int32(send_buffer, CH1, (int32_t)(Get_U_PWR_I_Val())); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        // 点发送模式 int32类型（32bit有符号整数） 
        len = ws_point_int32(send_buffer, CH2, (int32_t)(Get_M_PWR_I_Val())); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        // 点发送模式 int32类型（32bit有符号整数） 
        len = ws_point_int32(send_buffer, CH3, (int32_t)(Get_U_PWR_V_Val())); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        // 点发送模式 int32类型（32bit有符号整数） 
        len = ws_point_int32(send_buffer, CH4, (int32_t)(Get_M_PWR_V_Val())); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        
        // 点发送模式 int32类型（32bit有符号整数） 
        len = ws_point_int32(send_buffer, CH5, (int32_t)(Get_BAT_OUT_V_Val())); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        // 点发送模式 int32类型（32bit有符号整数） 
        len = ws_point_int32(send_buffer, CH6, (int32_t)(Get_BAT_OUT_I_Val())); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        // 点发送模式 int32类型（32bit有符号整数） 
        len = ws_point_int32(send_buffer, CH7, (int32_t)(Get_BAT_CHG_V_Val())); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        // 点发送模式 int32类型（32bit有符号整数） 
        len = ws_point_int32(send_buffer, CH8, (int32_t)(Get_BAT_CHG_I_Val())); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        
        // 点发送模式 int32类型（32bit有符号整数） 
        len = ws_point_int32(send_buffer, CH9, (int32_t)(Get_INPUT_CHG_V_Val())); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        // 点发送模式 int32类型（32bit有符号整数） 
        len = ws_point_int32(send_buffer, CH10, (int32_t)(Get_VCC_5V_I_Val())); 
        sendBuffer(send_buffer, len); // 串口发送数据
        
        
        // 点发送模式 int32类型（32bit有符号整数） 
        len = ws_point_int32(send_buffer, CH11, (int32_t)(RTC_GetCounter())); 
        sendBuffer(send_buffer, len); // 串口发送数据
      #else        
        // 点发送模式 float类型（单精度浮点型） 
        len = ws_point_float(send_buffer, CH1, (float)(Get_U_PWR_I_Val()*1.0f)); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        // 点发送模式 float类型（单精度浮点型） 
        len = ws_point_float(send_buffer, CH2, (float)(Get_M_PWR_I_Val()*1.0f)); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        // 点发送模式 float类型（单精度浮点型） 
        len = ws_point_float(send_buffer, CH3, (float)(Get_U_PWR_V_Val()*1.0f)); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        // 点发送模式 float类型（单精度浮点型） 
        len = ws_point_float(send_buffer, CH4, (float)(Get_U_PWR_V_Val()*1.0f)); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        
        // 点发送模式 float类型（单精度浮点型） 
        len = ws_point_float(send_buffer, CH5, (float)(Get_BAT_OUT_V_Val()*1.0f)); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        // 点发送模式 float类型（单精度浮点型） 
        len = ws_point_float(send_buffer, CH6, (float)(Get_BAT_OUT_I_Val()*1.0f)); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        // 点发送模式 float类型（单精度浮点型） 
        len = ws_point_float(send_buffer, CH7, (float)(Get_BAT_CHG_V_Val()*1.0f)); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        // 点发送模式 float类型（单精度浮点型） 
        len = ws_point_float(send_buffer, CH8, (float)(Get_BAT_CHG_I_Val()*1.0f)); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        
        // 点发送模式 float类型（单精度浮点型） 
        len = ws_point_float(send_buffer, CH9, (float)(Get_INPUT_CHG_V_Val()*1.0f)); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        // 点发送模式 float类型（单精度浮点型） 
        len = ws_point_float(send_buffer, CH10, (float)(Get_VCC_5V_I_Val()*1.0f)); 
        sendBuffer(send_buffer, len); // 串口发送数据 
        
        
        // 点发送模式 float类型（单精度浮点型） 
        len = ws_point_float(send_buffer, CH11, (float)(RTC_GetCounter()*1.0f)); 
        sendBuffer(send_buffer, len); // 串口发送数据 
      #endif
    }
}

