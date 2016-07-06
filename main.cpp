#include <msp430.h> 
#include <stdint.h>
#include <stdlib.h>
#include "nokia5110.h"
//#include "swiic.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"



#define fan BIT3					// Ventiliatorius PORT2
#define LED_ON P2OUT |= BIT2;		// LED priekiná ájungti
#define LED_OFF P2OUT &= ~BIT2;		// LED priekiná iðjungti


												// PORT1
static const unsigned LED			= BIT0;
static const unsigned RXD 			= BIT1;
static const unsigned TXD 			= BIT2;
static const unsigned SWITCH 		= BIT3;
static const unsigned LCD_CLK 		= BIT5;
static const unsigned LCD_BACKLIGHT = BIT6;
static const unsigned LCD_DATA 		= BIT7;
												// PORT2
static const unsigned LCD_DC 		= BIT0;
static const unsigned LCD_CE 		= BIT1;
static const unsigned SCL			= BIT2;
static const unsigned SDA			= BIT3;
												// LCD template
static nokia5110::Nokia5110<P1OUT, LCD_CLK, LCD_DATA, P2OUT, LCD_DC, P2OUT, LCD_CE, 10000> lcd;



void print_int(unsigned r, unsigned n) // r yra y kordinates, N kintamasis, itampa arba srove
{
    div_t d = div(n, 10);// lcd.print(71, r, d.rem + '0'); // Dalina is 10 ir isveda liekanas
    d = div(d.quot, 10);// lcd.print(65, r, d.rem + '0');  //
    d = div(d.quot, 10); lcd.print(65, r, d.rem + '0');  //
     lcd.print(59, r, '.');  // fiksuotas kablelis
    d = div(d.quot, 10); lcd.print(53, r, d.rem + '0');
    d = div(d.quot, 10); lcd.print(47, r, d.rem + '0');
}
void print_ints(unsigned r, unsigned n) // r yra y korinates, N kintamasis, itampa arba srove
{
    div_t d = div(n, 10); // Dalina is 10 ir isveda liekanas
    d = div(d.quot, 10); lcd.print(65, r, d.rem + '0');  //
    d = div(d.quot, 10); lcd.print(59, r, d.rem + '0');  //
     lcd.print(53, r, '.');  // fiksuotas kablelis
    d = div(d.quot, 10); lcd.print(47, r, d.rem + '0');

}
void print_proc(unsigned r, unsigned n)		 // r yra y korinates, N kintamasis, procntai
{                                               // Print register in hex and decimal
         // 4th digit
    div_t d = div(n, 10); lcd.print(65, r, d.rem + '0'); // Dalina is 10 ir isveda liekanas
    d = div(d.quot, 10); lcd.print(59, r, d.rem + '0');

    d = div(d.quot, 10); lcd.print(53, r, d.rem + '0');

}

void print_reg(void)
{												// Print
	lcd.print(8, 0, "Pb baterijos"); //8 x postumis, 0 y postumis
	lcd.print(14, 1, "monitorius");
	lcd.print(0, 3, "Itampa");
	lcd.print(0, 4, "Srove");
	lcd.print(0, 5, "Talpa");
	lcd.print(78, 3, "V");
	lcd.print(78, 4, "A");
	lcd.print(78, 5, "%");

}


//-------------------------------------ADC pradzia-----------------------------------//
unsigned int ADC_read(void){
	unsigned int adc;

	 ADC10CTL0 &= ~ENC;
	    while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
	    ADC10SA = (unsigned int)adc;			// Copies data in ADC10SA to unsigned int adc array
	    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start

	    __bis_SR_register(GIE);        // LPM0, ADC10_ISR will force exit

	    return adc;
}


void ADC_init(void){

	 ADC10CTL1 = INCH_4 + CONSEQ_1;            // A2/A1/A0, single sequence
	 ADC10CTL0 = SREF_1+ADC10SHT_2 + MSC + ADC10ON + ADC10IE+REFON;
	 ADC10CTL0 &= ~REF2_5V;
	 ADC10DTC1 = 0x02;                         // 3 conversions
	 ADC10AE0 |= 0x0C;                         // Disable digital I/O on P1.0 to P1.2
}



/*
 * main.c
 */
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;					// Disable watchdog reset

	BCSCTL1 = CALBC1_1MHZ;						// Run at 1 MHz
	DCOCTL  = CALDCO_1MHZ;						//

	P1REN |= RXD ;
	P1DIR |= LED | TXD | LCD_CLK | LCD_BACKLIGHT | LCD_DATA;
	P1OUT |= RXD | TXD | LCD_CLK;
	P1OUT &= ~BIT6;


	P2DIR |= LCD_DC | LCD_CE ;
	P2OUT |= LCD_DC | LCD_CE|0X04;
	P1SEL |= BIT3;					// ADC input pin P1.3
	P1SEL |= BIT4;					// ADC input pin P1.3


	P2DIR |= fan|BIT2;


	P2OUT &= ~fan;



	lcd.reset();								// Init LCD
	lcd.init();									//
	lcd.clear();								//


	ADC_init();

//	lcd.print(12, 2, "Dummy Load");

	LED_OFF;

	 print_reg();
	while(1){




	}
}

















