//-------------------------------------------------------------//
//Project: 
//		Calculator with ATMEGA32
//
//By: 	Jaime-Andres Arteaga
//
//		http://digital-day.webs.com/
//		jaime.andres@rocketmail.com
//
//AVR Interrupts: 
//		http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
//		http://www.jmnlab.com/interrupciones/interrupciones.html
//		http://www.jmnlab.com/robotzero/rzvd8.html
//-------------------------------------------------------------//


#include <avr/io.h>    		// Standard AVR header
#include <util/delay.h> 	// Delay loop functions
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdio.h>
#include <math.h>


#include "lcd_routines.c"
static unsigned int key;
static unsigned int new_key;
static unsigned int op;
static char char_op;


//-------------------------------------------------------------//
// External Interrupt 0 service routine
// key stores the pressed ky by user.
ISR(INT0_vect)
{   
	unsigned char temp;
    temp=PINC&0x0F;
	
	new_key=1;
	
	if(temp==7)
		key=0;
		
	else if(temp==2)
		key=1;
		
	else if(temp==6)
		key=2;
				
	else if(temp==10)
		key=3;
		
	else if(temp==1)
		key=4;
		
	else if(temp==5)
		key=5;
		
	else if(temp==9)
		key=6;
		
	else if(temp==0)
		key=7;
		
	else if(temp==4)
		key=8;
		
	else if(temp==8)
		key=9;
		
	else if(temp==15)
	{
		key=0;//add
		op=1;
		char_op='+';
	}
		
	else if(temp==14)
	{	
		key=0;//subtraction
		op=2;
		char_op='-';
	}		
		
	else if(temp==13)
	{
		key=0;//mult
		op=3;
		char_op='x';
	}		

	else if(temp==12)
	{		
		key=0;//div
		op=4;
		char_op='/';
	}		
		
	else if(temp==11)
	{	
		key=0;//execute
		op=5;
	}		
	
	else if(temp==3)
	{
		key=0;//on/c
		op=6;
	}		
		
}
//-------------------------------------------------------------//


//-------------------------------------------------------------//
//Configure ports as inputs and outputs.
void ports_init(void)
{
    // set port A for output
    DDRA = 0xFF;
    // set port B for output
    DDRB = 0xFF; 
    // set port C for input 
    DDRC = 0x00; 
    // set port D for output (PD2=INT0 input)
    DDRD = 0b11111011;
}
//-------------------------------------------------------------//

//-------------------------------------------------------------//
int main(void) 
{

	static int i, j, times, key1, key2;
	long a, b;
	
	static double result;
	static char buf_lcd1[16];
	
	result=3.14;
	sprintf(buf_lcd1, "%2.2f", result);

	new_key=1;
	result=3.14;
	key=0;
	key1=0;
	key2=0;
	a=0;
	b=0;
	result=0;
	op=0;
	times=0;
	char_op='+';
	
	//Init port pins
	ports_init();
    
    // External Interrupt(s) initialization
    // INT0: On
    // INT0 Mode: Rising Edge
    // INT1: On
    // INT1 Mode: Rising Edge
    // INT2: Off
    GICR|=0xC0;
    MCUCR=0x0F;
    MCUCSR=0x00;
    GIFR=0xC0;
	
	// Global enable interrupts
	asm("sei");
		
	//Initialize lcd
	lcd_init();
	lcd_send_command (0x0C);

	//--------------------------------------//
	//Send wellcome message
	sprintf(buf_lcd1, "ATMEGA32 Calc");
	lcd_displaystring_f(1,2,buf_lcd1);
	delay(8);
	
	sprintf(buf_lcd1, "J. A. Arteaga");
	lcd_displaystring_f(2,2,buf_lcd1);
	delay(8);
	//--------------------------------------//
	
	
	//--------------------------------------//
	//Shifts Message
	for(i=0; i<16; i++)
	{
		//lcd_send_command (0x18);//left
		lcd_send_command (0x1C);//right
		delay(2);		

	}
	//--------------------------------------//
	
	//Clear lcd
	lcd_clear();
		
	while(1)
	{
		if(new_key==1)
		{
			if(op==0)
			{
				a=a*10+key;
				sprintf(buf_lcd1, "%ld",a);
				lcd_displaystring_f(1,1,buf_lcd1);
			}
			
			else if(op==1 || op==2 || op==3 || op==4)
			{
				b=b*10+key;
				sprintf(buf_lcd1, "%ld%c%ld",a,char_op,b);
				lcd_displaystring_f(1,1,buf_lcd1);
			}
			
			else if(op==5)
			{
				if(char_op=='+')
					result=(float)(a+b);
					
				else if(char_op=='-')
					result=(float)(a-b);					
					
				else if(char_op=='x')
					result=(float)(a*b);							
					
				else if(char_op=='/')
					result=(float)(a/b);				
	
				result=(double)(3.14);
				sprintf(buf_lcd1, "%g", result);
				lcd_displaystring_f(2,1,buf_lcd1);
			}
			
			else if(op==6)
			{
				a=0;
				b=0;
				result=0;
				lcd_clear();
				sprintf(buf_lcd1, "0");
				lcd_displaystring_f(1,1,buf_lcd1);
				op=0;
			}
			
			//sprintf(buf_lcd1, "a=%ld, b=%ld",a, b);
			//lcd_displaystring_f(1,1,buf_lcd1);
			
			//sprintf(buf_lcd1, "k=%d,k1=%d,k2=%d,t=%d", key,key1,key2,times);
			//lcd_displaystring_f(2,1,buf_lcd1);
			
			
			
			
			new_key=0;
		}
		
		{
			
			asm("nop");
			//if(op==0)
			//{
			//	key2=key1;
			//	key1=key;
			//	a=key1+key2*(int)(times_f);
			//	times++;
			//	times_f=pow(10, (float)(times));
			//}
			
			//sprintf(buf_lcd1, "a=%d, b=%d",a, b);
			//lcd_displaystring_f(1,1,buf_lcd1);
			
			//sprintf(buf_lcd1, "k=%d,k1=%d,k2=%d,t=%d", key,key1,key2,times);
			//lcd_displaystring_f(2,1,buf_lcd1);
			
			//new_key=0;
		}
		
	} 

}
//-------------------------------------------------------------//



