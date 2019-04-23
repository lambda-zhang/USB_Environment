//#################################################################################
//#################################################################################
//#################################################################################
/*	Library to use BMP085 with ATMEL Atmega family

	Copyright (C) 2010 Stefan Sicklinger

	For support check out http://www.sicklinger.com

	This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/
//#################################################################################
//#################################################################################
//#################################################################################

#ifndef _ATMEGA_BMP085_LIB_H_
#define _ATMEGA_BMP085_LIB_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <util/twi.h>
#include <math.h>
//----------------------------------------
// Set Ports
//----------------------------------------
//###########################
// I n p u t s
//###########################
#define input_DDR DDRC       //DDR for EOC
#define input_PORT PORTC     //PORT for EOC
#define input_PIN PINC       //PIN for EOC
#define BMP085_pin PC2       //Pin of EOC
//----------------------------------------
// Set BMP085 oversampling
//----------------------------------------
#define OSS 2	// Oversampling Setting works for 0,1,2 NOT for 3 (not implemented)
//----------------------------------------
// Do NOT change
//----------------------------------------
#define BMP085_R 0xEF
#define BMP085_W 0xEE
//----------------------------------------
// Prototypes
//----------------------------------------
void input_init(void);
void BMP085_Calibration(int16_t BMP085_calibration_int16_t[],int16_t BMP085_calibration_uint16_t[], uint8_t* errorcode);
uint16_t bmp085ReadShort(uint8_t address, uint8_t* errorcode);
int32_t bmp085ReadTemp(uint8_t* error_code);
int32_t bmp085ReadPressure(uint8_t* errorcode);
void bmp085Convert(int16_t BMP085_calibration_int16_t[],int16_t BMP085_calibration_uint16_t[],int32_t* temperature, int32_t* pressure, uint8_t* error_code);
int32_t bmp085CalcAltitude(int32_t pressure);
#endif
