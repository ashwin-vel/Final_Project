#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>


// Define LCD memory addresses
#define LCD_START (*(char *)0x2000)
#define LCDD (*(char *)0x2100)

#define FLOWER_TIME 6000 // 5-second timeout


// Function prototypes
void lcd_init(void);
void lcd_puts(char *disp);
void lcd_start(char start);
void lcd_lcdd(char lcdd);
void UART_Init(void);
char getcha(void);
void UART_Transmit(char data);

int main(void)
{
	MCUCR = 0x80; // Enable external memory interface
	DDRB = 0x0F;  // PORTB as output (pumps)
	DDRD = 0x00;  // PORTD as input (sensors)
	DDRE = 0x00;  // PORTE as input for drink selection buttons
	lcd_init();
	UART_Init();
	
	// used as flag to not constantly initalize the LCD
	char lastb = -1;
	char yflowing = 0; // used to keep a time for dispensing (not as accurate)
	int timer = 0;
	char cstat = '\0'; // Default: "Select Drink :)"
	char command;
	
	// used to only send the command when there is an error 
	int noerror = 0; 
	// Main loop
	
	
	lcd_init();
	lcd_puts("SafeFlow Starting... !");
	lcd_start(0xC0);
	int loading=0;
	
	while (loading <= 15)
	{
		lcd_lcdd(0x7E);
		_delay_ms(500);
		loading ++;
	}

	
	while (1)
	{
		// check all the sensor first

		// the sensor for overflow and spillage
		if ((PINE & 0x01) != 0x01) //#5
		{
			if (lastb != 45)
			{
				PORTB = 0x00;
				lcd_init();
				lcd_puts("OV/Spillage !");
				lastb = 45;
			}

			cstat = 'F';
			noerror = 1;
			yflowing=0;
		}

		// sensor to indicate that sensor detect full level on user cup
		else if ((PINE & 0x04) == 0x04) //#6
		{
			if (lastb != 46)
			{
				PORTB = 0x00;
				lcd_init();
				lcd_puts("Done!");
				lastb = 46;
			}

			cstat = 'D';
			noerror = 1;
			yflowing=0;
		}

		// sensor to indicate that sensor detect to fill up coke
		else if ((PIND & 0x04) == 0x04)//#7
		{
			PORTB = 0x00;
			if (lastb != 1)
			{

				lcd_init();
				lcd_puts("Fill up Coke...");
				lastb = 1;
			}
			cstat = 'K';
			noerror = 1;
			yflowing=0;
		}
		// sensor to indicate that sensor detect to fill up sprite
		else if ((PIND & 0x08) == 0x08)//#8
		{
			PORTB = 0x00;
			if (lastb != 2)
			{
				lcd_init();
				lcd_puts("Fill up Sprite...");
				lastb = 2;
			}
			cstat = 'M';
			noerror = 1;
			yflowing=0;
		}
		// sensor to indicate that sensor detect to fill up water
		else if ((PIND & 0x10) == 0x10) //#12
		{
			PORTB = 0x00;
			if (lastb != 3)
			{
				lcd_init();
				lcd_puts("Fill up Water...");
				lastb = 3;
			}
			cstat = 'P';
			noerror = 1;
			yflowing=0;
		}
		// sensor to indicate that sensor detect to detect cup
		else if ((PIND & 0x20) == 0x20) //#13-
		
		{
			PORTB = 0x00;
			if (lastb != 4)
			{
				lcd_init();
				lcd_puts("Waiting for Cup");
				lastb = 4;
			}
			cstat = 'J';
			noerror = 1;
			yflowing=0;
		}

		else
		{

		if (noerror == 1)    // bits 0 and 2 are overflow/full sensors
		{
			cstat = '#';
			if (lastb != 5)
			{
				lcd_init();
				lcd_puts("Select Drink :)");
				lastb = 5;
			}
			noerror = 0;
		}
			else
			{
				command = getcha();
				if (command != '\0') // do this if there is a character to process
				{
					if (command == 'C')
					{
						if (yflowing)
						{
							PORTB = 0x00; // Turn off pump if already dispensing
						}
						else
						{
							PORTB = 0x01; // Activate pump
						}

						lcd_init();

						if (yflowing)
						{
							lcd_puts("Select Drink :)");
						}
						else
						{
							lcd_puts("Dispensing Coke");
						}

						yflowing = !yflowing; //so it set yflowing flag to one 
						timer = 0; //puts timer to zero

						if (yflowing)
						{
							cstat = 'Q'; // Drink is flowing
						}
						else
						{
							cstat = 'X'; // Drink is idle
						}
					}

					else if (command == 'S')
					{
						if (yflowing)
						{
							PORTB = 0x00; // turn off pump if alrady dispensing
						}
						else
						{
							PORTB = 0x02; // activate Sprite pump
						}

						lcd_init();

						if (yflowing)
						{
							lcd_puts("Select Drink :)");
						}
						else
						{
							lcd_puts("Dispensing Sprite");
						}

						yflowing = !yflowing;
						timer = 0;

						if (yflowing)
						{
							cstat = 'R'; // Dispensing Sprite
						}
						else
						{
							cstat = 'X'; // Idle
						}
					}
					else if (command == 'W')
					{
						if (yflowing)
						{
							PORTB = 0x00; // turn off pump if already dispensing
						}
						else
						{
							PORTB = 0x04; // activate water pump
						}

						lcd_init();

						if (yflowing)
						{
							lcd_puts("Select Drink :)");
						}
						else
						{
							lcd_puts("Dispensing Water");
						}

						yflowing = !yflowing;
						timer = 0;

						if (yflowing)
						{
							cstat = 'T'; // Dispensing Water
						}
						else
						{
							cstat = 'X';
						}
					}
					
					else if (command == 'G')
					{
						PORTB=0x00;
						yflowing =0;
						timer=0;
						cstat= '#';
						lcd_init();
						lcd_puts("Select Drink :)");
						
					}
		
						
					else
					{
						PORTB = 0x00;
						if (lastb != 56)
						{
							lcd_init();
							lcd_puts("Select Drink :)");
							lastb = 56;
						}
						
					}
				}
			}
		} 
		// if the command is not nul then process it
		if (cstat != '\0')
		{
			UART_Transmit(cstat);
		}

		_delay_ms(200);
		// if no command is received to stop disp, then wait for x ammount
		// seconds and stop dispensing

		if (yflowing)
		{
			_delay_ms(100);
			timer += 100;			  // increase timer by 100ms
			if (timer >= FLOWER_TIME) // if reached max time
			{
				PORTB = 0x00;
				lcd_init();
				lcd_puts("Select Drink :)");
				yflowing = 0; // reset time to zero
				cstat = 'X';  // send charac to update the UI
			}
		}
		
		
	
	}
}
// LCD FUNC
void lcd_init(void)
{
	lcd_start(0x38);
	_delay_us(37);
	lcd_start(0x0F);
	_delay_us(37);
	lcd_start(0x01);
	_delay_ms(1.52);
	lcd_start(0x06);
	_delay_us(37);
}
void lcd_start(char start)
{
	*(char *)0x2000 = start;
	_delay_us(37);
}
void lcd_lcdd(char lcdd)
{
	*(char *)0x2100 = lcdd;
	_delay_us(100);
}
void lcd_puts( char *disp)
{
	while (*disp)
	{
		lcd_lcdd(*disp++);
		_delay_ms(10);
	}
}
// UART FUNC
void UART_Init(void)
{
	//setting baud rate to 9600 with U2X bit high
	UBRRH = 0x00; 
	UBRRL = 12;
	
	//enables both tx and rx
	UCSRB = 0x18;
	
	//setting frame format (8bits, no parity and one stop bit)
	UCSRC = 0x86; 
	
	//setting U2X bit high for double the trans. speed
	UCSRA = 0x02;
}
char getcha(void)
{
	if (UCSRA & 0x80) // only give valid charac
	{
		return UDR;
	}
	return '\0'; // send a null charac if no data there
}

void UART_Transmit(char data)
{
	while (!(UCSRA & 0x20)) // only give valid charac (change accordinly
	{
	}
	UDR = data;
}
