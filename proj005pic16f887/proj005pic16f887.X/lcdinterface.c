/*
 >>>>> File Name: lcdinterface.c
 >>>>> Aim: Interface of PIC16F887 with 16x2 Character matrix 5x8 dot 
 * matrix LCD.
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
void lcd_init();
void command(unsigned char);
void data(unsigned char);
//----------------------------START OF MAIN()---------------------------------
void main(void) 
{
    init();  //Initializing the MCU with needed frequency and ports.
    lcd_init();  //Initializing LCD (HD44780) screen.
    
    unsigned char counter, data1[16]={"proj005pic16f887"},data2[12]={"lcdinterface"};
    
    command(0x80);  //Selecting the first position of the first line.
    for(counter=0;counter<16;counter++)  //Displaying data1
    {
        data(data1[counter]);
    }
    
    command(0xc2);  //Selecting the third position of the second line.
    for(counter=0;counter<12;counter++)  //Displaying data2
    {
        data(data2[counter]);
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
void lcd_init()
{
    //Few commands are sent twice just to make sure they are executed without miss.
    command(0x38);  //5x7 matrix and 2 lines selected to operate on LCD.
    command(0x38);  //5x7 matrix and 2 lines selected to operate on LCD.
    
    command(0x0f);  //Display on, cursor on, cursor blink on, auto-increment.
    command(0x0f);  //Display on, cursor on, cursor blink on, auto-increment.
    
    command(0x01);  //Clear display screen.
    command(0x01);  //Clear display screen.

}
//----------------------------------------------------------------------------
void command(unsigned char c)
{
    __delay_ms(200);
    PORTB=c;  //PORTB is holding byte of code which is a command for LCD.
    RC5=0;  //RS (Register Select) made register a command (instruction) register. Meaning the byte on PORTB is considered to be command.
    RC6=0;  //RW (Read/Write_bar) Write to LCD from MCU.
    RC7=1;  //Enabling the instruction given to LCD.
    __delay_ms(1);  //Do nothing for 1 milli-second.
    RC7=0;  //Disabling the instruction given to LCD.
    
}
//----------------------------------------------------------------------------
void data(unsigned char c)
{
    __delay_ms(200);
    PORTB=c;  //PORTB is holding byte of code which is a data for LCD.
    RC5=1;  //RS (Register Select) made register a data (instruction) register. Meaning the byte on PORTB is considered to be data.
    RC6=0;  //RW (Read/Write_bar) Write to LCD from MCU.
    RC7=1;  //Enabling the instruction given to LCD.
    __delay_ms(1);  //Do nothing for 1 milli-second.
    RC7=0;  //Disabling the instruction given to LCD.
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