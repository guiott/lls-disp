/*
 * File:   config.h
 * Author: guiott
 *
 * Created on 9 giugno 2012, 10.20
 */

#ifndef config_H
#define config_H

//----- Status bits
#pragma config OSC = INTIO67  // internal oscillator 8MHz with RA6 RA7 available

#ifdef WATCH_DOG
    #pragma config WDT = ON
#else
    #pragma config WDT = OFF
#endif

#pragma config LVP = OFF
#pragma config BOREN = OFF

#endif
