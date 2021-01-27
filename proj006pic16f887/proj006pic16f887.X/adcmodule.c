/*
 >>>>> File Name: adcmodule.c
 >>>>> Aim: To demonstrate Analog to Digital Conversion using internal ADC module of PIC16F887.
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
void adc_without_interrupts();
void adc_with_interrupts();
void adc_init_without_interrupts();
void adc_init_with_interrupts();
    //----------------------------START OF MAIN()---------------------------------
void main(void) 
{
    unsigned char adcmax[16]={"ADC max.: 0-1023"};
    unsigned char value[11]={"ADC Value: "};
    unsigned int adcval;
    
    init();  //Initializing the MCU with needed frequency and ports.
    lcd_init();  //Initializing LCD (HD44780) screen.
    
    command(0x80); //Cursor at first line first position.
    for(int i=0;i<16;i++)  //Display string in variable value.
        data(adcmax[i]);
    
    command(0xc0); //Cursor at second line first position.
    for(int i=0;i<11;i++)  //Display string in variable value.
        data(value[i]);
    
    //When using adc_init_without_interrupts() function use adc_without_interrupts() function for ADC operation
    //When using adc_init_with_interrupts() function use adc_with_interrupts() function for ADC operation
    //adc_init_without_interrupts();
    adc_init_with_interrupts();
    
    while(1)
    {
        //adc_without_interrupts();  //Call in ADC operation
        adc_with_interrupts();  //Call in ADC operation
        
        adcval=ADRESH*256+ADRESL;  //Converting ADC result to integer (base 10).
        
        //To display the ADC result using a LCD screen.
        command(0xcb);  //Cursor at second line eleventh position
        data((adcval/1000)+48);
        data((adcval%1000)/100+48);
        data((adcval%100)/10+48);
        data((adcval%10)+48);
        
        __delay_ms(20);
    }
    
}
//-------------------------------END OF MAIN()--------------------------------
void init()
{
    OSCCON=0x61;  //To select 4MHz Oscillator frequency and internal clock selection.
    __delay_ms(500);  //Power up delay. Half a second real time delay.
    //Bit 0 of the PORTA is the Analog input.
    TRISA0=1;  //Making RA0 as input pin. RA0 receives the analog input.
    ANS0=1;  //Make RA0 an analog line.
    //Complete PORTB is used as data lines for LCD screen(HD44780).
    TRISB=0x00;  //To make the PORTB as an output port.
    ANSELH=0X00;  //To make PORTB as digital.
    //bit 7,6 and 5 of PORTC are used as control lines for LCD.
    TRISC=0x00;  //To make the PORTC as an output port. Higher nibble of the PORTC is what we are using here.
    RC4=0;  //Outputting low voltage through the specified line. Using to drive LED.
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
void adc_init_without_interrupts()
{
    //Configure the ADC module
    ADCON0=0x41;
      //ADCS<1:0>=01;  Fosc/8 => 4MHz/8
      //CHS<3:0>=0000;  Channel selected AN0.
      //GO/DONEbar=0;  Conversion not progress. Must be zero at this stage.
      //ADON=1;  ADC is enabled.
    ADCON1=0x80;
      //ADCON1.7===ADFM=1;  A/D conversion result is right justified.
      //ADCON1.5===VCFG1=0;  -ve voltage reference is chose to be Vss.
      //ADCON1.4===VCFG0=0;  +ve voltage reference is chose to be Vdd.
    
}
//----------------------------------------------------------------------------
void adc_init_with_interrupts()
{
    //Configure the ADC module
    ADCON0=0x41;
      //ADCS<1:0>=01;  Fosc/8 => 4MHz/8
      //CHS<3:0>=0000;  Channel selected AN0.
      //GO/DONEbar=0;  Conversion not progress. Must be zero at this stage.
      //ADON=1;  ADC is enabled.
    ADCON1=0x80;
      //ADCON1.7===ADFM=1;  A/D conversion result is right justified.
      //ADCON1.5===VCFG1=0;  -ve voltage reference is chose to be Vss.
      //ADCON1.4===VCFG0=0;  +ve voltage reference is chose to be Vdd.
    //Configure ADC interrupt
    ADIF=0;  //PIR1.6=== ADC interrupt flag is cleared.
    ADIE=1;  //PIE1.6=== ADC interrupt is enabled.
    PEIE=1;  //INTCON.6=== Enable peripheral interrupt.
    GIE=1;  //INTCON.7=== Enable global interrupts.
    
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
void adc_without_interrupts()
{
    GIE=0;  //Disabling all interrupts.
    __delay_ms(1);  //Hoping more than acquisition time is provided. Refer calculation example in page 107 in datasheet.
    GO_nDONE=1;  //GO/DONEbar bit. Start conversion.
    while(GO_nDONE)  //Wait till the conversion is over.
        RC4=1;  //Turn on LED.
    RC4=0;  //Turn off LED.
    GIE=1;  //Enable all the interrupts.
}
//----------------------------------------------------------------------------
void adc_with_interrupts()
{
    __delay_ms(1);  //Hoping more than acquisition time is provided. Refer calculation example in page 107 in datasheet.
    GO_nDONE=1;  //GO/DONEbar bit. Start conversion.
    while(!ADIF)  //Wait till the ADC interrupt flag gets set (till conversion).
        RC4=1;  //Turn on LED.
    RC4=0;  //Turn off LED.
    ADIF=0;  //Clear ADC interrupt flag.
    
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