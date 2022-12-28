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

#include "uart_drv.h"
#include "hwconf.h"
#include "systick.h"

static uint32_t Prev_DMA_Time_ms = 0;
static uint32_t Curr_DMA_Time_ms = 0;

#ifdef USART1_ENABLE
UART_ObjectTypeDef USART1_OBJ;
static UART_Rx_Buf_Cell_TypeDef USART1_Rx_Buf_Cell[USART1_RX_BUF_CELL_CNT];

extern uint8_t  USART1_Rx_Mem_Region[USART1_RX_BUF_CELL_CNT * USART1_RX_BUF_CELL_SIZE];
extern uint8_t  USART1_Tx_Mem_Region[USART1_TX_BUF_CELL_CNT * USART1_TX_BUF_CELL_SIZE];
#endif

#ifdef USART2_ENABLE
UART_ObjectTypeDef USART2_OBJ;
static UART_Rx_Buf_Cell_TypeDef USART2_Rx_Buf_Cell[USART2_RX_BUF_CELL_CNT];

extern uint8_t  USART2_Rx_Mem_Region[USART2_RX_BUF_CELL_CNT * USART2_RX_BUF_CELL_SIZE];
extern uint8_t  USART2_Tx_Mem_Region[USART2_TX_BUF_CELL_CNT * USART2_TX_BUF_CELL_SIZE];
#endif

#ifdef USART3_ENABLE
UART_ObjectTypeDef USART3_OBJ;
static UART_Rx_Buf_Cell_TypeDef USART3_Rx_Buf_Cell[USART3_RX_BUF_CELL_CNT];

extern uint8_t  USART3_Rx_Mem_Region[USART3_RX_BUF_CELL_CNT * USART3_RX_BUF_CELL_SIZE];
extern uint8_t  USART3_Tx_Mem_Region[USART3_TX_BUF_CELL_CNT * USART3_TX_BUF_CELL_SIZE];
#endif

#ifdef UART4_ENABLE
UART_ObjectTypeDef UART4_OBJ;
static UART_Rx_Buf_Cell_TypeDef UART4_Rx_Buf_Cell[UART4_RX_BUF_CELL_CNT];

extern uint8_t  UART4_Rx_Mem_Region[UART4_RX_BUF_CELL_CNT * UART4_RX_BUF_CELL_SIZE];
extern uint8_t  UART4_Tx_Mem_Region[UART4_TX_BUF_CELL_CNT * UART4_TX_BUF_CELL_SIZE];
#endif

#ifdef UART5_ENABLE
UART_ObjectTypeDef UART5_OBJ;
static UART_Rx_Buf_Cell_TypeDef UART5_Rx_Buf_Cell[UART5_RX_BUF_CELL_CNT];

extern uint8_t  UART5_Rx_Mem_Region[UART5_RX_BUF_CELL_CNT * UART5_RX_BUF_CELL_SIZE];
extern uint8_t  UART5_Tx_Mem_Region[UART5_TX_BUF_CELL_CNT * UART5_TX_BUF_CELL_SIZE];
#endif

static void UART_IT_CONT_Rx_Ring_Buffer_Init(UART_ObjectTypeDef *uart_obj);

static void UART_DMA_Rx_Ring_Buffer_Init(UART_ObjectTypeDef *uart_obj);

static bool UART_IT_CONT_Rx_Ring_Enqueue(uint8_t data, UART_IT_CONT_Rx_Ring_Buffer_TypeDef *p_ring);

static bool UART_DMA_Rx_Ring_Enqueue(UART_Rx_Buf_Cell_TypeDef** p_cell, UART_DMA_Rx_Ring_Buffer_TypeDef *p_ring);

static bool UART_DMA_DBM_Rx_Ring_Enqueue(UART_Rx_Buf_Cell_TypeDef** p_cell, UART_DMA_Rx_Ring_Buffer_TypeDef *p_ring);

static bool UART_IT_CONT_Rx_Ring_Dequeue(uint8_t** p_data, UART_IT_CONT_Rx_Ring_Buffer_TypeDef *p_ring);

static bool UART_DMA_Rx_Ring_Dequeue(UART_Rx_Buf_Cell_TypeDef** p_cell, UART_DMA_Rx_Ring_Buffer_TypeDef *p_ring);

static uint8_t* Get_Curr_Ready_UART_IT_CONT_Rx_Buffer_Addr(UART_ObjectTypeDef *uart_obj);

static UART_Rx_Buf_Cell_TypeDef* Get_Curr_Ready_UART_DMA_Rx_Buffer_Addr(UART_ObjectTypeDef *uart_obj);


#ifdef USART1_ENABLE
bool USART1_Object_Init(void)
{
    USART1_OBJ.UART = USART1;
  
    if(USART1_OBJ.UART == NULL)
        return false;
    
    USART1_OBJ.Max485_Work_State = MAX485_DEV_WORK_ON_RX;
    
    USART1_OBJ.EN_Max485_Tx = NULL;
    
    USART1_OBJ.EN_Max485_Rx = NULL;
    
    USART1_OBJ.UART_DMA_Tx_State = IDLE;
    
    USART1_OBJ.UART_Rx_Active_Mode = NONE;
  
    USART1_OBJ.p_UART_Rx_Mem = USART1_Rx_Mem_Region;
    USART1_OBJ.p_UART_Tx_Mem = USART1_Tx_Mem_Region;
  
    USART1_OBJ.p_UART_Rx_Buf_Cell = USART1_Rx_Buf_Cell;
  
    if((USART1_OBJ.p_UART_Rx_Mem == NULL) || (USART1_OBJ.p_UART_Tx_Mem == NULL))
        return false;
  
    USART1_OBJ.UART_Rx_Buf_Cell_Cnt  = USART1_RX_BUF_CELL_CNT;
    USART1_OBJ.UART_Rx_Buf_Cell_Size = USART1_RX_BUF_CELL_SIZE;
    USART1_OBJ.UART_Rx_Buf_Byte_Size = sizeof(USART1_Rx_Mem_Region);
  
    USART1_OBJ.UART_Tx_Buf_Cell_Cnt  = USART1_TX_BUF_CELL_CNT;
    USART1_OBJ.UART_Tx_Buf_Cell_Size = USART1_TX_BUF_CELL_SIZE;
    USART1_OBJ.UART_Tx_Buf_Byte_Size = sizeof(USART1_Tx_Mem_Region);
    
    USART1_OBJ.UART_IT_CONT_Rx_Ready_Size = 0;
    USART1_OBJ.UART_DMA_DBM_Rx_Ready_Size = 0;
    
    USART1_OBJ.UART_IT_CONT_Rx_Lost_Size = 0;
    USART1_OBJ.UART_DMA_DBM_Rx_Lost_Size = 0;
    
    return true;
}
#endif

