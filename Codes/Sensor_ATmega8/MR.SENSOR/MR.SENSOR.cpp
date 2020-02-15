/*
 * MR.cpp
 *
 * Created: 30-Sep-17 1:24:49 AM
 *  Author: RHITVIK
 */ 

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

#define F_CPU 1000000UL

#define UNLOCKED false
#define LOCKED  true

#define OBJECT_not_DETECTED false
#define OBJECT_DETECTED true

#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<string.h> 

void DROP_SENSOR_FUNCTION (unsigned char DSF_parameter);
void RESTORE_INITIAL_GRID_STATUS (void);
void OPEN_SESEME (void);
void DEBUG(void);
void DEBUG2(void);

unsigned int sensor_no = 0;
unsigned int sensor_link_group_no = 0;

bool sensor_link_group_1 = OBJECT_not_DETECTED;
bool sensor_link_group_2 = OBJECT_not_DETECTED;
bool sensor_link_group_3 = OBJECT_not_DETECTED;
bool sensor_link_group_4 = OBJECT_not_DETECTED;

bool loop_lock_status = UNLOCKED; 

int main(void)
{
	DDRC  |= 0b111111;
	PORTC |= 0b111111;
	
	DDRB  |= 0x00;
	PORTB |= 0xFF;
	
	DDRD  |= 0x00;
	PORTD |= 0xFF;	
	
	RESTORE_INITIAL_GRID_STATUS();
	DEBUG2();
	
	while(32)
	{
		if (bit_is_clear (PINB,0))
		{
			sensor_link_group_no = 1;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PINB,1))
		{
			sensor_link_group_no = 1;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		
		else
		if (bit_is_clear (PINB,2))
		{
			sensor_link_group_no = 1;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PINB,3))
		{
			sensor_link_group_no = 1;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		
		else
		if (bit_is_clear (PINB,4))
		{
			sensor_link_group_no = 2;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PINB,5))
		{
			sensor_link_group_no = 2;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		
		else
		if (bit_is_clear (PINB,6))
		{
			sensor_link_group_no = 2;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PINB,7))
		{
			sensor_link_group_no = 2;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		
		else
		
		if (bit_is_clear (PIND,0))
		{
			sensor_link_group_no = 3;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PIND,1))
		{
			sensor_link_group_no = 3;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		
		else
		if (bit_is_clear (PIND,2))
		{
			sensor_link_group_no = 3;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PIND,3))
		{
			sensor_link_group_no = 3;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		
		else
		if (bit_is_clear (PIND,4))
		{
			sensor_link_group_no = 4;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PIND,5))
		{
			sensor_link_group_no = 4;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		
		else
		if (bit_is_clear (PIND,6))
		{
			sensor_link_group_no = 4;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PIND,7))
		{
			sensor_link_group_no = 4;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		
		else
		if (bit_is_set (PINB,0) && bit_is_set (PINB,1) && bit_is_set (PINB,2) && bit_is_set (PINB,3) && bit_is_set (PINB,4) && bit_is_set (PINB,5) && bit_is_set (PINB,6) && bit_is_set (PINB,7) && bit_is_set (PIND,0) && bit_is_set (PIND,1) && bit_is_set (PIND,2) && bit_is_set (PIND,3) && bit_is_set (PIND,4) && bit_is_set (PIND,5) && bit_is_set (PIND,6) && bit_is_set (PIND,7))
		{
			RESTORE_INITIAL_GRID_STATUS();
		}
	}
	
	while(33)
	{
		if (bit_is_clear (PINB,0))
		{
			sensor_link_group_no = 1;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PINB,1))
		{
			sensor_link_group_no = 1;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PINB,2))
		{
			sensor_link_group_no = 2;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PINB,3))
		{
			sensor_link_group_no = 2;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PINB,4))
		{
			sensor_link_group_no = 3;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PINB,5))
		{
			sensor_link_group_no = 3;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PINB,6))
		{
			sensor_link_group_no = 4;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PINB,7))
		{
			sensor_link_group_no = 4;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		
		else
		if (bit_is_set (PINB,0) && bit_is_set (PINB,1) && bit_is_set (PINB,2) && bit_is_set (PINB,3) && bit_is_set (PINB,4) && bit_is_set (PINB,5) && bit_is_set (PINB,6) && bit_is_set (PINB,7))
		{
			RESTORE_INITIAL_GRID_STATUS();
		}
	}
	
	while(1)
	{
		if (bit_is_clear (PIND,0))
		{
			sensor_link_group_no = 1;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PIND,1))
		{
			sensor_link_group_no = 1;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PIND,2))
		{
			sensor_link_group_no = 2;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PIND,3))
		{
			sensor_link_group_no = 2;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PIND,4))
		{
			sensor_link_group_no = 3;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PIND,5))
		{
			sensor_link_group_no = 3;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PIND,6))
		{
			sensor_link_group_no = 4;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_clear (PIND,7))
		{
			sensor_link_group_no = 4;
			sensor_link_group_1 = OBJECT_DETECTED;
			DROP_SENSOR_FUNCTION(sensor_link_group_no);
		}
		else
		if (bit_is_set (PIND,0) && bit_is_set (PIND,1) && bit_is_set (PIND,2) && bit_is_set (PIND,3) && bit_is_set (PIND,4) && bit_is_set (PIND,5) && bit_is_set (PIND,6) && bit_is_set (PIND,7))
		{
			RESTORE_INITIAL_GRID_STATUS();
		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
 
}	

