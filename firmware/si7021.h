#ifndef _SI7021_H_
#define _SI7021_H_ 1

// si7005 STROBE, CONTROL AND STATUS REGSITER配置寄存器定义
/* Si7005 Registers */
#define REG_STATUS         0x00
#define REG_DATA           0x01
#define REG_CONFIG         0x03
#define REG_ID             0x11

/* Status Register */
#define STATUS_NOT_READY   0x01

/* Config Register */
#define CONFIG_START       0x01
#define CONFIG_HEAT        0x02
#define CONFIG_HUMIDITY    0x00
#define CONFIG_TEMPERATURE 0x10
#define CONFIG_FAST        0x20

/* ID Register */
//#define ID_SAMPLE          0xF0
#define ID_SI7005          0x50

/* Coefficients */
#define TEMPERATURE_OFFSET   50
#define TEMPERATURE_SLOPE    32
#define HUMIDITY_OFFSET      24
#define HUMIDITY_SLOPE       16
#define SCALAR            16384
#define A0              (-78388)    /* -4.7844  * SCALAR */
#define A1                 6567     /*  0.4008  * SCALAR */
#define A2                 (-64)    /* -0.00393 * SCALAR */
#define Q0                 3233     /*  0.1973  * SCALAR */
#define Q1                   39     /*  0.00237 * SCALAR */

/* I2C slave address of Si70xx */
#define SI70XX_ADDR                     0x41

/* Commands */
#define CMD_MEASURE_HUMIDITY_HOLD       0xE5
#define CMD_MEASURE_HUMIDITY_NO_HOLD    0xF5
#define CMD_MEASURE_TEMPERATURE_HOLD    0xE3
#define CMD_MEASURE_TEMPERATURE_NO_HOLD 0xF3
#define CMD_MEASURE_THERMISTOR_HOLD     0xEE
#define CMD_READ_PREVIOUS_TEMPERATURE   0xE0
#define CMD_RESET                       0xFE
#define CMD_WRITE_REGISTER_1            0xE6
#define CMD_READ_REGISTER_1             0xE7
#define CMD_WRITE_REGISTER_2            0x50
#define CMD_READ_REGISTER_2             0x10
#define CMD_WRITE_REGISTER_3            0x51
#define CMD_READ_REGISTER_3             0x11
#define CMD_WRITE_COEFFICIENT           0xC5
#define CMD_READ_COEFFICIENT            0x84

/* User Register 1 */
#define REG1_RESOLUTION_MASK            0x81
#define REG1_RESOLUTION_H12_T14         0x00
#define REG1_RESOLUTION_H08_T12         0x01
#define REG1_RESOLUTION_H10_T13         0x80
#define REG1_RESOLUTION_H11_T11         0x81
#define REG1_LOW_VOLTAGE                0x40
#define REG1_ENABLE_HEATER              0x04

/* User Register 2 */
#define REG2_VOUT                       0x01
#define REG2_VREF_VDD                   0x02
#define REG2_VIN_BUFFERED               0x04
#define REG2_RESERVED                   0x08
#define REG2_FAST_CONVERSION            0x10
#define REG2_MODE_CORRECTION            0x20
#define REG2_MODE_NO_HOLD               0x40

/* Device Identification */
#define ID_SAMPLE                       0xFF
#define ID_SI7013                       0x0D
#define ID_SI7020                       0x14
#define ID_SI7021                       0x15

/* Coefficients */
#define COEFFICIENT_BASE                0x82
#define COEFFICIENT_COUNT               9


long SI7021_temperature();
unsigned long SI7021_humidity();
#endif
