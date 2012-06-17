/*
 * File:   Common.h
 * Author: guiott
 *
 * Created on 9 giugno 2012, 23.52
 */

#ifndef Common_H
    #define Common_H

//    #define DEBUG_MODE
//    #define WATCH_DOG

    #include <p18f2620.h>

    #define XTAL_FREQ  32000000     //internal oscillator frequency * 4
    #define PRESCALER0 8            //TIMER0 selected prescaler

    #define FCY   (XTAL_FREQ / 4)       //crystal divided by four
    #define TCY   1 / (float)FCY        //single instruction period

    //----- Flags
    #define TIMER0_FLAG VARbits1.bit0       // Timer Interrupt occurred
    #define LONG_TIMER0_FLAG VARbits1.bit1  // Long Timer occurred
    #define SLOW_BLINK_FLAG VARbits1.bit2   // Slow Blink Toggle
    #define FAST_BLINK_FLAG VARbits1.bit3   // Fast Blink Toggle

    //----- Macro
    #define ABS(x) ((x) > 0 ? (x) : -(x)) // ABS() for int, long, float...

    #define I2C_ADDR 0X44
    // State 1B flag. If set the following byte will be the register pointer
    #define I2C_POINTER_FLAG VARbits1.bit4


#endif
