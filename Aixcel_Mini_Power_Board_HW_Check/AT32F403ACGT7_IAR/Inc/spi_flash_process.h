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

#ifndef SPI_FLASH_PROCESS_H
#define SPI_FLASH_PROCESS_H

#ifdef __cplusplus
  extern "C" {
#endif

#include "common.h"
#include "gd25qxx_drv.h"

typedef struct __SPI_Flash_Drv_Typedef
{
    GD25Qxx_Drv_Typedef GD25Qxx_Drv;
    
} SPI_Flash_Dev_Typedef;

extern SPI_Flash_Dev_Typedef SPIFlashDev;

int32_t SPI_Flash_Dev_Init(void);

int32_t SPI_Flash_Chip_Select(void);

int32_t SPI_Flash_Chip_Deselect(void);

int32_t SPI_Flash_Write_Data(SPI_Type* handle, uint8_t* bufp, uint16_t len);

int32_t SPI_Flash_Read_Data(SPI_Type* handle, uint8_t* bufp, uint16_t len);

bool SPI_Flash_Store_Capacity_State(SPI_Flash_Dev_Typedef *dev);

bool SPI_Flash_Check_Capacity_State(SPI_Flash_Dev_Typedef *dev);
    
#ifdef __cplusplus
}
#endif

#endif /* SPI_FLASH_PROCESS_H */
