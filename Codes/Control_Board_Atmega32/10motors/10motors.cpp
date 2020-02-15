/*
 * _10motors.cpp
 *
 * Created: 10-Nov-16 1:44:31 PM
 *  Author: RHITVIK
 */ 


/*
 * coinAcceptor_innterruptProgram.cpp
 *
 * Created: 26-Jul-16 7:34:29 PM
 *  Author: RHITVIK
 */ 

// #include <avr/io.h>
// 
// int main(void)
// {
// 	GICR |= (1<<INT1);
// 	MCUCR |= (1<<ISC10) | (1<<ISC11);
// 	
//     while(1)
//     {
//         //TODO:: Please write your application code 
//     }
// }

#define F_CPU 12000000UL
#define OFF 0
#define ON  1

#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<string.h>

void disable_driver(void);

void accept_coin(void);
void return_coin(void);

void initialise_PWM(void);
void PWM1(void);
void PWM2(void);

void sw_debounce_pressed(void);
void sw_debounce_released(void);

void KILL_PWM1(void);
void KILL_PWM2(void);

void initialise_counter(void);
void stop_counter(void);

void detect_coin(void);
void vend_STATIONERY(void);
void check_IF_successful(void);
void CHECK_STOCK(void);

void resetParameters(void);

void motor(int a,bool b); 
 
 void spi_init_master_INTERRUPT (void);
 void spi_init_master (void);
 void spi_init_slave (void);
 char spi_tranceiver (unsigned char data);
 void spi_kill_master(void);
 void spi_kill_slave(void);
 
 void recieve_packets(void);
 void library(void);
  
static volatile uint8_t count=0;  //for interrupt generation
static volatile uint8_t count1=0; //10 sec delay initializer
static volatile uint8_t FAIL = 0;

unsigned char packet = 0;
unsigned char packet1 = 0;
unsigned char packet2 = 0;
unsigned char packet3 = 0;
unsigned char packet4 = 0;

uint8_t p = 0;
uint8_t kick = 0;
uint8_t STOCK = 0;
   
uint8_t coin_10RS = 0;
uint8_t RS10_coin_no = 0;
uint8_t coin_count = 0;

uint8_t coin_05RS = 0;
uint8_t RS05_coin_no = 0;

int money = 0;
int u = 0;

int pressed=0;
int pressed_1=0;
int pressed_2=0;

int refresh = 0;
int refresh2 = 0;

char b = 0;

int main(void)
{
	MCUCSR |= (1<<JTD);
	MCUCSR |= (1<<JTD);
	
	DDRA |= 0xFF;
	DDRB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2) | (1<<PINB3);
	DDRC |= 0xFF;
	DDRD |= 0b11110011;
	
	PORTA = 0; 
	PORTB &=~ (1<<PINB0);
	PORTB &=~ (1<<PINB1);
	PORTB &=~ (1<<PINB2);
	PORTB &=~ (1<<PINB3);
	PORTC = 0;
	PORTD |= 0b00001100;
	
	char a = 0;
		
//  	GICR |= (1<<INT1);
//  	MCUCR |=  (1<<ISC10);//|(1<<ISC11);//// //
// 	
 	sei();	
	
	initialise_PWM();
	PWM1();
	PWM2();
	
	spi_init_slave();

// 	while(100)
// 	{ 
// 			PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
// 			_delay_ms(1000);
// 			PORTB &= ~(1<<PINB0);
// 			PORTB &= ~(1<<PINB1);
// 			PORTB &= ~(1<<PINB2);
// 			_delay_ms(1000);
// 	}
	
	while(3)
	{
		spi_tranceiver(0);
		packet = SPDR;
						
		library();
	}	
		//TODO:: Please write your application code  
}	

void initialise_PWM(void)
{
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM12) |(1<<WGM13) |(1<<CS10);
	ICR1=19999; //top value
}
void PWM1(void)
{
	TCCR1A |= (1<<COM1A1);
	OCR1A = 19990; ///////put the ocr value
}
void PWM2(void)
{
	TCCR1A |= (1<<COM1B1);
	OCR1B = 19990; ///////put the ocr value
}

