/*
Copyright (c) 2016, prpl Foundation

Permission to use, copy, modify, and/or distribute this software for any purpose with or without 
fee is hereby granted, provided that the above copyright notice and this permission notice appear 
in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE 
FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM 
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, 
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

This code was written by Carlos Moratelli at Embedded System Group (GSE) at PUCRS/Brazil.

*/

/* Simple UART and Blink Bare-metal application sample using virtualized IO. */

#include <arch.h>
#include <libc.h>
#include <hypercalls.h>
#include <guest_interrupts.h>
#include <platform.h>
#include <io.h>
// uint8_t testkey[64];
int main() {
    
    // interrupt_register(irq_timer, GUEST_TIMER_INT);
    
	// ENABLE_LED1;
    //targetid starts w/ 1
    //1 = blink
    //2 = testhcall
    int32_t targetid = 1;
   
    // uint32_t testkey[16] = "104a4acb6329a651c4aa3dfc45746fa1f6315eee6c9d5aa7d2968f620bf23044b7bfb124aa098e540611d94c3ad02ffc02294aef182eed215cb292d197013064";

    // printf("UC != UI -> %zu != %zu\n",sizeof(unsigned char),sizeof(uint8_t));
    
    // printf("\n\n{");
    // for(i = 0; i < 64; i++){
    //     printf("%x",((uint8_t*)testkey)[i] | 0);
    // }
    // printf("}\n\n");
        const unsigned char testkey[64] = {
    0x10,0x4a, 0x4a,0xcb, 0x63,0x29, 0xa6,0x51,
    0xc4,0xaa, 0x3d,0xfc, 0x45,0x74, 0x6f,0xa1,
    0xf6,0x31, 0x5e,0xee, 0x6c,0x9d, 0x5a,0xa7,
    0xd2,0x96, 0x8f,0x62, 0x0b,0xf2, 0x30,0x44,
    0xb7,0xbf, 0xb1,0x24, 0xaa,0x09, 0x8e,0x54,
    0x06,0x11, 0xd9,0x4c, 0x3a,0xd0, 0x2f,0xfc,
    0x02,0x29, 0x4a,0xef, 0x18,0x2e, 0xed,0x21,
    0x5c,0xb2, 0x92,0xd1, 0x97,0x01, 0x30,0x64
    };



    // memset(testkey, 0, sizeof(testkey));
     // uint32_t testkey = 0x104a4acb6329a651c4aa3dfc45746fa1f6315eee6c9d5aa7d2968f620bf23044b7bfb124aa098e540611d94c3ad02ffc02294aef182eed215cb292d197013064;
    // uint32_t testkey = 3;
    // uint32_t *testkey = malloc(sizeof(uint32_t) * 16);
    uint32_t i;
    // for(i = 0; i < 16; i++)testkey[i] = 0;
    while (1){

        printf("\nTesting Hypercall\n");
        // printf("\n\n{");
        // for(i = 0; i < 64; i++){
        //     printf("%x",((uint8_t*)testkey)[i] | 0);
        // }
        // printf("}\n\n");


        /* 1 second delay */
        udelay(3000000);
        // update_vm(targetid,NULL,NULL);
        get_allowed_update(testkey);
        

        
   }
    
    return 0;
}