#ifdef USART2_ENABLE
bool USART2_Object_Init(void)
{
    USART2_OBJ.UART = USART2;
  
    if(USART2_OBJ.UART == NULL)
        return false;
    
    USART2_OBJ.Max485_Work_State = MAX485_DEV_WORK_ON_RX;
    
    USART2_OBJ.EN_Max485_Tx = NULL;
    
    USART2_OBJ.EN_Max485_Rx = NULL;
    
    USART2_OBJ.UART_DMA_Tx_State = IDLE;
    
    USART2_OBJ.UART_Rx_Active_Mode = NONE;
  
    USART2_OBJ.p_UART_Rx_Mem = USART2_Rx_Mem_Region;
    USART2_OBJ.p_UART_Tx_Mem = USART2_Tx_Mem_Region;
  
    USART2_OBJ.p_UART_Rx_Buf_Cell = USART2_Rx_Buf_Cell;
  
    if((USART2_OBJ.p_UART_Rx_Mem == NULL) || (USART2_OBJ.p_UART_Tx_Mem == NULL))
        return false;
  
    USART2_OBJ.UART_Rx_Buf_Cell_Cnt  = USART2_RX_BUF_CELL_CNT;
    USART2_OBJ.UART_Rx_Buf_Cell_Size = USART2_RX_BUF_CELL_SIZE;
    USART2_OBJ.UART_Rx_Buf_Byte_Size = sizeof(USART2_Rx_Mem_Region);
  
    USART2_OBJ.UART_Tx_Buf_Cell_Cnt  = USART2_TX_BUF_CELL_CNT;
    USART2_OBJ.UART_Tx_Buf_Cell_Size = USART2_TX_BUF_CELL_SIZE;
    USART2_OBJ.UART_Tx_Buf_Byte_Size = sizeof(USART2_Tx_Mem_Region);
    
    USART2_OBJ.UART_IT_CONT_Rx_Ready_Size = 0;
    USART2_OBJ.UART_DMA_DBM_Rx_Ready_Size = 0;
    
    USART2_OBJ.UART_IT_CONT_Rx_Lost_Size = 0;
    USART2_OBJ.UART_DMA_DBM_Rx_Lost_Size = 0;
    
    return true;
}
#endif

#ifdef USART3_ENABLE
bool USART3_Object_Init(void)
{
    USART3_OBJ.UART = USART3;
  
    if(USART3_OBJ.UART == NULL)
        return false;
    
    USART3_OBJ.Max485_Work_State = MAX485_DEV_WORK_ON_RX;
    
    USART3_OBJ.EN_Max485_Tx = USART3_EN_M485_Tx;
    
    USART3_OBJ.EN_Max485_Rx = USART3_EN_M485_Rx;
    
    USART3_OBJ.UART_DMA_Tx_State = IDLE;
    
    USART3_OBJ.UART_Rx_Active_Mode = NONE;
  
    USART3_OBJ.p_UART_Rx_Mem = USART3_Rx_Mem_Region;
    USART3_OBJ.p_UART_Tx_Mem = USART3_Tx_Mem_Region;
  
    USART3_OBJ.p_UART_Rx_Buf_Cell = USART3_Rx_Buf_Cell;
  
    if((USART3_OBJ.p_UART_Rx_Mem == NULL) || (USART3_OBJ.p_UART_Tx_Mem == NULL))
        return false;
  
    USART3_OBJ.UART_Rx_Buf_Cell_Cnt  = USART3_RX_BUF_CELL_CNT;
    USART3_OBJ.UART_Rx_Buf_Cell_Size = USART3_RX_BUF_CELL_SIZE;
    USART3_OBJ.UART_Rx_Buf_Byte_Size = sizeof(USART3_Rx_Mem_Region);
  
    USART3_OBJ.UART_Tx_Buf_Cell_Cnt  = USART3_TX_BUF_CELL_CNT;
    USART3_OBJ.UART_Tx_Buf_Cell_Size = USART3_TX_BUF_CELL_SIZE;
    USART3_OBJ.UART_Tx_Buf_Byte_Size = sizeof(USART3_Tx_Mem_Region);
    
    USART3_OBJ.UART_IT_CONT_Rx_Ready_Size = 0;
    USART3_OBJ.UART_DMA_DBM_Rx_Ready_Size = 0;
    
    USART3_OBJ.UART_IT_CONT_Rx_Lost_Size = 0;
    USART3_OBJ.UART_DMA_DBM_Rx_Lost_Size = 0;
    
    return true;
}
#endif

#ifdef UART4_ENABLE
bool UART4_Object_Init(void)
{
    UART4_OBJ.UART = UART4;
  
    if(UART4_OBJ.UART == NULL)
        return false;
    
    UART4_OBJ.Max485_Work_State = MAX485_DEV_WORK_ON_RX;
    
    UART4_OBJ.EN_Max485_Tx = NULL;
    
    UART4_OBJ.EN_Max485_Rx = NULL;
    
    UART4_OBJ.UART_DMA_Tx_State = IDLE;
    
    UART4_OBJ.UART_Rx_Active_Mode = NONE;
  
    UART4_OBJ.p_UART_Rx_Mem = UART4_Rx_Mem_Region;
    UART4_OBJ.p_UART_Tx_Mem = UART4_Tx_Mem_Region;
  
    UART4_OBJ.p_UART_Rx_Buf_Cell = UART4_Rx_Buf_Cell;
  
    if((UART4_OBJ.p_UART_Rx_Mem == NULL) || (UART4_OBJ.p_UART_Tx_Mem == NULL))
        return false;
  
    UART4_OBJ.UART_Rx_Buf_Cell_Cnt  = UART4_RX_BUF_CELL_CNT;
    UART4_OBJ.UART_Rx_Buf_Cell_Size = UART4_RX_BUF_CELL_SIZE;
    UART4_OBJ.UART_Rx_Buf_Byte_Size = sizeof(UART4_Rx_Mem_Region);
  
    UART4_OBJ.UART_Tx_Buf_Cell_Cnt  = UART4_TX_BUF_CELL_CNT;
    UART4_OBJ.UART_Tx_Buf_Cell_Size = UART4_TX_BUF_CELL_SIZE;
    UART4_OBJ.UART_Tx_Buf_Byte_Size = sizeof(UART4_Tx_Mem_Region);
    
    UART4_OBJ.UART_IT_CONT_Rx_Ready_Size = 0;
    UART4_OBJ.UART_DMA_DBM_Rx_Ready_Size = 0;
    
    UART4_OBJ.UART_IT_CONT_Rx_Lost_Size = 0;
    UART4_OBJ.UART_DMA_DBM_Rx_Lost_Size = 0;
    
    return true;
}
#endif

