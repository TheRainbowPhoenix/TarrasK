//
// Created by Phoebe on 20/01/2022.
//

#ifndef TARRASK_UTILS_H
#define TARRASK_UTILS_H

/* Checking if an address is writable by comparing it on a predefined table. */
int isAddressWritable(uint32_t registerAddr);

/* Prints some bytes as hexdump on screen at carret_x position */
int printBytes(uint32_t registerAddr, int reg, int carret_x);

#endif //TARRASK_UTILS_H
