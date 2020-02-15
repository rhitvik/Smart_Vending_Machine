/*
 * password_based.cpp
 *
 * Created: 8/28/2015 4:45:51 PM
 *  Author: rhitvik
 */ 

# define F_CPU 1000000UL

#define LightSwitch    0
#define ReadWrite      1
#define BiPolarMood    2

#define MrLCDsCrib            PORTA
#define DataDir_MrLCDsCrib	  DDRA

#define MrLCDsControl		  PORTB
#define DataDir_MrLCDsControl DDRB

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

 void spi_init_master_INTERRUPT (void);
 void spi_init_master (void);
 void spi_init_slave (void);
 char spi_tranceiver (unsigned char data);
 void spi_kill_master(void);
 void spi_kill_slave(void);

void Check_IF_MrLCD_isBusy(void);
void Peek_A_Boo(void);
void Send_A_Command(char command);
void Send_A_Character(char character);
void delete_a_character(char *character);
void Send_A_String(char *stringOfCharacters);
void Send_A_String_instantly(char *stringOfCharacters);
void LCD_line_1 (void);
void LCD_line_2 (void);
void LCD_line_3 (void);
void LCD_line_4 (void);
void Clear_LCD_line_1 (void);
void Clear_LCD_line_2 (void);
void Clear_LCD_line_3 (void);
void Clear_LCD_line_4 (void);

void detect_key_press(void);
void detect_key(void);
void transient(void);
void run_keypad(void);
void COMM_Function(void);
void sensor_Function(void);
void sensor_COMM_Function(void);
void OTP_funct(void);
void resetParameters(void);

	bool pressed = 0;
	uint8_t row = 0;
	uint8_t column = 0;
	bool commParameter = false;
	unsigned char pressed_key = 0;
	unsigned int OTP[5];
	int i = 4;
	unsigned int password = 0;	
	unsigned char packet = 0;
	uint8_t sensor_PARAMETER = 0;
	bool t=0;
	bool u=0;
	
int main(void)
{
	MCUCSR |= (1<<JTD);
	MCUCSR |= (1<<JTD);
	
	DDRB |= (1<<PINB3);
	PORTB &=~ (1<<PINB3);
	
	DDRD = 0b11110000;
	PORTD = 0b00001111;
	
	DDRC = 0b00000000;
	PORTC = 0xFF;
	
	DataDir_MrLCDsControl |= (1<<LightSwitch) | (1<<ReadWrite) | (1<<BiPolarMood);
		
	_delay_ms(15);
	Send_A_Command(0x01);//clrscr();
	_delay_ms(2);
	Send_A_Command(0x38);
	_delay_us(50);
	Send_A_Command(0b00001100);
	_delay_us(50);
	
 	spi_init_slave();
// 	
// 	spi_tranceiver('x');
// 	spi_tranceiver('y');
	
	while (10)
	{		
		while (9)
		{
			resetParameters();
			_delay_ms(2);
			run_keypad();
			break;
		}
		
		while (8)
		{
			spi_tranceiver('x');
			packet = SPDR;
			Send_A_Character(packet);
			
			if(packet == 255)
			{				
				_delay_us(1);
			}
			else
			if (packet == 'q')//sync achieved
			{
				COMM_Function();
			}
			else			
			if (packet == 'Q')
			{
				Clear_LCD_line_1();
				LCD_line_1();
				Send_A_String("HANG ON !!");
				_delay_ms(2);////
				
				sensor_Function();				
				//check for dispatch
				//sensor control grid
				//sensor_COMM_Function();
				if (commParameter == true)
				{
					break;			
				}
			}
			else
			if (packet == 'f')
			{
				commParameter = true;
				Clear_LCD_line_1();
				LCD_line_1();
				Send_A_String("SORRY!! NO Internet");
				_delay_ms(2000);////
				
				break;
			}
			else
			if (packet == 'T')
			{
				commParameter = true;
				Clear_LCD_line_1();
				LCD_line_1();
				Send_A_String("INVALID PIN");
				_delay_ms(2000);////
				
				break;
			}
						
		}
		
		while (12)
		{									
			spi_tranceiver('O');
			packet = SPDR;		
			
			if (packet == 'q')
			{
				Clear_LCD_line_1();
				LCD_line_1();
				_delay_ms(2);
				Send_A_String("Hmmm...");
				_delay_ms(2);
				_delay_ms(20);
				
				break;
			}
		}
	}
}

