<!-- README.md file for proj008pic16f887 -->

## PROJECT: proj008pic16f887
---


## Aim:
---
To demonstrate production of PWM (Pulse Width Modulation) signal of particualar frequency and varying its On-time (duty-cycle) with regular delays using Timer0 of the controller and internal clock as the source for the timer0.

### Apparatus/Software applications: 
---
- MPLAB X IDE v5.35
- SimulIDE_0.4.14-SR5

**Main Program**: [pwmtmr0.c](proj008pic16f887.X/pwmtmr0.c)

<object data="proj008pic16f887.X/pwmtmr0.c" type="text/c" width="600"></object>

|Components/Category|Components names in Circuit|
|---|---|
|Integrated Circuits (IC)|PIC16F887|
|LCD|HD44780|
|Resistors|R(100 Ohm)|
|Voltmeters|Voltmeter|
|LEDs|LED|
|Scopes|Oscilloscope|

<br>

### Circuit Diagram: 
---

**Note:** Lit LEDs turn yellow in color.

![proj008pic16f887.png](proj008pic16f887.png "proj008pic16f887.png")  
<i>**Figure:** proj008pic16f887.png: Producing variable PWM using timer0.</i>

[A video clip of circuit simulation.](proj008pic16f887.mp4
)

<video src="proj008pic16f887.mp4" type="video/mp4" controls muted width="500" height="400">Video tag not supported</video>  <!--atributes: autoplay loop -->

## Description: 
---
- `PORTB` is used to send 8-bit data to display characters on the LCD screen or 8-bit command value to control the possible aspects of the LCD (like cursor position, clearing screen, etc.). `PORTB` direction of operation is controlled using `TRISB`. Just to make sure the port to be a digital port, we can assign `ANSELH` register with value `0x00` manually in the program though it is set so on MCU power-up.
- `RC7`, `RC6` and `RC5` of `PORTC` are used as control lines of LCD (viz, `EN`, `RW` and `RS` respectively).
- `RD4`, which is actually the pin that outputs the PWM signal, is used to drive an LED and Oscilloscope. `RD4` pis made an output pin using `TRISD4`.
- `OPTION_REG` is configured in such a way that PORTB pull-ups are disabled, clock source for timer0 is internal instruction cycle clock (FOSC/4) (i.e., timer0 is in timer mode), TMR0 increments on low-to-high transition, prescaler is assigned to the Timer0 module and prescaler rate is selected as needed using selectio bits. We chose the prescaler rate of 1:2 with intent to generate 2KHz PWM signal. `TMR0` is an 8-bit register that can count 256 counts in the range from 0 to 255.  Based on this limitation we choose the prescaler rate using the following figure.
![prescaler_formula.jpg](prescaler_formula.jpg "prescaler_formula.jpg")  
<i>**Figure:** prescaler_forula.jpg: Prescaler rate selection formula.</i>
- *If we do not use prescaler:* We chose the frequency of our controller to be 4MHz. Therefore, the TMR0 runs(increments its value) at the frequency equal to fosc/4, i.e., 1 mega counts in a second. Which means, by the time the controller completes 4 mega counts, the TMR0 completes 1 mega counts. Thus the output frequency will be 1MHz. But still, this is not possible beacause TMR0 cannot count upto 1000000, since its value can range from 0 to 255.
- *When we choose a prescaler (say 1:2):* Lets say our output frequency to be 2KHz. On substituting in the formula, `count=fosc/(4*prescaler*fout)`, count=4000000/(4\*2\*2000), will produce the count value equal to 250, which can fit in TMR0 and thus the prescaler we selected is good to go.
- Let us consider another example where we want to generate the signal of frequency 50Hz. If we select the prescaler rate as 1:2, count=4000000/(4\*2\*50) will be count=10000. Which will fail to fit in TMR0. Therefore, we cannot choose 1:2 prescaler rate. Prescaler rate, 1:4 results 5000 counts, 1:8 results 2500 counts, 1:16 results 1250 counts, 1:32 results 625 counts, 1:64 results 312.5 counts, which will fail. But prescaler rate 1:128 will result in 156.25 counts (fits in TMR0) which should be round-figured to either 157 or 156 accordingly there will be slight variation in th output frequency.
- From that derived count value, we split it as needed to decide On-time and Off-time of the signal. Lets say we want 25% duty cycle. 25% On-time in counts is 156/4=39. So we assign `Ton=256-39` that is Ton=217, which will be fed to TMR0. Now TMR0 has the value 217 and TMR0 starts to increment and rolls-over after reaching 255, making the `T0IF=1`, which needs to be cleared everytime in the program. Similarly, `Toff=256-(157-39)=139` is calculated and assigned to TMR0. TMR0 starts to increment from 139 and rolls-over after reaching 255, making the `T0IF=1`.

- **HD44780**
> - **Pinout diagram**  
![hd44780_pinout.jpg](hd44780_pinout.jpg "hd44780_pinout.jpg")  
*NOTE:* In our circuit diagram created from simulIDE, the potentiometer connections and few other pins are hidden. But are necessary during practical circuit.

