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

#include "gd25qxx_drv.h"

#define __GD25QXX_DEBUG

int32_t GD25Qxx_Read_Dev_ID(GD25Qxx_Ctx_Typedef *ctx, GD25Qxx_JEDEC_Typedef *jedec)
{
    uint8_t pkt[4] = {GD25QXX_MANUFACTURER_DEVICE_ID, GD25QXX_DUMMY_BYTE, GD25QXX_DUMMY_BYTE, GD25QXX_DUMMY_BYTE};
    
    uint8_t data[2] = {0};
    
    memset((uint8_t*)jedec, 0, sizeof(GD25Qxx_JEDEC_Typedef));
    
    ctx->chip_select();
    
    ctx->write_data(ctx->handle, (uint8_t*)pkt, sizeof(pkt));
    
    ctx->read_data(ctx->handle, (uint8_t*)data, sizeof(data));
    
    ctx->chip_deselect();
    
    jedec->Manufacturer_ID = data[0];
    jedec->Dev_ID = data[1];
    
    return 0;
}

int32_t GD25Qxx_Read_JEDEC(GD25Qxx_Ctx_Typedef *ctx, GD25Qxx_JEDEC_Typedef *jedec)
{
    uint8_t cmd = GD25QXX_READ_IDENTIFICATION;
    
    uint8_t data[3] = {0};
    
    ctx->chip_select();
    
    ctx->write_data(ctx->handle, (uint8_t*)&cmd, sizeof(cmd));
    
    ctx->read_data(ctx->handle, (uint8_t*)data, sizeof(data));
    
    ctx->chip_deselect();
    
    jedec->Manufacturer_ID = data[0];
    jedec->Memory_Type = data[1];
    jedec->Capacity_Type = data[2];
      
    return 0;
}

int32_t GD25Qxx_Write_Enable(GD25Qxx_Ctx_Typedef *ctx)
{
    uint8_t cmd = GD25QXX_WRITE_ENABLE;
    
    ctx->chip_select();
    
    ctx->write_data(ctx->handle, (uint8_t*)&cmd, sizeof(cmd));
    
    ctx->chip_deselect();
    
    GD25Qxx_Delay(1);
    
    return 0;
}

int32_t GD25Qxx_Write_Disable(GD25Qxx_Ctx_Typedef *ctx)
{
    uint8_t cmd = GD25QXX_WRITE_DISABLE;
    
    ctx->chip_select();
    
    ctx->write_data(ctx->handle, (uint8_t*)&cmd, sizeof(cmd));
    
    ctx->chip_deselect();
    
    GD25Qxx_Delay(1);
    
    return 0;
}

int32_t GD25Qxx_Read_SR(GD25Qxx_Ctx_Typedef *ctx, uint8_t sr_n, uint8_t *val)
{
    if(sr_n == 0)
        return -1;    //经过测试, 对于GD25Q127C, 连续读取状态寄存器会读到同一个值, 所以临时禁用连续读取模式
  
    uint8_t cmd = 0;
    uint8_t data[3] = {0};

    if((sr_n == 0) || (sr_n == 1))
    {
        cmd = GD25QXX_READ_SR_1;     
    }
    else if(sr_n == 2)
    {
        cmd = GD25QXX_READ_SR_2;
    }
    else if(sr_n == 3)
    {
        cmd = GD25QXX_READ_SR_3;
    }
    else
    {
        return -1;
    }
    
    ctx->chip_select();
    
    ctx->write_data(ctx->handle, (uint8_t*)&cmd, sizeof(cmd));
    
    if(sr_n == 0)
    {
        ctx->read_data(ctx->handle, (uint8_t*)data, sizeof(data));
    }
    else
    {
        ctx->read_data(ctx->handle, (uint8_t*)data, 1);
    }
    
    ctx->chip_deselect();
    
    if(sr_n == 0)
    {
        *val = data[0];
        *(val+1) = data[1];
        *(val+2) = data[2];
    }
    else
    {
        *val = data[0];
    }
    
    return 0;
}

int32_t GD25Qxx_Write_SR(GD25Qxx_Ctx_Typedef *ctx, uint8_t sr_n, uint8_t val)
{
    uint8_t pkt[2] = {0};
    
    if(sr_n == 1)
    {
        pkt[0] = GD25QXX_WRITE_SR_1;     
    }
    else if(sr_n == 2)
    {
        pkt[0] = GD25QXX_WRITE_SR_2;
    }
    else if(sr_n == 3)
    {
        pkt[0] = GD25QXX_WRITE_SR_3;
    }
    else
    {
        return -1;
    }
    
    pkt[1] = val;
    
    if(GD25Qxx_Write_Enable(ctx) != 0)
        return -1;
    
    ctx->chip_select();
    ctx->write_data(ctx->handle, (uint8_t*)pkt, sizeof(pkt));
    ctx->chip_deselect();
    
    GD25Qxx_Delay(1);    //For GD25Q127C must have delay here!!!
    
    return 0;
}

bool GD25Qxx_Wait_For_Not_Busy(GD25Qxx_Ctx_Typedef *ctx, uint16_t timeout_ms)
{
    uint8_t reg = 0;
  
    uint16_t wait_time_ms = 0;

    do
    {
        GD25Qxx_Read_SR(ctx, 1, (uint8_t*)&reg);
        
        if((reg & 0x01) == 0x00)
            break;
        
        GD25Qxx_Delay(1);
        
        wait_time_ms++;
        
        if(wait_time_ms >= timeout_ms)
            return false;
        
    } while((reg & 0x01) == 0x01);
    
    return true;
}

int32_t GD25Qxx_Info_Init(GD25Qxx_Drv_Typedef *gd25qxx_drv)
{
    if((gd25qxx_drv->DevCtx.chip_select == NULL) || (gd25qxx_drv->DevCtx.write_data == NULL) || (gd25qxx_drv->DevCtx.read_data == NULL) || (gd25qxx_drv->DevCtx.chip_deselect == NULL) || (gd25qxx_drv->DevCtx.handle == NULL))
        return -1;    

    uint8_t retry_time = 0;
    
    do
    {
        GD25Qxx_Read_Dev_ID((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), &(gd25qxx_drv->JEDEC));
        
        Delay_ms(20);
    
        retry_time++;
        
    } while((gd25qxx_drv->JEDEC.Dev_ID != GD25Q127C_DEVICE_ID) && (retry_time <= 3));
    
    GD25Qxx_Read_JEDEC((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), &(gd25qxx_drv->JEDEC));
    
    switch(gd25qxx_drv->JEDEC.Capacity_Type & 0x000000FF)
    {
		case 0x20:	// 	GD25Q512
          gd25qxx_drv->Type = GD25Q512;
          gd25qxx_drv->BlockCount = 1024;
          #ifdef __GD25QXX_DEBUG
          printf("GD25Qxx Chip: GD25Q512\r\n");
          #endif
          break;
          
        case 0x19:	// 	GD25Q256
          gd25qxx_drv->Type = GD25Q256;
          gd25qxx_drv->BlockCount = 512;
          #ifdef __GD25QXX_DEBUG
          printf("GD25Qxx Chip: GD25Q256\r\n");
          #endif
          break;
          
        case 0x18:	// 	GD25Q128
          gd25qxx_drv->Type = GD25Q128;
          gd25qxx_drv->BlockCount = 256;
          #ifdef __GD25QXX_DEBUG
          printf("GD25Qxx Chip: GD25Q128\r\n");
          #endif
          break;
          
        case 0x17:	//	GD25Q64
          gd25qxx_drv->Type = GD25Q64;
          gd25qxx_drv->BlockCount = 128;
          #ifdef __GD25QXX_DEBUG
          printf("GD25Qxx Chip: GD25Q64\r\n");
          #endif
          break;
          
        case 0x16:	//	GD25Q32
          gd25qxx_drv->Type = GD25Q32;
          gd25qxx_drv->BlockCount = 64;
          #ifdef __GD25QXX_DEBUG
          printf("GD25Qxx Chip: GD25Q32\r\n");
          #endif
          break;
          
        case 0x15:	//	GD25Q16
          gd25qxx_drv->Type = GD25Q16;
          gd25qxx_drv->BlockCount = 32;
          #ifdef __GD25QXX_DEBUG
          printf("GD25Qxx Chip: GD25Q16\r\n");
          #endif
          break;
          
        case 0x14:	//	GD25Q80
          gd25qxx_drv->Type = GD25Q80;
          gd25qxx_drv->BlockCount = 16;
          #ifdef __GD25QXX_DEBUG
          printf("GD25Qxx Chip: GD25Q80\r\n");
          #endif
          break;
          
        case 0x13:	//	GD25Q40
          gd25qxx_drv->Type = GD25Q40;
          gd25qxx_drv->BlockCount = 8;
          #ifdef __GD25QXX_DEBUG
          printf("GD25Qxx Chip: GD25Q40\r\n");
          #endif
          break;
          
        case 0x12:	//	GD25Q20
          gd25qxx_drv->Type = GD25Q20;
          gd25qxx_drv->BlockCount = 4;
          #ifdef __GD25QXX_DEBUG
          printf("GD25Qxx Chip: GD25Q20\r\n");
          #endif
          break;
        case 0x11:	//	GD25Q10
          gd25qxx_drv->Type = GD25Q10;
          gd25qxx_drv->BlockCount = 2;
          #ifdef __GD25QXX_DEBUG
          printf("GD25Qxx Chip: GD25Q10\r\n");
          #endif
          break;
          
        default:
          return -1;	
    }
    
    gd25qxx_drv->PageSize = GD25QXX_PAGE_SIZE;
    
    gd25qxx_drv->SectorSize = GD25QXX_SECTOR_SIZE;
    
    gd25qxx_drv->SectorCount = gd25qxx_drv->BlockCount * 16;
    
    gd25qxx_drv->PageCount = (gd25qxx_drv->SectorCount * gd25qxx_drv->SectorSize) / gd25qxx_drv->PageSize;
    
    gd25qxx_drv->BlockSize = gd25qxx_drv->SectorSize * 16;
    
    gd25qxx_drv->CapacityInKByte=(gd25qxx_drv->SectorCount * gd25qxx_drv->SectorSize) / 1024;

    GD25Qxx_Read_SR((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), 1, (uint8_t*)&(gd25qxx_drv->StatusReg.SR1));
    GD25Qxx_Read_SR((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), 2, (uint8_t*)&(gd25qxx_drv->StatusReg.SR2));
    GD25Qxx_Read_SR((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), 3, (uint8_t*)&(gd25qxx_drv->StatusReg.SR3));
    
    gd25qxx_drv->StatusReg.SRLock = 0;
    
    #ifdef __GD25QXX_DEBUG
	printf("GD25Qxx PageSize: %d Bytes\r\n", gd25qxx_drv->PageSize);
	printf("GD25Qxx PageCount: %d\r\n", gd25qxx_drv->PageCount);
	printf("GD25Qxx SectorSize: %d Bytes\r\n", gd25qxx_drv->SectorSize);
	printf("GD25Qxx SectorCount: %d\r\n", gd25qxx_drv->SectorCount);
	printf("GD25Qxx BlockSize: %d Bytes\r\n", gd25qxx_drv->BlockSize);
	printf("GD25Qxx BlockCount: %d\r\n", gd25qxx_drv->BlockCount);
	printf("GD25Qxx Capacity: %d KBytes\r\n", gd25qxx_drv->CapacityInKByte);
	printf("GD25Qxx Init Done\r\n");
	#endif
    
    return 0;
}

int32_t GD25Qxx_Erase_Chip(GD25Qxx_Ctx_Typedef *ctx, uint32_t timeout_ms)
{
    if(GD25Qxx_Write_Enable(ctx) != 0)
        return -1;
    
    int32_t ret = 0;
    
    uint8_t cmd = GD25QXX_CHIP_ERASE; 
    
    GD25Qxx_Write_Enable(ctx);
    
    ctx->chip_select();
    
    ctx->write_data(ctx->handle, (uint8_t*)&cmd, sizeof(cmd));
    
    ctx->chip_deselect();
    
    if(GD25Qxx_Wait_For_Not_Busy(ctx, timeout_ms) == false)
        ret = -1;
    
    GD25Qxx_Delay(10);
    
    return ret;
}

int32_t GD25Qxx_Erase_Sector(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type, uint16_t sector_n, uint16_t timeout_ms)    //sector_n: 0~4095
{
    if(GD25Qxx_Write_Enable(ctx) != 0)
        return -1;
    
    int32_t ret = 0;
    
    uint8_t i = 0;
    
    uint8_t pkt[5] = {0};
    
    uint32_t sector_addr = GD25QXX_GET_SECTOR_n_ADDR(sector_n);
    
    //: fix me if sector_addr beyond chip's max addr ....
    
    pkt[i++] = GD25QXX_SECTOR_ERASE;
    
    if(type >= GD25Q256)
        pkt[i++] = (sector_addr & 0xFF000000) >> 24;
    
    pkt[i++] = (sector_addr & 0xFF0000) >> 16;
    pkt[i++] = (sector_addr & 0xFF00) >> 8;
    pkt[i++] = (sector_addr & 0xFF);
    
    if(GD25Qxx_Wait_For_Not_Busy(ctx, GD25Q127C_TYPICAL_SECTOR_ERASE_TIME_MS) == false)
        return -1;
    
    GD25Qxx_Write_Enable(ctx);
    
    ctx->chip_select();
    
    ctx->write_data(ctx->handle, (uint8_t*)pkt, i);
    
    ctx->chip_deselect();
    
    if(GD25Qxx_Wait_For_Not_Busy(ctx, timeout_ms) == false)
        ret = -1;
    
    GD25Qxx_Delay(1);
    
    return ret;
}

int32_t GD25Qxx_Erase_Block(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type, uint16_t block_n, uint16_t timeout_ms)    //block_n: 0~255
{
    if(GD25Qxx_Write_Enable(ctx) != 0)
        return -1;
    
    int32_t ret = 0;
    
    uint8_t i = 0;
    
    uint8_t pkt[5] = {0};
    
    uint32_t block_addr = GD25QXX_GET_BLOCK_n_ADDR(block_n);
    
    //: fix me if block_addr beyond chip's max addr ....
    
    pkt[i++] = GD25QXX_BLOCK_ERASE_64K;
    
    if(type >= GD25Q256)
        pkt[i++] = (block_addr & 0xFF000000) >> 24;
    
    pkt[i++] = (block_addr & 0xFF0000) >> 16;
    pkt[i++] = (block_addr & 0xFF00) >> 8;
    pkt[i++] = (block_addr & 0xFF);
    
    if(GD25Qxx_Wait_For_Not_Busy(ctx, GD25Q127C_TYPICAL_BLOCK_ERASE_TIME_MS) == false)
        return -1;
    
    GD25Qxx_Write_Enable(ctx);
    
    ctx->chip_select();
    
    ctx->write_data(ctx->handle, (uint8_t*)pkt, i);
    
    ctx->chip_deselect();
    
    if(GD25Qxx_Wait_For_Not_Busy(ctx, timeout_ms) == false)
        ret = -1;
    
    GD25Qxx_Delay(1);
    
    return ret;
}

/* 对GD25Qxx进行写入的时候只能按页(page)操作, 如果数据字节数超过了256字节, 地址将自动回到页的起始地址(不会自动跨页)，覆盖掉之前的数据, 对于长度超过256字节, 需要用户代码判断写入下一页 */
int32_t GD25Qxx_Write_Data(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type, uint32_t page_n, uint32_t offset, uint8_t *buf, uint32_t len)    //offset: 0~255
{
    if((offset > 0xFF) || (len == 0))
        return 0;
    
    uint8_t i = 0;
    
    uint8_t pkt[5] = {0};
    
    uint32_t write_addr = GD25QXX_GET_PAGE_n_ADDR(page_n) + offset;
    
    uint32_t real_cnt = GD25QXX_GET_REAL_BYTE_CNT(offset, len);
    
    //: fix me if write_addr beyond chip's max addr ....
    
    pkt[i++] = GD25QXX_PAGE_PROGRAM;
    
    if(type >= GD25Q256)
        pkt[i++] = (write_addr & 0xFF000000) >> 24;
    
    pkt[i++] = (write_addr & 0xFF0000) >> 16;
    pkt[i++] = (write_addr & 0xFF00) >> 8;
    pkt[i++] = (write_addr & 0xFF);
    
    if(GD25Qxx_Wait_For_Not_Busy(ctx, GD25Q127C_TYPICAL_SECTOR_ERASE_TIME_MS) == false)
        return 0;

    if(GD25Qxx_Write_Enable(ctx) != 0)
        return -1;
    
    ctx->chip_select();
    
    ctx->write_data(ctx->handle, (uint8_t*)pkt, i);
    
    ctx->write_data(ctx->handle, (uint8_t*)buf, real_cnt);
    
    ctx->chip_deselect();
    
    GD25Qxx_Wait_For_Not_Busy(ctx, GD25Q127C_TYPICAL_PAGE_PROGRAM_TIME_MS);
    
    GD25Qxx_Delay(1);
    
    return real_cnt;
}

int32_t GD25Qxx_Read_Data(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type, uint32_t page_n, uint32_t offset, uint8_t *buf, uint32_t len)    //offset: 0~255
{
    if((offset > 0xFF) || (len == 0))
        return 0;
    
    uint8_t i = 0;
    
    uint8_t pkt[5] = {0};
    
    uint32_t read_addr = GD25QXX_GET_PAGE_n_ADDR(page_n) + offset;
    
    uint32_t real_cnt = 0;
    
    //: fix me if write_addr beyond chip's max addr ....
    
    pkt[i++] = GD25QXX_READ_DATA;
    
    if(type >= GD25Q256)
        pkt[i++] = (read_addr & 0xFF000000) >> 24;
    
    pkt[i++] = (read_addr & 0xFF0000) >> 16;
    pkt[i++] = (read_addr & 0xFF00) >> 8;
    pkt[i++] = (read_addr & 0xFF);
    
    if(GD25Qxx_Wait_For_Not_Busy(ctx, GD25Q127C_TYPICAL_SECTOR_ERASE_TIME_MS) == false)
        return 0;
    
    ctx->chip_select();
    
    ctx->write_data(ctx->handle, (uint8_t*)pkt, i);
    
    ctx->read_data(ctx->handle, (uint8_t*)buf, len);
    
    ctx->chip_deselect();
    
    real_cnt = len;
    
    return real_cnt;
}

/* 对GD25Qxx进行读出的时候, 给出页(page)和偏移量之后, 理论上只要SPI CLK一直在就可以一直吐出数据(写操作则不能够这样, 写操作不能自动跨页, 读则可以自动跨页), 实测单次读取不要超过64K!!! */
int32_t GD25Qxx_Fast_Read_Data(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type, uint32_t page_n, uint32_t offset, uint8_t *buf, uint32_t len)    //offset: 0~255
{
    if((offset > 0xFF) || (len == 0))
        return 0;
    
    uint8_t i = 0;
    
    uint8_t pkt[6] = {0};
    
    uint32_t read_addr = GD25QXX_GET_PAGE_n_ADDR(page_n) + offset;
    
    uint32_t real_cnt = 0;
    
    //: fix me if write_addr beyond chip's max addr ....
    
    pkt[i++] = GD25QXX_FAST_READ;
    
    if(type >= GD25Q256)
        pkt[i++] = (read_addr & 0xFF000000) >> 24;
    
    pkt[i++] = (read_addr & 0xFF0000) >> 16;
    pkt[i++] = (read_addr & 0xFF00) >> 8;
    pkt[i++] = (read_addr & 0xFF);
    
    if(GD25Qxx_Wait_For_Not_Busy(ctx, GD25Q127C_TYPICAL_SECTOR_ERASE_TIME_MS) == false)
        return 0;
    
    ctx->chip_select();
    
    ctx->write_data(ctx->handle, (uint8_t*)pkt, i + 1);
    
    ctx->read_data(ctx->handle, (uint8_t*)buf, len);
    
    ctx->chip_deselect();
    
    real_cnt = len;
    
    return real_cnt;
}

bool Enable_The_Top_Sector_Protection(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type)
{
    GD25Qxx_Status_Reg_Typedef status_reg = {0};
    
    switch(type)
    {
        case GD25Q128:
          {
              if((GD25Qxx_Read_SR(ctx, 1, (uint8_t*)&status_reg.SR1.byte) == 0) && ((GD25Qxx_Read_SR(ctx, 2, (uint8_t*)&status_reg.SR1.byte) == 0)))
              {
                  GD25QXX_SET_BIT((status_reg.SR1.byte), GD25QXX_SR1_BP0_BIT);
                  GD25QXX_CLEAR_BIT(status_reg.SR1.byte, GD25QXX_SR1_BP1_BIT);
                  GD25QXX_CLEAR_BIT(status_reg.SR1.byte, GD25QXX_SR1_BP2_BIT);
                  GD25QXX_CLEAR_BIT(status_reg.SR1.byte, GD25QXX_SR1_BP3_BIT);
                  GD25QXX_SET_BIT((status_reg.SR1.byte), GD25QXX_SR1_BP4_BIT);
                  
                  GD25QXX_CLEAR_BIT((status_reg.SR2.byte), GD25QXX_SR2_CMP_BIT);
                  
                  if((GD25Qxx_Write_SR(ctx, 1, (uint8_t)(status_reg.SR1.byte)) == 0) && (GD25Qxx_Write_SR(ctx, 2, (uint8_t)(status_reg.SR2.byte)) == 0))
                  {
                      return true;
                  }
              }
          }
          break;
          
        default:
          break;
    }
    
    return false;
}

bool Disable_The_Top_Sector_Protection(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type)
{
    GD25Qxx_Status_Reg_Typedef status_reg = {0};
    
    switch(type)
    {
        case GD25Q128:
          {
              if((GD25Qxx_Read_SR(ctx, 1, (uint8_t*)&status_reg.SR1.byte) == 0) && ((GD25Qxx_Read_SR(ctx, 2, (uint8_t*)&status_reg.SR1.byte) == 0)))
              {
                  GD25QXX_SET_BIT((status_reg.SR1.byte), GD25QXX_SR1_BP0_BIT);
                  GD25QXX_CLEAR_BIT(status_reg.SR1.byte, GD25QXX_SR1_BP1_BIT);
                  GD25QXX_CLEAR_BIT(status_reg.SR1.byte, GD25QXX_SR1_BP2_BIT);
                  GD25QXX_CLEAR_BIT(status_reg.SR1.byte, GD25QXX_SR1_BP3_BIT);
                  GD25QXX_SET_BIT((status_reg.SR1.byte), GD25QXX_SR1_BP4_BIT);
                            
                  GD25QXX_SET_BIT((status_reg.SR2.byte), GD25QXX_SR2_CMP_BIT);
                  
                  if((GD25Qxx_Write_SR(ctx, 1, (uint8_t)(status_reg.SR1.byte)) == 0) && (GD25Qxx_Write_SR(ctx, 2, (uint8_t)(status_reg.SR2.byte)) == 0))
                  {
                      return true;
                  }
              }
          }
          break;
          
        default:
          break;
    }
    
    return false;
}

bool GD25Qxx_Is_Page_Empty(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type, uint32_t page_n)
{
    uint32_t i = 0, j = 0;
      
    uint8_t buf[32] = {0};
    
    uint32_t temp = GD25QXX_PAGE_SIZE/sizeof(buf);
      
    for(i = 0; i < temp; i++)
    {
        GD25Qxx_Fast_Read_Data(ctx, type, page_n, 0, buf, sizeof(buf));
        
        for(j = 0; j < sizeof(buf); j++)
        {
            if(buf[j] != GD25QXX_DEFAULT_BYTE)
                return false;
        }
    }
    
    return true;
}

bool GD25Qxx_Is_Sector_Empty(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type, uint32_t sector_n)
{
    uint32_t i = 0;
    
    uint32_t temp =  GD25QXX_SECTOR_SIZE/GD25QXX_PAGE_SIZE;
    
    for(i = 0; i < temp; i++)
    {
        if(GD25Qxx_Is_Page_Empty(ctx, type, GD25QXX_GET_SECTOR_n_START_PAGE_n(sector_n) + i) == false)
            return false;
    }
    
    return true;
}

bool GD25Qxx_Is_Block_Empty(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type, uint32_t block_n)
{
    uint32_t i = 0;
    
    uint32_t temp = GD25QXX_BLOCK_SIZE/GD25QXX_PAGE_SIZE;
    
    for(i = 0; i < temp; i++)
    {
        if(GD25Qxx_Is_Page_Empty(ctx, type, GD25QXX_GET_BLOCK_n_START_PAGE_n(block_n) + i) == false)
            return false;
    }
    
    return true;
}

bool GD25Qxx_Store_Capacity_State(GD25Qxx_Drv_Typedef *gd25qxx_drv)
{
    if(GD25Qxx_Wait_For_Not_Busy((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), 0xFF) == false)
        return false;
    
    Disable_The_Top_Sector_Protection((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), gd25qxx_drv->Type);

    //GD25Qxx_Read_SR((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), 1, (uint8_t*)&(gd25qxx_drv->StatusReg.SR1));
    
    //GD25Qxx_Read_SR((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), 2, (uint8_t*)&(gd25qxx_drv->StatusReg.SR2));
    
    //GD25Qxx_Read_SR((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), 3, (uint8_t*)&(gd25qxx_drv->StatusReg.SR3));
    
    GD25Qxx_Erase_Sector((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), gd25qxx_drv->Type, gd25qxx_drv->SectorCount - 1, GD25Q127C_TYPICAL_SECTOR_ERASE_TIME_MS);
    
    GD25Qxx_Write_Data((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), gd25qxx_drv->Type, gd25qxx_drv->PageCount - 1, 0, (uint8_t*)&(gd25qxx_drv->BlockState), sizeof(gd25qxx_drv->BlockState));
    
    Enable_The_Top_Sector_Protection((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), gd25qxx_drv->Type);
  
    //GD25Qxx_Read_SR((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), 1, (uint8_t*)&(gd25qxx_drv->StatusReg.SR1));
    
    //GD25Qxx_Read_SR((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), 2, (uint8_t*)&(gd25qxx_drv->StatusReg.SR2));
    
    //GD25Qxx_Read_SR((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), 3, (uint8_t*)&(gd25qxx_drv->StatusReg.SR3));
    
    return true;
}

bool GD25Qxx_Check_Capacity_State(GD25Qxx_Drv_Typedef *gd25qxx_drv)
{
    uint32_t i = 0;
    
    if(GD25Qxx_Wait_For_Not_Busy((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), 0xFF) == false)
        return false;
    
    for(i = 0; i < gd25qxx_drv->BlockCount; i++)
    {
        if(GD25Qxx_Is_Block_Empty((GD25Qxx_Ctx_Typedef*)&(gd25qxx_drv->DevCtx), gd25qxx_drv->Type, i) == true)
            (gd25qxx_drv->BlockState[i/8].byte) &= ~(1 << (i % 8));
        else
            (gd25qxx_drv->BlockState[i/8].byte) |=  (1 << (i % 8));
    }
    
    GD25Qxx_Delay(1);
    
    return true;
}