void resetParameters(void)
{
	_delay_ms(15);
	Send_A_Command(0x01);//clrscr();
	_delay_ms(2);
	
	LCD_line_1();
	_delay_ms(2);
	Send_A_String("SERVESTER VENDING");
	_delay_ms(2);
	
	LCD_line_2();
	_delay_ms(2);
	Send_A_String("Download APP to Shop");
	_delay_ms(2);
	
	LCD_line_3();
	_delay_ms(2);
	Send_A_String("  Type 'SERVESTER' ");
	_delay_ms(2);
	
	LCD_line_4();
	_delay_ms(2);
	Send_A_String("For Help: 7737698200");
	_delay_ms(2);
	
	_delay_ms(10);
	
	pressed = 0;
	row = 0;
	column = 0;
	
	pressed_key = 0;
	OTP[4]=0;OTP[3]=0;OTP[2]=0;OTP[1]=0;OTP[0]=0;
	i = 4;
	password = 0;
	sensor_PARAMETER = 0;
	commParameter = false;
	packet = 0;
	t=0;
	u=0;
	//SPDR=0;
}
void detect_key_press(void)
{
	 while(1)
	 {
		 if (bit_is_clear(PIND,0))
		 {
			 row = 1;
			 pressed = 1;
			 break;
		 }
		 else
		 if (bit_is_clear(PIND,1))
		 {
			 row = 2;
			 pressed = 1;
			 break;
		 }
		 else
		 if (bit_is_clear(PIND,2))
		 {
			 row = 3;
			 pressed = 1;
			 break;
		 }
		 else
		 if (bit_is_clear(PIND,3))
		 {
			 row = 4;
			 pressed = 1;
			 break;
		 }
		 else
		 {
			 PORTD &=~ (1<<PIND4);
			 PORTD &=~ (1<<PIND5);
			 PORTD &=~ (1<<PIND6);
			 PORTD &=~ (1<<PIND7);
		 }
	 }
	 
	 while(2)
	 {
		 PORTD |= (1<<PIND4);
		 _delay_ms(1);
		 if (bit_is_set(PIND,0) && bit_is_set(PIND,1) && bit_is_set(PIND,2) && bit_is_set(PIND,3))
		 {
			 column = 1;
			 PORTD &=~ (1<<PIND4);
			 break;
		 }
		 else
		 
		 PORTD |= (1<<PIND5);
		 _delay_ms(1);
		 if (bit_is_set(PIND,0) && bit_is_set(PIND,1) && bit_is_set(PIND,2) && bit_is_set(PIND,3))
		 {
			 column = 2;
			 PORTD &=~ (1<<PIND5);
			 break;
		 }
		 
		 PORTD |= (1<<PIND6);
		 _delay_ms(1);
		 if (bit_is_set(PIND,0) && bit_is_set(PIND,1) && bit_is_set(PIND,2) && bit_is_set(PIND,3))
		 {
			 column = 3;
			 PORTD &=~ (1<<PIND6);
			 break;
		 }
		 else
		 
		 PORTD |= (1<<PIND7);
		 _delay_ms(1);
		 
		 if (bit_is_set(PIND,0) && bit_is_set(PIND,1) && bit_is_set(PIND,2) && bit_is_set(PIND,3))
		 {
			 column = 4;
			 PORTD &=~ (1<<PIND7);
			 break;
		 }
	 }
}
void detect_key(void)
{	
	if (row == 1 && column == 1)
	{
		pressed_key = 1;
	}
	else
	if (row == 1 && column == 2)
	{
		pressed_key = 2;
	}
	else
	if (row == 1 && column == 3)
	{
		pressed_key = 3;
	}
	else
	if (row == 1 && column == 4)
	{
		pressed_key = 'A';
	}
	else
	if (row == 2 && column == 1)
	{
		pressed_key = 4;
	}
	else
	if (row == 2 && column == 2)
	{
		pressed_key = 5;
	}
	else
	if (row == 2 && column == 3)
	{
		pressed_key = 6;
	}
	else
	if (row == 2 && column == 4)
	{
		pressed_key = 'B';
	}
	else
	if (row == 3 && column == 1)
	{
		pressed_key = 7;
	}
	else
	if (row == 3 && column == 2)
	{
		pressed_key = 8;
	}
	else
	if (row == 3 && column == 3)
	{
		pressed_key = 9;
	}
	else
	if (row == 3 && column == 4)
	{
		pressed_key = 'C';
	}
	else
	if (row == 4 && column == 1)
	{
		pressed_key = 'Q';
	}
	else
	if (row == 4 && column == 2)
	{
		pressed_key = 0;
	}
	else
	if (row == 4 && column == 3)
	{
		pressed_key = 'R';
	}
	else
	if (row == 4 && column == 4)
	{
		pressed_key = 'D';
	}
}
void transient(void)
{	
	_delay_ms(250);	
	switch(pressed_key)
	{
		case 1:
		Send_A_String("1");
		break;
		
		case 2:
		Send_A_String("2");
		break;
		
		case 3:
		Send_A_String("3");
		break;
		
		case 'A':
		Send_A_String("A");
		break;
		
		case 4:
		Send_A_String("4");
		break;
		
		case 5:
		Send_A_String("5");
		break;
		
		case 6:
		Send_A_String("6");
		break;
		
		case 'B':
		Send_A_String("B");
		break;
		
		case 7:
		Send_A_String("7");
		break;
		
		case 8:
		Send_A_String("8");
		break;
		
		case 9:
		Send_A_String("9");
		break;
		
		case 'C':
		Send_A_String("C");
		break;
		
		case 'Q':
		Send_A_String("*");
		break;
		
		case 0:
		Send_A_String("0");
		break;
		
		case 'R':
		Send_A_String("#");
		break;
		
		case 'D':
		Send_A_String("D");
		break;
	}

}
void run_keypad(void)
{
	Clear_LCD_line_1();
	LCD_line_1();
	_delay_ms(2);
	Send_A_String("PIN: ");
	_delay_ms(2);
	
	while(3)	
	{			
		if (bit_is_set(PIND,0) && bit_is_set(PIND,1) && bit_is_set(PIND,2) && bit_is_set(PIND,3))
		{				
			detect_key_press();
			detect_key();
			transient();
			OTP_funct();
			
			if (commParameter == true)
			{
				PORTD &=~ (1<<PIND4);
				PORTD &=~ (1<<PIND5);
				PORTD &=~ (1<<PIND6);
				PORTD &=~ (1<<PIND7);
				_delay_us(10);
				break;
			}			
		}			
		
	}
}
void OTP_funct(void)
{
	OTP[i] = pressed_key;
	i--;
	if (i == 0)
	{
		i = 4;
		commParameter = true;
	}
}
void COMM_Function(void)
{
	if (commParameter == true)
	{
		commParameter = false;
		
		spi_tranceiver(255);
			
		spi_tranceiver(OTP[4]);
		spi_tranceiver(OTP[3]);
		spi_tranceiver(OTP[2]);
		spi_tranceiver(OTP[1]);
			
		spi_tranceiver('X');				
	}
}
/////////////////
void sensor_Function(void)
{
	Send_A_Command(0x01);//clrscr();
	_delay_ms(2);
	Send_A_String("Please Wait...");
	_delay_ms(2);
		
	while (13) //check dispatch
	{
		if( (bit_is_clear(PINC,2)) ||(bit_is_clear(PINC,3)) ||(bit_is_clear(PINC,4)) ||(bit_is_clear(PINC,5)) ) //product falls
		{
			sensor_PARAMETER = 1;
			_delay_ms(10);
			sensor_COMM_Function();
			if (commParameter == true)
			{
				Clear_LCD_line_1();
				LCD_line_1();
				Send_A_String("Please Collect");
				_delay_ms(2);
				_delay_ms(1000);
				
				break;
			}
		}
		else 
		if ( (bit_is_set(PINC,2)) && (bit_is_set(PINC,3)) && (bit_is_set(PINC,4)) && (bit_is_set(PINC,5)) )//product doesn't fall
		{
			sensor_PARAMETER = 0;
// 			if (sensor_PARAMETER == 1)
// 			{
// 				sensor_PARAMETER = 0;
// 			}
		}
	}
}

