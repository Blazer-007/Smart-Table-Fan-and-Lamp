/*
 * Embedded_Final_Code_1.c
 *
 * Created: 20-09-2019 15:56:37
 * Author : Vivek Rai
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <LCDSUDO_NEW.h>

void getStr (uint8_t* str, uint8_t len, uint32_t val)
{
	uint8_t i;

	for(i=1; i<=len; i++)
	{
		str[len-i] = (uint8_t) ((val % 10UL) + '0');
		val/=10;
	}

	str[i-1] = '\0';
	LCD_print(str);
}

void PWM_init()
{
	TCCR0  |= (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00) | (1<<CS01);
	TCCR1A |= (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00) | (1<<CS01);
	TCCR1B |= (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00) | (1<<CS01);
	TCCR2  |= (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00) | (1<<CS01);
	DDRB   |= (1<<PB3);
	DDRD   |= (1<<PD7) | (1<<PD4) | (1<<PD5);

}

void ADC_init(){
	
	ADMUX   |=  (1<<REFS0) ;
	ADCSRA  |=  (1<<ADEN)  ;
	
}

int ADC_read(char  channel)
{
	unsigned int resultH,resultL,result=0;
	ADMUX   |= (channel & 0x0f);
	ADCSRA  |= (1<<ADSC);
	
	while (!(ADCSRA & (1<<ADIF)));
	
	ADCSRA |= (1<<ADIF);
	_delay_ms(1);
	
	resultL  = ADCL;
	resultH  = ADCH;
	result  |= (resultH<<8);
	result  |=  resultL;
	
	return result;
}


int main(void)
{
    /* Replace with your application code */
	
	LCD_init();
	PWM_init();
	ADC_init();
	
	int pot1_reading,pot2_reading,ldr_reading,lm_reading;
	float celsius;
	char str[100];
	
    while (1) 
    {
		
		//ADC_init();
		pot1_reading = (int)((ADC_read(0) / 1024.0) * 256) ;  // For LED
		OCR0   =  pot1_reading ;
		_delay_ms(500);
		
		
		//ADC_init();
		pot2_reading = (int)((ADC_read(1) / 1024.0) * 256) ;  // For DC Motor
		OCR2   =  pot2_reading  ;
		_delay_ms(500);
		
		//ADC_init();
		ldr_reading  = (int)((ADC_read(2) / 1024.0) * 256) ;  // For LED
		OCR1A = ldr_reading;
		_delay_ms(500);
		
		//ADC_init();
		celsius = ADC_read(3) * 4.88;
		celsius = celsius/10.00 ;
		lm_reading   = (int)celsius ;  // For DC Motor
		
		 //Displaying Status on LCD
		if(pot2_reading<30 && pot2_reading<30){
			LCD_go_index(1,0);
			LCD_print("Automatic Mode On");
			_delay_ms(200);
			LCD_clear();
			LCD_print("Fan Speed ");
			getStr(str,4,lm_reading);
			LCD_go_index(2,0);
			LCD_print("Light Intensity ");
			getStr(str,4,ldr_reading);
			
		}else{
			LCD_go_index(1,0);
			LCD_print("Manual Mode On");
			_delay_ms(200);
			LCD_clear();
			LCD_print("Fan Speed ");
			getStr(str,4,pot2_reading);
			LCD_go_index(2,0);
			LCD_print("Light Intensity ");
			getStr(str,4,pot1_reading);
			}
				
		
		_delay_ms(1000);
		
		//OCR1A  =  pot2_reading ;
		if(lm_reading<20)
		OCR1B  =  0   ;
		else if(lm_reading>=20 && lm_reading <=30)
		OCR1B =  100 ;
		else
		OCR1B = 255;
		
    }
}