void DROP_SENSOR_FUNCTION (unsigned char DSF_parameter)
{
	switch (DSF_parameter)
	{		
		case 1:
		PORTC &=~ (1<<PINC0);
		
// 		PORTC &=~ (1<<PINC4);
// 		PORTC &=~ (1<<PINC5);
		_delay_ms(50);
		break;
		
		case 2:
		PORTC &=~ (1<<PINC1);
		
// 		PORTC &=~ (1<<PINC4);
// 		PORTC &=~ (1<<PINC5);
		_delay_ms(50);
		break;
		
		case 3:
		PORTC &=~ (1<<PINC2);
		
// 		PORTC &=~ (1<<PINC4);
// 		PORTC &=~ (1<<PINC5);
		_delay_ms(50);
		break;
		
		case 4:
		PORTC &=~ (1<<PINC3);
	
// 		PORTC &=~ (1<<PINC4);
// 		PORTC &=~ (1<<PINC5);
		_delay_ms(50);
		break;	
		
		case 5:
		RESTORE_INITIAL_GRID_STATUS();
		_delay_ms(50);
		break;
	}
}

void RESTORE_INITIAL_GRID_STATUS (void)
{
	PORTC |= 0b111111;
}

void OPEN_SESEME (void)
{
	if (loop_lock_status == LOCKED)
	{
		loop_lock_status = UNLOCKED;
	}
}

void DEBUG (void)
{
	PORTC &=~ (1<<PINC4);
	PORTC &=~ (1<<PINC5);
	_delay_ms(500);
	PORTC |= (1<<PINC4);
	PORTC |= (1<<PINC5);
	_delay_ms(500);
}