void sw_debounce_pressed(void)
{
	//code
	unsigned int pressed_confidence_level=0;
	pressed_confidence_level++;
	if(pressed_confidence_level>=500 )//put a debouncing value///////////////
	{
		pressed_confidence_level=0;
	}
}
void sw_debounce_released(void)
{
	unsigned int released_confidence_level=0;
	released_confidence_level++;

	if(released_confidence_level>=500 )//put a debouncing value//////////////
	{
		released_confidence_level=0;
	}
}

void KILL_PWM1(void)
{
	TCCR1A &=~ (1<<COM1A1);
	OCR1A = 0;
}
void KILL_PWM2(void)
{
	TCCR1A &=~ (1<<COM1B1);
	OCR1B = 0;
}

void initialise_counter(void)
{
	TCCR0 |= (1<<CS01)|(1<<CS00);
	TIMSK |= (1<<TOIE0);
	TCNT0 = 0;
}
void stop_counter(void)
{
	TCCR0 &=~ (1<<CS00);
	TCCR0 &=~ (1<<CS01);
	TIMSK &=~ (1<<TOIE0);
	_delay_ms(1);
	TCNT0=0;
}

void resetParameters(void)
{
	count=0;  //for interrupt generation
	count1=0; //10 sec delay initializer
	FAIL = 0;

	p = 0;
	kick = 0;
	STOCK = 0;
	
	coin_10RS = 0;
	RS10_coin_no = 0;
	coin_count = 0;

	coin_05RS = 0;
	RS05_coin_no = 0;

	money = 0;

	pressed=0;
	pressed_1=0;
	pressed_2=0;

	
	
	refresh = 0;
	refresh2 = 0;
	
	stop_counter();
	PORTC = 0;
}

void motor(int a,bool b)
{
	if (b == ON)
	{
		switch (a)
		{
			case 1:
			PORTA |= (1<<PINA0);
			PORTA &=~(1<<PINA1);
			break;
			
			case 2:
			PORTA |= (1<<PINA2);
			PORTA &=~(1<<PINA3);
			break;
			
			case 3:
			PORTA |= (1<<PINA4);
			PORTA &=~(1<<PINA5);
			break;
			
			case 4:
			PORTA |= (1<<PINA6);
			PORTA &=~(1<<PINA7);
			break;
			
			case 5:
			PORTC |= (1<<PINC0);
			PORTC &=~(1<<PINC1);
			break;
			
			case 6:
			PORTC |= (1<<PINC2);
			PORTC &=~(1<<PINC3);
			break;
			
			case 7:
			PORTC |= (1<<PINC4);
			PORTC &=~(1<<PINC5);
			break;
			
			case 8:
			PORTC |= (1<<PINC6);
			PORTC &=~(1<<PINC7);
			break;
			
			case 9:
			PORTD |= (1<<PIND0);
			PORTD &=~(1<<PIND1);
			break;
			
			case 10:
			PORTD |= (1<<PIND6);
			PORTD &=~(1<<PIND7);
			break;
		}

	}
	
	else
	if (b == OFF)
	{
		switch (a)
		{
			case 1:
			PORTA &=~(1<<PINA0);
			PORTA &=~(1<<PINA1);
			break;
			
			case 2:
			PORTA &=~(1<<PINA2);
			PORTA &=~(1<<PINA3);
			break;
			
			case 3:
			PORTA &=~(1<<PINA4);
			PORTA &=~(1<<PINA5);
			break;
			
			case 4:
			PORTA &=~(1<<PINA6);
			PORTA &=~(1<<PINA7);
			break;
			
			case 5:
			PORTC &=~(1<<PINC0);
			PORTC &=~(1<<PINC1);
			break;
			
			case 6:
			PORTC &=~(1<<PINC2);
			PORTC &=~(1<<PINC3);
			break;
			
			case 7:
			PORTC &=~(1<<PINC4);
			PORTC &=~(1<<PINC5);
			break;
			
			case 8:
			PORTC &=~(1<<PINC6);
			PORTC &=~(1<<PINC7);
			break;
			
			case 9:
			PORTD &=~(1<<PIND0);
			PORTD &=~(1<<PIND1);
			break;
			
			case 10:
			PORTD &=~(1<<PIND6);
			PORTD &=~(1<<PIND7);
			break;
		}
	}
}

