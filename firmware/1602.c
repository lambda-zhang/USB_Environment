#include "1602.h"
#include <avr/io.h>

/* RS    0---cmd   1---data
 * RW    0---write 1---read
 * EN    trailing edge---write 1---read
 *
 */

#define LCD_CTL_PORT    PORTD
#define LCD_CTL_PIN     PIND
#define LCD_CTL_DDR     DDRD
#define LCD_RS         (1<<5) //   portd5       out
#define LCD_RW         (1<<6) //   portd6       out
#define LCD_EN         (1<<7) //   portd7       out

#define LCD_DATA_PORT  PORTC
#define LCD_DATA_DDR   DDRC
#define LCD_DATA_PIN   PINC
#define LCD_DATA       ((1<<0)|(1<<1)|(1<<2)|(1<<3)) //0xf0   portc 0/1/2/3 out
/*----------------------------------------------------------------------------*/
//void delay_1us(void)
void delay_1us(void)
{
	asm("nop");
}

void delay_nus(unsigned int n)
{
	volatile unsigned int i=0;
	for (i=0;i<n;i++)
		delay_1us();
}

void delay_1ms(void)
{
	volatile unsigned int i;
	for (i=0;i<1140;i++);
}

void delay_nms(unsigned int n)
{
	volatile unsigned int i=0;
	for (i=0;i<n;i++)
		delay_1ms();
}

/*----------------------------------------------------------------------------*/
void LCD_en_write(void)
{
	LCD_CTL_PORT = LCD_CTL_PIN | LCD_EN;
	delay_nus(3);
	LCD_CTL_PORT = LCD_CTL_PIN & (~LCD_EN);
}

void check_busy()
{
	unsigned char busyflag;
	LCD_DATA_DDR &= ~LCD_DATA;
	do {
		LCD_CTL_PORT = LCD_CTL_PIN & (~LCD_RS);
		LCD_CTL_PORT = LCD_CTL_PIN | LCD_RW;
		LCD_CTL_PORT = LCD_CTL_PIN | LCD_EN;
		busyflag = LCD_DATA_PIN & 0x08;
	} while(busyflag);
	LCD_DATA_DDR |= LCD_DATA;
}

/*----------------------------------------------------------------------------*/
void LCD_write_command(unsigned char command)
{
	delay_nus(16);
	//check_busy();
	LCD_CTL_PORT = LCD_CTL_PIN & (~LCD_RS);
	LCD_CTL_PORT = LCD_CTL_PIN & (~LCD_RW);
	LCD_CTL_PORT = LCD_CTL_PIN & (~LCD_EN);

	LCD_DATA_PORT = (LCD_DATA_PIN & 0xf0) | ((command >> 4) & 0x0f);
	LCD_en_write();
	LCD_DATA_PORT = (LCD_DATA_PIN & 0xf0) | (command & 0x0f);
	LCD_en_write();

	delay_nus(16);
}
/*----------------------------------------------------------------------------*/
void LCD_init(void)
{
	LCD_DATA_DDR |= LCD_DATA;
	LCD_CTL_DDR |= LCD_EN;
	LCD_CTL_DDR |= LCD_RW;
	LCD_CTL_DDR |= LCD_RS;

//	LCD_write_command(0x28);
//	LCD_en_write();
//	delay_nus(40);
//	LCD_write_command(0x28);
//	LCD_write_command(0x0c);
//	LCD_write_command(0x01);
//	delay_nms(2);

	LCD_write_command(0x20);
	delay_nms(40);
	//check_busy();
	LCD_write_command(four_bus | twoline_on | fxs);//0x28
	LCD_write_command(fulltext_movoff | cursor_right);//0x06
	LCD_write_command(display_on | cursor_off | cursor_flashoff);//0x0d
	LCD_write_command(clear_screen);//0x01
	delay_nms(40);
}


/*----------------------------------------------------------------------------*/
void LCD_write_data(unsigned char data)
{
	delay_nus(16);
	LCD_CTL_PORT = LCD_CTL_PIN | LCD_RS;        //RS=1
	LCD_CTL_PORT = LCD_CTL_PIN & (~LCD_RW);
	LCD_CTL_PORT = LCD_CTL_PIN & (~LCD_EN);

	LCD_DATA_PORT = (LCD_DATA_PIN & 0xf0) | ((data >> 4) & 0x0f);
	LCD_en_write();
	LCD_DATA_PORT = (LCD_DATA_PIN & 0xf0) | (data & 0x0f);
	LCD_en_write();
	delay_nus(16);
}
/*----------------------------------------------------------------------------*/
void LCD_set_xy( unsigned char x, unsigned char y )
{
	unsigned char address;
	if (y == 0) address = 0x80 + x;
	else   address = 0xc0 + x;
	LCD_write_command( address);
}
/*----------------------------------------------------------------------------*/
void LCD_write_string(unsigned char X,unsigned char Y,char *s)
{
	LCD_set_xy( X, Y );
	while (*s) {
		LCD_write_data( *s );
		s ++;
	}
}

/*----------------------------------------------------------------------------*/
void LCD_write_char(unsigned char X,unsigned char Y,unsigned char data)
{
	LCD_set_xy( X, Y );
	LCD_write_data( data);

}

void lcd_test(void)
{
   LCD_init();
   LCD_write_string(0, 0, "Welcome !");
   LCD_write_string(0, 1, "I am lambda !");
}

void lcd1602_puts(const char *s)
{
	int pos = 0;
	char *str = s;

	while (*str) {
		if (pos == 0)
			LCD_set_xy(0, 0);
		if (pos == 16)
			LCD_set_xy(0, 1);
		if (pos > 31)
			break;

		LCD_write_data(*str);
		str++;
		pos++;
	}
}

