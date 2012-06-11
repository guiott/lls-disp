/*
 * File:   settings.h
 * Author: guiott
 *
 * Created on 9 giugno 2012, 20.29
 */

#ifndef settings_H
#define settings_H

    extern unsigned int Timer0Count;
    extern unsigned char Timer0CountHigh;

    extern struct Bits
    {
            unsigned bit0:1;
            unsigned bit1:1;
            unsigned bit2:1;
            unsigned bit3:1;
            unsigned bit4:1;
            unsigned bit5:1;
            unsigned bit6:1;
            unsigned bit7:1;
    };
    extern struct Bits VARbits1;
#endif
