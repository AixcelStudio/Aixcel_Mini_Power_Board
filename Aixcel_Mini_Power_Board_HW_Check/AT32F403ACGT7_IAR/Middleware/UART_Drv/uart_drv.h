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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UART_DRV_H
#define UART_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdbool.h>
#include "common.h"
  
#define    MAX485_DEV_WORK_ON_RX                       0x00  
#define    MAX485_DEV_WORK_ON_TX                       0x01                      

typedef  uint8_t   MAX485_DEV_WORK_STATE_T;  

typedef enum
{
    IDLE = 0,
    BUSY,
} UART_DMA_Tx_State_TypeDef; 

typedef enum
{
    NONE = 0,
    IT_CONT_MODE_ACTIVE,
    DMA_MODE_ACTIVE,
    DMA_DBM_MODE_ACTIVE,
} UART_Rx_Active_Mode_TypeDef;    

typedef enum
{
    IS_EMPTY = 0,
    IS_WRITING,
    IS_READY,
    IS_READING, 
} UART_Rx_Buf_Cell_State_TypeDef;

typedef struct __UART_Rx_Buf_Cell_TypeDef
{
    UART_Rx_Buf_Cell_State_TypeDef         State;    //用于表征UART Rx的DMA缓冲区的单个Cell的状态
    
    uint32_t                   Start_DMA_Time_ms;    //开始DMA接收的时间
    
    uint32_t                  Finish_DMA_Time_ms;    //完成DMA接收的时间
    
    uint32_t                  ValidPos;              //用于表征UART Rx的DMA缓冲区的单个Cell中多少数据就绪
    
    uint32_t                  ReadPos;               //用于表征UART Rx的DMA缓冲区的单个Cell中接下来应被读取的数据的下标

    uint8_t                   *pData;                //UART Rx的DMA缓冲区的单个Cell的指针   
 
} UART_Rx_Buf_Cell_TypeDef;

typedef struct __UART_IT_Rx_Ring_Buffer_TypeDef
{
    uint8_t           *pBuffer;
    
    uint32_t          Head; 
    
    uint32_t          Tail; 
    
    uint32_t          Buffer_Size;

} UART_IT_Rx_Ring_Buffer_TypeDef, UART_IT_CONT_Rx_Ring_Buffer_TypeDef;

typedef struct __UART_DMA_Rx_Ring_Buffer_TypeDef
{
    UART_Rx_Buf_Cell_TypeDef   *pBuffer;
    
    uint32_t          Head; 
    
    uint32_t          Tail; 
    
    uint32_t          Buffer_Size;

} UART_DMA_Rx_Ring_Buffer_TypeDef, UART_DMA_DBM_Rx_Ring_Buffer_TypeDef;


typedef void(*MAX485_TR_CTRL_FUNC_T)(void);

typedef struct __UART_ObjectTypeDef
{
    USART_Type* UART;
    
    MAX485_DEV_WORK_STATE_T Max485_Work_State;
    
    MAX485_TR_CTRL_FUNC_T EN_Max485_Tx;
    
    MAX485_TR_CTRL_FUNC_T EN_Max485_Rx;
    
    UART_DMA_Tx_State_TypeDef UART_DMA_Tx_State;
    
    UART_Rx_Active_Mode_TypeDef UART_Rx_Active_Mode;
  
    uint8_t *p_UART_Rx_Mem;
    uint8_t *p_UART_Tx_Mem;
  
    UART_Rx_Buf_Cell_TypeDef *p_UART_Rx_Buf_Cell;
  
    uint8_t  UART_Rx_Buf_Cell_Cnt;
    uint8_t  UART_Rx_Buf_Cell_Size;
    uint32_t UART_Rx_Buf_Byte_Size;
  
    uint8_t  UART_Tx_Buf_Cell_Cnt;
    uint8_t  UART_Tx_Buf_Cell_Size;
    uint32_t UART_Tx_Buf_Byte_Size;

    uint32_t UART_IT_CONT_Rx_Ready_Size;
    uint32_t UART_DMA_DBM_Rx_Ready_Size;
    
    uint32_t UART_IT_CONT_Rx_Lost_Size;
    uint32_t UART_DMA_DBM_Rx_Lost_Size;
  
    UART_IT_CONT_Rx_Ring_Buffer_TypeDef  UART_IT_CONT_Rx_Ring_Buffer;
    UART_DMA_Rx_Ring_Buffer_TypeDef      UART_DMA_Rx_Ring_Buffer;
  
} UART_ObjectTypeDef;

#ifdef USART1_ENABLE
extern UART_ObjectTypeDef USART1_OBJ;
bool USART1_Object_Init(void);
#endif

#ifdef USART2_ENABLE
extern UART_ObjectTypeDef USART2_OBJ;
bool USART2_Object_Init(void);
#endif

#ifdef USART3_ENABLE
extern UART_ObjectTypeDef USART3_OBJ;
bool USART3_Object_Init(void);
#endif

#ifdef UART4_ENABLE
extern UART_ObjectTypeDef UART4_OBJ;
bool UART4_Object_Init(void);
#endif

#ifdef UART5_ENABLE
extern UART_ObjectTypeDef UART5_OBJ;
bool UART5_Object_Init(void);
#endif

uint8_t Start_UART_IT_CONT_Rx_Task(UART_ObjectTypeDef *uart_obj, uint8_t init_flag);

uint8_t Start_UART_DMA_Rx_Task(UART_ObjectTypeDef *uart_obj, UART_Rx_Buf_Cell_TypeDef *pBuffer, uint8_t init_flag);

uint8_t Start_UART_DMA_DBM_Rx_Task(UART_ObjectTypeDef *uart_obj, uint8_t *pData, uint8_t init_flag);

uint8_t Abort_UART_Rx_Task(UART_ObjectTypeDef *uart_obj);

int32_t Send_Chars_From_UART(UART_ObjectTypeDef *uart_obj, uint8_t *p_data, uint32_t size);

bool Wait_Chars_From_UART(UART_ObjectTypeDef *uart_obj, uint32_t wait_size, uint32_t *time_out_ms);

int32_t Recv_Chars_From_UART(UART_ObjectTypeDef *uart_obj, uint8_t *addr, uint32_t length);

uint32_t Get_UART_Ready_Byte_Size(UART_ObjectTypeDef *uart_obj);

void UART_TxCpltCallback(USART_Type* usart_periph);

void UART_ErrorCallback(USART_Type* usart_periph);

void UART_IT_CONT_RxCpltCallback(USART_Type* usart_periph);

void UART_DMA_RxCpltCallback(USART_Type* usart_periph);

__weak void UART_Clear_Rx_Buffer(UART_ObjectTypeDef *uart_obj);

__weak void UART_Clear_Tx_Buffer(UART_ObjectTypeDef *uart_obj);

#ifdef __cplusplus
}
#endif

#endif /* UART_H */