#ifdef UART5_ENABLE
bool UART5_Object_Init(void)
{
    UART5_OBJ.UART = UART5;
  
    if(UART5_OBJ.UART == NULL)
        return false;
    
    UART5_OBJ.Max485_Work_State = MAX485_DEV_WORK_ON_RX;
    
    UART5_OBJ.EN_Max485_Tx = NULL;
    
    UART5_OBJ.EN_Max485_Rx = NULL;
    
    UART5_OBJ.UART_DMA_Tx_State = IDLE;
    
    UART5_OBJ.UART_Rx_Active_Mode = NONE;
  
    UART5_OBJ.p_UART_Rx_Mem = UART5_Rx_Mem_Region;
    UART5_OBJ.p_UART_Tx_Mem = UART5_Tx_Mem_Region;
  
    UART5_OBJ.p_UART_Rx_Buf_Cell = UART5_Rx_Buf_Cell;
  
    if((UART5_OBJ.p_UART_Rx_Mem == NULL) || (UART5_OBJ.p_UART_Tx_Mem == NULL))
        return false;
  
    UART5_OBJ.UART_Rx_Buf_Cell_Cnt  = UART5_RX_BUF_CELL_CNT;
    UART5_OBJ.UART_Rx_Buf_Cell_Size = UART5_RX_BUF_CELL_SIZE;
    UART5_OBJ.UART_Rx_Buf_Byte_Size = sizeof(UART5_Rx_Mem_Region);
  
    UART5_OBJ.UART_Tx_Buf_Cell_Cnt  = UART5_TX_BUF_CELL_CNT;
    UART5_OBJ.UART_Tx_Buf_Cell_Size = UART5_TX_BUF_CELL_SIZE;
    UART5_OBJ.UART_Tx_Buf_Byte_Size = sizeof(UART5_Tx_Mem_Region);
    
    UART5_OBJ.UART_IT_CONT_Rx_Ready_Size = 0;
    UART5_OBJ.UART_DMA_DBM_Rx_Ready_Size = 0;
    
    UART5_OBJ.UART_IT_CONT_Rx_Lost_Size = 0;
    UART5_OBJ.UART_DMA_DBM_Rx_Lost_Size = 0;
    
    return true;
}
#endif

static void UART_IT_CONT_Rx_Ring_Buffer_Init(UART_ObjectTypeDef *uart_obj)
{
    uart_obj->UART_IT_CONT_Rx_Ring_Buffer.pBuffer = (uint8_t*)(uart_obj->p_UART_Rx_Mem);
    uart_obj->UART_IT_CONT_Rx_Ring_Buffer.Head = 0;
    uart_obj->UART_IT_CONT_Rx_Ring_Buffer.Tail = 0;
    uart_obj->UART_IT_CONT_Rx_Ring_Buffer.Buffer_Size = uart_obj->UART_Rx_Buf_Byte_Size - 1;      //Buffer_Size必须为实际大小 - 1
}

static void UART_DMA_Rx_Ring_Buffer_Init(UART_ObjectTypeDef *uart_obj)
{
    uart_obj->UART_DMA_Rx_Ring_Buffer.pBuffer = (UART_Rx_Buf_Cell_TypeDef*)(uart_obj->p_UART_Rx_Buf_Cell);
    uart_obj->UART_DMA_Rx_Ring_Buffer.Head = 0;
    uart_obj->UART_DMA_Rx_Ring_Buffer.Tail = 0;
    uart_obj->UART_DMA_Rx_Ring_Buffer.Buffer_Size = uart_obj->UART_Rx_Buf_Cell_Cnt - 1;           //Buffer_Size必须为实际大小 - 1
    
    for(uint32_t i = 0; i < uart_obj->UART_Rx_Buf_Cell_Cnt; i++)
    {
        uart_obj->p_UART_Rx_Buf_Cell[i].State = IS_EMPTY;
        uart_obj->p_UART_Rx_Buf_Cell[i].Start_DMA_Time_ms = 0;
        uart_obj->p_UART_Rx_Buf_Cell[i].Finish_DMA_Time_ms = 0;
        uart_obj->p_UART_Rx_Buf_Cell[i].ValidPos = 0;
        uart_obj->p_UART_Rx_Buf_Cell[i].ReadPos = 0;
        uart_obj->p_UART_Rx_Buf_Cell[i].pData = &(uart_obj->p_UART_Rx_Mem[i * uart_obj->UART_Rx_Buf_Cell_Size]);
        
        for(uint32_t j = 0; j < uart_obj->UART_Rx_Buf_Cell_Size; j++)
        {
            uart_obj->p_UART_Rx_Buf_Cell[i].pData[j] = 0;
        }
    }
}

static bool UART_IT_CONT_Rx_Ring_Enqueue(uint8_t data, UART_IT_CONT_Rx_Ring_Buffer_TypeDef *p_ring)
{
    if(p_ring == NULL)
        return false;
    
    if(((p_ring->Tail+1) == p_ring->Head) || ((p_ring->Tail == p_ring->Buffer_Size) && (p_ring->Head==0)))
    {
        return false;
    }
    else
    {
        p_ring->pBuffer[p_ring->Tail] = data;         
        p_ring->Tail++;								               
        if(p_ring->Tail > p_ring->Buffer_Size)	               
            p_ring->Tail = 0;							           
        return true;
    }
}