// Initialize SPI Master Device (with SPI interrupt)
void spi_init_master_INTERRUPT (void)
{
	// Set MOSI, SCK as Output
	
	DDRB |= (1<<PINB5)|(1<<PINB7);
	
	DDRB &=~ (1<<PINB6);
	PORTB |= (1<<PINB6);
	// Enable SPI, Set as Master
	// Prescaler: Fosc/16, Enable Interrupts
	//The MOSI, SCK pins are as per ATMega8
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPIE);
	
	// Enable Global Interrupts
	//sei();
}
// Initialize SPI Master Device (without interrupt)
void spi_init_master (void)
{
	// Set MOSI, SCK as Output
	DDRB |= (1<<PINB5)|(1<<PINB7);
	
	DDRB &=~ (1<<PINB6);
	PORTB |= (1<<PINB6);
	// Enable SPI, Set as Master
	//Prescaler: Fosc/16, Enable Interrupts
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}
// Initialize SPI Slave Device
void spi_init_slave (void)
{
 	DDRB &=~ (1<<PINB6);     //MISO as OUTPUT
 	PORTB |= (1<<PINB6);     // but here in input
	
	DDRB &=~ (1<<PINB5);	//MOSI as input
	PORTB |= (1<<PINB5);
	
	DDRB &=~ (1<<PINB4);	//SS as input
	PORTB |= (1<<PINB4);
	
	DDRB &=~ (1<<PINB7);	//SCK as input
	PORTB |= (1<<PINB7);
	
	SPCR |= (1<<SPE);		//Enable SPI
	SPCR &=~ (1<<MSTR);		// disable master control
}
//Function to send and receive data for both master and slave
char spi_tranceiver (unsigned char data)
{
	// Load data into the buffer
	SPDR = data;
	
	//Wait until transmission complete
	while(!(SPSR & (1<<SPIF) ));
	
	// Return received data
	return SPDR;
}
//kill spi
void spi_kill_master(void)
{
	// Set MOSI, SCK as Output
	DDRB |= (1<<PINB5)|(1<<PINB7);
	
	DDRB &=~ (1<<PINB6);
	PORTB |= (1<<PINB6);
	
	SPCR &=~ (1<<SPE);
	SPCR &=~ (1<<MSTR);
	SPCR &=~ (1<<SPR0);
}
void spi_kill_slave(void)
{
	DDRB |= (1<<PINB6);     //MISO as OUTPUT
		
	DDRB &=~ (1<<PINB5);
	PORTB |= (1<<PINB5);
		
	DDRB &=~ (1<<PINB7);
	PORTB |= (1<<PINB7);
		
	SPCR &=~ (1<<SPE);   //disable SPI
	SPCR &=~ (1<<MSTR);
}

void recieve_packets(void)
{
	
	spi_tranceiver('a');
	packet = SPDR;
	
	switch (packet)
	{
		case 'W':
		spi_tranceiver('W');
		packet1 = SPDR;
		break;
		//first packet is to be received
		
		case 'X':
		spi_tranceiver('X');
		packet2 = SPDR;
		break;
		//second packet is to be received
		
		case 'Y':
		spi_tranceiver('y');
		packet3 = SPDR;
		break;
		///third packet is to be received
		
		case 'Z':
		spi_tranceiver('Z');
		packet4 = SPDR;
		break;
		//fourth packet is to be received
	}
	
}

