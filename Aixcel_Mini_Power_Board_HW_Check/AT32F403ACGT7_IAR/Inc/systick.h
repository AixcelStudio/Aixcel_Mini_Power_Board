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

#ifndef SYS_TICK_H
#define SYS_TICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define  HAL_MAX_DELAY         0xFFFFFFFFU

typedef enum
{
    HAL_TICK_FREQ_10HZ         = 100U,
    HAL_TICK_FREQ_100HZ        = 10U,
    HAL_TICK_FREQ_1KHZ         = 1U,
    HAL_TICK_FREQ_DEFAULT      = HAL_TICK_FREQ_1KHZ
} HAL_TickFreqTypeDef;

void Systick_Config(void);

void Systick_Inc(void);

uint32_t Systick_Get(void);

uint32_t Systick_Diff_Get(uint32_t new_tick, uint32_t last_tick);

void Delay_ms(uint32_t count);

void Delay_us(uint32_t nus);

void Delay_4Dot16_ns(uint32_t n);

#define DEMCR_TRCENA    0x01000000

/* Core Debug registers */
#define DEM_CR           (*((volatile uint32_t *)0xE000EDFC))
#define DWT_CTRL        (*(volatile uint32_t *)0xe0001000)
#define CYCCNTENA       (1<<0)
#define DWT_CYCCNT      ((volatile uint32_t *)0xE0001004)
#define CPU_CYCLES      *DWT_CYCCNT
#define CLK_SPEED       240000000 // EXAMPLE for CortexM4, EDIT as needed

#define STOPWATCH_START { m_nStart = *((volatile unsigned int *)0xE0001004);}
#define STOPWATCH_STOP  { m_nStop = *((volatile unsigned int *)0xE0001004);}

void stopwatch_reset(void);

uint32_t stopwatch_getticks(void);

void stopwatch_delay(uint32_t ticks);//240ticks mean 1us

uint32_t CalcNanosecondsFromStopwatch(uint32_t nStart, uint32_t nStop);

#ifdef __cplusplus
}
#endif

#endif /* SYS_TICK_H */
