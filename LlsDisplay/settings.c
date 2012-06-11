#include "settings.h"
#include "common.h"

/*Settings********************************************************************/
void Settings(void)
{
/*-------Oscillator*/
OSCCONbits.IRCF=7;      // Internal oscillator 8MHz
OSCTUNEbits.PLLEN=1;    // PLL * 4 enable -> 8 * 4 = 32MHz

/*-------Port A
 * RA0 = Display Segment A
 * RA1 = Display Segment B
 * RA2 = Display Segment C
 * RA3 = Display Segment D
 * RA4 = Display Segment E
 * RA5 = Display Segment F
 * RA6 = Display Segment G

 */
TRISA = 0b00000000;

/*-------Port B
 * RB0 = Common Anode Right Display
 * RB1 = Common Anode Left Display
 * RB2 = Buzzer
 * RB3 = Common Anode Right LED Bar
 * RB4 = Common Anode Left LED Bar
 
*/
TRISB = 0b00000000;

/*-------Port	C
 * RC3 = I2C SCL
 * RC4 = I2C SDA
 * In Slave mode, the SCL and SDA pins must be configured as inputs 
 * (TRISC<4:3> set). The MSSP module will override the input state 
 * with the output data when required (slave-transmitter).
*/
TRISC = 0b00011000;

/*
//-------A/D converter
// From AN0 to AN3 analog ports
ADCON1bits.PCFG=0b1011;
*/

//-------TIMER0
T0CONbits.TMR0ON=0;     //TMR0 off
T0CONbits.T08BIT=0;     //16 bit Timer
T0CONbits.T0CS=0;       //Timer on internal clock
T0CONbits.T0SE=0;       //increment on low-to-high transition
T0CONbits.PSA=0;        //Prescaler assigned
T0CONbits.T0PS=2;       //Prescaler 1:8 clock = 40MHz


// period = CLKOUT * prescaler * TMR0
Timer0Count = 65535 - (0.005 * FCY / PRESCALER0); // 5ms
Timer0CountHigh = Timer0Count >> 8; // byte High

TMR0H = Timer0CountHigh; // byte High
TMR0L = Timer0Count;     // byte Low

/*
//-------TIMER1
T1CONbits.TMR1CS=0;     //Internal clock (Fosc/4)
T1CONbits.RD16=1;       //16 bit Timer
T1CONbits.T1OSCEN=0;    //Timer1 osc off
T1CONbits.T1RUN=0;      //Not Timer1 clock
T1CONbits.T1SYNC=1;     //Do not synchronize external clock input
T1CONbits.TMR1ON=1;     //TMR1 on
T1CONbits.T1CKPS=3;     //Prescaler 1:8 clock = 40MHz
*/

//-------USART
/*Baud Rate
FOSC/[4*(BRG + 1)]
SPBRGH:SBPRG   FOSC   des. bps  Error
0x0056         40MHz	115200    0,22%
0x00AD         40MHz	57600     0,22%
0x0129         40MHz	33600     0,13%
0x0208         40MHz	19200     0,03%
0x0411         40MHz	9600     	0,03%
0x0822         40MHz	4800     -0,02%
 *
 *
SPBRGH:SBPRG   FOSC   des. bps	Error
0x002A         20MHz	115200   -0,94%
0x0056         20MHz	57600     0,22%
0x0094         20MHz	33600     0,13%
0x0103         20MHz	19200    -0,16%
0x0208         20MHz	9600      0,03%
0x0411         20MHz	4800      0,03%
 

#ifdef PLL //	40MhZ
  SPBRGH=0;
	SPBRG=0x56;        //	115200	@	40MHz
#else
  SPBRGH=0;
	SPBRG=0x2A;       //	115200	@	20MHz
#endif

TXSTAbits.BRGH=1;   //High baud rate
TXSTAbits.SYNC=0;   //asynchronous
BAUDCONbits.BRG16=1;//16 bit Baud Rate Generator

  
RCSTAbits.SPEN=1;   //enable serial port pins
RCSTAbits.CREN=1;   //enable reception
RCSTAbits.SREN=0;   //no effect
PIE1bits.TXIE=0;    //disable tx interrupts
PIE1bits.RCIE=0;    //disable rx interrupts
TXSTAbits.TX9=0;    //8-bit transmission
RCSTAbits.RX9=0;    //8-bit reception
TXSTAbits.TXEN=0;   //reset transmitter
TXSTAbits.TXEN=1;   //enable the transmitter
*/

/*
//-------PWM
CCP1CONbits.CCP1M=0b1100;   // PWM mode

PR2=249;                    //PWM freq = 40KHz @ 40 MHz

T2CONbits.T2CKPS=0;         // prescaler = 1
T2CONbits.TOUTPS=0;         // postscaler = 1
T2CONbits.TMR2ON=1;         // Turn on PWM
*/

/*
//-------Capture
T3CONbits.T3CCP1=0;         //Timer1 is the capture/compare clock source
T3CONbits.T3CCP2=0;         //for the CCP modules
PIR2bits.CCP2IF = 0;        // reset of interrupt flag
CCP2CONbits.CCP2M=0b0101;   // Capture mode, every rising edge
PIR2bits.CCP2IF = 0;        // reset of interrupt flag
CAPTURE_FLAG = 0;
ECHO_FLAG=0;
*/

/*
//-------Analog Comparator
CMCONbits.CM=0b001;         //One Independent Comparator with Output on R4
CMCONbits.C1INV=1;          //C1 output inverted
CMCONbits.C2INV=1;          //C2 output inverted
CMCONbits.CIS=0;

/*-------Comparator Voltage Reference
If CVRR = 1: CVref = (CVR/24) x CVRsrc
If CVRR = 0: CVref = (CVRsrc x 1/4) + ((CVR/32) x CVRsrc)

CVRCONbits.CVREN=1;         //CVREF circuit powered on
CVRCONbits.CVROE=1;         //CVREF voltage is connected to the RA2 pin
CVRCONbits.CVRR=1;          //0 to 0.667 CVRSRC, with CVRSRC/24 step size
CVRCONbits.CVRSS=0;         //Comparator reference source, CVRsrc = VDD ? VSS
CVRCONbits.CVR=3;           //if CVRR=1: CVref =? CVR*0.21 @5V VDD
*/


//------I2C

PIE1bits.SSPIE=1;       //SSP (I2C events) int enabled
PIE2bits.BCLIE=1;       //BUS COLLISION int enabled
IPR1bits.SSPIP=0;       //SSP int = low priority
IPR2bits.BCLIP=0;       //BUS COLLISION int = low priority

//-------Interrupts
RCONbits.IPEN=1;        //interrupt priority enabled
INTCONbits.TMR0IE=1;    //interrupt on TMR0 overflow enabled
INTCON2bits.TMR0IP=1;   //TMR0 interrupt high priority

//-------Not used Peripheral Interrupts
PIE1bits.TMR2IE=0;      //interrupt on TMR2 overflow disabled
PIE1bits.TMR1IE=0;      //interrupt on TMR1 overflow disabled
PIE2bits.TMR3IE=0;      //interrupt on TMR3 overflow disabled
PIE1bits.ADIE=0;        //AD int disabled
PIE1bits.RCIE=0;        //USART RX int disabled
PIE1bits.TXIE=0;        //USART TX int disabled
PIE1bits.CCP1IE=0;      //CCP1 int disabled
PIE2bits.EEIE=0;        //EEPROM int disabled
PIE2bits.LVDIE=0;       //LOW VOLTAGE int disabled
PIE2bits.CCP2IE=0;      //CCP2 int enabled only when needed
IPR2bits.CCP2IP=0;      //CCP2 interrupt high priority


//-------Interrupt flags
//INTCON3bits.INT1IF    //  interrupt on INT1
//INTCON3bits.INT2IF    //  interrupt on INT2
//INTCONbits.INT0IF     //  interrupt on INT0
//PIR1bits.TMR2IF       //  interrupt on TMR2 overflow
//PIR1bits.TMR1IF       //  interrupt on TMR1 overflow
//PIR1bits.PSPIF        //  PSP int
//PIR1bits.ADIF         //  AD int
//PIR1bits.RCIF         //  USART RXint
//PIR1bits.TXIF         //  USART TX int
//PIR1bits.SSPF         //  SSP int
//PIR1bits.CCP1IF       //  CCP1 int
//PIR2bits.CCP2IF       //  CCP2 int
//PIR2bits.EEIF         //  EEPROM int
//PIR2bits.BCLIF        //  BUS COLLISION int
//PIR2bits.LVDIF        //  LOW VOLTAGE int
//PIR2bits.TMR3IF       //  interrupt on TMR3 overflow

}
/*settings*********************************************************************/
