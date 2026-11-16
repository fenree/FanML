/*#########################*\
# Project   FanML           #
# Author    Mads Bay Jensen #
\*#########################*/
#include "Particle.h"
#include "sensor.h"

SYSTEM_MODE(AUTOMATIC);

SerialLogHandler logHandler(LOG_LEVEL_INFO);

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

  pinMode(S3, INPUT);
  attachInterrupt(S3, sensorInterrupt, RISING);
}

void loop() {
	read_sensor();
    delay( 1000 );

    if (!sensorReady) {
        delay(1);
        return;
    }

    noInterrupts();
    sensorReady = false;
    interrupts();

    ADXL345_DATA data = read_sensor();
}