> - **Pin Description**  
> *Source:* <https://en.wikipedia.org/wiki/Hitachi_HD44780_LCD_controller>  
<table>
    <tr><th align="center">Pin No.</th><th align="center">Pin Description</th></tr>
    <tr><td>1</td><td>Ground</td></tr>
    <tr><td>2</td><td>VCC +3.3 to +5V (typical)</td></tr>
    <tr><td>3</td><td>Contrast adjustment (VO) This is an analog input, typically connected to a potentiometer. The user must be able to control this voltage independent of all other adjustments, in order to optimise visibility of the display that varies i.a. with temperature, and, in some cases height above the sea level. With a wrong adjustment the display will seem to malfunction.</td></tr>
    <tr><td>4</td><td>Register Select (RS). RS=0: Command, RS=1: Data</td></tr>
    <tr><td>5</td><td>Read/Write (R/W). R/W=0: Write, R/W=1: Read (In most applications reading from the HD44780 makes no sense. In that case this pin can be permanently connected to ground and no io pins need to be allocated to steer it.)</td></tr>
    <tr><td>6</td><td>Clock (Enable). Falling edge triggered</td></tr>
    <tr><td>7</td><td>Bit 0 (Not used in 4-bit operation)</td></tr>
    <tr><td>8</td><td>Bit 1 (Not used in 4-bit operation)</td></tr>
    <tr><td>9</td><td>Bit 2 (Not used in 4-bit operation)</td></tr>
    <tr><td>10</td><td>Bit 3 (Not used in 4-bit operation)</td></tr>
    <tr><td>11</td><td>Bit 4</td></tr>
    <tr><td>12</td><td>Bit 5</td></tr>
    <tr><td>13</td><td>Bit 6</td></tr>
    <tr><td>14</td><td>Bit 7</td></tr>
    <tr><td>15</td><td>Backlight Anode (+) (If applicable)</td></tr>
    <tr><td>16</td><td>Backlight Cathode (-) (If applicable)</td></tr>
</table>

> - **Instruction Set**  
> *Source:* <https://en.wikipedia.org/wiki/Hitachi_HD44780_LCD_controller>  

<table>
    <tr>
        <th rowspan="2" align="center">Instruction</th>
        <th colspan="10" align="center">Code</th>
        <th rowspan="2" align="center">Description</th>
        <th rowspan="2" align="center">Execution time (max)(when fcp = 270 kHz)</th>
    </tr>
    <tr>
        <td>RS</td><td>R/W</td>
        <td>B7</td><td>B6</td><td>B5</td><td>B4</td><td>B3</td><td>B2</td><td>B1</td><td>B0</td>
    </tr>
    <tr>
        <td>Clear Display</td><td>0</td><td>0</td>
        <td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>1</td>
        <td>Clears display and returns cursor to the home position (address 0).</td>
        <td>1.52 ms</td>
    </tr>
	<tr>
        <td>Cursor home</td><td>0</td><td>0</td>
        <td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>1</td><td>*</td>
        <td>Returns cursor to home position. Also returns display being shifted to the original position. DDRAM content remains unchanged.</td>
        <td>1.52 ms</td>
    </tr>
    <tr>
        <td>Entry mode set</td><td>0</td><td>0</td>
        <td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>1</td><td>I/D</td><td>S</td>
        <td>Sets cursor move direction (I/D); specifies to shift the display (S). These operations are performed during data read/write.</td>
        <td>37 μs</td>
    </tr>
    <tr>
        <td>Display on/off control</td><td>0</td><td>0</td>
        <td>0</td><td>0</td><td>0</td><td>0</td><td>1</td><td>D</td><td>C</td><td>B</td>
        <td>Sets on/off of all display (D), cursor on/off (C), and blink of cursor position character (B).</td>
        <td>37 μs</td>
    </tr>
	<tr>
        <td>Cursor/display shift</td><td>0</td><td>0</td>
        <td>0</td><td>0</td><td>0</td><td>1</td><td>S/C</td><td>R/L</td><td>*</td><td>*</td>
        <td>Sets cursor-move or display-shift (S/C), shift direction (R/L). DDRAM content remains unchanged.</td>
        <td>37 μs</td>
    </tr>
	<tr>
        <td>Function set</td><td>0</td><td>0</td>
        <td>0</td><td>0</td><td>1</td><td>DL</td><td>N</td><td>F</td><td>*</td><td>*</td>
        <td>Sets interface data length (DL), number of display line (N), and character font (F).</td>
        <td>37 μs</td>
    </tr>
	<tr>
        <td>Set CGRAM address</td><td>0</td><td>0</td>
        <td>0</td><td>1</td>
        <td colspan="6" align="center">CGRAM address</td>
        <td>Sets the CGRAM address. CGRAM data are sent and received after this setting.</td>
        <td>37 μs</td>
    </tr>
	<tr>
        <td>Set DDRAM address</td><td>0</td>
        <td>0</td><td>1</td><td colspan="7" align="center">DDRAM address</td>
        <td>Sets the DDRAM address. DDRAM data are sent and received after this setting.</td>
        <td>37 μs</td>
    </tr>
	<tr>
        <td>Read busy flag & address counter</td><td>0</td><td>1</td>
        <td>BF</td><td align="center" colspan="7">CGRAM/DDRAM address</td>
        <td>Reads busy flag (BF) indicating internal operation being performed and reads CGRAM or DDRAM address counter contents (depending on previous instruction).</td>
        <td>0 μs</td>
    </tr>
	<tr>
        <td>Write CGRAM or DDRAM</td><td>1</td><td>0</td>
        <td align="center" colspan="8">Write Data</td>
        <td>Write data to CGRAM or DDRAM.</td>
        <td>37 μs</td>
    </tr>
    <tr>
        <td>Read from CG/DDRAM</td><td>1</td><td>1</td>
        <td align="center" colspan="8">Read Data</td>
        <td>Read data from CGRAM or DDRAM.</td>
        <td>37 μs</td>
    </tr>
    <tr>
        <td colspan="13" align="justify">
        Instruction bit names —