static bool UART_DMA_Rx_Ring_Enqueue(UART_Rx_Buf_Cell_TypeDef** p_cell, UART_DMA_Rx_Ring_Buffer_TypeDef *p_ring)
{
    if(p_ring == NULL)
        return false;
    
    if(((p_ring->Tail+1) == p_ring->Head) || ((p_ring->Tail == p_ring->Buffer_Size) && (p_ring->Head==0)))
    {
	*p_cell = NULL;
        return false;
    }
    else
    {
        *p_cell = &(p_ring->pBuffer[p_ring->Tail]);
        //p_ring->Tail++;								               
        //if(p_ring->Tail > p_ring->Buffer_Size)	//真正的入列操作仍然要在DMA接收中断中完成，真正的入列完成后才能修改队列尾巴	               
        //    p_ring->Tail = 0;							           
        return true;
    }  
}

static bool UART_DMA_DBM_Rx_Ring_Enqueue(UART_Rx_Buf_Cell_TypeDef** p_cell, UART_DMA_Rx_Ring_Buffer_TypeDef *p_ring)
{
    if(p_ring == NULL)
        return false;
    
    if(((p_ring->Tail+1) == p_ring->Head) || ((p_ring->Tail == p_ring->Buffer_Size) && (p_ring->Head==0)))
    {
	*p_cell = NULL;
        return false;
    }
    else
    {
        p_ring->Tail++;			     	        //提前模拟下次DMA接收已经完成 					               
        if(p_ring->Tail > p_ring->Buffer_Size)
            p_ring->Tail = 0;
        
        if(((p_ring->Tail+1) == p_ring->Head) || ((p_ring->Tail == p_ring->Buffer_Size) && (p_ring->Head==0)))//在模拟下次DMA接收已经完成的情况下判断FIFO是否已满  		
        {
	    *p_cell = NULL;                             //如果在模拟下次DMA接收已经完成的情况下判断FIFO已满, 那么返回NULL
            
            if(p_ring->Tail == 0)
                p_ring->Tail = p_ring->Buffer_Size;     //为了提前模拟下次DMA接收已经完成而对FIFO进行的操作必须还原
            else
                p_ring->Tail--;                         //为了提前模拟下次DMA接收已经完成而对FIFO进行的操作必须还原
            
            return false;
        }
        else
        {
            *p_cell = &(p_ring->pBuffer[p_ring->Tail]); //如果在模拟下次DMA接收已经完成的情况下判断FIFO仍然未满, 那么返回FIFO的尾巴
            
            if(p_ring->Tail == 0)	              
                p_ring->Tail = p_ring->Buffer_Size;     //为了提前模拟下次DMA接收已经完成而对FIFO进行的操作必须还原
            else
                p_ring->Tail--;                         //为了提前模拟下次DMA接收已经完成而对FIFO进行的操作必须还原
            
            //p_ring->Tail++;								               
            //if(p_ring->Tail > p_ring->Buffer_Size)	//真正的入列操作仍然要在DMA接收中断中完成，真正的入列完成后才能修改队列尾巴	               
            //    p_ring->Tail = 0;							           
            return true;
        }
    }
}

static bool UART_IT_CONT_Rx_Ring_Dequeue(uint8_t** p_data, UART_IT_CONT_Rx_Ring_Buffer_TypeDef *p_ring)
{
    if(p_ring == NULL)
        return false;
    
    if(p_ring->Head == p_ring->Tail)					          
    {
        return false;                                        	 
    }
    else
    {
        *p_data = &(p_ring->pBuffer[p_ring->Head]);
        //p_ring->Head++;							            	  
        //if(p_ring->Head > p_ring->Buffer_Size)         //真正的出列操作在Recv_Chars()函数中完成，真正的出列完成后才能修改队列头         
        //    p_ring->Head = 0;                          
        return true;
    }  
}

static bool UART_DMA_Rx_Ring_Dequeue(UART_Rx_Buf_Cell_TypeDef** p_cell, UART_DMA_Rx_Ring_Buffer_TypeDef *p_ring)
{
    if(p_ring == NULL)
        return false;
    
    if(p_ring->Head == p_ring->Tail)					          
    {
        *p_cell = NULL;
        return false;                                        	 
    }
    else
    {
        *p_cell = &(p_ring->pBuffer[p_ring->Head]);
        //p_ring->Head++;							            	  
        //if(p_ring->Head > p_ring->Buffer_Size)	//真正的出列操作在Recv_Chars()函数中完成，真正的出列完成后才能修改队列头	          
        //    p_ring->Head = 0;                          
        return true;
    }
}

static uint32_t Get_UART_IT_CONT_Rx_Ready_Size(UART_ObjectTypeDef *uart_obj)
{
    return  uart_obj->UART_IT_CONT_Rx_Ready_Size;
    
    /*实践证明，下面的方法会导致应读的下标出错，进而导致应写的下标出错，进而导致缓冲区未读的数据被覆盖，进而导致雷达点云缺一部分！！！
    if(uart_obj->UART_IT_CONT_Rx_Ring_Buffer.Tail >= uart_obj->UART_IT_CONT_Rx_Ring_Buffer.Head)
    {
        return (uart_obj->UART_IT_CONT_Rx_Ring_Buffer.Tail - uart_obj->UART_IT_CONT_Rx_Ring_Buffer.Head);
    }
    else
    {
        return (uart_obj->UART_IT_CONT_Rx_Ring_Buffer.Buffer_Size + uart_obj->UART_IT_CONT_Rx_Ring_Buffer.Tail - uart_obj->UART_IT_CONT_Rx_Ring_Buffer.Head);
    }*/
}

static uint32_t Get_UART_DMA_Rx_Ready_Size(UART_ObjectTypeDef *uart_obj)
{
    return  uart_obj->UART_DMA_DBM_Rx_Ready_Size;
    
    /*实践证明，下面的方法会导致应读的下标出错，进而导致应写的下标出错，进而导致缓冲区未读的数据被覆盖，进而导致雷达点云缺一部分！！！
    if(uart_obj->UART_DMA_Rx_Ring_Buffer.Tail >= uart_obj->UART_DMA_Rx_Ring_Buffer.Head)
    {
        return (uart_obj->UART_DMA_Rx_Ring_Buffer.Tail - uart_obj->UART_DMA_Rx_Ring_Buffer.Head)*uart_obj->UART_Rx_Buf_Cell_Size;
    }
    else
    {
        return (uart_obj->UART_DMA_Rx_Ring_Buffer.Buffer_Size + uart_obj->UART_DMA_Rx_Ring_Buffer.Tail - uart_obj->UART_DMA_Rx_Ring_Buffer.Head)*uart_obj->UART_Rx_Buf_Cell_Size;
    }*/
}

