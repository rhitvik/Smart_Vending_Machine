/*
 * _10_motors_master_control.cpp
 *
 * Created: 15-Nov-16 5:06:33 PM
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



#define F_CPU 16000000UL///change to release
//remove vaildate otp function from line 251
//remove vaildate otp function from line 251
//remove vaildate otp function from line 251
//remove vaildate otp function from line 251
//remove vaildate otp function from line 251
//remove vaildate otp function from line 251
//remove vaildate otp function from line 251
//#define F_CPU 1000000UL///change to release

#define LightSwitch    0
#define ReadWrite      1
#define BiPolarMood    2

#define MrLCDsCrib            PORTD
#define DataDir_MrLCDsCrib	  DDRD

#define MrLCDsControl		           PORTB
#define MrLCDsAUXILLARYControl		   PORTC
#define DataDir_MrLCDsControl          DDRB
#define DataDir_MrLCDsAUXILLARYControl DDRC

#define OFF 0
#define ON  1

#define actuator_control 1
#define sensor_control 2
#define IO_control 3

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include <string.h>

void initialise_clock(void);
void stop_clock(void);

void talkFunc (unsigned char val);
void listenFunc (void);
void initialise_common_parameters(void);
void disable_common_parameters(void);
void initialise_listening_parameters(void);
void initialise_speaking_parameters(void);
void send_OTP_to_arduino(void);

void spi_init_master_INTERRUPT (void);// Initialize SPI Master Device (with SPI interrupt)
void spi_init_master (void);// Initialize SPI Master Device (without interrupt)
void spi_init_slave (void);// Initialize SPI Slave Device
char spi_tranceiver (unsigned int chip_select, unsigned char data);//Function to send and receive data for both master and slave
void spi_kill_master(void);
void spi_kill_slave(void);

void toggle_slave_select(unsigned int chip_number);
void initialize_vending_process (unsigned char inventory);
void vend_some(bool state, unsigned char product);
void retrieve_OTP (void);
void process_VEND_REQUEST(void);
void validate_OTP(void);

void reset_Everything(void);
// 
// void Check_IF_MrLCD_isBusy(void);
// void Peek_A_Boo(void);
// void Send_A_Command(char command);
// void Send_A_Character(char character);
// void delete_a_character(char *character);
// void Send_A_String(char *stringOfCharacters);


bool b = false;
bool talkFunc_exitParameter = false;

volatile int count = 0;
volatile int count1 = 0;
volatile int count2 = 0;
volatile bool clock_init_patameter = false;
volatile bool loadValue = true;
volatile bool readValue = false;

unsigned int otp_number = 0;

unsigned char ARD_BUS_val = 0; 

unsigned char masterDATA = 0;
unsigned char data = 0; 
bool retrieving_parameter = true;
unsigned char a = 0;

unsigned char IO_receivedDATA = 0;
unsigned char ACTUATOR_control_DATA = 0;

unsigned char OTP[5];				

int main(void)
{
	DDRB |= (1<<PINB0) | (1<<PINB1) | (1<<PINB6) | (1<<PINB7);
// 	DataDir_MrLCDsControl |= (1<<LightSwitch) | (1<<ReadWrite);
// 	DataDir_MrLCDsAUXILLARYControl |= (1<<BiPolarMood);
	
	PORTB &=~ (1<<PINB6);
	PORTB &=~ (1<<PINB7);
	
	DDRC |=  (1<<PINC2) |(1<<PINC3) | (1<<PINC4) | (1<<PINC5);
	DDRC &=~ (1<<PINC0);
	DDRC &=~ (1<<PINC1);
			
	PORTC |= (1<<PINC0) | (1<<PINC1) | (1<<PINC3) | (1<<PINC4);	
	
	DDRD  |= (1<<PIND6);	//set D6 as output
	PORTD |= (1<<PIND6);	//pull up D6
		
	DDRD &=~ (1<<PIND7);	//set D7 as input
	PORTD |= (1<<PIND7);	//pull up D7
	
	sei();
	
	_delay_ms(5);
	spi_init_master();
	_delay_ms(5);
	
// 		_delay_ms(15);
// 		Send_A_Command(0x01);//clrscr();
// 		_delay_ms(2);
// 		Send_A_Command(0x38);
// 		_delay_us(50);
// 		Send_A_Command(0b00001100);
// 		_delay_us(50);
// 		
// 		Send_A_Command(0x01);//clrscr();
// 		_delay_ms(2);
// 		Send_A_String("CHECK");
// 		_delay_ms(2);


	masterDATA = 'q';
	
// 	while (100)
// 		{		
// 			PORTC |= (1<<PINC2);
// 			PORTC |= (1<<PINC5);
// 			PORTB |= (1<<PINB0) | (1<<PINB1);
// 			_delay_ms(1000);
// 			PORTB&=~ (1<<PINB0);
// 			PORTB&=~ (1<<PINB1);
// 			PORTC&=~ (1<<PINC2);
// 			PORTC&=~ (1<<PINC5);
// 			_delay_ms(1000);
// 		}
		
	while (6)
	{
		_delay_ms(100);
		
		spi_tranceiver(IO_control,masterDATA);
		IO_receivedDATA = SPDR;
		
		//Send_A_Character( IO_receivedDATA);
		
		if (IO_receivedDATA == 12)
		{
			_delay_us(1);
		}
		
		else
		if (IO_receivedDATA == 'P')
		{
			PORTC |= (1<<PINC2);
			PORTC |= (1<<PINC5);
			
			_delay_ms(10);
			spi_tranceiver(actuator_control,255);
			_delay_ms(10);
			spi_tranceiver(actuator_control,'Y');
			_delay_ms(10);
			spi_tranceiver(IO_control,'o');
			_delay_ms(500);//change or introduce loop
			spi_tranceiver(IO_control,'o');
			_delay_ms(10);
			
			reset_Everything();
			//_delay_ms(1200);
			//trig2
			PORTD ^= (1<<PIND6);_delay_ms(20);
			PORTD ^= (1<<PIND6);_delay_ms(20);
			//trig3
			PORTD ^= (1<<PIND6);_delay_ms(20);
			PORTD ^= (1<<PIND6);_delay_ms(20);
			//trig4
			PORTD ^= (1<<PIND6);_delay_ms(20);
			PORTD ^= (1<<PIND6);_delay_ms(20);
// 			
// 			PORTD ^= (1<<PIND6);_delay_ms(20);
// 			PORTD ^= (1<<PIND6);_delay_ms(20);
			
		}

		else
		if (IO_receivedDATA == 255)
		{
			//Send_A_String("255");
			_delay_ms(100);//have to be re-calibrated
			spi_tranceiver(IO_control, 0);
			OTP[4] = SPDR;

			_delay_ms(100);//have to be re-calibrated
			spi_tranceiver(IO_control, 0);
			OTP[3] = SPDR;

			_delay_ms(100);//have to be re-calibrated
			spi_tranceiver(IO_control, 0);
			OTP[2] = SPDR;
			
			_delay_ms(100);//have to be re-calibrated
			spi_tranceiver(IO_control, 0);
			OTP[1] = SPDR;

			_delay_ms(100);
			spi_tranceiver(IO_control,0);
			IO_receivedDATA = SPDR;
			//Send_A_Character(IO_receivedDATA);
			
			
			if (IO_receivedDATA == 'X')
			{	
// 				while (5)
// 				{
// 					PORTC |= (1<<PINC2);
// 					PORTC |= (1<<PINC5);
// 					_delay_ms(70);
// 					PORTC &=~ (1<<PINC2);
// 					PORTC &=~ (1<<PINC5);
// 					_delay_ms(70);
// 				}
											
				//validate_OTP();
// 				initialise_speaking_parameters();
// 				_delay_us(10);
// 				initialise_clock();				
				send_OTP_to_arduino();
				_delay_us(10);//calibrate to optimize
				initialise_listening_parameters();
				//_delay_ms(1000);					
				
				while (17)
				{					
					if (bit_is_clear(PIND,7))
					{						
						//allow to read the 4-bit bus
						listenFunc();
						//initialise_speaking_parameters();
						_delay_us(10);						
						
						process_VEND_REQUEST();				
						
						break;
					}
					
					else
					if (bit_is_set(PIND,7))
					{
						//wait for a go signal
					}
				}
				
				if (ARD_BUS_val == 0)
				{
					//send 'T' for error
					spi_tranceiver(IO_control,'T');
					_delay_ms(10);
					spi_tranceiver(IO_control,'T');
					_delay_ms(10);
					reset_Everything();
				}
				else
				if ((ARD_BUS_val != 0) && (ARD_BUS_val != 'f')) 
				{
					//send 'Q' for product
					spi_tranceiver(IO_control,'Q');
					_delay_ms(10);
					spi_tranceiver(IO_control,'Q');
					_delay_ms(10);
				}
				else
				if (ARD_BUS_val == 'f')
				{
					//send 'f' for no Internet
					spi_tranceiver(IO_control,'f');
					_delay_ms(10);
					spi_tranceiver(IO_control,'f');
					_delay_ms(10);
				}						
			}
		}			
	}
}	
// reset function
void reset_Everything(void)
{
	b = false;
	talkFunc_exitParameter = false;

	count = 0;
	count1 = 0;
	count2 = 0;
	clock_init_patameter = false;
	loadValue = true;
	readValue = false;

	otp_number = 0;

	ARD_BUS_val = 0;

	masterDATA = 'q';
	data = 0; 
	retrieving_parameter = true;
	a = 0;

	IO_receivedDATA = 0;
	ACTUATOR_control_DATA = 0;

	OTP[0]=0;
	OTP[1]=0;
	OTP[2]=0;
	OTP[3]=0;
	OTP[4]=0;
	
	PORTD |= (1<<PIND6);	//pull up D6
	
	PORTB &=~ (1<<PINB0);
	PORTB &=~ (1<<PINB1);
	PORTC &=~ (1<<PINC2);
	PORTC &=~ (1<<PINC5);
	
	_delay_ms(10);
}
// Initialize SPI Master Device (with SPI interrupt)
void spi_init_master_INTERRUPT (void)
{
	// Set MOSI, SCK as Output
	
	DDRB |= (1<<PINB3)|(1<<PINB5);
	
	DDRB &=~ (1<<PINB4);
	PORTB |= (1<<PINB4);
	
	DDRB &=~ (1<<PINB2);	//SS as input
	PORTB |= (1<<PINB2);
	
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
	DDRB |= (1<<PINB3)|(1<<PINB5);
	PORTB |= (1<<PINB3)|(1<<PINB5);	
		
	DDRB &=~ (1<<PINB4);	//MISO as input
	PORTB |= (1<<PINB4);
	
	DDRB &=~ (1<<PINB2);	//SS as input
	PORTB |= (1<<PINB2);
	
	// Enable SPI, Set as Master
	//Prescaler: Fosc/16, Enable Interrupts
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPR1);//
}
// Initialize SPI Slave Device
void spi_init_slave (void)
{
	DDRB |= (1<<PINB4);     //MISO as OUTPUT
	PORTB &=~ (1<<PINB4);
	
	DDRB &=~ (1<<PINB5);	//SCK as input
	PORTB |= (1<<PINB5);
	
	DDRB &=~ (1<<PINB3);	//MOSI as input
	PORTB |= (1<<PINB3);
	
	DDRB &=~ (1<<PINB2);	//SS as input
	PORTB |= (1<<PINB2);
	
	SPCR |= (1<<SPE);		//Enable SPI
	SPCR &=~ (1<<MSTR);		//disable master mode
}
//Function to send and receive data for both master and slave
char spi_tranceiver (unsigned int chip_select,unsigned char data)
{
	if (chip_select == actuator_control)
	{
		PORTC &=~ (1<<PINC3);
		
		SPDR = data;// Load data into the buffer
		while(!(SPSR & (1<<SPIF) ));//Wait until transmission complete
		return SPDR;// Return received data
				
		PORTC |= (1<<PINC3);
	}
	
	if (chip_select == IO_control)
	{
		PORTC &=~ (1<<PINC4);
		
		SPDR = data;// Load data into the buffer
		while(!(SPSR & (1<<SPIF) ));//Wait until transmission complete
		return SPDR;// Return received data
			
		PORTC |= (1<<PINC4);
	}
}
//function to send the string
void spi_kill_master(void)
{
	// Set MOSI, SCK as Output
	
	// Enable SPI, Set as Master
	//Prescaler: Fosc/16, Enable Interrupts
	SPCR &=~ (1<<SPE);
	SPCR &=~ (1<<MSTR);
	SPCR &=~ (1<<SPR0);
}
void spi_kill_slave(void)
{
	SPCR &=~ (1<<SPE);   //disable SPI
	SPCR &=~ (1<<MSTR);
}

void vend_some(bool state, unsigned char product)
{
	spi_tranceiver(actuator_control, 255);_delay_ms(1);
	spi_tranceiver(actuator_control, 255);_delay_ms(1);
	spi_tranceiver(actuator_control, 255);_delay_ms(1);
	
	if (state == ON)
	{
		switch (product)
		{
			case 1:
			
			spi_tranceiver(actuator_control, 'A');
			data = SPDR;
			break;
			
			case 2:
			
			spi_tranceiver(actuator_control, 'B');
			data = SPDR;
			break;
			
			case 3:
			
			spi_tranceiver(actuator_control, 'C');
			data = SPDR;
			break;
			
			case 4:
			
			spi_tranceiver(actuator_control, 'D');
			data = SPDR;
			break;
			
			case 5:
			
			spi_tranceiver(actuator_control, 'E');
			data = SPDR;
			break;
			
			case 6:
			
			spi_tranceiver(actuator_control, 'F');
			data = SPDR;
			break;
			
			case 7:
			
			spi_tranceiver(actuator_control, 'G');
			data = SPDR;
			break;
			
			case 8:
			
			spi_tranceiver(actuator_control, 'H');
			data = SPDR;
			break;
						
			case 9:
			
			spi_tranceiver(actuator_control, 'I');
			data = SPDR;
			break;
			
			case 10:
			
			spi_tranceiver(actuator_control, 'J');
			data = SPDR;
			break;
		}
	}
	else
	if (state == OFF)
	{
		switch (product)
		{
			case 1:
			
			spi_tranceiver(actuator_control, 'a');
			data = SPDR;
			break;
			
			case 2:
			
			spi_tranceiver(actuator_control, 'b');
			data = SPDR;
			break;
			
			case 3:
			
			spi_tranceiver(actuator_control, 'c');
			data = SPDR;
			break;
			
			case 4:
			
			spi_tranceiver(actuator_control, 'd');
			data = SPDR;
			break;
			
			case 5:
			
			spi_tranceiver(actuator_control, 'e');
			data = SPDR;
			break;
			
			case 6:
			
			spi_tranceiver(actuator_control, 'f');
			data = SPDR;
			break;
			
			case 7:
			
			spi_tranceiver(actuator_control, 'g');
			data = SPDR;
			break;
			
			case 8:
			
			spi_tranceiver(actuator_control, 'h');
			data = SPDR;
			break;
			
			case 9:
			
			spi_tranceiver(actuator_control, 'i');
			data = SPDR;
			break;
			
			case 10:
			
			spi_tranceiver(actuator_control, 'j');
			data = SPDR;
			break;
		}
	}
}
void retrieve_OTP (void)
{
	if (retrieving_parameter==true)
	{
		while (8)///////run IO control service routine
		{
			_delay_ms(100);//have to be re-calibrated
			
			spi_tranceiver(IO_control, 'q');
			IO_receivedDATA = SPDR;
			
			if (IO_receivedDATA == 'x')
			{
				_delay_ms(10);//have to be re-calibrated
				spi_tranceiver(IO_control, 0);
				OTP[4] = SPDR;
				
				_delay_ms(10);//have to be re-calibrated
				spi_tranceiver(IO_control, 0);
				OTP[3] = SPDR;
				
				_delay_ms(10);//have to be re-calibrated
				spi_tranceiver(IO_control, 0);
				OTP[2] = SPDR;
				
				_delay_ms(10);//have to be re-calibrated
				spi_tranceiver(IO_control, 0);
				OTP[1] = SPDR;
				
				PORTC |= (1<<PINC2) | (1<<PINC5);
				_delay_ms(10);
			}
			
			else
			if (IO_receivedDATA == 'X')
			{
				PORTC |= (1<<PINC2) | (1<<PINC5);
				PORTB |= (1<<PINB0) | (1<<PINB1);

				_delay_ms(150);

				PORTC &=~ (1<<PINC2);
				PORTC &=~ (1<<PINC5);
				PORTB &=~ (1<<PINB0);
				PORTB &=~ (1<<PINB1);

				_delay_ms(150);
				
				break;
			}
		}
	}	
}
void validate_OTP(void)//
{
	if ((OTP[4] == 1 ) && (OTP[3] == 2) && (OTP[2] ==  1) && (OTP[1] == 2 ))
	{
		PORTB |= (1<<PINB0) | (1<<PINB1);
		_delay_ms(1000);
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'A');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
	}	
	else
	if ((OTP[4] == 1) && (OTP[3] == 1) && (OTP[2] == 1) && (OTP[1] == 1))
	{
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'B');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
	}
	else
	if ((OTP[4] ==  2) && (OTP[3] == 2) && (OTP[2] ==  2) && (OTP[1] == 2 ))
	{
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'C');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
	}
	else
	if ((OTP[4] ==  3) && (OTP[3] == 3) && (OTP[2] ==  3) && (OTP[1] ==  3))
	{
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'D');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
	}
	else
	if ((OTP[4] ==  4) && (OTP[3] == 4) && (OTP[2] ==  4) && (OTP[1] == 4 ))
	{
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'E');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
	}
	else
	if ((OTP[4] ==  5) && (OTP[3] == 5) && (OTP[2] ==  5) && (OTP[1] ==  5))
	{
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'F');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
	}
	else
	if ((OTP[4] ==  6) && (OTP[3] == 6) && (OTP[2] == 6 ) && (OTP[1] == 6 ))
	{
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'G');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
	}
	else
	if ((OTP[4] == 7 ) && (OTP[3] == 7) && (OTP[2] == 7 ) && (OTP[1] == 7 ))
	{
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'H');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
	}
	else
	if ((OTP[4] == 8 ) && (OTP[3] == 8) && (OTP[2] == 8 ) && (OTP[1] == 8 ))
	{
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'I');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
	}
	else
	if ((OTP[4] == 9 ) && (OTP[3] == 9) && (OTP[2] ==  9) && (OTP[1] ==  9))
	{
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'J');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
	}
}

void process_VEND_REQUEST(void)
{
	switch (ARD_BUS_val)
	{		
		case 0:
		PORTB |= (1<<PINB0) | (1<<PINB1);
		_delay_ms(1000);
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,0);
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
		break;
		
		case 1:
		PORTB |= (1<<PINB0) | (1<<PINB1);
		_delay_ms(1000);
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'A');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
		break;
		
		case 2:
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'B');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
		break;
		
		case 3:
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'C');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
		break;
		
		case 4:
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'D');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
		break;
		
		case 5:
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'E');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
		break;
		
		case 6:
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'F');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
		break;
		
		case 7:
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'G');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
		break;
		
		case 8:
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'H');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
		break;
		
		case 9:
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'I');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
		break;
		
		case 10:
		spi_tranceiver(actuator_control,255);
		_delay_ms(10);
		spi_tranceiver(actuator_control,'J');
		_delay_ms(10);
		ACTUATOR_control_DATA = SPDR;
		SPDR = 0;
		break;
	}
}

void initialise_clock(void)
{
	TCCR0B |= (1<<CS01)|(1<<CS00);
	TIMSK0 |= (1<<TOIE0);
	TCNT0 = 0;
}
void stop_clock(void)
{
	TCCR0B &=~ (1<<CS00);
	TCCR0B &=~ (1<<CS01);
	TIMSK0 &=~ (1<<TOIE0);
	_delay_ms(1);
	TCNT0=0;
	PORTD &=~ (1<<PIND5); //pull down clock pin
}
ISR(TIMER0_OVF_vect)
{
	count++;
	if (clock_init_patameter == false)
	{
		clock_init_patameter = true;
	}
	else
	if (count==50)////100count==50
	{
		// 1 second has elapsed
		count=0;
		
		count1++;
		if (count1 == 1)
		{
			count1 = 0;
			count2++;
			PORTD ^= (1<<PIND5);
			
			PORTC ^= (1<<PINC2);
			PORTC ^= (1<<PINC5);
		}	
	}
}

void talkFunc (unsigned char val)
{
	switch (val)
	{
		case 0:
		PORTD &=~ (1<<PIND0);
		PORTD &=~ (1<<PIND1);
		PORTD &=~ (1<<PIND2);
		PORTD &=~ (1<<PIND3);
		break;
		
		case 1:
		PORTD |=  (1<<PIND0);
		PORTD &=~ (1<<PIND1);
		PORTD &=~ (1<<PIND2);
		PORTD &=~ (1<<PIND3);
		break;
		
		case 2:
		PORTD &=~ (1<<PIND0);
		PORTD |=  (1<<PIND1);
		PORTD &=~ (1<<PIND2);
		PORTD &=~ (1<<PIND3);
		break;
		
		case 3:
		PORTD |=  (1<<PIND0);
		PORTD |=  (1<<PIND1);
		PORTD &=~ (1<<PIND2);
		PORTD &=~ (1<<PIND3);
		break;
		
		case 4:
		PORTD &=~ (1<<PIND0);
		PORTD &=~ (1<<PIND1);
		PORTD |=  (1<<PIND2);
		PORTD &=~ (1<<PIND3);
		break;
		
		case 5:
		PORTD |=  (1<<PIND0);
		PORTD &=~ (1<<PIND1);
		PORTD |=  (1<<PIND2);
		PORTD &=~ (1<<PIND3);
		break;
		
		case 6:
		PORTD &=~ (1<<PIND0);
		PORTD |=  (1<<PIND1);
		PORTD |=  (1<<PIND2);
		PORTD &=~ (1<<PIND3);
		break;
		
		case 7:
		PORTD |=  (1<<PIND0);
		PORTD |=  (1<<PIND1);
		PORTD |=  (1<<PIND2);
		PORTD &=~ (1<<PIND3);
		break;
		
		case 8:
		PORTD &=~ (1<<PIND0);
		PORTD &=~ (1<<PIND1);
		PORTD &=~ (1<<PIND2);
		PORTD |=  (1<<PIND3);
		break;
		
		case 9:
		PORTD |=  (1<<PIND0);
		PORTD &=~ (1<<PIND1);
		PORTD &=~ (1<<PIND2);
		PORTD |=  (1<<PIND3);
		break;
		
		case 'A':
		PORTD &=~ (1<<PIND0);
		PORTD |=  (1<<PIND1);
		PORTD &=~ (1<<PIND2);
		PORTD |=  (1<<PIND3);
		break;
		
		case 'B':
		PORTD |=  (1<<PIND0);
		PORTD |=  (1<<PIND1);
		PORTD &=~ (1<<PIND2);
		PORTD |=  (1<<PIND3);
		break;
		
		case 'C':
		PORTD &=~ (1<<PIND0);
		PORTD &=~ (1<<PIND1);
		PORTD |=  (1<<PIND2);
		PORTD |=  (1<<PIND3);
		
		case 'D':
		PORTD |=  (1<<PIND0);
		PORTD &=~ (1<<PIND1);
		PORTD |=  (1<<PIND2);
		PORTD |=  (1<<PIND3);
		break;
		
		case 'E':
		PORTD &=~ (1<<PIND0);
		PORTD |=  (1<<PIND1);
		PORTD |=  (1<<PIND2);
		PORTD |=  (1<<PIND3);
		break;
		
		case 'F':
		PORTD |=  (1<<PIND0);
		PORTD |=  (1<<PIND1);
		PORTD |=  (1<<PIND2);
		PORTD |=  (1<<PIND3);
		break;
	}
	
}

void listenFunc (void)
{
	if ((bit_is_clear(PIND,0)) && (bit_is_clear(PIND,1)) && (bit_is_clear(PIND,2)) && (bit_is_clear(PIND,3)))
	{
		//0
		ARD_BUS_val = 0;
	}
	
	else
	if ((bit_is_set(PIND,0)) && (bit_is_clear(PIND,1)) && (bit_is_clear(PIND,2)) && (bit_is_clear(PIND,3)))
	{
		//1
		ARD_BUS_val = 1;
	}
	
	else
	if ((bit_is_clear(PIND,0)) && (bit_is_set(PIND,1)) && (bit_is_clear(PIND,2)) && (bit_is_clear(PIND,3)))
	{
		//2
		ARD_BUS_val = 2;
	}
	
	else
	if ((bit_is_set(PIND,0)) && (bit_is_set(PIND,1)) && (bit_is_clear(PIND,2)) && (bit_is_clear(PIND,3)))
	{
		//3
		ARD_BUS_val = 3;
	}
	
	else
	if ((bit_is_clear(PIND,0)) && (bit_is_clear(PIND,1)) && (bit_is_set(PIND,2)) && (bit_is_clear(PIND,3)))
	{
		//4
		ARD_BUS_val = 4;
	}
	
	else
	if ((bit_is_set(PIND,0)) && (bit_is_clear(PIND,1)) && (bit_is_set(PIND,2)) && (bit_is_clear(PIND,3)))
	{
		//5
		ARD_BUS_val = 5;
	}
	
	else
	if ((bit_is_clear(PIND,0)) && (bit_is_set(PIND,1)) && (bit_is_set(PIND,2)) && (bit_is_clear(PIND,3)))
	{
		//6
		ARD_BUS_val = 6;
	}
	
	else
	if ((bit_is_set(PIND,0)) && (bit_is_set(PIND,1)) && (bit_is_set(PIND,2)) && (bit_is_clear(PIND,3)))
	{
		//7
		ARD_BUS_val = 7;
	}
	
	else
	if ((bit_is_clear(PIND,0)) && (bit_is_clear(PIND,1)) && (bit_is_clear(PIND,2)) && (bit_is_set(PIND,3)))
	{
		//8
		ARD_BUS_val = 8;
	}
	
	else
	if ((bit_is_set(PIND,0)) && (bit_is_clear(PIND,1)) && (bit_is_clear(PIND,2)) && (bit_is_set(PIND,3)))
	{
		//9
		ARD_BUS_val = 9;
	}
	
	else
	if ((bit_is_clear(PIND,0)) && (bit_is_set(PIND,1)) && (bit_is_clear(PIND,2)) && (bit_is_set(PIND,3)))
	{
		//A
		ARD_BUS_val = 'a';
	}
	
	else
	if ((bit_is_set(PIND,0)) && (bit_is_set(PIND,1)) && (bit_is_clear(PIND,2)) && (bit_is_set(PIND,3)))
	{
		//B
		ARD_BUS_val = 'b';
	}
	
	else
	if ((bit_is_clear(PIND,0)) && (bit_is_clear(PIND,1)) && (bit_is_set(PIND,2)) && (bit_is_set(PIND,3)))
	{
		//C
		ARD_BUS_val = 'c';
	}
	
	else
	if ((bit_is_set(PIND,0)) && (bit_is_clear(PIND,1)) && (bit_is_set(PIND,2)) && (bit_is_set(PIND,3)))
	{
		//D
		ARD_BUS_val = 'd';
	}
	
	
	else
	if ((bit_is_clear(PIND,0)) && (bit_is_set(PIND,1)) && (bit_is_set(PIND,2)) && (bit_is_set(PIND,3)))
	{
		//E
		ARD_BUS_val = 'e';
	}
	
	
	else
	if ((bit_is_set(PIND,0)) && (bit_is_set(PIND,1)) && (bit_is_set(PIND,2)) && (bit_is_set(PIND,3)))
	{
		//F
		ARD_BUS_val = 'f';
	}
}
//rectification needed

void initialise_common_parameters(void)
{
	DDRD &=~ (1<<PIND4);	//set clock pin to input
	PORTD |= (1<<PIND4);	//pull up clock pin
	
	DDRD |= (1<<PIND5);		//set clock generator pin to output
	PORTD &=~ (1<<PIND5);	//sink clock generator pin
	_delay_us(10);			//little delay to avoid overlap
}

void disable_common_parameters(void)
{
	DDRD &=~ (1<<PIND5);	//disable_clock by making clock generator pin input
	PORTD |= (1<<PIND5);	//see if there is anything u can do here as well !!
	_delay_us(10);			//little delay to avoid overlap
}

void initialise_listening_parameters(void)
{
	DDRD &=~ (1<<PIND0); PORTD |= (1<<PIND0);	//set D0-D3 input & D0-D3 enable pull-up
	DDRD &=~ (1<<PIND1); PORTD |= (1<<PIND1);	//set D0-D3 input & D0-D3 enable pull-up
	DDRD &=~ (1<<PIND2); PORTD |= (1<<PIND2);	//set D0-D3 input & D0-D3 enable pull-up
	DDRD &=~ (1<<PIND3); PORTD |= (1<<PIND3);	//set D0-D3 input & D0-D3 enable pull-up
	
}

void initialise_speaking_parameters(void)
{
	
	DDRD |= (1<<PIND0); PORTD &=~ (1<<PIND0);	//set D0-D3 output & D0-D3 pull down
	DDRD |= (1<<PIND1); PORTD &=~ (1<<PIND1);	//set D0-D3 output & D0-D3 pull down
	DDRD |= (1<<PIND2); PORTD &=~ (1<<PIND2);	//set D0-D3 output & D0-D3 pull down
	DDRD |= (1<<PIND3); PORTD &=~ (1<<PIND3);	//set D0-D3 output & D0-D3 pull down
}

void send_OTP_to_arduino(void)
{
	initialise_common_parameters();
	//D0-D3 set to output
	initialise_speaking_parameters();
	//_delay_ms(100);
	//pull down pin D6 arduino will be waiting
	PORTD &=~ (1<<PIND6);	
	_delay_ms(1);
	//_delay_ms(1000);
	//PORTC |= (1<<PINC5);
	PORTD |= (1<<PIND6);
	
	while(11)
	{
		if (bit_is_clear(PIND,7))//pulled low
		{			
			//initiate clock
			_delay_ms(100);
			initialise_clock();
			//first pulse will indicate ready
			
			while(2)
			{
				if (talkFunc_exitParameter == true)
				{
					//stop clock
					_delay_ms(20);//CHANGE TO OPTIMISE
					stop_clock();
					_delay_us(1);
					//PULL down D0-D3
					PORTD &=~ (1<<PIND0);
					PORTD &=~ (1<<PIND1);
					PORTD &=~ (1<<PIND2);
					PORTD &=~ (1<<PIND3);
					
					PORTC &=~ (1<<PINC2);//lights out
					PORTC &=~ (1<<PINC5);					
					
					//_delay_ms(10);
					//pull up pin D6
					
					break;
				}
				else
				if (clock_init_patameter == true)
				{
					while (3)
					{
						if (talkFunc_exitParameter == true)
						{
							break;
						}
						else
						if (bit_is_clear(PIND,4))
						{
							//load and latch
							if (b == false)
							{
								b = true;							
								otp_number++;
								
								switch (otp_number)
								{									
									case 1:
									talkFunc(OTP[1]);
									break;
									
									case 2:
									talkFunc(OTP[2]);
									break;
									
									case 3:
									talkFunc(OTP[3]);
									break;
									
									case 4:
									talkFunc(OTP[4]);
									break;
									
									case 5:
									//talkFunc('F');
									talkFunc_exitParameter = true;
									//_delay_ms(500);
									break;								
								}								
							}
						}
						else
						if (bit_is_set(PIND,4))
						{
							if (b == true)
							{
								b = false;
							}
							//wait
						}
					}
				}
			}
		}
		
		else
		if (bit_is_set(PIND,7))// pulled high
		{
			if (talkFunc_exitParameter == true)

			{
				break;
			}
		}
	}
	
	
	//send all 4 otps
}


// void Check_IF_MrLCD_isBusy(void)
// {
// 	DataDir_MrLCDsCrib=0;
// 	MrLCDsControl |= 1<<ReadWrite;
// 	MrLCDsAUXILLARYControl &=~ 1<<BiPolarMood;
// 	
// 	while(MrLCDsCrib >= 0x80)
// 	{
// 		Peek_A_Boo();
// 	}
// 	
// 	DataDir_MrLCDsCrib=0xFF;
// }
// void Peek_A_Boo(void)
// {
// 	MrLCDsControl |= 1<<LightSwitch;
// 	_delay_us(10);/////CALIBRATE IT FURTHER
// 	MrLCDsControl &=~  1<<LightSwitch;
// }
// void Send_A_Command(char command)
// {
// 	Check_IF_MrLCD_isBusy();
// 	MrLCDsCrib = command;
// 	MrLCDsControl &=~ (1<<ReadWrite);
// 	MrLCDsAUXILLARYControl &=~ (1<<BiPolarMood);
// 	Peek_A_Boo();
// 	MrLCDsCrib = 0;
// }
// void Send_A_Character(char character)
// {
// 	Check_IF_MrLCD_isBusy();
// 	MrLCDsCrib = character;
// 	MrLCDsControl &=~ (1<<ReadWrite);
// 	MrLCDsAUXILLARYControl |= (1<<BiPolarMood);
// 	Peek_A_Boo();
// 	MrLCDsCrib = 0;
// }
// void delete_a_character(char *character)
// {
// 	while(*character > 0)
// 	{
// 		*character--;
// 		Send_A_Character(*character);
// 		_delay_ms(1);
// 	}
// 	
// }
// void Send_A_String(char *stringOfCharacters)
// {
// 	while(*stringOfCharacters > 0)
// 	{
// 		Send_A_Character(*stringOfCharacters++);
// 		_delay_ms(40);
// 	}
// }

// while (14)
// {
// 	spi_tranceiver(actuator_control,'Y');
// 	ACTUATOR_control_DATA = SPDR;
// 	if (ACTUATOR_control_DATA == 0)
// 	{
// 		PORTC ^= (1<<PINC2);
// 	}
// 	_delay_ms(1000);
// 	
// 	
// 	spi_tranceiver(IO_control,'a');
// 	IO_receivedDATA = SPDR;
// 	if (IO_receivedDATA == 0xCC)
// 	{
// 		PORTC ^= (1<<PINC5);
// 	}
// 	_delay_ms(1000);
// 	
// }

// 	while (7)
// 	{
	// 		spi_tranceiver(actuator_control,'P');
	// 		IO_receivedDATA = SPDR;
	// 		if (IO_receivedDATA == 'Z')
	// 		{
		// 			_delay_ms(10);
		// 			spi_tranceiver(actuator_control,'z');
	// 		}
	// 		_delay_ms(100);
// 	}
// 
// if (IO_receivedDATA == 12)
// {
// 	_delay_ms(100);
// }
// else
// if (IO_receivedDATA == 'P')
// {
// 	PORTC |= (1<<PINC2) | (1<<PINC5);
// 	_delay_ms(100);
// 
// 	PORTC &=~ (1<<PINC2);
// 	PORTC &=~ (1<<PINC5);
// 	PORTB &=~ (1<<PINB0);
// 	PORTB &=~ (1<<PINB1);
// 	
// 	spi_tranceiver(actuator_control,'X');
// 	_delay_ms(10);
// 	spi_tranceiver(actuator_control,'X');
// 	_delay_ms(10);
// 	spi_tranceiver(actuator_control,'X');
// 	
// 	masterDATA = 'q';
// 	
// 	_delay_ms(100);
// }
// else
// if (IO_receivedDATA == 255)
// {
// 	_delay_ms(10);//have to be re-calibrated
// 	spi_tranceiver(IO_control, 0);
// 	OTP[4] = SPDR;
// 	
// 	_delay_ms(10);//have to be re-calibrated
// 	spi_tranceiver(IO_control, 0);
// 	OTP[3] = SPDR;
// 	
// 	_delay_ms(10);//have to be re-calibrated
// 	spi_tranceiver(IO_control, 0);
// 	OTP[2] = SPDR;
// 	
// 	_delay_ms(10);//have to be re-calibrated
// 	spi_tranceiver(IO_control, 0);
// 	OTP[1] = SPDR;
// 	
// 	_delay_ms(10);
// 	spi_tranceiver(IO_control,0);
// 	IO_receivedDATA = SPDR;
// 	
// 	if (IO_receivedDATA == 'X')
// 	{
// 		masterDATA = 'Q';
// 	}
// 	validate_OTP();
// }
// 
// _delay_ms(100);

// 		if (IO_receivedDATA == 255)
// 		{
// 			IO_receivedDATA = 0;
// 			
// 		}
// 		else
// 		if (IO_receivedDATA == 'x')
// 		{
// 			IO_receivedDATA = 0;
// 			
// 			_delay_ms(100);
// 			spi_tranceiver(IO_control,0);
// 			OTP[0] = SPDR;
// 			
// 			_delay_ms(100);//have to be re-calibrated
// 			spi_tranceiver(IO_control, 0);
// 			OTP[4] = SPDR;
// 			
// 			_delay_ms(100);//have to be re-calibrated
// 			spi_tranceiver(IO_control, 0);
// 			OTP[3] = SPDR;
// 			
// 			_delay_ms(100);//have to be re-calibrated
// 			spi_tranceiver(IO_control, 0);
// 			OTP[2] = SPDR;
// 			
// 			_delay_ms(100);//have to be re-calibrated
// 			spi_tranceiver(IO_control, 0);
// 			OTP[1] = SPDR;
// 			
// 			if ( (OTP[4] == 6) && (OTP[3] == 6) && (OTP[2] == 6) && (OTP[1] == 6) )
// 			{
// 				PORTC |= (1<<PINC2) | (1<<PINC5);
// 				PORTB |= (1<<PINB0) | (1<<PINB1);
// 				
// 				_delay_ms(500);
// 				
// 				PORTC &=~ (1<<PINC2);
// 				PORTC &=~ (1<<PINC5);
// 				PORTB &=~ (1<<PINB0);
// 				PORTB &=~ (1<<PINB1);
// 				
// 				_delay_ms(150);
// 			}
// 		}
// 		
// 		else
// 		if (IO_receivedDATA == 'X')
// 		{
// 			IO_receivedDATA = 0;
// 			spi_tranceiver(IO_control, 'Q');
// 		}
// 		else
// 		if (IO_receivedDATA == 'Z')
// 		{
// 			IO_receivedDATA = 0;
// 			
// 			_delay_ms(10);
// 			spi_tranceiver(IO_control,'z');
// 		}
// 		_delay_ms(10);

// 			if ( (OTP[4] == 6) || (OTP[3] == 6) || (OTP[2] == 6) || (OTP[1] == 6) )
// 			{
	// 				PORTC |= (1<<PINC2) | (1<<PINC5);
	// 				PORTB |= (1<<PINB0) | (1<<PINB1);
	//
	// 				_delay_ms(500);
	//
	// 				PORTC &=~ (1<<PINC2);
	// 				PORTC &=~ (1<<PINC5);
	// 				PORTB &=~ (1<<PINB0);
	// 				PORTB &=~ (1<<PINB1);
	//
	// 				_delay_ms(150);
	
	// 				while (7)
	// 				{
		// 					spi_tranceiver(actuator_control,255);
		// 					ACTUATOR_control_DATA = SPDR;
		// 					if (ACTUATOR_control_DATA == 'x')
		// 					{
			// 						_delay_ms(10);
			// 						spi_tranceiver(actuator_control,'A');
			// 						break;
		// 					}
		// 					_delay_ms(100);
	// 				}
//			}

//algorithm for the vending process
//run keypad functions for OTP input//////////////////
//receive OTP from IO control services////////////////

//receive data from arduino garbage garble for achieving synchronization**********
//transmit OTP to the arduino *********
//wait for arduino to communicate to the server*********
//receive response of server about OTP*********
// if OTP is correct

//vend stuff
//get the specific preset code loaded that will be assigned one specific vending task
//communicate with sensor grid
//initialize the sensor grid
//sensor grid can be programmed to be self sustaining
//communicate with IO control for displaying the transient status
//communicate with Actuator Control Service for initialization of dispatch
//communicate with sensor grid continuously for product dispatch status acknowledgment
//if product received
//communicate IO control for success message
//send to server that the vending process was completed successfully
//reset the initialization parameters
//start all over
//if product not received
//start counter based on ISR inside the master control services
//ISRs in an open loop are highly NOT RECOMMENDED
//alternatively the code without ISR in the sensor grid control or the master itself
//if timer count exceeds the preset time
//communicate with arduino acknowledging the failure of the event
//communicate with the IO board to display the error message
//reset the initialization parameters
//start all over
//else

//if OTP is not correct
//don't vend
//just display to enter the pin again
//reset the initialization parameters

//task over the cloud
//if OTP is correct but product not available
//saving for a later purpose
//updating the current status in real time
//allocating location based vending services
// 		PORTC |= (1<<PINC2) | (1<<PINC5);
// 		_delay_ms(100);
// 		PORTC &=~ (1<<PINC2);
// 		PORTC &=~ (1<<PINC5);
		 
// while (5) //check for any signal pins
// {
// 	//signal from IO control will be Primary input for the system...
// 	if (bit_is_clear(PINC, 1))
// 	{
// 		retrieving_parameter=true;
// 		_delay_us(10);
// 	}
// 	else
// 	if (bit_is_set(PINC, 1))
// 	{
// 		_delay_ms(10);
// 		retrieve_OTP();
// 		retrieving_parameter=false;
// 		
// 		
// 		if (OTP[4] == 6 && OTP[3] == 6 && OTP[2] == 6 && OTP[1] == 6)
// 		{
// 			OTP[4]=0; OTP[3]=0; OTP[2]=0; OTP[1]=0;
// 			
// 			PORTC |= (1<<PINC2) | (1<<PINC5);
// 			PORTB |= (1<<PINB0) | (1<<PINB1);
// 
// 			_delay_ms(150);
// 
// 			PORTC &=~ (1<<PINC2);
// 			PORTC &=~ (1<<PINC5);
// 			PORTB &=~ (1<<PINB0);
// 			PORTB &=~ (1<<PINB1);
// 
// 			_delay_ms(150);
// 			
// 			vend_some(ON,1);
// 		}
// 		
// 		else
// 		if (OTP[4] == 5 && OTP[3] == 5 && OTP[2] == 5 && OTP[1] == 5)
// 		{
// 			OTP[4]=0; OTP[3]=0; OTP[2]=0; OTP[1]=0;
// 			PORTC |= (1<<PINC2) | (1<<PINC5);
// 			PORTB |= (1<<PINB0) | (1<<PINB1);
// 
// 			_delay_ms(150);
// 
// 			PORTC &=~ (1<<PINC2);
// 			PORTC &=~ (1<<PINC5);
// 			PORTB &=~ (1<<PINB0);
// 			PORTB &=~ (1<<PINB1);
// 
// 			_delay_ms(150);
// 			
// 			vend_some(ON,2);
// 		}
// 	}
// }
			

			

//
// 		else  //signal from the sensor control will be the intruder alert warning distress signal
// 		if (bit_is_set(PINC, 0))
// 		{
	// 			//continue;//run sensor grid service routine
// 		}
//
// 		else //if any signal pins is low
// 		if ((bit_is_clear(PINC, 0)) && (bit_is_clear(PINC, 1)) && (bit_is_clear(PINC, 2)) )
// 		{
	// 			//continue;
// 		}
//run SPI routine


//void initialize_vending_process (unsigned char inventory)
// {
// 	spi_tranceiver(actuator_control,inventory);
// }

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

//run actuator control service routine
//check the present status of the motor and drivers
//see if the instruction received ask to start the motor or to stop it
//send the signals for completing the purpose
//send the IO service to display the information about dispatch status
//end the loop


// while(1)
// {
// 	if (bit_is_set(PINC,1))
// 	{
// 		_delay_ms(100);
// 		spi_tranceiver(IO_control, 'a');
// 		a = SPDR;
// 		_delay_ms(1);
// 		if (a == 2)
// 		{
// 			PORTD ^= (1<<PIND0);
// 		}
// 		else
// 		if (a == 3)
// 		{
// 			PORTD ^= (1<<PIND1);
// 		}
// 		else
// 		if (a == 1)
// 		{
// 			PORTD ^= (1<<PIND2);
// 		}
// 		else
// 		if (a == 6)
// 		{
// 			PORTD ^= (1<<PIND3);
// 		}
// 	}
// 	
// 	else
// 	if (bit_is_clear(PINC,1))
// 	{
// 		continue;
// 	}
// }


// while(4)
// {
// 	vend_some(ON,10);
// 	_delay_ms(1000);
// 	vend_some(ON,9);
// 	_delay_ms(1000);
// 	vend_some(ON,8);
// 	_delay_ms(1000);
// 	vend_some(ON,7);
// 	_delay_ms(1000);
// 	vend_some(ON,6);
// 	_delay_ms(1000);
// 	vend_some(ON,5);
// 	_delay_ms(1000);
// 	vend_some(ON,4);
// 	_delay_ms(1000);
// 	vend_some(ON,3);
// 	_delay_ms(1000);
// 	vend_some(ON,2);
// 	_delay_ms(1000);
// 	vend_some(ON,1);
// 	_delay_ms(1000);
// 	
// 	vend_some(OFF,10);
// 	_delay_ms(1000);
// 	vend_some(OFF,9);
// 	_delay_ms(1000);
// 	vend_some(OFF,8);
// 	_delay_ms(1000);
// 	vend_some(OFF,7);
// 	_delay_ms(1000);
// 	vend_some(OFF,6);
// 	_delay_ms(1000);
// 	vend_some(OFF,5);
// 	_delay_ms(1000);
// 	vend_some(OFF,4);
// 	_delay_ms(1000);
// 	vend_some(OFF,3);
// 	_delay_ms(1000);
// 	vend_some(OFF,2);
// 	_delay_ms(1000);
// 	vend_some(OFF,1);
// 	_delay_ms(1000);
//  }


//while (3)
//{
// 	spi_tranceiver(actuator_control, 'A');
// 	//a=SPDR;
// 	if (a == 5)
// 	{
// 		//PORTB ^= (1<<PINB6);
// 		a=0;
// 	}
// 	_delay_ms(1000);
// 	
// 	spi_tranceiver(actuator_control, 'B');
// 	//a=SPDR;
// 	if (a == 5)
// 	{
// 		//PORTB ^= (1<<PINB6);
// 		a=0;
// 	}
// 	_delay_ms(1000);
// 	
// 	spi_tranceiver(actuator_control, 'a');
// 	//a=SPDR;
// 	if (a == 5)
// 	{
// 		//PORTB ^= (1<<PINB6);
// 		a=0;
// 	}
// 	_delay_ms(1000);
// 	
// 	spi_tranceiver(actuator_control, 'b');
// 	//a=SPDR;
// 	if (a == 5)
// 	{
// 		//PORTB ^= (1<<PINB6);
// 		a=0;
// 	}
// 	_delay_ms(1000);
//}