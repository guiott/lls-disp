/*
 * File:   definition.h
 * Author: guiott
 *
 * Created on 9 giugno 2012, 10.16
 */

#ifndef definition_H
#define definition_H

struct Bits
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

struct Bits VARbits1;

//----- Variables
int i = 0; // contatore generico
int j = 0; // contatore generico
char TimerStatus = 0;

unsigned int Timer0Count;
unsigned char Timer0CountHigh;

#define  DISP_SEG_A_OFF LATAbits.LATA3 = 1
#define  DISP_SEG_B_OFF LATAbits.LATA4 = 1
#define  DISP_SEG_C_OFF LATAbits.LATA5 = 1
#define  DISP_SEG_D_OFF LATAbits.LATA0 = 1
#define  DISP_SEG_E_OFF LATAbits.LATA1 = 1
#define  DISP_SEG_F_OFF LATAbits.LATA2 = 1
#define  DISP_SEG_G_OFF LATAbits.LATA6 = 1
#define  DISP_SEG_A_ON  LATAbits.LATA3 = 0
#define  DISP_SEG_B_ON  LATAbits.LATA4 = 0
#define  DISP_SEG_C_ON  LATAbits.LATA5 = 0
#define  DISP_SEG_D_ON  LATAbits.LATA0 = 0
#define  DISP_SEG_E_ON  LATAbits.LATA1 = 0
#define  DISP_SEG_F_ON  LATAbits.LATA2 = 0
#define  DISP_SEG_G_ON  LATAbits.LATA6 = 0

#define  DISP_SEG_OFF PORTA = 0XFF;

#define  DISP_R_OFF LATBbits.LATB0 = 1
#define  DISP_R_ON  LATBbits.LATB0 = 0
#define  DISP_R     LATBbits.LATB0

#define  DISP_L_OFF LATBbits.LATB1 = 1
#define  DISP_L_ON  LATBbits.LATB1 = 0
#define  DISP_L     LATBbits.LATB1

#define  BUZZER_OFF LATBbits.LATB2 = 1
#define  BUZZER_ON  LATBbits.LATB2 = 0
#define  BUZZER     LATBbits.LATB2

#define  BAR_R_OFF  LATBbits.LATB3 = 1
#define  BAR_R_ON   LATBbits.LATB3 = 0
#define  BAR_R      LATBbits.LATB3

#define  BAR_L_OFF  LATBbits.LATB4 = 1
#define  BAR_L_ON   LATBbits.LATB4 = 0
#define  BAR_L      LATBbits.LATB4

#define  DISP_ON   0
#define  DISP_OFF  1

unsigned char BlinkFlag[4];

#define  TEST_BIT   LATBbits.LATB5

const unsigned char DispSeg[16]={0x40,0X4F,0X24,0X06,0X0b,0X12,0X10,0X47,
                                 0X00,0X02,0X01,0X18,0X70,0X0C,0X30,0X31};
const unsigned char DispBar[8]={0XFF,0XFE,0XFC,0XF8,0XF0,0XE0,0XC0,0X80};
const unsigned char DispAnim[8]={0XFE,0XFD,0XFB,0XF7,0XEF,0XDF,0XBF,0X7F};
unsigned char DispNum[4]={0X47,0X58,0X48,0X47};
unsigned int LongTimer=0;
unsigned char Anim=0;   // Display animation counting
#define LONG_TIMER 50 // 250 ms @ INT0 = 5 ms
#endif
