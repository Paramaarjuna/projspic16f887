/*
 >>>>> File Name: dcmotorpwmtmr0adc.c
 >>>>> Aim: To demonstrate Pulse Width Modulation. Duty cycle of the output 
 * signal is controlled using ADC and TIMER0 of the controller. Thus produced 
 * PWM is used to drive DC motor and control its speed.
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
#include <pic16f887.h>
//---------------------------------------------------------------------------
#define _XTAL_FREQ 4000000  //_XTAL_FREQ represents the system frequency.
                              //Here the system is PIC16F887 and its operating 
                              //frequency is set to 4MHZ
unsigned char Ton, Toff;
unsigned long adcval;  //32-bit
//---------------------------------------------------------------------------
void init();
void lcd_init();
void initial_displays();
void command(unsigned char);
void data(unsigned char);
void adc_init_without_interrupts();
void adc_init_with_interrupts();
void adc_without_interrupts();
void adc_with_interrupts();
void adc_data_display();
//---------------------------------------------------------------------------
void __interrupt() TMR0_ISR(void)  //T0IE is made 1 to get here.
{
	//Before loading TMR0: Disable all interrupts and clear flags.
	GIE=0;  //Disabling all interrupts
	T0IE=0;  //Disabling interrupts from Timer0.
	T0IF=0;  //Clearing Timer0 interrupt flag.
	
	//Generate PWM signal. Load On-time or Off-time of the signal.
	if(RD4==0)
	{
		RD4=1;  //Make pin high
		TMR0=Ton;  //Load timer0 with on-time of the PWM signal.
		
	}
	else if(RD4==1)
	{
		RD4=0;  //Make pin low.
		TMR0=Toff;  //Load timer0 with off-time of the PWM signal.
		
	}
	//Timer0 is loaded and run. And the T0IF is now 1.
	
	T0IF=0;  //Clearing Timer0 interrupt flag.
	
	T0IE=1;  //Enable Timer0 interrupts.
	GIE=1;  //Enable global interrupts.
	
}  //End of 'void __interrupt() TMR0_ISR(void)'
//----------------------------START OF MAIN()---------------------------------
void main(void)
{
    unsigned char T, i;
	unsigned char data1[2]={"V="};  //Each char 8-bit
    unsigned char data2[13]={"2KHz=250 Ton="};  //Each char 8-bit
    unsigned char count;  //32-bit
    
	T=250;  //NOTE1: Read explanation at the bottom.
	
    init();  //Initializing the MCU with needed frequency and ports.
    lcd_init();  //Initializing LCD (HD44780) screen.
    
	initial_displays();  //Initially display few settings of project
	
	command(0x88); //Cursor at first line ninth position.
    data(data1[0]);
    command(0x89); //Cursor at first line tenth position.
    data(data1[1]);
    
	command(0xc0); //Cursor at second line first position.
    for(int i=0;i<13;i++)  //Display string in variable.
        data(data2[i]);
    
	//When using adc_init_without_interrupts() function use adc_without_interrupts() function for ADC operation
    //When using adc_init_with_interrupts() function use adc_with_interrupts() function for ADC operation
    adc_init_without_interrupts();
    //adc_init_with_interrupts();
    
	while(1)  //[[[WHILE1]]]
	{
		adc_without_interrupts();  //Call in ADC operation
        //adc_with_interrupts();  //Call in ADC operation
        
        adc_data_display();  //Displaying voltage value and corresponding ADC count
		
        count=adcval*T/1023;  //Slicing total counts of output frequency to max ADC count.
		if(count==0)
			RD4=0;  //Duty cycle of the PWM signal is theoretically 0%.
		else if(count==250)
			RD4=1;  //Duty cycle of the PWM signal is theoretically 100%
		else
		{
			Ton=256-count;  //On-time of the signal
			Toff=256-(T-count);  //Off-time of the signal

		}
		
		//Just printing out the counts.
		command(0xcd); //Cursor at second line sixth position.
		data((256-Ton)/100+48);  //First digit
		data(((256-Ton)%100)/10+48);  //Second digit
		data((256-Ton)%10+48);  //Third digit

	}  //End of [[[WHILE1]]]
    
}
//-------------------------------END OF MAIN()--------------------------------
void init()
{
    __delay_ms(10);
	OSCCON=0x61;  //NOTE1: at bottom. To select 4MHz Oscillator frequency and internal clock selection.
    __delay_ms(100);  //Power up delay.
    //Bit 0 of the PORTA is the Analog input.
    TRISA0=1;  //Making RA0 as input pin. RA0 receives the analog input.
    ANS0=1;  //Make RA0 an analog line.
    //Bit 5 of the PORTA outputs the PWM signal.
    TRISD4=0;  //Making RD4 as output pin. RD4 outputs PWM. Which is then used to control DC motor speed.
    //Complete PORTB is used as data lines for LCD screen(HD44780).
    TRISB=0x00;  //To make the PORTB as an output port.
    ANSELH=0X00;  //To make PORTB as digital.
    //bit 7,6 and 5 of PORTC are used as control lines for LCD.
    TRISC=0x00;  //To make the PORTC as an output port. Higher nibble of the PORTC is what we are using here.
    RD4=0;  //Making RA5 low.
	
	OPTION_REG=0x80;  //NOTE3: At bottom === 1:2 prescaler
	GIE=1;  //Enable global interrupts.
	PEIE=1;  //Peripheral interrupts enabled.
	T0IE=1;  //Timer0 interrupts.
	
}

//----------------------------------------------------------------------------
void lcd_init()
{
    //Power up delay for the LCD is covered by power up delay given for the complete circuit.
	command(0x38);  //5x7 matrix and 2 lines selected to operate on LCD.
    __delay_ms(10);
    
	command(0x0f);  //Display on, cursor on, cursor blink on, auto-increment.
    __delay_ms(10);
    
    command(0x01);  //Clear display screen.
    __delay_ms(10);
    
}
//----------------------------------------------------------------------------
void initial_displays()
{
	unsigned char analog_range1[14]={"Analog voltage"};
	unsigned char analog_range2[15]={"range= 0V to 5V"};
	command(0x80); //Cursor at first line first position.
    for(int i=0;i<14;i++)  //Display string in variable value.
        data(analog_range1[i]);
    command(0xc0); //Cursor at second line first position.
    for(int i=0;i<15;i++)  //Display string in variable value.
        data(analog_range2[i]);
    __delay_ms(3000);
	
	command(0x01);  //Clear display screen.
    __delay_ms(10);
    
    unsigned char adc_range1[15]={"ADC count range"};  //Each char 8-bit
    unsigned char adc_range2[11]={"= 0 to 1023"};  //Each char 8-bit
    command(0x80); //Cursor at first line first position.
    for(int i=0;i<15;i++)  //Display string in variable value.
        data(adc_range1[i]);
    command(0xc0); //Cursor at second line first position.
    for(int i=0;i<11;i++)  //Display string in variable value.
        data(adc_range2[i]);
    __delay_ms(3000);
	
	command(0x01);  //Clear display screen.
    __delay_ms(10);
    
	unsigned char tmr0_capacity1[13]={"TMR0 capacity"};  //Each char 8-bit
    unsigned char tmr0_capacity2[16]={"= 256 [0 to 255]"};  //Each char 8-bit
    command(0x80); //Cursor at first line first position.
    for(int i=0;i<13;i++)  //Display string in variable value.
        data(tmr0_capacity1[i]);
    command(0xc0); //Cursor at second line first position.
    for(int i=0;i<16;i++)  //Display string in variable value.
        data(tmr0_capacity2[i]);
    __delay_ms(3000);
	
	command(0x01);  //Clear display screen.
    __delay_ms(10);
    
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void command(unsigned char c)
{
    __delay_ms(10);
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
    __delay_ms(10);
    PORTB=c;  //PORTB is holding byte of code which is a data for LCD.
    RC5=1;  //RS (Register Select) made register a data (instruction) register. Meaning the byte on PORTB is considered to be data.
    RC6=0;  //RW (Read/Write_bar) Write to LCD from MCU.
    RC7=1;  //Enabling the instruction given to LCD.
    __delay_ms(1);  //Do nothing for 1 milli-second.
    RC7=0;  //Disabling the instruction given to LCD.
}
//----------------------------------------------------------------------------
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
void adc_data_display()
{
	unsigned long voltval;  //32-bit
    
	adcval=ADRESH*256+ADRESL;  //Converting ADC result to integer (base 10).
        
	//To display the ADC count using a LCD screen.
	command(0x8c);  //Cursor at first line twelfth position
	data((adcval/1000)+48);
	data((adcval%1000)/100+48);
	data((adcval%100)/10+48);
	data((adcval%10)+48);

	//To display the voltage equivalent of ADC value.
	command(0x84);  //Cursor at second line fifth position

	voltval=(adcval*500)/1023;
	data((voltval/100)+48);
	data('.');  //Display decimal point.
	data((voltval%100)/10+48);
	data((voltval%10)+48);
}
//----------------------------------------------------------------------------
/*
 >>>>> NOTE1: OSCCON: OSCILLATOR CONTROL REGISTER
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
 >>>>> NOTE2:
 * * TMR0 range is 0 to 255.
	 At MCU frequency (Fosc=)4MHz, Fosc/(4*Required frequency(--50Hz here--)) will produce the
	 * count value of 20000. After loading TMR0 with a value it increments its value
	 * at the rate of Fosc/4. Therefore, the meaning of 'count value of 20000' is
	 * that 50Hz signal should be 20000 times of its value to reach the speed of
	 * the TMR0. But, we are not using this count since it wont fit into TMR0.
	 * 
	 At prescaler setting of 1:128, Fosc/(4*(50)*128) will produce the value 156.25
	 * which we will round it off to 157. Meaning, the TMR0 will increment its value 
	 * by one when the MCU completes its 4MHz number of cycles 128 times. Now we load 
	 * TMR0 will be loaded with value 157 (which represents one complete cycle of 
	 * 50Hz signal)and allowed to increment till it overflows.
	 * But here we are splitting the count 157 into two to assign them to On-time
	 * and Off-time of the signal.
	 
 * >>>>> NOTE3: OPTION_REG: Option Register
bit 7 RBPU: PORTB Pull-up Enable bit
	1 = PORTB pull-ups are disabled
	0 = PORTB pull-ups are enabled by individual PORT latch values
bit 6 INTEDG: Interrupt Edge Select bit
	1 = Interrupt on rising edge of INT pin
	0 = Interrupt on falling edge of INT pin
bit 5 T0CS: TMR0 Clock Source Select bit
	1 = Transition on T0CKI pin
	0 = Internal instruction cycle clock (FOSC/4)
bit 4 T0SE: TMR0 Source Edge Select bit
	1 = Increment on high-to-low transition on T0CKI pin
	0 = Increment on low-to-high transition on T0CKI pin
bit 3 PSA: Prescaler Assignment bit
	1 = Prescaler is assigned to the WDT
	0 = Prescaler is assigned to the Timer0 module
bit 2-0 PS<2:0>: Prescaler Rate Select bits  = (1:128) prescaler selected.
 */