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
#include "systick.h"

volatile uint32_t uwTick = 0; /* While uwTick is ms, "uint32_t" max count to 49 Days!!! */

HAL_TickFreqTypeDef uwTickFreq = HAL_TICK_FREQ_DEFAULT;  /* 1KHz */

void Systick_Config(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if(SysTick_Config(SystemCoreClock / 1000U / uwTickFreq))
    {
        /* capture error */
        while (1)
        {
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

void Systick_Inc(void)
{
    uwTick += uwTickFreq;
}

uint32_t Systick_Get(void)
{
    return uwTick;
}

uint32_t Systick_Diff_Get(uint32_t new_tick, uint32_t last_tick)
{
    return (new_tick >= last_tick) ? (new_tick - last_tick) : (0xFFFFFFFF - last_tick + new_tick);
}

void Delay_ms(uint32_t count)
{
    uint32_t tickstart = Systick_Get();
    uint32_t wait = count;

    /* Add a freq to guarantee minimum wait */
    if(wait < HAL_MAX_DELAY)
    {
        wait += (uint32_t)(uwTickFreq);
    }

    while((Systick_Diff_Get(Systick_Get(), tickstart)) < wait)
    {
    }
}

void Delay_us(uint32_t nus)
{
	uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;
    told = SysTick->VAL;
    ticks = nus * 240;                 //AT32F403ACGT7  240MHz
        
    while(1)
    {
        tnow = SysTick->VAL;

        if(tnow != told)
        {
            if(tnow < told)
                tcnt += told - tnow;
            else 
                tcnt += reload - tnow + told;
            
            if(tcnt >= ticks)
                break;
            
			told = tnow;
        }
    }  
}

void Delay_4Dot16_ns(uint32_t n)
{
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;
    told = SysTick->VAL;
    
    while(1)
    {
        tnow = SysTick->VAL;

        if(tnow != told)
        {
            if(tnow < told)
                tcnt += told - tnow;
            else 
                tcnt += reload - tnow + told;
            
            if(tcnt >= n)
                break;
            
			told = tnow;
        }
    }
}

uint32_t m_nStart;               //DEBUG Stopwatch start cycle counter value
uint32_t m_nStop;                //DEBUG Stopwatch stop cycle counter value

void stopwatch_reset(void)
{
    /* Enable DWT */
    DEM_CR |= DEMCR_TRCENA; 
    *DWT_CYCCNT = 0;             
    /* Enable CPU cycle counter */
    DWT_CTRL |= CYCCNTENA;
}

uint32_t stopwatch_getticks(void)
{
    return CPU_CYCLES;
}

void stopwatch_delay(uint32_t ticks)//240ticks mean 1us
{
    stopwatch_reset();
    
    uint32_t end_ticks = ticks + stopwatch_getticks();
    while(1)
    {
        if (stopwatch_getticks() >= end_ticks)
            break;
    }
}

uint32_t CalcNanosecondsFromStopwatch(uint32_t nStart, uint32_t nStop)
{
    uint32_t nDiffTicks;
    uint32_t nSystemCoreTicksPerMicrosec;

    // Convert (clk speed per sec) to (clk speed per microsec)
    nSystemCoreTicksPerMicrosec = CLK_SPEED / 1000000;

    // Elapsed ticks
    nDiffTicks = nStop - nStart;

    // Elapsed nanosec = 1000 * (ticks-elapsed / clock-ticks in a microsec)
    return 1000 * nDiffTicks / nSystemCoreTicksPerMicrosec;
} 

