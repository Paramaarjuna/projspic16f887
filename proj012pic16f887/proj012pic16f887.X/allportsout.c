/*
 >>>>> File Name: allportsout.c
 >>>>> Aim: Drive LEDs from all the available I/O lines of PIC16F887
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
//---------------------------------------------------------------------------
void init();
//----------------------------START OF MAIN()---------------------------------
void main(void) 
{
    init();  //Initializing the MCU with needed frequency and ports.
	PORTA=PORTB=PORTC=PORTD=PORTE=0X00;
	
	while(1)  //start of [[while1]]
	{
		PORTA=0XFF;
		__delay_ms(150);
		PORTA=0X00;
		
		PORTB=0XFF;
		__delay_ms(150);
		PORTB=0X00;
		
		PORTC=0XFF;
		__delay_ms(150);
		PORTC=0X00;
		
		PORTD=0XFF;
		__delay_ms(150);
		PORTD=0X00;
		
		PORTE=0XFF;
		__delay_ms(150);
		PORTE=0X00;
		
	}  //End of [[while1]]
}
//-------------------------------END OF MAIN()--------------------------------
void init()
{
    OSCCON=0x61;  //To select 4MHz Oscillator frequency and internal clock selection.
    __delay_ms(500);  //Power up delay. Half a second real time delay.
    TRISA=0x00;  //To make the PORTA as an output port.
    TRISB=0x00;  //To make the PORTB as an output port.
    TRISC=0x00;  //To make the PORTC as an output port.
    TRISD=0x00;  //To make the PORTD as an output port.
    TRISE=0x00;  //To make the PORTE as an output port.
    
	ANSEL=0x00;  //Analog channels from ANS0 to ANS7 are made digital
	ANSELH=0x00;  //Analog channels from ANS8 to ANS13 are made digital
	
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
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