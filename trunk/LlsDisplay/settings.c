#include "settings.h"
#include "common.h"

/*Settings********************************************************************/
void Settings(void)
{
/*-------Oscillator*/
OSCCONbits.IRCF=7;      // Internal oscillator 8MHz
OSCTUNEbits.PLLEN=1;    // PLL * 4 enable -> 8 * 4 = 32MHz

// All digital ports
ADCON1bits.PCFG=0b1111;


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


//------I2C
SSPADD = I2C_ADDR << 1; //Address shifted to the upper bits (7:1)
SSPCON1bits.SSPEN = 1;  //Enable I2C on I/O pins
SSPCON1bits.CKP = 1;    //Release clock
SSPCON1bits.SSPM = 6;   //I2C Slave mode, 7-bit address
SSPCON2bits.SEN = 1;    //Clock stretching enabled
SSPSTATbits.SMP = 1;    //Standard speed mode
SSPBUF = 0;

I2C_POINTER_FLAG = 0;   // reset State 1B


//-------Interrupts
RCONbits.IPEN=1;        //interrupt priority enabled

PIR1bits.SSPIF = 0;     //Clear MSSP Interrupt flag
PIE1bits.SSPIE=1;       //SSP (I2C events) int enabled
IPR1bits.SSPIP=1;       //SSP int = high priority

INTCONbits.TMR0IE=1;    //interrupt on TMR0 overflow enabled
INTCON2bits.TMR0IP=0;   //TMR0 interrupt low priority

}
/*settings*********************************************************************/