I/D - 0 = decrement cursor position, 1 = increment cursor position; S - 0 = no display shift, 1 = display shift; D - 0 = display off, 1 = display on; C - 0 = cursor off, 1 = cursor on; B - 0 = cursor blink off, 1 = cursor blink on ; S/C - 0 = move cursor, 1 = shift display; R/L - 0 = shift left, 1 = shift right; DL - 0 = 4-bit interface, 1 = 8-bit interface; N - 0 = 1/8 or 1/11 duty (1 line), 1 = 1/16 duty (2 lines); F - 0 = 5×8 dots, 1 = 5×10 dots; BF - 0 = can accept instruction, 1 = internal operation in progress.
        </td>
    </tr>
</table>

> - Few specific byte-codes to highlight:
>> - `0x04` - move cursor right, don’t shift display.
>> - `0x05` - move cursor right, do shift display (left).
>> - `0x06` - move cursor right, don’t shift display (this is the most common).
>> - `0x07` - move cursor right, do shift display (left).
>> - `0x08 ... 0x0B` - turn display off.
>> - `0x0c` - display on, cursor off.
>> - `0x0e` - display on, cursor on, steady cursor.
>> - `0x0f` - display on, cursor on, blinking cursor.
>> - `0x10` - Shift cursor left.
>> - `0x14` - Shift cursor right.
>> - `0x18` - Shift display left.
>> - `0x1c` - Shift display right.
>> - `0x80 + position` - Cursor position
> - HD44780 is designed to control a 40 character 4-line display. If you have a 16×2 then you will only see the first 16 characters of the top two lines. In a 16x2 display, the first line is position 0-15. Therefore, 0x80 is the first position, 0x80 + 12 = 0x8C is the 13th (remember, they are zero indexed). 0xc0 is the first position in the second line in the LCD screen.

- ***What happens:***
- We run the controller at a particular frequency. We use `TMR0` to produce the PWM signal of desired frequency within the limited range at pin `RD4`. And acordingly `OPTION_REG` is configured. The output at the `RD4` is fed to Oscilloscope to observe the PWM signal. Corresponding Ton counts are displayed on the LCD screen.
- Once the Ton and Toff are calculated, they are assigned to TMR0 individually. We vary the duty cycle of the signal after every 3 seconds by changing the values on Ton and Toff.
> - 8-bit HEX code is put on `PORTB`; `EN` is made high; `RW` is made low to ensure that the controller is writing to the LCD; `RS`, called as Register Select pin based on which the HEX code on PORTB is considered as either *command* or *data*. `RS=0` for the HEX code to be a command and `RS=1` for the HEX code to be data which gets displayed on the screen.
> - In our program we wrote two functions named as command and data to choose between the operation type. We use command function to set cursor position (if needed), to clear display, whether the cursor gets automatically incremented and etc. Then we use data function to display the HEX code with corresponding ASCII code on the LCD screen.
- `__delay_ms(x)` is a macro to produce time delay in terms of milli-seconds. And it's definition is as follows:  
`#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))`  
    > where 
    > - `_delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))` is `_delay(x)`.  
    > - `x` in `_delay(x)` is the count of instruction cycles.  
    > - `_XTAL_FREQ` is the operating frequency of the system (PIC16F887 in our case).
* At 4MHz, `__delay_ms(100)` is equal to `_delay(100000)`. That is, 100000 instruction cycles. Which makes 1 milli-second equal to 1 instruction cycle. Which finally means that `NOP` _--assembly instruction for no operation (which is a single cycle instruction.)--_ is performed 100000 times to achieve the 100 milli-second delay.  
* At 8MHz, `__delay_ms(100)` is equal to `_delay(200000)`. That is, 200000 instruction cycles to execute `NOP` instruction. At this frequency of the system, 2 instruction cycles perform `NOP` to achieve 1ms delay.  
* Just notice, how many instruction cycles are need to be `NOP` filled to achieve desired real time delay.

---

