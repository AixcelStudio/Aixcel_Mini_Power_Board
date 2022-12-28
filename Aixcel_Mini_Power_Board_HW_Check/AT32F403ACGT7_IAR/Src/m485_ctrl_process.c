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

#include "m485_ctrl_process.h"
#include "uart_drv.h"
#include "mbcrc.h"

void M485_Clear_Rx_Buffer(void *fd)
{
    UART_Clear_Rx_Buffer((UART_ObjectTypeDef*)fd);
}

void M485_Clear_Tx_Buffer(void *fd)
{
    UART_Clear_Tx_Buffer((UART_ObjectTypeDef*)fd);
}

uint32_t Get_M485_Ready_Byte_Size(void *fd)
{
    return Get_UART_Ready_Byte_Size((UART_ObjectTypeDef*)fd);
}

int32_t Send_Chars_From_M485(void *fd, uint8_t *addr, uint32_t count)
{
    if(((UART_ObjectTypeDef*)fd)->EN_Max485_Tx != NULL)
    {
        ((UART_ObjectTypeDef*)fd)->EN_Max485_Tx();
    }
  
    ((UART_ObjectTypeDef*)fd)->Max485_Work_State = MAX485_DEV_WORK_ON_TX;
    
    return Send_Chars_From_UART((UART_ObjectTypeDef*)fd, addr, count);
}

int32_t Recv_Chars_From_M485(void *fd, uint8_t *addr, uint32_t count)
{
    return Recv_Chars_From_UART((UART_ObjectTypeDef*)fd, addr, count);
}

bool Wait_Chars_From_M485(void *fd, uint32_t wait_size, uint32_t *time_out_ms)
{
    return Wait_Chars_From_UART((UART_ObjectTypeDef*)fd, wait_size, time_out_ms);
}