void DEBUG2 (void)
{
	PORTC &=~ (1<<PINC4);
	PORTC &=~ (1<<PINC5);
	_delay_ms(50);
	PORTC |= (1<<PINC4);
	PORTC |= (1<<PINC5);
	_delay_ms(50);
	PORTC &=~ (1<<PINC4);
	PORTC &=~ (1<<PINC5);
	_delay_ms(50);
	PORTC |= (1<<PINC4);
	PORTC |= (1<<PINC5);
	_delay_ms(50);
	PORTC &=~ (1<<PINC4);
	PORTC &=~ (1<<PINC5);
	_delay_ms(50);
	PORTC |= (1<<PINC4);
	PORTC |= (1<<PINC5);
	_delay_ms(50);
	PORTC &=~ (1<<PINC4);
	PORTC &=~ (1<<PINC5);
	_delay_ms(50);
	PORTC |= (1<<PINC4);
	PORTC |= (1<<PINC5);
	_delay_ms(50);
	PORTC &=~ (1<<PINC4);
	PORTC &=~ (1<<PINC5);
	_delay_ms(50);
	PORTC |= (1<<PINC4);
	PORTC |= (1<<PINC5);
	_delay_ms(50);
	PORTC &=~ (1<<PINC4);
	PORTC &=~ (1<<PINC5);
	_delay_ms(50);
	PORTC |= (1<<PINC4);
	PORTC |= (1<<PINC5);
	_delay_ms(50);
}
// 
// 
// if ((bit_is_set(PINB,0)) || (bit_is_set(PINB,1)) || (bit_is_set(PINB,2)) || (bit_is_set(PINB,3)))
// {
// 	if (loop_lock_status == UNLOCKED)
// 	{
// 		loop_lock_status = LOCKED;
// 		sensor_link_group_no = 1;
// 		sensor_link_group_1 = OBJECT_DETECTED;
// 		DROP_SENSOR_FUNCTION(sensor_link_group_no);
// 	}
// }
// 
// else
// if ((bit_is_set(PINB,4)) || (bit_is_set(PINB,5)) || (bit_is_set(PINB,6)) || (bit_is_set(PINB,7)))
// {
// 	if (loop_lock_status == UNLOCKED)
// 	{
// 		loop_lock_status = LOCKED;
// 		sensor_link_group_no = 2;
// 		sensor_link_group_2 = OBJECT_DETECTED;
// 		DROP_SENSOR_FUNCTION(sensor_link_group_no);
// 	}
// }
// 
// else
// if ((bit_is_set(PIND,0)) || (bit_is_set(PIND,1)) || (bit_is_set(PIND,2)) || (bit_is_set(PIND,3)))
// {
// 	if (loop_lock_status == UNLOCKED)
// 	{
// 		loop_lock_status = LOCKED;
// 		sensor_link_group_no = 3;
// 		sensor_link_group_3 = OBJECT_DETECTED;
// 		DROP_SENSOR_FUNCTION(sensor_link_group_no);
// 	}
// }
// 
// else
// if ((bit_is_set(PIND,4)) || (bit_is_set(PIND,5)) || (bit_is_set(PIND,6)) || (bit_is_set(PIND,7)))
// {
// 	if (loop_lock_status == UNLOCKED)
// 	{
// 		loop_lock_status = LOCKED;
// 		sensor_link_group_no = 4;
// 		sensor_link_group_4 = OBJECT_DETECTED;
// 		DROP_SENSOR_FUNCTION(sensor_link_group_no);
// 	}
// }
// //////////////////////////////////////////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////////////////////////////////////////////
// else
// if ((bit_is_clear(PINB,0)) && (bit_is_clear(PINB,1)) && (bit_is_clear(PINB,2)) && (bit_is_clear(PINB,3)))
// {
// 	if (sensor_link_group_1 == OBJECT_DETECTED)
// 	{
// 		sensor_link_group_1 = OBJECT_not_DETECTED;
// 		OPEN_SESEME();
// 	}
// }
// 
// else
// if ((bit_is_clear(PINB,4)) && (bit_is_clear(PINB,5)) && (bit_is_clear(PINB,6)) && (bit_is_clear(PINB,7)))
// {
// 	if (sensor_link_group_2 == OBJECT_DETECTED)
// 	{
// 		sensor_link_group_2 = OBJECT_not_DETECTED;
// 		OPEN_SESEME();
// 	}
// }
// 
// else
// if ((bit_is_clear(PIND,0)) && (bit_is_clear(PIND,1)) && (bit_is_clear(PIND,2)) && (bit_is_clear(PIND,3)))
// {
// 	if (sensor_link_group_3 == OBJECT_DETECTED)
// 	{
// 		sensor_link_group_3 = OBJECT_not_DETECTED;
// 		OPEN_SESEME();
// 	}
// }
// 
// else
// if ((bit_is_clear(PIND,4)) && (bit_is_clear(PIND,5)) && (bit_is_clear(PIND,6)) && (bit_is_clear(PIND,7)))
// {
// 	if (sensor_link_group_4 == OBJECT_DETECTED)
// 	{
// 		sensor_link_group_4 = OBJECT_not_DETECTED;
// 		OPEN_SESEME();
// 	}
// }
// ///////////////////////////////////////////////
// ///////////////////////////////////////////////
// else
// if ((sensor_link_group_1 == OBJECT_not_DETECTED) && (sensor_link_group_2 == OBJECT_not_DETECTED) && (sensor_link_group_3 == OBJECT_not_DETECTED) && (sensor_link_group_4 == OBJECT_not_DETECTED))
// {
// 	if (loop_lock_status == UNLOCKED)
// 	{
// 		PORTC &=~ (1<<PINC4);
// 		PORTC &=~ (1<<PINC5);
// 		_delay_ms(500);
// 		PORTC |= (1<<PINC4);
// 		PORTC |= (1<<PINC5);
// 		
// 		loop_lock_status = UNLOCKED;
// 		RESTORE_INITIAL_GRID_STATUS();
// 		sensor_link_group_no = 0;
// 	}
// }
// 
// 	while(32)
// 	{
// 		if (bit_is_clear (PINB,0))
// 		{
// 			
// 		}
// 		
// 		if (bit_is_clear (PINB,1))
// 		{
// 			
// 		}
// 		
// 		if (bit_is_clear (PINB,2))
// 		{
// 			
// 		}
// 		
// 		if (bit_is_clear (PINB,3))
// 		{
// 			
// 		}
// 		
// 		else
// 		if (bit_is_clear (PINB,4))
// 		{
// 			
// 		}
// 		
// 		if (bit_is_clear (PINB,5))
// 		{
// 			
// 		}
// 		
// 		else
// 		if (bit_is_clear (PINB,6))
// 		{
// 			
// 		}
// 		
// 		if (bit_is_clear (PINB,7))
// 		{
// 			
// 		}
// 		
// 		else
// 		if (bit_is_set (PINB,0))
// 		{
// 			
// 		}
// 		
// 		else
// 		if (bit_is_set (PINB,1))
// 		{
// 			
// 		}
// 		
// 		if (bit_is_clear (PINB,2))
// 		{
// 			
// 		}
// 		
// 		else
// 		if (bit_is_set (PINB,3))
// 		{
// 			
// 		}
// 		
// 		if (bit_is_set (PINB,4))
// 		{
// 			
// 		}
// 		
// 		else
// 		if (bit_is_set (PINB,5))
// 		{
// 			
// 		}
// 		
// 		else
// 		if (bit_is_set (PINB,6))
// 		{
// 			
// 		}
// 		
// 		else
// 		if (bit_is_set (PINB,7))
// 		{
// 			
// 		}
// 	}

