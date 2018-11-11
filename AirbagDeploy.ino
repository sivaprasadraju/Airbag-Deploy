#include<stdint.h>
#include<Servo.h>
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<stdbool.h>

#define SET_BIT(PORT,BIT) PORT|= (1<<BIT)
#define CLR_BIT(PORT,BIT) PORT&= ~(1<<BIT)
#define ADC_PIN 0