static uint8_t* Get_Curr_Ready_UART_IT_CONT_Rx_Buffer_Addr(UART_ObjectTypeDef *uart_obj)
{
    if(uart_obj == NULL)
        return NULL;
    
    uint8_t* ready_addr = NULL;
    
    if(UART_IT_CONT_Rx_Ring_Dequeue(&ready_addr, &(uart_obj->UART_IT_CONT_Rx_Ring_Buffer)) == true)
    {
        return ready_addr;
    }
    
    return ready_addr;
}

static UART_Rx_Buf_Cell_TypeDef* Get_Curr_Ready_UART_DMA_Rx_Buffer_Addr(UART_ObjectTypeDef *uart_obj)
{
    if(uart_obj == NULL)
        return NULL;
    
    UART_Rx_Buf_Cell_TypeDef* ready_addr = NULL;
    
    if(UART_DMA_Rx_Ring_Dequeue(&ready_addr, &(uart_obj->UART_DMA_Rx_Ring_Buffer)) == true)
    {
        return ready_addr;
    }
    
    return ready_addr;
}

uint8_t Start_UART_IT_CONT_Rx_Task(UART_ObjectTypeDef *uart_obj, uint8_t init_flag)
{
    if(uart_obj == NULL)
        return HAL_ERROR;
    
    if(init_flag == 1)
    {
        uart_obj->UART_IT_CONT_Rx_Ready_Size = 0;
        
        uart_obj->UART_IT_CONT_Rx_Lost_Size = 0;
        
        UART_IT_CONT_Rx_Ring_Buffer_Init(uart_obj);
        
        uart_obj->UART->CTRL1 |= USART_Mode_Rx;
        
        USART_INTConfig(uart_obj->UART, USART_INT_RDNE, ENABLE);                    //使能串口接收非空中断 !!!
    }
    
    uart_obj->UART->CTRL1 |= USART_Mode_Rx;
    
    USART_INTConfig(uart_obj->UART, USART_INT_RDNE, ENABLE);
    
    USART_DMACmd(uart_obj->UART, USART_DMAReq_Rx, DISABLE);
    
    #ifdef USART1_ENABLE
    if(uart_obj->UART == USART1)
    {
        DMA_ChannelEnable(DMA1_Channel5, DISABLE); 
    }
    #endif
    #ifdef USART2_ENABLE
    else if(uart_obj->UART == USART2)
    {
        DMA_ChannelEnable(DMA1_Channel6, DISABLE); 
    }
    #endif
    #ifdef USART3_ENABLE
    else if(uart_obj->UART == USART3)
    {
        DMA_ChannelEnable(DMA1_Channel3, DISABLE);
    }
    #endif
    #ifdef UART4_ENABLE
    else if(uart_obj->UART == UART4)
    {
        ;
    }
    #endif
    #ifdef UART5_ENABLE
    else if(uart_obj->UART == UART5)
    {
        ;
    }
    #endif
    
    uart_obj->UART_Rx_Active_Mode = IT_CONT_MODE_ACTIVE;
    
    return HAL_OK;
}

uint8_t Start_UART_DMA_Rx_Task(UART_ObjectTypeDef *uart_obj, UART_Rx_Buf_Cell_TypeDef *pBuffer, uint8_t init_flag)
{
    if(uart_obj == NULL)
        return HAL_ERROR;
    
    if((pBuffer == NULL) && (init_flag != 1))
        return HAL_ERROR;
        
    if(init_flag == 1)
    {
        uart_obj->UART_DMA_DBM_Rx_Ready_Size = 0;
        uart_obj->UART_DMA_DBM_Rx_Lost_Size = 0;
        
        UART_DMA_Rx_Ring_Buffer_Init(uart_obj);
        
        uart_obj->UART_DMA_Rx_Ring_Buffer.pBuffer[0].State = IS_WRITING;
        
        uart_obj->UART->CTRL1 |= USART_Mode_Rx;
        
        USART_INTConfig(uart_obj->UART, USART_INT_RDNE, DISABLE);                   //开启DMA接收就是为了避免大量串口数据涌入时频繁触发中断, 因此禁用串口接收非空中断 !!!
        
        USART_DMACmd(uart_obj->UART, USART_DMAReq_Rx, ENABLE);
        
        #ifdef USART1_ENABLE
        if(uart_obj->UART == USART1)
        {
            DMA_ChannelEnable(DMA1_Channel5, DISABLE); 
            DMA_SetCurrDataCounter(DMA1_Channel5, uart_obj->UART_Rx_Buf_Cell_Size);
            DMA_ChannelEnable(DMA1_Channel5, ENABLE); 
        }
        #endif
        #ifdef USART2_ENABLE
        else if(uart_obj->UART == USART2)
        {
            DMA_ChannelEnable(DMA1_Channel6, DISABLE);
            DMA_SetCurrDataCounter(DMA1_Channel6, uart_obj->UART_Rx_Buf_Cell_Size);
            DMA_ChannelEnable(DMA1_Channel6, ENABLE); 
        }
        #endif
        #ifdef USART3_ENABLE
        else if(uart_obj->UART == USART3)
        {
            DMA_ChannelEnable(DMA1_Channel3, DISABLE);
            DMA_SetCurrDataCounter(DMA1_Channel3, uart_obj->UART_Rx_Buf_Cell_Size);
            DMA_ChannelEnable(DMA1_Channel3, ENABLE);
        }
        #endif
        #ifdef UART4_ENABLE
        else if(uart_obj->UART == UART4)
        {
            ;
        }
        #endif
        #ifdef UART5_ENABLE
        else if(uart_obj->UART == UART5)
        {
            ;
        }
        #endif
        
        return HAL_OK;
    }
    else
    {
        pBuffer->State = IS_WRITING;

        #ifdef USART1_ENABLE
        if(uart_obj->UART == USART1)
        {
            DMA_ChannelEnable(DMA1_Channel5, DISABLE); 
            DMA1_Channel5->CMBA = (uint32_t)pBuffer->pData;//首次启动DMA接收的时候如果调用dma_memory_address_config(), 会导致: 第一次DAM接收完成中断虽然能够到，但是缓冲区却没有收到数据, 因此把去掉此语句switch段落复制了一份到上面
            DMA1_Channel5->TCNT = uart_obj->UART_Rx_Buf_Cell_Size;
            DMA_ChannelEnable(DMA1_Channel5, ENABLE); 
        }
        #endif
        #ifdef USART2_ENABLE
        else if(uart_obj->UART == USART2)
        {
            DMA_ChannelEnable(DMA1_Channel6, DISABLE);
            DMA1_Channel6->CMBA = (uint32_t)pBuffer->pData;//首次启动DMA接收的时候如果调用dma_memory_address_config(), 会导致: 第一次DAM接收完成中断虽然能够到，但是缓冲区却没有收到数据, 因此把去掉此语句switch段落复制了一份到上面
            DMA1_Channel6->TCNT = uart_obj->UART_Rx_Buf_Cell_Size;
            DMA_ChannelEnable(DMA1_Channel6, ENABLE); 
        }
        #endif
        #ifdef USART3_ENABLE
        else if(uart_obj->UART == USART3)
        {
            DMA_ChannelEnable(DMA1_Channel3, DISABLE);
            DMA1_Channel3->CMBA = (uint32_t)pBuffer->pData;//首次启动DMA接收的时候如果调用dma_memory_address_config(), 会导致: 第一次DAM接收完成中断虽然能够到，但是缓冲区却没有收到数据, 因此把去掉此语句switch段落复制了一份到上面
            DMA1_Channel3->TCNT = uart_obj->UART_Rx_Buf_Cell_Size;
            DMA_ChannelEnable(DMA1_Channel3, ENABLE);
        }
        #endif
        #ifdef UART4_ENABLE
        else if(uart_obj->UART == UART4)
        {
            ;
        }
        #endif
        #ifdef UART5_ENABLE
        else if(uart_obj->UART == UART5)
        {
            ;
        }
        #endif
        
        uart_obj->UART_Rx_Active_Mode = DMA_MODE_ACTIVE;
        
        return HAL_OK;
    }
}

