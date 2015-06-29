/* //////////////////////////////////////////////////////////////////////////////////////
** File:      LlsDisplay.c

 * #define  Ver "LlsDisplay v.1.3.0\r      by Guiott"

/**
* \mainpage LlsDisplay.c
* \author    Guido Ottaviani-->guido@guiott.com<--
* \version 1.3.0
* \date 06/15
* \details 
*
*
-------------------------------------------------------------------------------
\copyright 2011 Guido Ottaviani
guido@guiott.com

    LlsDisplay is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LlsDisplay is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LlsDisplay.  If not, see <http://www.gnu.org/licenses/>.

-------------------------------------------------------------------------------
*/

// include standard
#include <stdlib.h>
#include <i2c.h>

#include "common.h"
#include "definition.h"
#include "prototypes.h"
#include "statusBits.h"


void main (void)	// ==========
{	
  
INTCONbits.GIEH=0;	// disable interrupt high
INTCONbits.GIEL=0;	// disable interrupt low


Settings(); // set ports and peripherals

// ==== enable interrupts
INTCONbits.GIEH=1;	// enable interrupt high
INTCONbits.GIEL=1;	// enable interrupt low

DISP_SEG_OFF;
DISP_L_OFF;
DISP_R_OFF;
BAR_L_OFF;
BAR_R_OFF;
BUZZER_OFF;

T0CONbits.TMR0ON=1;     //TMR0 on
TIMER0_FLAG=0;
TimerStatus=0;

I2C_POINTER_FLAG = 0;   // reset State 1B

while (1)   // main loop
{
    if(TIMER0_FLAG)
    {
        Cycle();
    }

    if(LONG_TIMER0_FLAG)
    {
        ClrWdt();
        LongCycle();
    }

}           // main loop
} // main ==========



/* Functions *****************************************************************/
void LongCycle(void)
{/**
 *\brief slow changings. Drives the slow and fast blinks and other changings
  * on display.
  * The High nibble of the display number to be shown controls in which  mode
  * the Low nibble will be displayed:
  * 0 = steady ON
  * 1 = OFF
  * 2 = slow blinking
  * 3 = fast blinking
  * 4 = CW/forward segments animation
  * 5 = CCW/backward segments animation
  *
  * mode 0 - 3 are valid also for buzzer, lower nibble not used
 */
   static unsigned char Blink;
   int LP;
   int DispMode;

   Blink++;
   if(Blink>=3)
   {
       Blink=0;
       SLOW_BLINK_FLAG = SLOW_BLINK_FLAG ^ 1;

    #ifdef DEBUG_MODE
        I2cRegRx[0]++;
        I2cRegRx[1]++;
        I2cRegRx[2]++;
        I2cRegRx[3]++;
    #endif
   }
   
   LONG_TIMER0_FLAG=0;
   FAST_BLINK_FLAG = FAST_BLINK_FLAG ^ 1;

   Anim++;
   if(Anim>=6)
   {
       Anim=0;
   }

   for(LP=0; LP<=(I2C_BUFF_SIZE_RX-1); LP++)
   {
        DispMode=I2cRegRx[LP]>>4;
        switch (DispMode)
        {
            case 0:
                BlinkFlag[LP] = DISP_ON;
                break;
 
           case 1: 
                BlinkFlag[LP] = DISP_OFF;
                
                break;

           case 2: 
                BlinkFlag[LP] = SLOW_BLINK_FLAG;
                
                break;
                
           case 3: 
                BlinkFlag[LP] = FAST_BLINK_FLAG;

                break;
                
           case 4: 
                BlinkFlag[LP] = 0X0E;   // forward animation
                
                break;
 
           case 5: 
                BlinkFlag[LP] = 0X0F;   // backward animation
                
                break;               
           
           default:
                BlinkFlag[LP] = DISP_ON;

                break;
        }

   }
   BUZZER = BlinkFlag[5];
  
 }


