#include <util/delay.h>
#include "twi.h"
#include "si7021.h"

void SI70XX_Write_UserRegister(unsigned char Data)
{
	i2cSendStart();

	i2cSendByte(0x80);
	checki2cReturnCode(TW_MT_SLA_ACK);

	i2cSendByte(CMD_WRITE_REGISTER_1);
	checki2cReturnCode(TW_MT_DATA_ACK);

	i2cSendByte(Data);
	checki2cReturnCode(TW_MT_DATA_ACK);
	i2cSendStop();

	return;
}

unsigned char SI70XX_Read_UserRegister(void)
{
	unsigned char Data;

	i2cSendStart();
	checki2cReturnCode(TW_START);

	i2cSendByte(0x80);  // write 0x80
	checki2cReturnCode(TW_MT_SLA_ACK);

	i2cSendByte(CMD_READ_REGISTER_1);   // write register address
	checki2cReturnCode(TW_MT_DATA_ACK);

	i2cSendByte(0x81);  // 地址读
	i2cWaitForComplete();
	checki2cReturnCode(TW_MR_SLA_ACK);

	i2cReceiveByteACK();
	i2cWaitForComplete();
	Data = i2cGetReceivedByte();
	checki2cReturnCode(TW_MR_DATA_NACK);
	i2cSendStop();

	return(Data);
}

unsigned char SI70XX_Sensor_Measure( unsigned char Config, unsigned int *Value )
{
	unsigned char Data[2] = {7, 8};

	i2cSendStart();
	i2cWaitForComplete();

	checki2cReturnCode(TW_START);
	i2cSendByte(0x80);  // write 0x80
	i2cWaitForComplete();

	checki2cReturnCode(TW_MT_SLA_ACK);
	i2cSendByte(Config);   // write register address
	i2cWaitForComplete();
	checki2cReturnCode(TW_MT_DATA_ACK);

	_delay_ms(20);
	i2cSendStart();
	i2cWaitForComplete();
	checki2cReturnCode(TW_REP_START);
	i2cSendByte(0x81);  // 地址读
	i2cWaitForComplete();
	checki2cReturnCode(TW_MR_SLA_ACK);

	i2cReceiveByteACK();
	i2cWaitForComplete();
	Data[0] = i2cGetReceivedByte();     // Get MSB result
	i2cWaitForComplete();
	checki2cReturnCode(TW_MR_DATA_ACK);

	i2cReceiveByteNACK();
	i2cWaitForComplete();
	Data[1] = i2cGetReceivedByte();     // Get LSB result
	i2cWaitForComplete();
	checki2cReturnCode(TW_MR_DATA_NACK);
	i2cSendStop();

	/* Swap the bytes and clear the status bits */
	*Value = ((Data[0]*256) + Data[1]) & ~3;   //*Value = (Data[0]*256) + Data[1];
	return(1);
}

long SI7021_temperature()
{
	unsigned int Value;
	unsigned char ucConversion_OK=0;//转换成功标记
	ucConversion_OK = SI70XX_Sensor_Measure(CMD_MEASURE_TEMPERATURE_HOLD, &Value);
	if(!ucConversion_OK) {//转换失败，直接退出，方便模块热插拔
		return -100;//错误=0
	}
	return (((((long)Value)*1757)>>16) - 469);
}

unsigned long SI7021_humidity()
{
	unsigned int Value;
	unsigned char ucConversion_OK=0;//转换成功标记
	ucConversion_OK = SI70XX_Sensor_Measure(CMD_MEASURE_HUMIDITY_HOLD, &Value);
        if(!ucConversion_OK) {//转换失败，直接退出，方便模块热插拔
                return 1000;//错误=0
        }
	return (((((long)Value)*625)>>15) - 60);
}
