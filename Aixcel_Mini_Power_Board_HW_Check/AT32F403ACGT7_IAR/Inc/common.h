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

#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f4xx.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include "systick.h"
  
//#define __DEBUG

#ifdef __DEBUG
#define DBG_OUT(...) printf(__VA_ARGS__)
#else
#define DBG_OUT(...)
#endif  
  
typedef enum
{
    HAL_OK       = 0x00U,
    HAL_ERROR    = 0x01U,
    HAL_BUSY     = 0x02U,
    HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

#ifndef M_PI
#define M_PI	(3.14159265358979323846f)
#endif
#ifndef ONE_G
#define ONE_G	(9.807f)
#endif

typedef enum
{
    Key_IS_IDLE,
    Key_IS_PRESSED,
    Key_IS_RELEASED,
} Key_State_Typedef;

typedef enum
{
    CAN_BAUDRATE_5K    =  0x00,
    CAN_BAUDRATE_10K   =  0x01,
    CAN_BAUDRATE_20K   =  0x02,
    CAN_BAUDRATE_50K   =  0x03,
    CAN_BAUDRATE_100K  =  0x04,
    CAN_BAUDRATE_125K  =  0x05,
    CAN_BAUDRATE_250K  =  0x06,
    CAN_BAUDRATE_500K  =  0x07,
    CAN_BAUDRATE_600K  =  0x08,
    CAN_BAUDRATE_750K  =  0x09,
    CAN_BAUDRATE_800K  =  0x0A,
    CAN_BAUDRATE_1M    =  0x0B,
} CAN_Baudrate_Typedef;

extern inline unsigned int enter_critical_section(void);
extern inline void leave_critical_section(unsigned int context);

uint8_t SPI_Read_Write_Byte(SPI_Type* handle, uint8_t ch);

int32_t SPI_Write_Data(SPI_Type* handle, uint8_t* bufp, uint16_t len, uint32_t timeout);

int32_t SPI_Read_Data(SPI_Type* handle, uint8_t* bufp, uint16_t len, uint32_t timeout);

int32_t SPI_Transmmit_Data(SPI_Type* handle, uint8_t* bufp, uint16_t len, uint32_t timeout);

int32_t SPI_Receive_Data(SPI_Type* handle, uint8_t* bufp, uint16_t len, uint32_t timeout);

uint8_t System_I2C_Master_Transmit(I2C_Type* i2c_periph, uint8_t addr, uint8_t *data, uint8_t len);

uint8_t System_I2C_Master_Receive(I2C_Type* i2c_periph, uint8_t addr, uint8_t *data, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */
