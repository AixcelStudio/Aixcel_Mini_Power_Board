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
#ifndef GD25QXX_DRV_H
#define GD25QXX_DRV_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "common.h"

#if (_GD25QXX_USE_FREERTOS==1)
#define	GD25Qxx_Delay(delay)		  osDelay(delay)
#include "cmsis_os.h"
#else
#define	GD25Qxx_Delay(delay)		  Delay_ms(delay)
#endif

/***********************************Instrcution*******************************************/
#define GD25QXX_WRITE_ENABLE                            0x06

#define GD25QXX_WRITE_DISABLE                           0x04

#define GD25QXX_SR_WRITE_ENABLE                         0x50

#define GD25QXX_READ_SR_1                               0x05

#define GD25QXX_READ_SR_2                               0x35

#define GD25QXX_READ_SR_3                               0x15

#define GD25QXX_WRITE_SR_1                              0x01

#define GD25QXX_WRITE_SR_2                              0x31

#define GD25QXX_WRITE_SR_3                              0x11

#define GD25QXX_READ_DATA                               0x03

#define GD25QXX_FAST_READ                               0x0B

#define GD25QXX_DUAL_OUTPUT_FAST_READ                   0x3B

#define GD25QXX_DUAL_IO_FAST_READ                       0xBB

#define GD25QXX_QUAD_OUTPUT_FAST_READ                   0x6B

#define GD25QXX_QUAD_IO_FAST_READ                       0xEB

#define GD25QXX_QUAD_IO_WORD_FAST_READ                  0xE7

#define GD25QXX_PAGE_PROGRAM                            0x02

#define GD25QXX_QUADPAGE_PROGRAM                        0x32

#define GD25QXX_SECTOR_ERASE                            0x20

#define GD25QXX_BLOCK_ERASE_32K                         0x52

#define GD25QXX_BLOCK_ERASE_64K                         0xD8

#define GD25QXX_CHIP_ERASE                              0xC7

#define GD25QXX_CHIP_ERA                                0x60

#define GD25QXX_ENABLE_RESET                            0x66

#define GD25QXX_RESET                                   0x99

#define GD25QXX_SET_BURST_WITH_WRAP                     0x77

#define GD25QXX_PROGRAM_ERASE SUSPEND                   0x75

#define GD25QXX_PROGRAM_ERASE_RESUME                    0x7A

#define GD25QXX_RELEASE_FROM_DPD_AND_READ_DEVICE_ID     0xAB

#define GD25QXX_DEEP_POWER_DOWN                         0xB9

#define GD25QXX_MANUFACTURER_DEVICE_ID                  0x90

#define GD25QXX_MANUFACTURER_DEVICE_ID_BY_DUAL_IO       0x92

#define GD25QXX_MANUFACTURER_DEVICE_ID_BY_QUAD_IO       0x94

#define GD25QXX_READ_IDENTIFICATION                     0x9F

#define GD25QXX_READ_SERIAL_FLASH_DISCOVERABLE_PARAM    0x5A

#define GD25QXX_ERASE_SECURITY_REGISTERS                0x44

#define GD25QXX_PROGRAM_SECURITY_REGISTERS              0x42

#define GD25QXX_READ_SECURITY_REGISTERS                 0x48

/***********************************Register*******************************************/
#define GD25QXX_SR1_WIP_BIT_Pos                         (0U)
#define GD25QXX_SR1_WIP_BIT                             (0x1U << GD25QXX_SR1_WIP_BIT_Pos)

#define GD25QXX_SR1_WEL_BIT_Pos                         (1U)
#define GD25QXX_SR1_WEL_BIT                             (0x1U << GD25QXX_SR1_WEL_BIT_Pos)

#define GD25QXX_SR1_BP0_BIT_Pos                         (2U)
#define GD25QXX_SR1_BP0_BIT                             (0x1U << GD25QXX_SR1_BP0_BIT_Pos)

#define GD25QXX_SR1_BP1_BIT_Pos                         (3U)
#define GD25QXX_SR1_BP1_BIT                             (0x1U << GD25QXX_SR1_BP1_BIT_Pos)

#define GD25QXX_SR1_BP2_BIT_Pos                         (4U)
#define GD25QXX_SR1_BP2_BIT                             (0x1U << GD25QXX_SR1_BP2_BIT_Pos)

#define GD25QXX_SR1_BP3_BIT_Pos                         (5U)
#define GD25QXX_SR1_BP3_BIT                             (0x1U << GD25QXX_SR1_BP3_BIT_Pos)

#define GD25QXX_SR1_BP4_BIT_Pos                         (6U)
#define GD25QXX_SR1_BP4_BIT                             (0x1U << GD25QXX_SR1_BP4_BIT_Pos)

#define GD25QXX_SR1_SRP0_BIT_Pos                        (7U)
#define GD25QXX_SR1_SRP0_BIT                            (0x1U << GD25QXX_SR1_SRP0_BIT_Pos)

#define GD25QXX_SR2_SRP1_BIT_Pos                        (0U)
#define GD25QXX_SR2_SRP1_BIT                            (0x1U << GD25QXX_SR2_SRP1_BIT_Pos)

#define GD25QXX_SR2_QE_BIT_Pos                          (1U)
#define GD25QXX_SR2_QE_BIT                              (0x1U << GD25QXX_SR2_QE_BIT_Pos)

#define GD25QXX_SR2_SUS2_BIT_Pos                        (2U)
#define GD25QXX_SR2_SUS2_BIT                            (0x1U << GD25QXX_SR2_SUS2_BIT_Pos)

#define GD25QXX_SR2_LB1_BIT_Pos                         (3U)
#define GD25QXX_SR2_LB1_BIT                             (0x1U << GD25QXX_SR2_LB1_BIT_Pos)

#define GD25QXX_SR2_LB2_BIT_Pos                         (4U)
#define GD25QXX_SR2_LB2_BIT                             (0x1U << GD25QXX_SR2_LB2_BIT_Pos)

#define GD25QXX_SR2_LB3_BIT_Pos                         (5U)
#define GD25QXX_SR2_LB3_BIT                             (0x1U << GD25QXX_SR2_LB3_BIT_Pos)

#define GD25QXX_SR2_CMP_BIT_Pos                         (6U)
#define GD25QXX_SR2_CMP_BIT                             (0x1U << GD25QXX_SR2_CMP_BIT_Pos)

#define GD25QXX_SR2_SUS1_BIT_Pos                        (7U)
#define GD25QXX_SR2_SUS1_BIT                            (0x1U << GD25QXX_SR2_SUS1_BIT_Pos)

#define GD25QXX_SR3_LPE_BIT_Pos                         (2U)
#define GD25QXX_SR3_LPE_BIT                             (0x1U << GD25QXX_SR3_LPE_BIT_Pos)

#define GD25QXX_SR3_DRV0_BIT_Pos                        (5U)
#define GD25QXX_SR3_DRV0_BIT                            (0x1U << GD25QXX_SR3_DRV0_BIT_Pos)

#define GD25QXX_SR3_DRV1_BIT_Pos                        (6U)
#define GD25QXX_SR3_DRV1_BIT                            (0x1U << GD25QXX_SR3_DRV1_BIT_Pos)

#define GD25QXX_SR3_HOLD_RST_BIT_Pos                    (7U)
#define GD25QXX_SR3_HOLD_RST_BIT                        (0x1U << GD25QXX_SR3_HOLD_RST_BIT_Pos)
   
#define GD25QXX_SET_BIT(REG, BIT)                       ((REG) |= (BIT))

#define GD25QXX_CLEAR_BIT(REG, BIT)                     ((REG) &= ~(BIT))

#define GD25QXX_READ_BIT(REG, BIT)                      ((REG) & (BIT))

#define GD25QXX_CLEAR_REG(REG)                          ((REG) = (0x0))

#define GD25QXX_WRITE_REG(REG, VAL)                     ((REG) = (VAL))

#define GD25QXX_READ_REG(REG)                           ((REG))

/***********************************Param*******************************************/
#define GD25QXX_PAGE_SIZE                               0x100

#define GD25QXX_SECTOR_SIZE                             0x1000

#define GD25QXX_BLOCK_SIZE                              0x10000
   
#define GD25QXX_DUMMY_BYTE                              0x00

#define GD25QXX_DEFAULT_BYTE                            0xFF

#define GD25QXX_SR_UNLOCKED                             0x00
   
#define GD25QXX_SR_LOCKED                               0x01

#define GD25QXX_MAX_BLOCK_COUNT                         0x200

#define GD25QXX_MAX_SECTOR_ARRAY_COUNT                  0x800

#define GD25QXX_MAX_BLOCK_ARRAY_COUNT                   0x40

#define GD25QXX_GET_PAGE_n_ADDR(n)                      (n*0x100)

#define GD25QXX_GET_SECTOR_n_ADDR(n)                    (n*0x1000)

#define GD25QXX_GET_BLOCK_n_ADDR(n)                     (n*0x10000)

