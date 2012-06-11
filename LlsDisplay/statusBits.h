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

#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF

//#pragma config CCP2MX = PORTC   // CCP2 capture on pin RC1
//#pragma config PWRT = ON not compatible with ICD2
//#pragma config XINST = ON

#endif
