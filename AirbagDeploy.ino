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