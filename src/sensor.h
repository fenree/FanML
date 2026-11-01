#ifndef SENSOR_H_
#define SENSOR_H_
#include <stdint.h>
#include "Particle.h"
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

typedef struct { int16_t x,y,z; } ADXL345_DATA;

void start_sensor(void);
ADXL345_DATA read_sensor(void);

#endif // SENSOR_H_