uint8_t Start_UART_DMA_DBM_Rx_Task(UART_ObjectTypeDef *uart_obj, uint8_t *pData, uint8_t init_flag)
{
    if(uart_obj == NULL)
        return HAL_ERROR;
    
    if(uart_obj->UART_Rx_Active_Mode == NONE)
    {
        if(init_flag == 1)
        {
            UART_DMA_Rx_Ring_Buffer_Init(uart_obj);
        }
        
        uart_obj->UART->CTRL1 |= USART_Mode_Rx;
        
        USART_INTConfig(uart_obj->UART, USART_INT_RDNE, DISABLE);                   //开启DMA接收就是为了避免大量串口数据涌入时频繁触发中断, 因此禁用串口接收非空中断 !!!
        
        USART_DMACmd(uart_obj->UART, USART_DMAReq_Rx, ENABLE);
        
        #ifdef USART1_ENABLE
        if(uart_obj->UART == USART1)
        {
            ; 
        }
        #endif
        #ifdef USART2_ENABLE
        else if(uart_obj->UART == USART2)
        {
            ;
        }
        #endif
        #ifdef USART3_ENABLE
        else if(uart_obj->UART == USART3)
        {
            ;
        }
        #endif
        #ifdef UART4_ENABLE
        else if(uart_obj->UART == UART4)
        {
            ;
        }
        #endif
        #ifdef UART5_ENABLE
        else if(uart_obj->UART == UART5)
        {
            ;
        }
        #endif
        
        uart_obj->UART_Rx_Active_Mode = DMA_DBM_MODE_ACTIVE;
        
        return HAL_OK;
    }
    
    return HAL_BUSY;
}

uint8_t Abort_UART_Rx_Task(UART_ObjectTypeDef *uart_obj)
{
    if(uart_obj == NULL)
        return HAL_ERROR;

    uart_obj->UART->CTRL1 &= ~(USART_Mode_Rx);
    
    USART_INTConfig(uart_obj->UART, USART_INT_RDNE, DISABLE);
    
    USART_DMACmd(uart_obj->UART, USART_DMAReq_Rx, DISABLE);
    
    uart_obj->UART_Rx_Active_Mode = NONE;
    
    return 0;
}

int32_t Send_Chars_From_UART(UART_ObjectTypeDef *uart_obj, uint8_t *p_data, uint32_t size)
{
    while(uart_obj->UART_DMA_Tx_State == BUSY);        //如果对应的UART 正工作在DMA发送状态，那么停在这里!!!
    
    uart_obj->UART_DMA_Tx_State = BUSY;
    
    if((size > 0) && (size < uart_obj->UART_Tx_Buf_Byte_Size))
    {
        memcpy(uart_obj->p_UART_Tx_Mem, p_data, size);
    }
    else
    {
        return HAL_BUSY*(-1);;
    }
    
    #ifdef USART1_ENABLE
    if(uart_obj->UART == USART1)
    {
      #ifdef USE_DMA_TX_USART1_DATA
        DMA_ChannelEnable(DMA1_Channel4, DISABLE);     //启动新的DMA传输前必须先Disable一下DMA通道，然后只需再次配置数量、发送起始地址　其他已设置的选项无需重新设置，再然后使能新的传输 
        DMA1_Channel4->CMBA = (uint32_t)USART1_Tx_Mem_Region;
        DMA1_Channel4->TCNT = size;
        DMA_ChannelEnable(DMA1_Channel4, ENABLE); 
      #else
        for(uint16_t i = 0; i < size; i++)
        {
            USART_SendData(USART1, (uint8_t)USART1_Tx_Mem_Region[i]);
            while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TDE));
        }
        while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TRAC));
        UART_TxCpltCallback(USART1);
      #endif
    }
    #endif
    #ifdef USART2_ENABLE
    else if(uart_obj->UART == USART2)
    {
      #ifdef USE_DMA_TX_USART2_DATA
        DMA_ChannelEnable(DMA1_Channel7, DISABLE);
        DMA1_Channel7->CMBA = (uint32_t)USART2_Tx_Mem_Region;
        DMA1_Channel7->TCNT = size;
        DMA_ChannelEnable(DMA1_Channel7, ENABLE); 
      #else
        for(uint16_t i = 0; i < size; i++)
        {
            USART_SendData(USART2, (uint8_t)USART2_Tx_Mem_Region[i]);
            while(RESET == USART_GetFlagStatus(USART2, USART_FLAG_TDE));
        }
        while(RESET == USART_GetFlagStatus(USART2, USART_FLAG_TRAC));
        UART_TxCpltCallback(USART2);
      #endif
    }
    #endif
    #ifdef USART3_ENABLE
    else if(uart_obj->UART == USART3)
    {
      #ifdef USE_DMA_TX_USART3_DATA
        DMA_ChannelEnable(DMA1_Channel2, DISABLE);
        DMA1_Channel2->CMBA = (uint32_t)USART3_Tx_Mem_Region;
        DMA1_Channel2->TCNT = size;
        DMA_ChannelEnable(DMA1_Channel2, ENABLE);
      #else
        for(uint16_t i = 0; i < size; i++)
        {
            USART_SendData(USART3, (uint8_t)USART3_Tx_Mem_Region[i]);
            while(RESET == USART_GetFlagStatus(USART3, USART_FLAG_TDE));
        }
        while(RESET == USART_GetFlagStatus(USART3, USART_FLAG_TRAC));
        UART_TxCpltCallback(USART3);
      #endif
    }
    #endif
    #ifdef UART4_ENABLE
    else if(uart_obj->UART == UART4)
    {
        ;
    }
    #endif
    #ifdef UART5_ENABLE
    else if(uart_obj->UART == UART5)
    {
        ;
    }
    #endif

    return size;
}