void Cycle()
{/**
 *\brief cycle the number to display to perform multiplexing
  * Digit 0 = Left bar first 5 segments, up and down arrow segments 6-7
  * Digit 1 = Left number 7 segments
  * Digit 2 = Right number 7 segments
  * Digit 3 = Right bar first 5 segments
 *
 */
   TIMER0_FLAG=0;
   DISP_SEG_OFF;
   DISP_L_OFF;
   DISP_R_OFF;
   BAR_L_OFF;
   BAR_R_OFF;

    switch (TimerStatus)
    {
        case 0: // Left Bar
          if(BlinkFlag[0]==0X0E)
          {// forward animation
            PORTA=DispBar[Anim];
            BAR_L_ON;
          }
          else if(BlinkFlag[0]==0X0F)
          {// backward animation
            PORTA=DispBar[5-Anim];
            BAR_L_ON;
          }
          else
          {
            PORTA=DispBar[(I2cRegRx[0]&0X07)];      //first 5 segments
            LATAbits.LATA5=I2cRegRx[4] & 0X01;      //down arrow
            LATAbits.LATA6=(I2cRegRx[4]>>1) & 0X01; //up arrow
            BAR_L = BlinkFlag[0];
          }
            break;

        case 1: // Left Display
          if(BlinkFlag[1]==0X0E)
          {// forward animation
            PORTA=DispAnim[Anim];
            DISP_L_ON;
          }
          else if(BlinkFlag[1]==0X0F)
          {// backward animation
            PORTA=DispAnim[(6-Anim)%6];
            DISP_L_ON;
          }
          else
          {
            PORTA=DispSeg[I2cRegRx[1]&0X0F];
            DISP_L = BlinkFlag[1];
          }
            break;

        case 2: // Right Display
          if(BlinkFlag[2]==0X0E)
          {// forward animation
            PORTA=DispAnim[Anim];
            DISP_R_ON;
          }
          else if(BlinkFlag[2]==0X0F)
          {// backward animation
            PORTA=DispAnim[(6-Anim)%6];
            DISP_R_ON;
          }
          else
          {
            PORTA=DispSeg[I2cRegRx[2]&0X0F];
            DISP_R = BlinkFlag[2];
          }
            break;

        case 3: // Right Bar
          if(BlinkFlag[3]==0X0E)
          {// forward animation
            PORTA=DispBar[Anim];
            BAR_R_ON;
          }
          else if(BlinkFlag[3]==0X0F)
          {// backward animation
            PORTA=DispBar[5-Anim];
            BAR_R_ON;
          }
          else
          {
            PORTA=DispBar[I2cRegRx[3]&0X07];//first 5 segments
            BAR_R = BlinkFlag[3];
          }
            break;

        default:

            break;
    }

    TimerStatus ++;
    if(TimerStatus > 3)
    {
        TimerStatus=0;
    }
}

void interrupt low_priority low_isr(void)
{
 if (INTCONbits.TMR0IF)   // timer 0 overflow?
 {
    TMR0H = Timer0CountHigh; // byte High
    TMR0L = Timer0Count;     // byte Low

    TIMER0_FLAG = 1;
    LongTimer ++;
    if(LongTimer >= LONG_TIMER)
    {
        LongTimer = 0;
        LONG_TIMER0_FLAG = 1;
    }

    INTCONbits.TMR0IF = 0;// reset of interrupt flag
 }
}   // Low Priority IntServiceRoutine
/*****************************************************************************/

