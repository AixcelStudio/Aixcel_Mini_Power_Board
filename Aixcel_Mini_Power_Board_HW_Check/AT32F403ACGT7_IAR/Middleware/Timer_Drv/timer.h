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
#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "common.h"

typedef void (*TIMEPROC)(void);
typedef void (*TIMEPROC_PARAM)(uint8_t param);

#define  FALSE          0
#define  TRUE           1

#define TIMER_POOL_SIZE 45

#define KILLTIMER(T) {KillTimer(T); T=0;}


typedef struct _TIMER_NODE
{
    uint8_t         id;
    uint8_t         busy                   :1;
    uint8_t         type                   :7;
    uint8_t         param;
    uint32_t        need_ticks;
    uint32_t        current_ticks;
    TIMEPROC        func;
    TIMEPROC_PARAM  func_param;
} TIMER_NODE;

typedef struct
{
    uint32_t        miseconds;
    uint8_t         seconds;
    uint8_t         minutes;
    uint32_t        hours;
} TIM_CLOCK;

extern volatile TIM_CLOCK TIM_Clock;


/**************************************************timer相关函数声明**************************************************/

/*  
* @fuction:      ISR_PIT0_1ms
* @brief:        PIT0定时器中断处理 
* @param[in]:    None
* @param[out]:   None
* @return:       None
* @others:       PIT0定时器中断响应处理 
*/

void ISR_PIT0_1ms(void);

/*  
* @fuction:      Timer_HAL_Init
* @brief:        软定时器池初始化 
* @param[in]:    None
* @param[out]:   None
* @return:       None
* @others:       软定时器池初始化时使用
*/

void TimerPoolInit(void);



/*  
* @fuction:      SetTimer
* @brief:        软定时器设定，开辟一个新定时器，并设定定时时间以及对应的用户函数 
* @param[in]:    a_elapse: 定时时间(单位：毫秒), a_event_func: 到达定时时间后希望执行的函数
* @param[out]:   None
* @return:       定时器编号
* @others:       软定时器设定时使用
*/

uint8_t SetTimer(uint32_t a_elapse, TIMEPROC a_event_func);



/*  
* @fuction:      KillTimer
* @brief:        软定时器删除，删除指定编号的软定时器 
* @param[in]:    a_timer_handle: 定时器编号
* @param[out]:   None
* @return:       None
* @others:       软定时器删除时使用
*/

void KillTimer(uint8_t a_timer_handle);



/*  
* @fuction:      ResetTimer
* @brief:        软定时器重置，将指定编号的软定时器重置一个新的定时时间
* @param[in]:    a_timer_handle: 定时器编号,  a_new_elapse: 新的定时时间
* @param[out]:   None
* @return:       None
* @others:       软定时器重置时使用
*/

void ResetTimer(uint8_t a_timer_handle, uint32_t a_new_elapse);



/*  
* @fuction:      TimerEventProcess
* @brief:        软定时器轮询执行，检查各个软定时器是否达到定时时间，如果达到，执行该定时器对应的用户函数
* @param[in]:    None
* @param[out]:   None
* @return:       None
* @others:       软定时器轮询执行时使用
*/

void TimerEventProcess(void);




#ifdef __cplusplus
}
#endif

#endif /* TIMER_H */