void library (void)
{
	switch (packet)	
	{		
		case 255:
		packet = 0;
		break;
		
		case 'A':
		PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
		_delay_ms(500);
		PORTB &= ~(1<<PINB0);
		PORTB &= ~(1<<PINB1);
		PORTB &= ~(1<<PINB2);
		packet = 0;
		motor(1 ,ON);
		b=1;
		break;
		
		case 'B':
		PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
		_delay_ms(500);
		PORTB &= ~(1<<PINB0);
		PORTB &= ~(1<<PINB1);
		PORTB &= ~(1<<PINB2);
		packet = 0;
		motor(2 ,ON);
		b=1;
		break;
		
		case 'C':
		PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
		_delay_ms(500);
		PORTB &= ~(1<<PINB0);
		PORTB &= ~(1<<PINB1);
		PORTB &= ~(1<<PINB2);
		packet = 0;
		motor(3 ,ON);
		b=1;
		break;
		
		case 'D':
		PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
		_delay_ms(500);
		PORTB &= ~(1<<PINB0);
		PORTB &= ~(1<<PINB1);
		PORTB &= ~(1<<PINB2);
		packet = 0;
		motor(4 ,ON);
		b=1;
		break;
		
		case 'E':
		PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
		_delay_ms(500);
		PORTB &= ~(1<<PINB0);
		PORTB &= ~(1<<PINB1);
		PORTB &= ~(1<<PINB2);
		packet = 0;
		motor(5 ,ON);
		b=1;
		break;
		
		case 'F':
		PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
		_delay_ms(500);
		PORTB &= ~(1<<PINB0);
		PORTB &= ~(1<<PINB1);
		PORTB &= ~(1<<PINB2);
		packet = 0;
		motor(6 ,ON);
		b=1;
		break;
		
		case 'G':
		PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
		_delay_ms(500);
		PORTB &= ~(1<<PINB0);
		PORTB &= ~(1<<PINB1);
		PORTB &= ~(1<<PINB2);
		motor(7 ,ON);
		b=1;
		break;
		
		case 'H':
		PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
		_delay_ms(500);
		PORTB &= ~(1<<PINB0);
		PORTB &= ~(1<<PINB1);
		PORTB &= ~(1<<PINB2);
		packet = 0;
		motor(8 ,ON);
		b=1;
		break;
		
		case 'I':
		PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
		_delay_ms(500);
		PORTB &= ~(1<<PINB0);
		PORTB &= ~(1<<PINB1);
		PORTB &= ~(1<<PINB2);
		packet = 0;
		motor(9 ,ON);
		b=1;
		break;
		
		case 'J':
		PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
		_delay_ms(500);
		PORTB &= ~(1<<PINB0);
		PORTB &= ~(1<<PINB1);
		PORTB &= ~(1<<PINB2);
		packet = 0;
		motor(10 ,ON);
		b=1;
		break;
				
		case 'Y':///////////
		packet = 0;
		
		PORTB ^= (1<<PINB0);
		PORTB ^= (1<<PINB1);
		PORTB ^= (1<<PINB2);
		
		motor(1,OFF);
		motor(2,OFF);
		motor(3,OFF);
		motor(4,OFF);
		motor(5,OFF);
		motor(6,OFF);
		motor(7,OFF);
		motor(8,OFF);
		motor(9,OFF);
		motor(10,OFF);
		break;
	}
}

// ISR(INT1_vect)
// {
// 	if (b==1)
// 	{		
// 		u++;
// 		if (u>=2)
// 		{
// 			b=0;
// 			u=0;
// 			PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
// 			_delay_ms(500);
// 			PORTB &= ~(1<<PINB0);
// 			PORTB &= ~(1<<PINB1);
// 			PORTB &= ~(1<<PINB2);
// 			
// 			motor(1,OFF);
// 			motor(2,OFF);
// 			motor(3,OFF);
// 			motor(4,OFF);
// 			motor(5,OFF);
// 			motor(6,OFF);
// 			motor(7,OFF);
// 			motor(8,OFF);
// 			motor(9,OFF);
// 			motor(10,OFF);
// 		}
// 	}
// 	
// 		
//  }	
	//b = 1;
	
	//spi_init_slave();
// 	

// 	
// 	//spi_tranceiver('a');


