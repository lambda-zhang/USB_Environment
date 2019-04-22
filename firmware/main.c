#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "usbdrv.h"
#include "oddebug.h"

#define VERSION_MAJOR 0
#define VERSION_MINOR 1

typedef struct _env {
	volatile unsigned short temperature;
	volatile unsigned short humidity;
	volatile unsigned short pressure;
} env;

env environment;

void set_led(int val)
{
    (val == 0) ? (PORTB &= ~(1<<2)) : (PORTB |= (1<<2));
}

uchar usbFunctionSetup(uchar data[8]) {
	env *penv;
	static uchar replyBuf[128];
	usbMsgPtr = replyBuf;
	//uchar len = (data[1] & 7)+1;       // 1 .. 4 bytes
	uchar target = (data[1] >> 3) & 3; // target 0 .. 3

    /* command format:
     * 7 6 5 4   3 2 1 0
     * |-+-| |-+-| |-+-|
     *   |     |     +------- number of bytes in transfer
     *   |     +------------- target
     *   +------------------- command
     *
     */
	switch(data[1] >> 5) {
		case 0: // echo (for transfer reliability testing)
			replyBuf[0] = data[2];
			replyBuf[1] = data[3];
			return 2;
			break;
		case 1: // command
			break;
		case 2: // data
			break;
		case 3: // set
			switch(target) {
				case 0:
                    set_led(data[2]);
					break;
				case 1:
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
				case 1: // all
					environment.temperature++;
					environment.humidity++;
					environment.pressure++;
					penv = (env *)replyBuf;
					penv->temperature = environment.temperature;
					penv->humidity = environment.humidity;
					penv->pressure = environment.pressure;
					return sizeof(environment);
					break;
				case 2: // controller map
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

int main(void)
{
	//	wdt_enable(WDTO_1S);
	/* let debug routines init the uart if they want to */
	odDebugInit();

	/* all outputs except INT0 and RxD/TxD */
	DDRD &= ~(_BV(2) | _BV(1) | _BV(0));
	PORTD = 0;

	/* USB Reset by device only required on Watchdog Reset */
	_delay_loop_2(40000);   // 10ms
	usbInit();
	sei();

	DDRB = 0xff;
	PORTB &= ~(1<<2);

	environment.temperature = 0;
	environment.humidity = 1;
	environment.pressure = 2;

	/* main event loop */
	for(;;) {
		//		wdt_reset();
		usbPoll();
	}
	return 0;
}
