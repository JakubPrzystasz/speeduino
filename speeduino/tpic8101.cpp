#include "tpic8101.h"
#include "globals.h"
#include <SPI.h>

SPISettings tpic8101_spi_settings(5000000, MSBFIRST, SPI_MODE1); // Max 5 MHz SPI

void initTPIC8101(void)
{
    pinMode(TPIC_CS_PIN, OUTPUT);
    pinMode(INT_HOLD_PIN, OUTPUT);
    digitalWrite(TPIC_CS_PIN, HIGH);
    digitalWrite(INT_HOLD_PIN, LOW); 

    tpic8101_set_prescaler(TPIC8101_PRESCALER_IDX, TPIC8101_SDO_STATUS);        // 8 MHz oscillator input
    tpic8101_set_channel(TPIC8101_CHANNEL_IDX);                                 // Use Channel 1
    tpic8101_set_filter(configPage13.knockFrequency);
    tpic8101_set_gain(configPage13.knockGain);
    tpic8101_set_timeconstant(TPIC8101_DEFAULT_TC);
    
    tpic8101_send(TPIC8101_ADVANCED_MODE_CMD);  // Enable advanced mode for digital readout
}
