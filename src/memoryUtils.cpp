//
// Created by Phoebe on 20/01/2022.
//
// Note: My C is really "rusty", I haven't work on it for literally decades (I learned it with c89...)
//

#include <stdint.h>
#include <sdk/os/debug.hpp>

/**
 * Doing guess about the registerAddr and trying to read it as a pointer
 * @param registerAddr something like 0x8c310000
 * @return 1 if could be writable, 0 if not
 */
int isAddressWritable(uint32_t registerAddr) {
    if (0x8c000000< registerAddr && registerAddr < 0x8cffffff) {
        return 1;
    }
    return 0;
}

uint8_t *memPtr = (uint8_t *)0x8c400000;
uint32_t *memPtr32 = (uint32_t *)0x8c400000;
unsigned char numToAscii[257]= "................................ !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~.................................................................................................................................";
#define pr_chr(x) numToAscii[memPtr[x]]

char buffer[32];

/**
 * Print the address values on screen.
 * @param registerAddr
 * @param carret_x
 */
int printBytes(uint32_t registerAddr, int reg, int carret_x) {

    memPtr = (uint8_t *)registerAddr;

    int i = 0;

//    uint32_t read=0;

//    asm volatile ("mov.l @%1, %0":"=r"(read):"r"( memPtr));
//    buffer[1]=(((read>>28)&0xf)<10)?(((read>>28)&0xf)+'0'):(((read>>28)&0xf)+('A'-10));
//    buffer[2]=(((read>>24)&0xf)<10)?(((read>>24)&0xf)+'0'):(((read>>24)&0xf)+('A'-10));
//    buffer[3]=(((read>>20)&0xf)<10)?(((read>>20)&0xf)+'0'):(((read>>20)&0xf)+('A'-10));
//    buffer[4]=(((read>>16)&0xf)<10)?(((read>>16)&0xf)+'0'):(((read>>16)&0xf)+('A'-10));
//    buffer[5]=(((read>>12)&0xf)<10)?(((read>>12)&0xf)+'0'):(((read>>12)&0xf)+('A'-10));
//    buffer[6]=(((read>> 8)&0xf)<10)?(((read>> 8)&0xf)+'0'):(((read>> 8)&0xf)+('A'-10));
//    buffer[7]=(((read>> 4)&0xf)<10)?(((read>> 4)&0xf)+'0'):(((read>> 4)&0xf)+('A'-10));
//    buffer[8]=(((read    )&0xf)<10)?(((read    )&0xf)+'0'):(((read    )&0xf)+('A'-10));

    Debug_Printf(0,carret_x,true, 0,"%-2d %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %c%c%c%c %c%c%c%c %c%c%c%c", reg,
                 memPtr[0+(16*i)],memPtr[1+(16*i)],memPtr[ 2+(16*i)],memPtr[ 3+(16*i)],
                 memPtr[ 4+(16*i)],memPtr[ 5+(16*i)],memPtr[ 6+(16*i)],memPtr[ 7+(16*i)],
                 memPtr[ 8+(16*i)],memPtr[ 9+(16*i)],memPtr[ 10+(16*i)],memPtr[ 11+(16*i)],
                 pr_chr(0+(16*i)),pr_chr(1+(16*i)),pr_chr( 2+(16*i)),pr_chr( 3+(16*i)),
                 pr_chr( 4+(16*i)),pr_chr( 5+(16*i)),pr_chr( 6+(16*i)),pr_chr( 7+(16*i)),
                 pr_chr( 4+(17*i)),pr_chr( 5+(18*i)),pr_chr( 6+(19*i)),pr_chr( 7+(20*i)));

    if (memPtr[0+(16*i)] == 0x8c) {
        // Likely to be a ptr
        memPtr32 = (uint32_t *)registerAddr;
        uint32_t lPtr = memPtr32[0+(16*i)];

        if (isAddressWritable(lPtr)) {
            carret_x = printBytes(lPtr, 80, ++carret_x);
        }
    }

    if (memPtr[4+(16*i)] == 0x8c) {
        // Likely to be a ptr
        memPtr32 = (uint32_t *)registerAddr;
        uint32_t lPtr = memPtr32[4+(16*i)];

        if (isAddressWritable(lPtr)) {
            carret_x = printBytes(lPtr, 81, ++carret_x);
        }
    }
    if (memPtr[8+(16*i)] == 0x8c) {
        // Likely to be a ptr
        memPtr32 = (uint32_t *)registerAddr;
        uint32_t lPtr = memPtr32[8+(16*i)];

        if (isAddressWritable(lPtr)) {
            carret_x = printBytes(lPtr, 82, ++carret_x);
        }
    }


//    for ( i = 0; i < 2; i++ ) {
//        printf( "*(p + %d) : %d\n", i, *(p + i));
//    }

    return carret_x;

}