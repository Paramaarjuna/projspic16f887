/*
 >>>>> File Name: sevensegment.c
 >>>>> Aim: Interface four 7-segment LEDs with PIC16F887 and display a 
 * four digit number (Display 4-digit integer number using the multiplexed 
 * 7-segment display interface).
 * 
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = ON          // Code Protection bit (Program memory code protection is enabled)
#pragma config CPD = ON         // Data Code Protection bit (Data memory code protection is enabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
//---------------------------------------------------------------------------
#define _XTAL_FREQ 4000000  //_XTAL_FREQ represents the system frequency.
                              //Here the system is PIC16F887 and its operating 
                              //frequency is set to 4MHZ
void init();
//----------------------------START OF MAIN()---------------------------------
void main(void) 
{
    init();  //Initializing the MCU with needed frequency and ports.
    
    unsigned int display_digit[4]={2,0,2,1};  //Change digits here to change the displaying 4-digit integer.
    unsigned int digit_code[10]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6};  //Hex code of digit 0 is 0xfc for the 7-segment LED display for current circuit connection.
    
    while(1)
    {
        PORTC=0x80;  //Enabling Segment-4 to display a digit and disabling other segments. Only RC7=1;
        PORTB=digit_code[display_digit[0]];  //Pass the digit
        __delay_ms(2);  //Hold for 10 milliseconds.

        PORTC=0x40;  //Enabling Segment-3 to display a digit and disabling other segments. Only RC6=1;
        PORTB=digit_code[display_digit[1]];  //Pass the digit
        __delay_ms(2);  //Hold for 10 milliseconds.
        
        PORTC=0x20;  //Enabling Segment-2 to display a digit and disabling other segments. Only RC5=1;
        PORTB=digit_code[display_digit[2]];  //Pass the digit
        __delay_ms(2);  //Hold for 10 milliseconds.
        
        PORTC=0x10;  //Enabling Segment-1 to display a digit and disabling other segments. Only RC4=1;
        PORTB=digit_code[display_digit[3]];  //Pass the digit
        __delay_ms(2);  //Hold for 10 milliseconds.
        
    }
}
//-------------------------------END OF MAIN()--------------------------------
void init()
{
    OSCCON=0x61;  //To select 4MHz Oscillator frequency and internal clock selection.
    __delay_ms(500);  //Power up delay. Half a second real time delay.
    TRISB=0x00;  //To make the PORTB as an output port.
    TRISC=0x00;  //To make the PORTC as an output port. Higher nibble of the PORTC is what we are using here.
    
}
//----------------------------------------------------------------------------
/*
 >>>>> OSCCON: OSCILLATOR CONTROL REGISTER
bit 7 Unimplemented: Read as '0'
bit 6-4 IRCF<2:0>: Internal Oscillator Frequency Select bits
111 = 8 MHz
110 = 4 MHz (default)
101 = 2 MHz
100 = 1 MHz
011 = 500 kHz
010 = 250 kHz
001 = 125 kHz
000 = 31 kHz (LFINTOSC-Low-Frequency Internal Oscillator.)
bit 3 OSTS: Oscillator Start-up Time-out Status bit(1)
1 = Device is running from the external clock defined by FOSC<2:0> of the CONFIG1 register
0 = Device is running from the internal oscillator (HFINTOSC or LFINTOSC)
bit 2 HTS: HFINTOSC Status bit (High Frequency - 8 MHz to 125 kHz)
1 = HFINTOSC is stable
0 = HFINTOSC is not stable
bit 1 LTS: LFINTOSC Stable bit (Low Frequency - 31 kHz)
1 = LFINTOSC is stable
0 = LFINTOSC is not stable
bit 0 SCS: System Clock Select bit
1 = Internal oscillator is used for system clock
0 = Clock source defined by FOSC<2:0> of the CONFIG1 register
 * 
 >>>>> 
 */