// ISR(TIMER0_OVF_vect)
// {
// 	count++;
// 	if (count==61)
// 	{
// 		// 1 second has elapsed
// 		count=0;
// 		
// 		count1++;
// 		
// 		PORTC ^= (1<<PINC3);
// 		
// 		if (count1==20)/////// calibrate it further
// 		{
// 			//return coin if not dispatched
// 			count1=0;
// 			FAIL=1;
// 		}
// 	}
// }


/*
case 'a':
packet = 0;
motor(1 ,OFF);
break;

case 'b':
packet = 0;
motor(2 ,OFF);
break;

case 'c':
packet = 0;
motor(3 ,OFF);
break;

case 'd':
packet = 0;
motor(4 ,OFF);
break;

case 'e':
packet = 0;
motor(5 ,OFF);
break;

case 'f':
packet = 0;
motor(6 ,OFF);
break;

case 'g':
packet = 0;
motor(7 ,OFF);
break;

case 'h':
packet = 0;
motor(8 ,OFF);
break;

case 'i':
packet = 0;
motor(9 ,OFF);
break;

case 'j':
packet = 0;
motor(10 ,OFF);
break;
*/

//void recieve_instruction () // motor motorID start/stop  






	// 	if (packet == 'a')
	// 	{
	// 		PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
	// 		_delay_ms(50);
	// 		PORTB &=~ (1<<PINB0);
	// 		PORTB &=~ (1<<PINB1);
	// 		PORTB &=~ (1<<PINB2);
	// 		_delay_ms(50);
	// 		
	// 		PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
	// 		_delay_ms(50);
	// 		PORTB &=~ (1<<PINB0);
	// 		PORTB &=~ (1<<PINB1);
	// 		PORTB &=~ (1<<PINB2);
	// 		_delay_ms(50);
	// 		
	// 		PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
	// 		_delay_ms(50);
	// 		PORTB &=~ (1<<PINB0);
	// 		PORTB &=~ (1<<PINB1);
	// 		PORTB &=~ (1<<PINB2);
	// 		_delay_ms(50);
	// 	}
	// 	else
	// 	if (packet == 'b')
	// 	{
	// 		PORTB |= (1<<PINB0);
	// 		_delay_ms(100);
	// 		PORTB |= (1<<PINB1);
	// 		_delay_ms(100);
	// 		PORTB |= (1<<PINB2);
	// 		_delay_ms(100);
	// 		PORTB &=~ (1<<PINB0);
	// 		PORTB &=~ (1<<PINB1);
	// 		PORTB &=~ (1<<PINB2);
	// 		_delay_ms(50);
	// 	}
	// 	
	// 	spi_tranceiver('y');
	// 	packet = SPDR;
	// 	
	// 	if (packet == 'a')
	// 	{
	// 		PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
	// 		_delay_ms(50);
	// 		PORTB &=~ (1<<PINB0);
	// 		PORTB &=~ (1<<PINB1);
	// 		PORTB &=~ (1<<PINB2);
	// 		_delay_ms(50);
	// 		
	// 		PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
	// 		_delay_ms(50);
	// 		PORTB &=~ (1<<PINB0);
	// 		PORTB &=~ (1<<PINB1);
	// 		PORTB &=~ (1<<PINB2);
	// 		_delay_ms(50);
	// 		
	// 		PORTB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB2);
	// 		_delay_ms(50);
	// 		PORTB &=~ (1<<PINB0);
	// 		PORTB &=~ (1<<PINB1);
	// 		PORTB &=~ (1<<PINB2);
	// 		_delay_ms(50);
	// 	}
	// 	else
	// 	if (packet == 'b')
	// 	{
	// 		PORTB |= (1<<PINB0);
	// 		_delay_ms(100);
	// 		PORTB |= (1<<PINB1);
	// 		_delay_ms(100);
	// 		PORTB |= (1<<PINB2);
	// 		_delay_ms(100);
	// 		PORTB &=~ (1<<PINB0);
	// 		PORTB &=~ (1<<PINB1);
	// 		PORTB &=~ (1<<PINB2);
	// 		_delay_ms(50);
	// 	}	 		