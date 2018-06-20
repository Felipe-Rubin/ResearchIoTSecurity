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


volatile uint8_t testkey[64] = {
        0x61,0x6e,0x2b,0xcf,0xf1,0x95,0x41,0x24,
        0x21,0xb3,0xf6,0xfc,0xbc,0x11,0x90,0xbe,
        0x35,0xec,0xb0,0xbd,0x12,0x45,0xfb,0x45,
        0x3c,0x0f,0x09,0xf7,0xee,0x2b,0x5e,0x3e,
        0xe0,0xd6,0xa6,0x68,0x9b,0xde,0xb1,0x54,
        0xe0,0x5a,0x1d,0x4a,0xd1,0xd9,0x4f,0x83,
        0x87,0x5d,0xe8,0x81,0x00,0x2f,0x19,0x90,
        0x1b,0xd2,0x12,0x8d,0xc9,0x6a,0xc1,0x23
    };

int main() {
    
    // int32_t targetid = 1;
   

// uint32_t testkey[] = {
//     0x616e2bcf,0xf1954124,0x21b3f6fc,0xbc1190be,
//     0x35ecb0bd,0x1245fb45,0x3c0f09f7,0xee2b5e3e,
//     0xe0d6a668,0x9bdeb154,0xe05a1d4a,0xd1d94f83,
//     0x875de881,0x002f1990,0x1bd2128d,0xc96ac123
// };




    uint32_t i;
    while (1){

        /* 3 seconds delay */
        
        udelay(3000000);
        get_allowed_update(testkey);
}
        
        
    return 0;

        
}