// 

	
	
	
	
	
	
	
	
	
	
	
	
	
// while(1)
// {
// 	if ((bit_is_clear(PINB,0)) || (bit_is_clear(PINB,1)) || (bit_is_clear(PINB,2)) || (bit_is_clear(PINB,3)))
// 	{
// 		//DEBUG();
// 		if (loop_lock_status == UNLOCKED)
// 		{
// 			loop_lock_status = LOCKED;
// 			sensor_link_group_no = 1;
// 			sensor_link_group_1 = OBJECT_DETECTED;
// 			DROP_SENSOR_FUNCTION(sensor_link_group_no);
// 		}
// 	}
// 	//else
// 	if ((bit_is_clear(PINB,4)) || (bit_is_clear(PINB,5)) || (bit_is_clear(PINB,6)) || (bit_is_clear(PINB,7)))
// 	{
// 		if (loop_lock_status == UNLOCKED)
// 		{
// 			loop_lock_status = LOCKED;
// 			sensor_link_group_no = 2;
// 			sensor_link_group_2 = OBJECT_DETECTED;
// 			DROP_SENSOR_FUNCTION(sensor_link_group_no);
// 		}
// 	}
// 		
// 	//else
// 	if ((bit_is_clear(PIND,0)) || (bit_is_clear(PIND,1)) || (bit_is_clear(PIND,2)) || (bit_is_clear(PIND,3)))
// 	{
// 		if (loop_lock_status == UNLOCKED)
// 		{
// 			loop_lock_status = LOCKED;
// 			sensor_link_group_no = 3;
// 			sensor_link_group_3 = OBJECT_DETECTED;
// 			DROP_SENSOR_FUNCTION(sensor_link_group_no);
// 		}
// 	}
// 
// 	//else
// 	if ((bit_is_clear(PIND,4)) || (bit_is_clear(PIND,5)) || (bit_is_clear(PIND,6)) || (bit_is_clear(PIND,7)))
// 	{
// 		if (loop_lock_status == UNLOCKED)
// 		{
// 			loop_lock_status = LOCKED;
// 			sensor_link_group_no = 4;
// 			sensor_link_group_4 = OBJECT_DETECTED;
// 			DROP_SENSOR_FUNCTION(sensor_link_group_no);
// 		}
// 	}
// 		
// 	//else
// 	if (sensor_link_group_1 == OBJECT_DETECTED)
// 	{
// 		DEBUG();
// 		if ((bit_is_set(PINB,0)) && (bit_is_set(PINB,1)) && (bit_is_set(PINB,2)) && (bit_is_set(PINB,3)))
// 		{
// 			sensor_link_group_1 = OBJECT_not_DETECTED;
// 			OPEN_SESEME();
// 		}
// 	}
// 		
// 	// else
// 	if (sensor_link_group_2 == OBJECT_DETECTED)
// 	{
// 		DEBUG2();
// 		if ((bit_is_set(PINB,4)) && (bit_is_set(PINB,5)) && (bit_is_set(PINB,6)) && (bit_is_set(PINB,7)))
// 		{
// 			sensor_link_group_2 = OBJECT_not_DETECTED;
// 			OPEN_SESEME();
// 		}
// 	}
// 		
// 	//else
// 	if (sensor_link_group_3 == OBJECT_DETECTED)
// 	{
// 		if ((bit_is_set(PIND,0)) && (bit_is_set(PIND,1)) && (bit_is_set(PIND,2)) && (bit_is_set(PIND,3)))
// 		{
// 			sensor_link_group_3 = OBJECT_not_DETECTED;
// 			OPEN_SESEME();
// 		}
// 	}
// 		
// 	//else
// 	if (sensor_link_group_4 == OBJECT_DETECTED)
// 	{
// 		if ((bit_is_set(PIND,4)) && (bit_is_set(PIND,5)) && (bit_is_set(PIND,6)) && (bit_is_set(PIND,7)))
// 		{
// 			sensor_link_group_4 = OBJECT_not_DETECTED;
// 			OPEN_SESEME();
// 		}
// 	}
// 		
// 	//else
// 	if ((sensor_link_group_1 == OBJECT_not_DETECTED) && (sensor_link_group_2 == OBJECT_not_DETECTED) && (sensor_link_group_3 == OBJECT_not_DETECTED) && (sensor_link_group_4 == OBJECT_not_DETECTED))
// 	{
// 		if (loop_lock_status == UNLOCKED)
// 		{
// 			loop_lock_status = UNLOCKED;
// 			RESTORE_INITIAL_GRID_STATUS();
// 			sensor_link_group_no = 0;
// 		}
// 	}
// }