#define GD25QXX_GET_SECTOR_n_START_PAGE_n(n)            (n*0x10)

#define GD25QXX_GET_BLOCK_n_START_PAGE_n(n)             (n*0x100)

#define GD25QXX_GET_Page_n_At_Sector_n(n)               (n/0x10)

#define GD25QXX_GET_Page_n_At_Block_n(n)                (n/0x100)

#define GD25QXX_GET_REAL_BYTE_CNT(offset, len)          (((offset + len) > GD25QXX_PAGE_SIZE) ? (GD25QXX_PAGE_SIZE - offset) : (len))

/***********************************GD25Q127C*******************************************/
#define GD25Q127C_MANUFACTURER_ID                       0xC8

#define GD25Q127C_MEMORY_ID                             0x40

#define GD25Q127C_CAPACITY_ID                           0x18
   
#define GD25Q127C_DEVICE_ID                             0x17

#define GD25Q127C_TYPICAL_PAGE_PROGRAM_TIME_MS             1
   
#define GD25Q127C_TYPICAL_SECTOR_ERASE_TIME_MS            50

#define GD25Q127C_TYPICAL_BLOCK_ERASE_TIME_MS            300

#define GD25Q127C_TYPICAL_CHIP_ERASE_TIME_MS           60000
/***************************************************************************************/

typedef enum
{
    GD25Q05 = 0,
    GD25Q10 = 1,
    GD25Q20 = 2,
    GD25Q40 = 3,
    GD25Q80 = 4,
    GD25Q16 = 5,
    GD25Q32 = 6,
    GD25Q64 = 7,
    GD25Q128 = 8,
    GD25Q256 = 9,
    GD25Q512 = 10,
    
} GD25QXX_TYPE_Typedef;

typedef int32_t (*gd25qxx_cs_ptr) (void);
typedef int32_t (*gd25qxx_wr_ptr) (SPI_Type*, uint8_t*, uint16_t);
typedef int32_t (*gd25qxx_rd_ptr) (SPI_Type*, uint8_t*, uint16_t);
typedef int32_t (*gd25qxx_ds_ptr) (void);

typedef struct __GD25Qxx_Ctx_Typedef 
{
    /** Component mandatory fields **/
    gd25qxx_cs_ptr    chip_select;
    gd25qxx_wr_ptr    write_data;
    gd25qxx_rd_ptr    read_data;
    gd25qxx_ds_ptr    chip_deselect;
    
    /** Customizable optional pointer **/
    SPI_Type*         handle;
    
} GD25Qxx_Ctx_Typedef;

typedef __packed struct __GD25Qxx_JEDEC_Typedef
{  
    uint8_t  Manufacturer_ID;
    
    uint8_t  Memory_Type;
    
    uint8_t  Capacity_Type;
    
    uint8_t  Dev_ID;
    
} GD25Qxx_JEDEC_Typedef;

typedef __packed union __GD25Qxx_SR1_Typedef
{
    struct
    {
        uint8_t  WIP              : 1;
        uint8_t  WEL              : 1;
        uint8_t  BP0              : 1;
        uint8_t  BP1              : 1;
        uint8_t  BP2              : 1;
        uint8_t  BP3              : 1;
        uint8_t  BP4              : 1;
        uint8_t  SRP0             : 1;
    }reg;
  
    uint8_t byte;
    
} GD25Qxx_SR1_Typedef;

typedef __packed union __GD25Qxx_SR2_Typedef
{
    struct
    {
        uint8_t  SRP1             : 1;
        uint8_t  QE               : 1;
        uint8_t  SUS2             : 1;
        uint8_t  LB1              : 1;
        uint8_t  LB2              : 1;
        uint8_t  LB3              : 1;
        uint8_t  CMP              : 1;
        uint8_t  SUS1             : 1;
    }reg;
  
  uint8_t byte;
  
} GD25Qxx_SR2_Typedef;

typedef __packed union __GD25Qxx_SR3_Typedef
{
    struct
    {
        uint8_t  Reserved16       : 1;
        uint8_t  Reserved17       : 1;
        uint8_t  LPE              : 1;
        uint8_t  Reserved19       : 1;
        uint8_t  Reserved20       : 1;
        uint8_t  DRV0             : 1;
        uint8_t  DRV1             : 1;
        uint8_t  HOLD_RST         : 1;
    }reg;
  
    uint8_t byte;
  
} GD25Qxx_SR3_Typedef;

typedef __packed struct __GD25Qxx_Status_Reg_Typedef
{
    GD25Qxx_SR1_Typedef           SR1;
    GD25Qxx_SR2_Typedef           SR2;
    GD25Qxx_SR3_Typedef           SR3;
    uint8_t                       SRLock;
    
} GD25Qxx_Status_Reg_Typedef;

typedef __packed union __GD25Qxx_Block_State_Typedef
{
    struct
    {
        uint8_t  BL0              : 1;
        uint8_t  BL1              : 1;
        uint8_t  BL2              : 1;
        uint8_t  BL3              : 1;
        uint8_t  BL4              : 1;
        uint8_t  BL5              : 1;
        uint8_t  BL6              : 1;
        uint8_t  BL7              : 1;
    }reg;
  
    uint8_t byte;
  
} GD25Qxx_Block_State_Typedef;

typedef __packed struct __GD25Qxx_Drv_Typedef
{
    GD25QXX_TYPE_Typedef          Type;
    
    GD25Qxx_Ctx_Typedef           DevCtx;
  
    GD25Qxx_JEDEC_Typedef	      JEDEC;
    
    GD25Qxx_Status_Reg_Typedef    StatusReg;
    
    GD25Qxx_Block_State_Typedef   BlockState[GD25QXX_MAX_BLOCK_ARRAY_COUNT];    //64 bytes means 512bits, For each bit, 0: block is empty, 1: block is used, block_size is 64KB, each block has 16 sectors or 256 pages
    
    uint8_t                       UniqID[8];    //Only for W25Qxx
    
    uint16_t                      PageSize;
    uint32_t                      PageCount;
    
    uint32_t                      SectorSize;
    uint32_t                      SectorCount;
    
    uint32_t                      BlockSize;
    uint32_t                      BlockCount;
    
    uint32_t                      CapacityInKByte;

} GD25Qxx_Drv_Typedef;

int32_t GD25Qxx_Info_Init(GD25Qxx_Drv_Typedef *gd25qxx_drv);

int32_t GD25Qxx_Read_Dev_ID(GD25Qxx_Ctx_Typedef *ctx, GD25Qxx_JEDEC_Typedef *jedec);

int32_t GD25Qxx_Read_JEDEC(GD25Qxx_Ctx_Typedef *ctx, GD25Qxx_JEDEC_Typedef *jedec);

int32_t GD25Qxx_Write_Enable(GD25Qxx_Ctx_Typedef *ctx);

int32_t GD25Qxx_Write_Disable(GD25Qxx_Ctx_Typedef *ctx);

int32_t GD25Qxx_Read_SR(GD25Qxx_Ctx_Typedef *ctx, uint8_t sr_n, uint8_t *val);

int32_t GD25Qxx_Write_SR(GD25Qxx_Ctx_Typedef *ctx, uint8_t sr_n, uint8_t val);

bool GD25Qxx_Wait_For_Not_Busy(GD25Qxx_Ctx_Typedef *ctx, uint16_t timeout_ms);

int32_t GD25Qxx_Erase_Chip(GD25Qxx_Ctx_Typedef *ctx, uint32_t timeout_ms);

int32_t GD25Qxx_Erase_Sector(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type, uint16_t sector_n, uint16_t timeout_ms);    //For GD25Q127C sector_n: 0~4095

int32_t GD25Qxx_Erase_Block(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type, uint16_t block_n, uint16_t timeout_ms);      //For GD25Q127C block_n: 0~255

int32_t GD25Qxx_Write_Data(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type, uint32_t page_n, uint32_t offset, uint8_t *buf, uint32_t len);        //offset: 0~255;

int32_t GD25Qxx_Read_Data(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type, uint32_t page_n, uint32_t offset, uint8_t *buf, uint32_t len);         //offset: 0~255

int32_t GD25Qxx_Fast_Read_Data(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type, uint32_t page_n, uint32_t offset, uint8_t *buf, uint32_t len);    //offset: 0~255

bool Enable_The_Top_Sector_Protection(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type);

bool Disable_The_Top_Sector_Protection(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type);

bool GD25Qxx_Is_Page_Empty(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type, uint32_t page_n);

bool GD25Qxx_Is_Sector_Empty(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type, uint32_t sector_n);

bool GD25Qxx_Is_Block_Empty(GD25Qxx_Ctx_Typedef *ctx, GD25QXX_TYPE_Typedef type, uint32_t block_n);

bool GD25Qxx_Store_Capacity_State(GD25Qxx_Drv_Typedef *gd25qxx_drv);

bool GD25Qxx_Check_Capacity_State(GD25Qxx_Drv_Typedef *gd25qxx_drv);

#ifdef __cplusplus
}
#endif

#endif /* GD25QXX_DRV_H */