void sensor_COMM_Function(void)	
{		
	if (sensor_PARAMETER == 1)
	{
		Clear_LCD_line_1();
		LCD_line_1();
		_delay_ms(2);
		Send_A_String("Thank You :-)");
		_delay_ms(2);
		_delay_ms(500);
		
		sensor_PARAMETER = 0;
		_delay_ms(10);
				
		while (14)
		{
			spi_tranceiver('P');
			packet = SPDR;
			
			if (packet==221)
			{
				//_delay_ms(10);
			}
			
			else
			if (packet == 'o')
			{				
				packet=0;
				
				Clear_LCD_line_1();
				LCD_line_1();
				_delay_ms(2);
				Send_A_String("Come Again :-)");
				_delay_ms(20);
				_delay_ms(500);
				
				commParameter = true;
				break;
			}
			
			else
			if (packet == 'q')
			{				
				packet=0;
				
				Clear_LCD_line_1();
				LCD_line_1();
				_delay_ms(2);
				Send_A_String("Come Again Soon !!");
				//_delay_ms(200);					
			}
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
	DDRB |= (1<<PINB6);     //MISO as OUTPUT
	PORTB &=~ (1<<PINB6);
		
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
	// Enable SPI, Set as Master
	//Prescaler: Fosc/16, Enable Interrupts
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
/////////////////
void Check_IF_MrLCD_isBusy(void)
{
	DataDir_MrLCDsCrib=0;
	MrLCDsControl |= 1<<ReadWrite;
	MrLCDsControl &=~ 1<<BiPolarMood;
	
	while(MrLCDsCrib >= 0x80)
	{
		Peek_A_Boo();
	}
	
	DataDir_MrLCDsCrib=0xFF;
}
void Peek_A_Boo(void)
{
	MrLCDsControl |= 1<<LightSwitch;
	_delay_us(10);/////CALIBRATE IT FURTHER
	MrLCDsControl &=~  1<<LightSwitch;
} 
void Send_A_Command(char command)
{
	Check_IF_MrLCD_isBusy();
	MrLCDsCrib = command;
	MrLCDsControl &=~ (1<<ReadWrite|1<<BiPolarMood);
	Peek_A_Boo();
	MrLCDsCrib = 0;
}
void Send_A_Character(char character)
{
	Check_IF_MrLCD_isBusy();
	MrLCDsCrib = character;
	MrLCDsControl &=~ (1<<ReadWrite);
	MrLCDsControl |= (1<<BiPolarMood);
	Peek_A_Boo();
	MrLCDsCrib = 0;
}
void delete_a_character(char *character)
{
	while(*character > 0)
	{
		*character--;
		Send_A_Character(*character);
		_delay_ms(1);
	}
	
}
void Send_A_String(char *stringOfCharacters)
{
	while(*stringOfCharacters > 0)
	{
		Send_A_Character(*stringOfCharacters++);
		_delay_ms(40);
	}
}
void Send_A_String_instantly(char *stringOfCharacters)
{
	while(*stringOfCharacters > 0)
	{
		Send_A_Character(*stringOfCharacters++);
	}
}

void LCD_line_1 (void)
{
	_delay_ms(2);
	Send_A_Command(0x80);
	_delay_ms(2);
}
void LCD_line_2 (void)
{
	_delay_ms(2);
	Send_A_Command(0xC0);
	_delay_ms(2);
}
void LCD_line_3 (void)
{
	_delay_ms(2);
	Send_A_Command(0x94);
	_delay_ms(2);
}
void LCD_line_4 (void)
{
	_delay_ms(2);
	Send_A_Command(0xD4);
	_delay_ms(2);
}

void Clear_LCD_line_1 (void)
{
	_delay_ms(2);
	Send_A_Command(0x80);
	_delay_ms(2);
	_delay_ms(2);
	Send_A_String_instantly("                    ");
	_delay_ms(2);
}
void Clear_LCD_line_2 (void)
{
	_delay_ms(2);
	Send_A_Command(0xC0);
	_delay_ms(2);
	_delay_ms(2);
	Send_A_String_instantly("                    ");
	_delay_ms(2);
}
void Clear_LCD_line_3 (void)
{
	_delay_ms(2);
	Send_A_Command(0x94);
	_delay_ms(2);
	_delay_ms(2);
	Send_A_String_instantly("                    ");
	_delay_ms(2);
}
void Clear_LCD_line_4 (void)
{
	_delay_ms(2);
	Send_A_Command(0xD4);
	_delay_ms(2);
	_delay_ms(2);
	Send_A_String_instantly("                    ");
	_delay_ms(2);
}


// 		Send_A_Command(0x01);//clrscr();
// 		_delay_ms(2);
// 		Send_A_String("incoming");
// 		_delay_ms(2);

// 		while (8)
// 		{
	// 			spi_tranceiver('a');
	// 			packet = SPDR;
	// 			if (packet == 'q')
	// 			{
		// 				Send_A_Command(0x01);//clrscr();
		// 				_delay_ms(2);
		// 				Send_A_String("done");
		// 				_delay_ms(2);
	// 			}
// 		}
// 
// while (43)
// {
// 	spi_tranceiver(0xCC);
// 	packet = SPDR;
// 	
// 	if (packet== 'a')
// 	{
// 		_delay_ms(15);
// 		Send_A_Command(0x01);//clrscr();
// 		_delay_ms(2);
// 		Send_A_String("received 'a' ");
// 		_delay_ms(10);
// 	}
// }	
// 	while (34)
// 	{
	// 		if (bit_is_set(PINC,1))
	// 		{
	// 			if (t==0)
	// 			{
	// 				t=1;
	// 				_delay_ms(15);
	// 				Send_A_Command(0x01);//clrscr();
	// 				_delay_ms(2);
	// 				Send_A_String("reading sensor");
	// 				_delay_ms(10);
	// 			}
	// 		}
	// 		else
	// 		if (bit_is_clear(PINC,1))
	// 		{
	// 			if (t==1)
	// 			{
	// 				t=0;
	// 				
	// 				_delay_ms(15);
	// 				Send_A_Command(0x01);//clrscr();
	// 				_delay_ms(2);
	// 				Send_A_String("waiting for sig");
	// 				_delay_ms(10);
	// 			}
	// 		}
	// 	}