#include "sensor.h"

void sensorInterrupt() 
{
	sensorReady = true;
}

void start_sensor(void) {
	uint8_t tx[4][2] = {
		{ ADXL345_REG_BW_RATE,     0x0F }, // 3200 Hz
		{ ADXL345_REG_INT_ENABLE,  0x80 }, // Interupt on DATA_READY
		{ ADXL345_REG_DATA_FORMAT, 0x08 }, // +/-2g, full resolution
		{ ADXL345_REG_POWER_CTL,   0x08 }, // Set measure bit
	};

	for(size_t i = 0; i < sizeof(tx)/sizeof(tx[0]); i++) {
		pinResetFast(SS);
		SPI.transfer(tx[i], nullptr, 2, nullptr);
		pinSetFast(SS);
	}
}

ADXL345_DATA read_sensor(void) {
	pinResetFast(SS);

	SPI.transfer(ADXL345_READ_MB(ADXL345_REG_DATAX0));
	
	ADXL345_DATA  data;
	SPI.transfer(NULL, &data, 6, NULL);

	pinSetFast(SS);
	
	Serial.println(data.x);
	Serial.println(data.y);
	Serial.println(data.z);
	return data;
}


