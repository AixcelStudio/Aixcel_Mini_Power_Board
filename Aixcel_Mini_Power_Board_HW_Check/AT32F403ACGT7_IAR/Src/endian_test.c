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

#include <stdio.h>
#include <stdint.h>
#include "endian_test.h"

struct Word
{
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
};

typedef union
{	
    uint32_t A;
    struct Word word;
    uint8_t arry[4];
} Endian_Test;


int System_Endian_Test(void)
{
    printf("System_Endian_Test Start...\n");
  
    Endian_Test test;
  
    printf("test RAM_Addr = 0x%p\n\n", &test);
  
    printf("test.A RAM_Addr = 0x%p\n\n", &test.A);
  
    printf("test.word RAM_Addr = 0x%p\n", &test.word);
    printf("test.word.a RAM_Addr = 0x%p\n", &test.word.a);
    printf("test.word.b RAM_Addr = 0x%p\n", &test.word.b);
    printf("test.word.c RAM_Addr = 0x%p\n", &test.word.c);
    printf("test.word.d RAM_Addr = 0x%p\n\n", &test.word.d);
    
    printf("test.arry RAM_Addr = 0x%p\n", test.arry);
    printf("test.arry[0] RAM_Addr = 0x%p\n", &test.arry[0]);
    printf("test.arry[1] RAM_Addr = 0x%p\n", &test.arry[1]);
    printf("test.arry[2] RAM_Addr = 0x%p\n", &test.arry[2]);
    printf("test.arry[3] RAM_Addr = 0x%p\n\n", &test.arry[3]);

    test.word.a = 0x11;
    test.word.b = 0x22;
    test.word.c = 0x33;
    test.word.d = 0x44;

    printf("test.A = 0x%x\n", test.A);
  
    printf("test.arry[0] = 0x%x\n", test.arry[0]);
    printf("test.arry[1] = 0x%x\n", test.arry[1]);
    printf("test.arry[2] = 0x%x\n", test.arry[2]);
    printf("test.arry[3] = 0x%x\n", test.arry[3]);

    test.A = 0x55667788;

    printf("test.A = 0x%x\n", test.A);
  
    printf("test.word.a = 0x%x\n", test.word.a);
    printf("test.word.b = 0x%x\n", test.word.b);
    printf("test.word.c = 0x%x\n", test.word.c);
    printf("test.word.d = 0x%x\n", test.word.d);
    
    printf("test.arry[0] = 0x%x\n", test.arry[0]);
    printf("test.arry[1] = 0x%x\n", test.arry[1]);
    printf("test.arry[2] = 0x%x\n", test.arry[2]);
    printf("test.arry[3] = 0x%x\n", test.arry[3]);
  
    *((uint32_t*)test.arry) = 0xAABBCCDD;
  
    printf("test.A = 0x%x\n", test.A);
  
    printf("test.word.a = 0x%x\n", test.word.a);
    printf("test.word.b = 0x%x\n", test.word.b);
    printf("test.word.c = 0x%x\n", test.word.c);
    printf("test.word.d = 0x%x\n", test.word.d);
    
    printf("test.arry[0] = 0x%x\n", test.arry[0]);
    printf("test.arry[1] = 0x%x\n", test.arry[1]);
    printf("test.arry[2] = 0x%x\n", test.arry[2]);
    printf("test.arry[3] = 0x%x\n", test.arry[3]);

    printf("System_Endian_Test Finish...\n");
  
    return 0;
}
