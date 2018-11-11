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