#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "usbdrv.h"
#include "oddebug.h"

#include "atmega_bmp085_lib.h"
#include "twi.h"
#include "si7021.h"

#define VERSION_MAJOR 0
#define VERSION_MINOR 1

typedef struct _env {
	volatile long temperature;
	volatile unsigned long pressure;
	volatile long temperature2;
	volatile unsigned long humidity;
} env;

env environment;
int16_t BMP085_calibration_int16_t[8];
int16_t BMP085_calibration_uint16_t[3];

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
					penv = (env *)replyBuf;
					penv->temperature = environment.temperature;
					penv->pressure = environment.pressure;
					penv->temperature2 = environment.temperature2;
					penv->humidity = environment.humidity;
					return sizeof(environment);
					break;
				case 2: // controller map
					break;
				case 3: // controller map
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
	int32_t temperature = 0;
	int32_t pressure = 0;
	#define CNTMAX 120000
	long cnt = CNTMAX;

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

	environment.temperature = 77;
	environment.humidity = 66;
	environment.pressure = 2;
	environment.temperature2 = 88;

	i2cSetBitrate(1000);        //Initialize TWI 1000kHz
	BMP085_Calibration(BMP085_calibration_int16_t, BMP085_calibration_uint16_t);////Initialize BMP085

	/* main event loop */
	for(;;) {
        cnt++;
		usbPoll();
        if(cnt>CNTMAX) {
			bmp085Convert(BMP085_calibration_int16_t, BMP085_calibration_uint16_t,&temperature, &pressure);
			environment.temperature = temperature;
			environment.pressure = pressure;
		}

		usbPoll();
        if(cnt>CNTMAX) {
			environment.temperature2 = SI7021_temperature();
		}

		usbPoll();
        if(cnt>CNTMAX) {
			environment.humidity = SI7021_humidity();
			cnt = 0;
		}
	}
	return 0;
}
