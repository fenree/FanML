/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

SYSTEM_MODE(AUTOMATIC);

SerialLogHandler logHandler(LOG_LEVEL_INFO);

// see table 26 in ADXL345 datasheet
#define ADXL345_REGS(X)  \
    X(DEVID,       0x00) \
    X(BW_RATE,     0x2C) \
    X(POWER_CTL,   0x2D) \
    X(INT_ENABLE,  0x2E) \
    X(INT_MAP,	   0x2F) \
    X(DATA_FORMAT, 0x31) \
    X(DATAX0,      0x32)

#define X_ENUM(name, addr) ADXL345_REG_##name = addr,
enum { ADXL345_REGS(X_ENUM) };

#define ADXL345_MULTIBYTE 0x40
#define ADXL345_READ(reg)     (0x80 | (reg))
#define ADXL345_READ_MB(reg)  (ADXL345_READ(ADXL345_MULTIBYTE) | (reg))

/* Data layout:                                  *
 * | Bit 7 | Bit 6     | Bit 5-0 | Databyte(s) | *
 * | (R/W) | Multibyte | Address | Databyte(s) | */
 
/* datasheet page 14
After the register
addressing and the first byte of data, each subsequent set of clock
pulses (eight clock pulses) causes the ADXL345 to point to the next
register for a read or write. This shifting continues until the clock
pulses cease and CS is deasserted. To perform reads or writes on
different, nonsequential registers, CS must be deasserted between
transmissions and the new register must be addressed separately.*/
/*
        0x0F, // 0x2C BW_RATE: 3000 Hz
        0x00, // 0x2D POWER_CTL: BLANK
        0x80, // 0x2E INT_ENABLE: DATA_READY
        0x00, // 0x2F INT_MAP: all to INT1
		0x00, // 0x30 : BLANK
		0x08, // 0x31 DATA_FORMAT: +/-2g, full resolution
 * */


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


typedef struct { int16_t x,y,z; } ADXL345_DATA;

void read_sensor(void) {
	pinResetFast(SS);

	SPI.transfer(ADXL345_READ_MB(ADXL345_REG_DATAX0));
	
	struct { int16_t x,y,z; } data;
	SPI.transfer(NULL, &data, 6, NULL);

	pinSetFast(SS);
	// SCK ticked during tx byte so index0 is irrelevant
	// even numbers are LSB
	
	Serial.println(data.x);
	Serial.println(data.y);
	Serial.println(data.z);
}

void setup() {
  Serial.begin(115200);
  // using SPI, which uses the labeled SPI pins (MI, MO, SCK) on the dev board
  SPI.begin(SPI_MODE_MASTER, SS);
  /* max speed supported by sensor is 5 MHz but theoretically we 
     should be seeing 3.125 MHz due to clock divider constraints  */
  SPI.setClockSpeed(5, MHZ);
  /* CPHA = 1 and CPOL = 1 according to datasheet page 14 */
  SPI.setDataMode(SPI_MODE3);

  start_sensor();
  

}

void loop() {
	read_sensor();
    delay( 1000 );
}


