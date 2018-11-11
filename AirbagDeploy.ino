#include<stdint.h>
#include<Servo.h>
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<stdbool.h>

#define SET_BIT(PORT,BIT) PORT|= (1<<BIT)
#define CLR_BIT(PORT,BIT) PORT&= ~(1<<BIT)
#define ADC_PIN 0

struct
{
  volatile unsigned int FLAG_ISR_TIMER0:1;
  volatile unsigned int FLAG_ISR_INT0:1;
}FLAG;

volatile uint16_t counter = 0x00;
volatile int value;
volatile int angle = 0;
Servo servo_test;

void idle_state(void)
{
  while(1)
  {
  	Serial.println("Entered into Idle State");
  	if(FLAG.FLAG_ISR_INT0 == 1)
  		{
    		Serial.println("After switch");
    		SET_BIT(PORTD,PD5);
    		ADCSRA |= (1<<ADEN);
    		start();
    	}
  	}
} 

void start(void)
{
  while(1){
    Serial.println("Entered into start");
    TIMSK0 |= (1<<TOIE0);
    if(FLAG.FLAG_ISR_TIMER0 == 1)
      {	
        Serial.println("After Timer");
        value = adc_read(0);
      	Serial.println(value);
        while(value <= 255)
          {
          	Serial.println("Entered into low state");
          	FLAG.FLAG_ISR_TIMER0 = 0;
            start();
          	value = adc_read(0);
          	Serial.println(value);
          }
        while(value > 255 && value <= 350)
          {
          	Serial.println("Entered into Buzzer State");
          	FLAG.FLAG_ISR_TIMER0 = 0;
            buzzer_state();
          	value = adc_read(0);
          	Serial.println(value);
          }
        while(value > 350 && value <= 790)
          {
          	Serial.println("Entered into medium state");
          	FLAG.FLAG_ISR_TIMER0 = 0;
            OCR2A = 128;
            OCR2B = 255;
          	value = adc_read(0);
          	Serial.println(value);
          }
        while(value > 790)
          {
          	Serial.println("Entered into airbag deployed");
          	FLAG.FLAG_ISR_TIMER0 = 0;
            OCR2A = 255;
            OCR2B = 255;
          	value = adc_read(0);
          	Serial.println(value);
          }
      }
  }
}

ISR(TIMER2_COMPA_vect)
{
	cli();
  	//SET_BIT(PORTD,PD6);
  	servo_motor_high();
  	sei();
}

ISR(TIMER2_COMPB_vect)
{
	cli();
  	CLR_BIT(PORTD,PD6);
  	sei();
}


void servo_motor_high(void)
{
	for(angle = 0; angle < 180; angle += 1) 	 
  {                                  
    servo_test.write(angle);              	 
    _delay_ms(15);                       
  } 
 
  _delay_ms(1000);
  
  for(angle = 180; angle>=1; angle-=5)    
  {                                
    servo_test.write(angle);          
    _delay_ms(5);                       
  } 

    _delay_ms(1000);
}

int main()
{
  SET_BIT(DDRD,PD7);
  SET_BIT(DDRD,PD6);
  SET_BIT(DDRD,PD5);

  
  CLR_BIT(PORTD,PD7);
  CLR_BIT(PORTD,PD6);
  CLR_BIT(PORTD,PD5);
  
  CLR_BIT(PORTD,PD2);
  CLR_BIT(DDRC,PC0); 
  
  TCCR0A = 0x00;
  TCNT0 = 0x00;
  TCCR0B |= ((1<<CS00)|(1<<CS02));
  TCCR0B &= ~(1<<CS01);
  
  TCCR2A = 0x00;
  TCNT2 = 0x00;
  TCCR2B|=((1<<CS20)|(1<<CS21)| (1<<CS22)); //ctc prescalar
  TIMSK2 |= (1<<OCIE2A) | (1<<OCIE2B); //ctc bits
  
  EICRA|=(1<<ISC00);
  EICRA &= ~(1<<ISC01); // Edge triggered (rising) 								//interrupt
  EIMSK |= (1<<INT0);
  
  Serial.begin(9600);
  
  sei();
  
  while(1)
  {
    Serial.println("Entered into main loop");
    idle_state();
  }
}