#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "usbdrv.h"
#include "oddebug.h"

#include "twi.h"

#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define BMP085_R 0xEF
#define BMP085_W 0xEE
#define OSS 0
#define FALSE 0
#define TRUE 1

typedef struct _env {
	volatile unsigned long temperature;
//	volatile unsigned long humidity;
	volatile unsigned long pressure;
} env;

env environment;

/////=========Global Variables======////////////////////
short ac1;
short ac2;
short ac3;
unsigned short ac4;
unsigned short ac5;
unsigned short ac6;
short b1;
short b2;
short mb;
short mc;
short md;

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
					//penv->humidity = environment.humidity;
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

void delay_ms(uint16_t x)
{
	uint8_t y, z;
	for ( ; x > 0 ; x--){
		for ( y = 0 ; y < 90 ; y++){
			for ( z = 0 ; z < 6 ; z++){
				asm volatile ("nop");
			}
		}
	}
}

// bmp085ReadShort will read two sequential 8-bit registers, and return a 16-bit value
// the MSB register is read first
// Input: First register to read
// Output: 16-bit value of (first register value << 8) | (sequential register value)
short bmp085ReadShort(unsigned char address)
{
	char msb, lsb;
	short data;

	twi_start();

	twi_write_data(BMP085_W);  // write 0xEE

	twi_write_data(address);   // write register address

	twi_start();

	twi_write_data(BMP085_R);  // write 0xEF

	msb = twi_read_data(TRUE);        //ack

	lsb = twi_read_data(FALSE);     //no ack

	twi_stop();

	data = msb << 8;
	data |= lsb;

	return data;
}

void BMP085_Calibration(void)
{
	ac1 = bmp085ReadShort(0xAA);
	ac2 = bmp085ReadShort(0xAC);
	ac3 = bmp085ReadShort(0xAE);
	ac4 = bmp085ReadShort(0xB0);
	ac5 = bmp085ReadShort(0xB2);
	ac6 = bmp085ReadShort(0xB4);
	b1 = bmp085ReadShort(0xB6);
	b2 = bmp085ReadShort(0xB8);
	mb = bmp085ReadShort(0xBA);
	mc = bmp085ReadShort(0xBC);
	md = bmp085ReadShort(0xBE);

	//printf("\tAC1 = %d\n", ac1);
	//printf("\tAC2 = %d\n", ac2);
	//printf("\tAC3 = %d\n", ac3);
	//printf("\tAC4 = %d\n", ac4);
	//printf("\tAC5 = %d\n", ac5);
	//printf("\tAC6 = %d\n", ac6);
	//printf("\tB1 = %d\n", b1);
	//printf("\tB2 = %d\n", b2);
	//printf("\tMB = %d\n", mb);
	//printf("\tMC = %d\n", mc);
	//printf("\tMD = %d\n", md);
}

long bmp085ReadTemp(void)
{
	twi_start();

	twi_write_data(BMP085_W);  // write 0xEE

	twi_write_data(0xF4);  // write register address

	twi_write_data(0x2E);  // write register data for temp

	twi_stop();

	delay_ms(10);   // max time is 4.5ms

	return (long) bmp085ReadShort(0xF6);
}

long bmp085ReadPressure(void)
{
	long pressure = 0;

	twi_start();

	twi_write_data(BMP085_W);  // write 0xEE

	twi_write_data(0xF4);  // write register address

	twi_write_data(0x34);  // write register data for temp

	twi_stop();

	delay_ms(10);   // max time is 4.5ms

	pressure = bmp085ReadShort(0xF6);
	pressure &= 0x0000FFFF;

	return pressure;

	//return (long) bmp085ReadShort(0xF6);
}

void bmp085Convert(long* temperature, long* pressure)
{
	long ut;
	long up;
	long x1, x2, b5, b6, x3, b3, p;
	unsigned long b4, b7;

	ut = bmp085ReadTemp();
	ut = bmp085ReadTemp();  // some bug here, have to read twice to get good data
	up = bmp085ReadPressure();
	up = bmp085ReadPressure();

	x1 = ((long)ut - ac6) * ac5 >> 15;
	x2 = ((long) mc << 11) / (x1 + md);
	b5 = x1 + x2;
	*temperature = (b5 + 8) >> 4;

	b6 = b5 - 4000;
	x1 = (b2 * (b6 * b6 >> 12)) >> 11;
	x2 = ac2 * b6 >> 11;
	x3 = x1 + x2;
	b3 = (((int32_t) ac1 * 4 + x3) + 2)/4;
	x1 = ac3 * b6 >> 13;
	x2 = (b1 * (b6 * b6 >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (ac4 * (unsigned long) (x3 + 32768)) >> 15;
	b7 = ((unsigned long) up - b3) * (50000 >> OSS);
	p = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2;
	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	*pressure = p + ((x1 + x2 + 3791) >> 4);
}

int main(void)
{
	long temperature = 0;
	long pressure = 0;
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

	twi_init();
	BMP085_Calibration();
	environment.temperature = 12;
	//environment.humidity = 1;
	environment.pressure = 2;

	/* main event loop */
	for(;;) {
		//		wdt_reset();
		bmp085Convert(&temperature, &pressure);
		environment.temperature = temperature;
		environment.pressure = pressure;
		usbPoll();
	}
	return 0;
}
