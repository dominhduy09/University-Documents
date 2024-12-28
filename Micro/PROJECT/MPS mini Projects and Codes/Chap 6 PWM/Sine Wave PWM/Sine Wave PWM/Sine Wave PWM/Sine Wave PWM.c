#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t Ref_val[625] = {0,1,3,4,5,6,8,9,10,12,13,14,
						15,17,18,19,20,22,23,24,26,27,28,29,
						31,32,33,35,36,37,38,40,41,42,43,45,
						46,47,48,50,51,52,53,55,56,57,58,60,
						61,62,63,65,66,67,68,70,71,72,73,75,
						76,77,78,79,81,82,83,84,85,87,88,89,
						90,91,93,94,95,96,97,99,100,101,102,103,
						104,106,107,108,109,110,111,113,114,115,116,117,
						118,119,121,122,123,124,125,126,127,128,130,131,
						132,133,134,135,136,137,138,139,140,141,143,144,
						145,146,147,148,149,150,151,152,153,154,155,156,
						157,158,159,160,161,162,163,164,165,166,167,168,
						169,170,171,172,173,174,175,175,176,177,178,179,
						180,181,182,183,184,185,185,186,187,188,189,190,
						191,191,192,193,194,195,196,196,197,198,199,200,
						201,201,202,203,204,204,205,206,207,207,208,209,
						210,210,211,212,213,213,214,215,215,216,217,217,
						218,219,219,220,221,221,222,223,223,224,224,225,
						226,226,227,227,228,229,229,230,230,231,231,232,
						232,233,233,234,234,235,235,236,236,237,237,238,
						238,239,239,240,240,240,241,241,242,242,243,243,
						243,244,244,244,245,245,245,246,246,247,247,247,
						247,248,248,248,249,249,249,249,250,250,250,250,
						251,251,251,251,252,252,252,252,252,253,253,253,
						253,253,253,254,254,254,254,254,254,254,254,254,
						254,255,255,255,255,255,255,255,255,255,255,255,
						255,255,255,255,255,255,255,255,255,255,255,255,
						255,254,254,254,254,254,254,254,254,254,254,253,
						253,253,253,253,253,252,252,252,252,252,251,251,
						251,251,250,250,250,250,249,249,249,249,248,248,
						248,247,247,247,247,246,246,245,245,245,244,244,
						244,243,243,243,242,242,241,241,240,240,240,239,
						239,238,238,237,237,236,236,235,235,234,234,233,
						233,232,232,231,231,230,230,229,229,228,227,227,
						226,226,225,224,224,223,223,222,221,221,220,219,
						219,218,217,217,216,215,215,214,213,213,212,211,
						210,210,209,208,207,207,206,205,204,204,203,202,
						201,201,200,199,198,197,196,196,195,194,193,192,
						191,191,190,189,188,187,186,185,185,184,183,182,
						181,180,179,178,177,176,175,175,174,173,172,171,
						170,169,168,167,166,165,164,163,162,161,160,159,
						158,157,156,155,154,153,152,151,150,149,148,147,
						146,145,144,143,141,140,139,138,137,136,135,134,
						133,132,131,130,128,127,126,125,124,123,122,121,
						119,118,117,116,115,114,113,111,110,109,108,107,
						106,104,103,102,101,100,99,97,96,95,94,93,
						91,90,89,88,87,85,84,83,82,81,79,78,
						77,76,75,73,72,71,70,68,67,66,65,63,
						62,61,60,58,57,56,55,53,52,51,50,48,
						47,46,45,43,42,41,40,38,37,36,35,33,
						32,31,29,28,27,26,24,23,22,20,19,18,
						17,15,14,13,12,10,9,8,6,5,4,3,1
						};

uint16_t i = 0;

void Fast_PWM1_8bit_Init(){
	TCCR1B |= (1<<WGM12) | (1<<CS10);
	TCCR1A |= 0xA1;			//(1<<COM1A1) | (0<<COM1A0) | (1<<COM1B1) | (0<<COM1B0) | (1<<WGM10);
	OCR1A = 0x00;
	OCR1B = 0x00;
	TIMSK |= (1<<TOIE1);
}

int main(void){
	DDRD |= (1<<PD4) | (1<<PD5);
	PORTD &= ~((1<<PD4) | (1<<PD5));
	Fast_PWM1_8bit_Init();
	sei();
    while(1){
	   	
    }
}

ISR(TIMER1_OVF_vect){
	if(i<625){
		OCR1A = Ref_val[i];
		OCR1B = 0;
	}
	else{
		OCR1A = 0;
		OCR1B = Ref_val[i-625];
	}
	i++;
	if(i == 1250) i = 0;
}