bool Wait_Chars_From_UART(UART_ObjectTypeDef *uart_obj, uint32_t wait_size, uint32_t *time_out_ms)
{
    uint32_t  start_time_ms = Systick_Get();           //获取系统当前时间
    uint32_t  elapsed_time_ms = 0;                     //本函数消耗掉的时间 
    
    if(wait_size <= 0)
        return false;

    if(uart_obj->UART_Rx_Active_Mode == DMA_MODE_ACTIVE)
    {
        do 
        {
            if(Get_UART_DMA_Rx_Ready_Size(uart_obj) >= wait_size)
            {
                *time_out_ms -= elapsed_time_ms;
                return true;
            }
            
            elapsed_time_ms = Systick_Get() - start_time_ms;
            
        } while(*time_out_ms > elapsed_time_ms);  
    }
    else if(uart_obj->UART_Rx_Active_Mode == IT_CONT_MODE_ACTIVE)
    {
        do 
        {
            if(Get_UART_IT_CONT_Rx_Ready_Size(uart_obj) >= wait_size)
            {
                *time_out_ms -= elapsed_time_ms;
                return true;
            }
            
            elapsed_time_ms = Systick_Get() - start_time_ms;
            
        } while(*time_out_ms > elapsed_time_ms);   
    }
    
    *time_out_ms = 0;
    
    return false;
}

int32_t Recv_Chars_From_UART(UART_ObjectTypeDef *uart_obj, uint8_t *addr, uint32_t length)
{
    uint32_t i = 0, j = 0;
    
    if((addr == NULL) || (length <= 0))
    {
        return 0;
    }
    
    if((uart_obj->UART_Rx_Active_Mode == DMA_MODE_ACTIVE) || (uart_obj->UART_Rx_Active_Mode == DMA_DBM_MODE_ACTIVE))
    {
        UART_Rx_Buf_Cell_TypeDef *src_buffer = Get_Curr_Ready_UART_DMA_Rx_Buffer_Addr(uart_obj);
        
        if(src_buffer == NULL)
            return 0;
        
        (src_buffer)->State = IS_READING;
        
        do
        {
            for(i = j; i < length; i++)
            {
                addr[i] = (src_buffer)->pData[(src_buffer)->ReadPos];
                
                (src_buffer)->ReadPos ++;
                
                uint32_t context = enter_critical_section ();    //重要: 进入临界区!!!
              
                uart_obj->UART_DMA_DBM_Rx_Ready_Size --;         //递减全局ready size
              
                leave_critical_section (context);                //重要: 退出临界区，挂起的中断允许继续触发!!!
                
                if((src_buffer)->ReadPos >= (src_buffer)->ValidPos)
                {
                    (src_buffer)->State = IS_EMPTY;
                    
                    /*******出列完成,调整队列*******/
                    uart_obj->UART_DMA_Rx_Ring_Buffer.Head++;							            	  
                    if(uart_obj->UART_DMA_Rx_Ring_Buffer.Head > uart_obj->UART_DMA_Rx_Ring_Buffer.Buffer_Size)		 	          
                        uart_obj->UART_DMA_Rx_Ring_Buffer.Head = 0;
                    /*******************************/
                    
                    if(i+1 < length)
                    {
                        src_buffer = Get_Curr_Ready_UART_DMA_Rx_Buffer_Addr(uart_obj);
                    
                        if (src_buffer == NULL)
                        {
                            return i++;
                        }
                        
                        (src_buffer)->State = IS_READING;
                    }
                    
                    i++;
                      
                    break;  
                }
            }
            
            j = i;
            
        } while(i < length);
    }
    else if(uart_obj->UART_Rx_Active_Mode == IT_CONT_MODE_ACTIVE)
    {
        uint8_t *src_buffer = NULL;
        
        for(i = 0; i < length; i++)
        {
            src_buffer = Get_Curr_Ready_UART_IT_CONT_Rx_Buffer_Addr(uart_obj);
            
            if(src_buffer == NULL)
                return i;
    
            addr[i] = *src_buffer; 
           
            uint32_t context = enter_critical_section ();    //重要: 进入临界区!!!
            
            uart_obj->UART_IT_CONT_Rx_Ready_Size --;         //递减全局ready size
            
            leave_critical_section (context);                //重要: 退出临界区，挂起的中断允许继续触发!!!
                
            /*******出列完成,调整队列*******/
            uart_obj->UART_IT_CONT_Rx_Ring_Buffer.Head++;							            	  
            if(uart_obj->UART_IT_CONT_Rx_Ring_Buffer.Head > uart_obj->UART_IT_CONT_Rx_Ring_Buffer.Buffer_Size)		 	          
                uart_obj->UART_IT_CONT_Rx_Ring_Buffer.Head = 0;
            /*******************************/
        }    
    }
    
    return i;
}