///////////////////////
////////////////////
////
//
////////////////
// while(1)
// {
// 	if ((bit_is_set(PINB,0)) || (bit_is_set(PINB,1)) || (bit_is_set(PINB,2)) || (bit_is_set(PINB,3)))
// 	{
// 		//DEBUG();
// 		if (loop_lock_status == UNLOCKED)
// 		{
// 			loop_lock_status = LOCKED;
// 			sensor_link_group_no = 1;
// 			sensor_link_group_1 = OBJECT_DETECTED;
// 			DROP_SENSOR_FUNCTION(sensor_link_group_no);
// 		}
// 	}
// 	//else
// 	if ((bit_is_set(PINB,4)) || (bit_is_set(PINB,5)) || (bit_is_set(PINB,6)) || (bit_is_set(PINB,7)))
// 	{
// 		if (loop_lock_status == UNLOCKED)
// 		{
// 			loop_lock_status = LOCKED;
// 			sensor_link_group_no = 2;
// 			sensor_link_group_2 = OBJECT_DETECTED;
// 			DROP_SENSOR_FUNCTION(sensor_link_group_no);
// 		}
// 	}
// 	
// 	//else
// 	if ((bit_is_set(PIND,0)) || (bit_is_set(PIND,1)) || (bit_is_set(PIND,2)) || (bit_is_set(PIND,3)))
// 	{
// 		if (loop_lock_status == UNLOCKED)
// 		{
// 			loop_lock_status = LOCKED;
// 			sensor_link_group_no = 3;
// 			sensor_link_group_3 = OBJECT_DETECTED;
// 			DROP_SENSOR_FUNCTION(sensor_link_group_no);
// 		}
// 	}
// 
// 	//else
// 	if ((bit_is_set(PIND,4)) || (bit_is_set(PIND,5)) || (bit_is_set(PIND,6)) || (bit_is_set(PIND,7)))
// 	{
// 		if (loop_lock_status == UNLOCKED)
// 		{
// 			loop_lock_status = LOCKED;
// 			sensor_link_group_no = 4;
// 			sensor_link_group_4 = OBJECT_DETECTED;
// 			DROP_SENSOR_FUNCTION(sensor_link_group_no);
// 		}
// 	}
// 	
// 	//else
// 	if (sensor_link_group_1 == OBJECT_DETECTED)
// 	{
// 		DEBUG();
// 		if ((bit_is_clear(PINB,0)) && (bit_is_clear(PINB,1)) && (bit_is_clear(PINB,2)) && (bit_is_clear(PINB,3)))
// 		{
// 			sensor_link_group_1 = OBJECT_not_DETECTED;
// 			OPEN_SESEME();
// 		}
// 	}
// 	
// 	// else
// 	if (sensor_link_group_2 == OBJECT_DETECTED)
// 	{
// 		DEBUG2();
// 		if ((bit_is_clear(PINB,4)) && (bit_is_clear(PINB,5)) && (bit_is_clear(PINB,6)) && (bit_is_clear(PINB,7)))
// 		{
// 			sensor_link_group_2 = OBJECT_not_DETECTED;
// 			OPEN_SESEME();
// 		}
// 	}
// 	
// 	//else
// 	if (sensor_link_group_3 == OBJECT_DETECTED)
// 	{
// 		if ((bit_is_clear(PIND,0)) && (bit_is_clear(PIND,1)) && (bit_is_clear(PIND,2)) && (bit_is_clear(PIND,3)))
// 		{
// 			sensor_link_group_3 = OBJECT_not_DETECTED;
// 			OPEN_SESEME();
// 		}
// 	}
// 	
// 	//else
// 	if (sensor_link_group_4 == OBJECT_DETECTED)
// 	{
// 		if ((bit_is_clear(PIND,4)) && (bit_is_clear(PIND,5)) && (bit_is_clear(PIND,6)) && (bit_is_clear(PIND,7)))
// 		{
// 			sensor_link_group_4 = OBJECT_not_DETECTED;
// 			OPEN_SESEME();
// 		}
// 	}
// 	
// 	//else
// 	if ((sensor_link_group_1 == OBJECT_not_DETECTED) && (sensor_link_group_2 == OBJECT_not_DETECTED) && (sensor_link_group_3 == OBJECT_not_DETECTED) && (sensor_link_group_4 == OBJECT_not_DETECTED))
// 	{
// 		if (loop_lock_status == UNLOCKED)
// 		{
// 			loop_lock_status = UNLOCKED;
// 			RESTORE_INITIAL_GRID_STATUS();
// 			sensor_link_group_no = 0;
// 		}
// 	}
// }	
//////////////////
//////////////////