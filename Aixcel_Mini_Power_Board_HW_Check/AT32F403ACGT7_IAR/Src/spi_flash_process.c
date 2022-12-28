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

#include "spi_flash_process.h"
#include "hwconf.h"

SPI_Flash_Dev_Typedef SPIFlashDev = {0x00};

#define __SPI_FLASH_DEBUG

#ifdef __SPI_FLASH_DEBUG
static uint8_t start_page[GD25QXX_PAGE_SIZE] = {0};
#endif

int32_t SPI_Flash_Dev_Init(void)
{
    SPIFlashDev.GD25Qxx_Drv.DevCtx.chip_select = SPI_Flash_Chip_Select;
    SPIFlashDev.GD25Qxx_Drv.DevCtx.write_data = SPI_Flash_Write_Data;
    SPIFlashDev.GD25Qxx_Drv.DevCtx.read_data = SPI_Flash_Read_Data;
    SPIFlashDev.GD25Qxx_Drv.DevCtx.chip_deselect = SPI_Flash_Chip_Deselect;
    
    SPIFlashDev.GD25Qxx_Drv.DevCtx.handle = (SPI_Type*)SPI2;
    
    SPIFlashDev.GD25Qxx_Drv.DevCtx.chip_deselect();
    
    Delay_ms(100);
    
    if(GD25Qxx_Info_Init((GD25Qxx_Drv_Typedef*)&SPIFlashDev.GD25Qxx_Drv) == 0)
    {
      #ifdef __SPI_FLASH_DEBUG
        printf("SPIFlashDev.GD25Qxx_Drv.JEDEC.Manufacturer_ID = 0x%02X\nSPIFlashDev.GD25Qxx_Drv.JEDEC.Memory_Type = 0x%02X\nSPIFlashDev.GD25Qxx_Drv.JEDEC.Capacity_Type = 0x%02X\nSPIFlashDev.GD25Qxx_Drv.JEDEC.Dev_ID = 0x%02X\n", SPIFlashDev.GD25Qxx_Drv.JEDEC.Manufacturer_ID, SPIFlashDev.GD25Qxx_Drv.JEDEC.Memory_Type, SPIFlashDev.GD25Qxx_Drv.JEDEC.Capacity_Type, SPIFlashDev.GD25Qxx_Drv.JEDEC.Dev_ID);
        
        GD25Qxx_Erase_Sector((GD25Qxx_Ctx_Typedef*)&(SPIFlashDev.GD25Qxx_Drv.DevCtx), SPIFlashDev.GD25Qxx_Drv.Type, 0, 0xFF);
        
        for(uint8_t n = 0; n < 0x80; n++)
        {
            GD25Qxx_Write_Data((GD25Qxx_Ctx_Typedef*)&(SPIFlashDev.GD25Qxx_Drv.DevCtx), SPIFlashDev.GD25Qxx_Drv.Type, 0, n, (uint8_t*)&n, 1);
        }
        
        SPI_Flash_Check_Capacity_State((SPI_Flash_Dev_Typedef*)&SPIFlashDev);   //For GD25Q127C it cost about 60s to read all pages!!!
        
        GD25Qxx_Fast_Read_Data((GD25Qxx_Ctx_Typedef*)&(SPIFlashDev.GD25Qxx_Drv.DevCtx), SPIFlashDev.GD25Qxx_Drv.Type, 0, 0, start_page, GD25QXX_PAGE_SIZE);
        
        for(uint32_t i = 0; i < GD25QXX_PAGE_SIZE; i++)
        {
            printf("start_page[%d] = 0x%X\r\n", i, start_page[i]);
        }
        
        GD25Qxx_Erase_Sector((GD25Qxx_Ctx_Typedef*)&(SPIFlashDev.GD25Qxx_Drv.DevCtx), SPIFlashDev.GD25Qxx_Drv.Type, 0, 0xFF);
      #endif
        
        return 0;
    }
    
    return -1;
}

int32_t SPI_Flash_Chip_Select(void)
{
    SPI_Flash_SELECT();

    return 0;
}

int32_t SPI_Flash_Chip_Deselect(void)
{
    SPI_Flash_DESELECT();

    return 0;
}

int32_t SPI_Flash_Write_Data(SPI_Type* handle, uint8_t* bufp, uint16_t len)
{
    return SPI_Transmmit_Data(handle, bufp, len, 0xFFFF);
}

int32_t SPI_Flash_Read_Data(SPI_Type* handle, uint8_t* bufp, uint16_t len)
{
    return SPI_Receive_Data(handle, bufp, len, 0xFFFF);
}

bool SPI_Flash_Store_Capacity_State(SPI_Flash_Dev_Typedef *dev)
{
    return GD25Qxx_Store_Capacity_State(&(dev->GD25Qxx_Drv));
}

bool SPI_Flash_Check_Capacity_State(SPI_Flash_Dev_Typedef *dev)
{
    return GD25Qxx_Check_Capacity_State(&(dev->GD25Qxx_Drv));
}