uint32_t Get_UART_Ready_Byte_Size(UART_ObjectTypeDef *uart_obj)
{
    if((uart_obj->UART_Rx_Active_Mode == DMA_MODE_ACTIVE) || (uart_obj->UART_Rx_Active_Mode == DMA_DBM_MODE_ACTIVE))
    {
        return uart_obj->UART_DMA_DBM_Rx_Ready_Size;
    }
    else if(uart_obj->UART_Rx_Active_Mode == IT_CONT_MODE_ACTIVE)
    {
        return uart_obj->UART_IT_CONT_Rx_Ready_Size;
    }
    else
    {
        return 0;
    }
}

void UART_TxCpltCallback(USART_Type* usart_periph)
{
    UART_ObjectTypeDef *uart_obj = NULL;
    
    #ifdef USART1_ENABLE
    if(usart_periph == USART1)
    {
        uart_obj = &USART1_OBJ; 
    }
    #endif
    #ifdef USART2_ENABLE
    else if(usart_periph == USART2)
    {
        uart_obj = &USART2_OBJ;
    }
    #endif
    #ifdef USART3_ENABLE
    else if(usart_periph == USART3)
    {
        uart_obj = &USART3_OBJ;
    }
    #endif
    #ifdef UART4_ENABLE
    else if(usart_periph == UART4)
    {
        uart_obj = &UART4_OBJ;
    }
    #endif
    #ifdef UART5_ENABLE
    else if(usart_periph == UART5)
    {
        uart_obj = &UART5_OBJ;
    }
    #endif
    
    if(uart_obj == NULL)
        return;
    
    uart_obj->UART_DMA_Tx_State = IDLE;
    
    if(uart_obj->Max485_Work_State == MAX485_DEV_WORK_ON_TX)
    {
        if(uart_obj->EN_Max485_Rx != NULL)
        {
            uart_obj->EN_Max485_Rx();
        }
        
        uart_obj->Max485_Work_State = MAX485_DEV_WORK_ON_RX;
    }
}

void UART_ErrorCallback(USART_Type* usart_periph)
{
    ;//Todo
}

void UART_IT_CONT_RxCpltCallback(USART_Type* usart_periph)
{
    UART_ObjectTypeDef *uart_obj = NULL;
    
    #ifdef USART1_ENABLE
    if(usart_periph == USART1)
    {
        uart_obj = &USART1_OBJ; 
    }
    #endif
    #ifdef USART2_ENABLE
    else if(usart_periph == USART2)
    {
        uart_obj = &USART2_OBJ;
    }
    #endif
    #ifdef USART3_ENABLE
    else if(usart_periph == USART3)
    {
        uart_obj = &USART3_OBJ;
    }
    #endif
    #ifdef UART4_ENABLE
    else if(usart_periph == UART4)
    {
        uart_obj = &UART4_OBJ;
    }
    #endif
    #ifdef UART5_ENABLE
    else if(usart_periph == UART5)
    {
        uart_obj = &UART5_OBJ;
    }
    #endif
    
    if(uart_obj == NULL)
        return;
    
    if(true == UART_IT_CONT_Rx_Ring_Enqueue(usart_periph->DT, &(uart_obj->UART_IT_CONT_Rx_Ring_Buffer)))
    {
        uart_obj->UART_IT_CONT_Rx_Ready_Size++;
    }
    else
    {
        uart_obj->UART_IT_CONT_Rx_Lost_Size++;
    }
}

void UART_DMA_RxCpltCallback(USART_Type* usart_periph)
{
    static UART_Rx_Buf_Cell_TypeDef  *pNext_Could_Use_Buf_Cell = NULL;    
    
    UART_ObjectTypeDef *uart_obj = NULL;
    
    #ifdef USART1_ENABLE
    if(usart_periph == USART1)
    {
        uart_obj = &USART1_OBJ; 
    }
    #endif
    #ifdef USART2_ENABLE
    else if(usart_periph == USART2)
    {
        uart_obj = &USART2_OBJ;
    }
    #endif
    #ifdef USART3_ENABLE
    else if(usart_periph == USART3)
    {
        uart_obj = &USART3_OBJ;
    }
    #endif
    #ifdef UART4_ENABLE
    else if(usart_periph == UART4)
    {
        uart_obj = &UART4_OBJ;
    }
    #endif
    #ifdef UART5_ENABLE
    else if(usart_periph == UART5)
    {
        uart_obj = &UART5_OBJ;
    }
    #endif
    
    if(uart_obj == NULL)
        return;
    
    UART_DMA_Rx_Ring_Buffer_TypeDef *p_ring = &(uart_obj->UART_DMA_Rx_Ring_Buffer);
    
    Prev_DMA_Time_ms = Curr_DMA_Time_ms;      
    Curr_DMA_Time_ms = Systick_Get();
    
    if(p_ring->pBuffer[p_ring->Tail].State == IS_WRITING)
    {
        p_ring->pBuffer[p_ring->Tail].State = IS_READY;
        
        p_ring->pBuffer[p_ring->Tail].Finish_DMA_Time_ms = Curr_DMA_Time_ms;
      
        p_ring->pBuffer[p_ring->Tail].ValidPos = uart_obj->UART_Rx_Buf_Cell_Size;
        
        p_ring->pBuffer[p_ring->Tail].ReadPos = 0;
        
        uart_obj->UART_DMA_DBM_Rx_Ready_Size += uart_obj->UART_Rx_Buf_Cell_Size;  //更新全局ready size

        p_ring->Tail++;       
        if(p_ring->Tail > p_ring->Buffer_Size)		               
            p_ring->Tail = 0;
                
        if(UART_DMA_Rx_Ring_Enqueue(&pNext_Could_Use_Buf_Cell, p_ring) == true)   //判断FIFO中是否有空闲的区域供后续DMA接收使用
        {
            Start_UART_DMA_Rx_Task(uart_obj, pNext_Could_Use_Buf_Cell, 0);
        }
        else
        {
            uart_obj->UART_DMA_DBM_Rx_Lost_Size += uart_obj->UART_Rx_Buf_Cell_Size;
        }
    }
}

__weak void UART_Clear_Rx_Buffer(UART_ObjectTypeDef *uart_obj)
{
    return;
}

__weak void UART_Clear_Tx_Buffer(UART_ObjectTypeDef *uart_obj)
{
    return;
}