void interrupt high_isr(void)
{      
/*----This is a porting of assembly code for Microchip AN734-------------------
 * over the I2C protocol it was added a 24Cxx EEPROM like data exchange mode.
 * The procedure is executed within the High Priority ISR in order to speed up
 * the response on I2C bus and avoid interruption, so an high care must be
 * taken on execution time and on external calls. If there are less strict
 * requirements the entire procedure can be executed in the main part after
 * the setting of a flag from the ISR.
    
/* SMP CKE D/A -P- -S- R/W U-A B-F
 * 
 *  0   0   0   0   1   0   0   1  State 1: I2C write operation,
 *                                          last byte was an address byte*/
#define STATE1 0X09

/*  0   0   1   0   1   0   0   1  State 2: I2C write operation,
 *                                          last byte was a data byte*/
#define STATE2 0X29

/*  0   0   0   0   1   1   0   X  State 3: I2C read operation,
 *                                          last byte was an address byte*/
#define STATE3 0X0C

/*  0   0   1   0   1   1   0   0  State 4: I2C read operation,
 *                                          last byte was a data byte*/
#define STATE4 0X2C

/*  0   0   1   0   1   X   0   0  State 5: Slave I2C logic reset
 *                       CKP = 1            by NACK from master*/
#define STATE5 0X28

#define STATE_MASK 0X2D        // to mask out unimportant bits in SSPSTAT
#define BF_MASK 0X2C           // to mask out BF bit in SSPSTAT
#define RW_MASK 0X28           // to mask out R/W bit in SSPSTAT
#define MAX_TRY 100            // maximum number of retries for transmission
//----------------------------------------------------------------------------*/
unsigned char SspstatMsk;
unsigned char I2cAddr; //to perform dummy reading of the SSPBUFF

 if (PIR1bits.SSPIF)   // I2C interrupt?
 {
    SspstatMsk=SSPSTAT & STATE_MASK; //mask out unimportant bits

//State 1------------------
    if(SspstatMsk == STATE1)
    {/* After State 1 the State Machine goes in State 1B, the next received
      * byte will be the register pointer*/
        I2C_POINTER_FLAG = 1;   // go into the State 1B
        I2cAddr = SSPBUF;  //dummy reading of the buffer to empty it
        SSPCON1bits.CKP = 1; //release clock immediately to free up the bus
    }

//State 2------------------
    else if(SspstatMsk == STATE2)
    {
        if(I2C_POINTER_FLAG)
        {//come from State 1B, the received data is the register pointer
            I2cRegPtr = SSPBUF;
            SSPCON1bits.CKP = 1; //release clock immediately to free up the bus
        }
        else
        {//the received data is a valid byte to store
            I2cRegRx[I2cRegPtr] = SSPBUF;
            SSPCON1bits.CKP = 1; //release clock immediately to free up the bus
            I2cRegPtr ++;
        }
        
        if(I2cRegPtr >= I2C_BUFF_SIZE_RX)
        {
            I2cRegPtr = I2C_BUFF_SIZE_RX - 1;
        }
        I2C_POINTER_FLAG = 0;
    }

//State 3------------------
    else if((SspstatMsk & BF_MASK) == STATE3)
    {//first reading from master after it sends address
        I2cAddr = SSPBUF;  //dummy reading of the buffer to empty it
        for(i=0; i<MAX_TRY; i++)
        {//check MAX_TRY times if buffer is empty
            if(!SSPSTATbits.BF)
            {//if buffer is empty try to send a byte
                for(j=0; j<MAX_TRY; j++)
                {//check MAX_TRY times to avoid collisions
                    SSPCON1bits.WCOL=0; //reset collision flag
                    SSPBUF = I2cRegTx[I2cRegPtr]; //send requested byte
                    if(!SSPCON1bits.WCOL)
                    {/*if no collision, sending was OK, point to the next byte*/
                        SSPCON1bits.CKP = 1; //free up the bus
                        I2cRegPtr ++;
                        if(I2cRegPtr >= I2C_BUFF_SIZE_TX)
                        {
                            I2cRegPtr = I2C_BUFF_SIZE_TX - 1;
                        }
                        // insert here an OK flag if needed
                        goto State3End; //everything's fine. Procedure over
                    }
                }
                // insert here a fault flag if needed
                goto State3End; //too many collisions. Exit.
            }
        }
        // insert here a fault flag if needed
        // if here the buffer was never empty
        Nop();

        State3End:
        I2C_POINTER_FLAG = 0;
    }

//State 4------------------
    else if((!SSPCON1bits.CKP) && (SspstatMsk == STATE4))
    {//subsequent readings. Usually it does the same as State 3

        for(i=0; i<MAX_TRY; i++)
        {//check MAX_TRY times if buffer is empty
            if(!SSPSTATbits.BF)
            {//if buffer is empty try to send a byte
                for(j=0; j<MAX_TRY; j++)
                {//check MAX_TRY times to avoid collisions
                    SSPCON1bits.WCOL=0; //reset collision flag
                    SSPBUF = I2cRegTx[I2cRegPtr]; //send requested byte
                    if(!SSPCON1bits.WCOL)
                    {/*if no collision, sending was OK, point to the next byte*/
                        SSPCON1bits.CKP = 1; //free up the bus
                        I2cRegPtr ++;
                        if(I2cRegPtr >= I2C_BUFF_SIZE_TX)
                        {
                            I2cRegPtr = I2C_BUFF_SIZE_TX - 1;
                        }
                        // insert here an OK flag if needed
                        goto State4End; //everything's fine. Procedure over
                    }
                }
                // insert here a fault flag if needed
                goto State4End; //too many collisions. Exit.
            }
        }
        // insert here a fault flag if needed
        // if here the buffer was never empty
        
        State4End:
        I2C_POINTER_FLAG = 0;
    }

//State 5------------------
    else if((SspstatMsk & RW_MASK) == STATE5)
    {//end cycle
        SSPCON1bits.CKP = 1; //release clock
        I2C_POINTER_FLAG = 0;
    }

//Default------------------
    else
    {//not in a known state. A different safety action could be performed here
        SSPCON1bits.CKP = 1; //release clock
        I2C_POINTER_FLAG = 0;
    }

    SSPCON1bits.CKP = 1; //release clock
    PIR1bits.SSPIF = 0;  //Clear MSSP Interrupt flag
}
    
}   // High Priority IntServiceRoutine

