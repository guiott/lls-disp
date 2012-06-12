/* //////////////////////////////////////////////////////////////////////////////////////
** File:      LlsDisplay.c

 * #define  Ver "LlsDisplay v.0.10\r      by Guiott"

/**
* \mainpage LlsDisplay.c
* \author    Guido Ottaviani-->guido@guiott.com<--
* \version 0.0.9
* \date 06/12
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

while (1)  // main loop
{
    if(TIMER0_FLAG)
    {
        Cycle();
    }

    if(LONG_TIMER0_FLAG)
    {
        LongCycle();
    }

}  // main loop
} // main ==========



/* Functions *****************************************************************/
void LongCycle(void)
{/**
 *\brief slow changings. Drive the slow and fast blinks and other changings
  * on display.
  * The High nibble of the display number to be shown, controls in which  mode
  * the Low nibble will be displayed:
  * 0 = steady ON
  * 1 = OFF
  * 2 = slow blinking
  * 3 = fast blinking
  * 4 = CW/forward segments animation
  * 5 = CCW/backward segments animation
 *
 */
   static unsigned char Blink;
   int LP;
   int DispMode;

   Blink++;
   if(Blink>=3)
   {
       Blink=0;
       SLOW_BLINK_FLAG = SLOW_BLINK_FLAG ^ 1;
       if(BlinkDot==0)
       {// Make left bar dots blinking as an hearthbeat
           BlinkDot=1;
       }
       else
       {
           BlinkDot=0;
       }
   }
   
   LONG_TIMER0_FLAG=0;
   FAST_BLINK_FLAG = FAST_BLINK_FLAG ^ 1;

   Anim++;
   if(Anim>=6)
   {
       Anim=0;
   }

   for(LP=0; LP<=3; LP++)
   {
        DispMode=DispNum[LP]>>4;
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
 }


void Cycle()
{/**
 *\brief cycle the number to display to perform multiplexing
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
            PORTA=DispBar[(DispNum[0]&0X07)];
            LATAbits.LATA5=BlinkDot;
            LATAbits.LATA6=BlinkDot;
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
            PORTA=DispSeg[DispNum[1]&0X0F];
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
            PORTA=DispSeg[DispNum[2]&0X0F];
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
            PORTA=DispBar[DispNum[3]&0X07];
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


/*===========================================================================*/

// Low priority interrupt vector

#pragma code LowVector = 0x18
void InterruptVectorLow (void)
{
  _asm
    goto InterruptHandlerLow //jump to interrupt routine
  _endasm
}

//----------------------------------------------------------------------------
// Low priority interrupt routine

#pragma code
#pragma interruptlow InterruptHandlerLow

/*===========================================================================*/

/*IntServiceRoutine***********************************************************/
void InterruptHandlerLow (void)

{  

}   // Low Priority IntServiceRoutine
/*****************************************************************************/


/*===========================================================================*/
// High priority interrupt vector

#pragma code HighVector = 0x08
void
InterruptVectorHigh (void)
{
  _asm
    goto InterruptHandlerHigh //jump to interrupt routine
  _endasm
}

//----------------------------------------------------------------------------
// High priority interrupt routine

#pragma code
#pragma interrupt InterruptHandlerHigh

/*===========================================================================*/


/*IntServiceRoutine***********************************************************/
void InterruptHandlerHigh (void)

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
}   // High Priority IntServiceRoutine

