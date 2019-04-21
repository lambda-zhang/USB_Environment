/* Name: main.c
 * Project: LCD2USB; lcd display interface based on AVR USB driver
 * Author: Till Harbaum
 * Creation Date: 2006-01-20
 * Tabsize: 4
 * Copyright: (c) 2005 by Till Harbaum <till@harbaum.org>
 * License: GPL
 * This Revision: $Id: main.c,v 1.2 2007/01/14 12:12:27 harbaum Exp $
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include <util/delay.h>

#include "lcd.h"

// use avrusb library
#include "usbdrv.h"
#include "oddebug.h"

#include "1602.h"

#define VERSION_MAJOR 1
#define VERSION_MINOR 9
#define VERSION_STR "1.09"
// change USB_CFG_DEVICE_VERSION in usbconfig.h as well

// EEMEM wird bei aktuellen Versionen der avr-lib in eeprom.h definiert
// hier: definiere falls noch nicht bekannt ("alte" avr-libc)
#ifndef EEMEM
// alle Textstellen EEMEM im Quellcode durch __attribute__ ... ersetzen
#define EEMEM  __attribute__ ((section (".eeprom")))
#endif

/* bitmask of detected lcd controllers */
uchar controller = 0;

/* ------------------------------------------------------------------------- */
uchar	usbFunctionSetup(uchar data[8]) {
	static uchar replyBuf[8];
	usbMsgPtr = replyBuf;
	uchar len = (data[1] & 3)+1;       // 1 .. 4 bytes
	uchar target = (data[1] >> 3) & 3; // target 0 .. 3
	uchar i;

	// request byte:

	// 7 6 5 4 3 2 1 0
	// C C C T T R L L

	// TT = target bit map
	// R = reserved for future use, set to 0
	// LL = number of bytes in transfer - 1

	switch(data[1] >> 5) {

		case 0: // echo (for transfer reliability testing)
			replyBuf[0] = data[2];
			replyBuf[1] = data[3];
			return 2;
			break;

		case 1: // command
			target &= controller;  // mask installed controllers

			if(target) // at least one controller should be used ...
				for(i=0;i<len;i++)
					LCD_write_command(data[2+i]);
					//lcd_command(target, data[2+i]);
			break;

		case 2: // data
			target &= controller;  // mask installed controllers

			if(target) // at least one controller should be used ...
				for(i=0;i<len;i++)
					LCD_write_data(data[2+i]);
					//lcd_data(target, data[2+i]);
			break;

		case 3: // set
			switch(target) {

				case 0:  // contrast
					//set_contrast(data[2]);
					break;

				case 1:  // brightness
					//set_brightness(data[2]);
					break;

				default:
					// must not happen ...
					break;   
			}
			break;

		case 4: // get
			switch(target) {
				case 0: // version
					replyBuf[0] = VERSION_MAJOR;
					replyBuf[1] = VERSION_MINOR;
					return 2;
					break;

				case 1: // keys
					replyBuf[0] = ((PINC & _BV(5))?0:1) |
						((PINB & _BV(0))?0:2);
					replyBuf[1] = 0;
					return 2;
					break;

				case 2: // controller map
					replyBuf[0] = controller;
					replyBuf[1] = 0;
					return 2;
					break;   

				case 3: // controller map
					replyBuf[0] = 'l';
					replyBuf[1] = 'a';
					replyBuf[2] = 'm';
					replyBuf[3] = 'b';
					replyBuf[4] = 'd';
					replyBuf[5] = 'd';
					return 6;
					break;   

				default:
					// must not happen ...
					break;   
			}
			break;

		default:
			// must not happen ...
			break;
	}

	return 0;  // reply len
}

/* ------------------------------------------------------------------------- */

int main(void)
{

	lcd_test();

	//	wdt_enable(WDTO_1S);

	/* let debug routines init the uart if they want to */
	odDebugInit();

	/* all outputs except INT0 and RxD/TxD */
	DDRD &= ~(_BV(2) | _BV(1) | _BV(0));
	PORTD = 0;

	/* USB Reset by device only required on Watchdog Reset */
	_delay_loop_2(40000);   // 10ms

	usbInit();

#if 0
	pwm_init();

	DDRC &= ~_BV(5);         /* input S1 */
	PORTC |= _BV(5);         /* with pullup */
	DDRB &= ~_BV(0);         /* input S2 */
	PORTB |= _BV(0);         /* with pullup */

	/* try to init two controllers */
	if(lcd_init(LCD_CTRL_0)) controller |= LCD_CTRL_0;
	if(lcd_init(LCD_CTRL_1)) controller |= LCD_CTRL_1;

	/* put string to display (line 1) with linefeed */
	if(controller & LCD_CTRL_0)
		lcd_puts(LCD_CTRL_0, "LCD2USB V" VERSION_STR);

	if(controller & LCD_CTRL_1)
		lcd_puts(LCD_CTRL_1, "2nd ctrl");

	if((controller & LCD_CTRL_0) && (controller & LCD_CTRL_1))
		lcd_puts(LCD_CTRL_0 | LCD_CTRL_1, " both!");
#endif
	controller |= LCD_CTRL_0;

	sei();

        DDRB = 0xff;
        PORTB &= ~(1<<2);

	for(;;) {	/* main event loop */
		//		wdt_reset();
		usbPoll();
	}
	return 0;
}

/* ------------------------------------------------------------------------- */
