#ifndef TPIC8101_H
#define TPIC8101_H

#include <SPI.h>
#include "globals.h"
#include "crankMaths.h"

#include BOARD_H //Note that this is not a real file, it is defined in globals.h.

const uint8_t TPIC_CS_PIN = 28;
const uint8_t INT_HOLD_PIN = 29;
static volatile uint16_t tpic8101_last_rev = 0;
static volatile int16_t tpic8101_rev_delta = 0;
static volatile uint8_t tpic8101_cmd;
static volatile uint8_t tpic8101_msb;
static volatile uint8_t tpic8101_lsb;
static volatile uint16_t tpic8101_rv;

extern SPISettings tpic8101_spi_settings;

static const unsigned char tpic8101_tc_lookup[561] = {
    // 40-49us -> index 0-1
    0, 0, 0, 1, 1, 1, 2, 2, 2, 2,           // 40-49
    2, 3, 3, 3, 3, 4, 4, 4, 4, 4,           // 50-59
    5, 5, 5, 5, 5, 6, 6, 6, 6, 6,           // 60-69
    7, 7, 7, 7, 7, 8, 8, 8, 8, 8,           // 70-79
    8, 8, 8, 8, 8, 9, 9, 9, 9, 9,           // 80-89
    9, 9, 9, 9, 9, 10, 10, 10, 10, 10,      // 90-99
    10, 10, 10, 10, 10, 11, 11, 11, 11, 11, // 100-109
    11, 11, 11, 11, 11, 12, 12, 12, 12, 12, // 110-119
    12, 12, 12, 12, 12, 13, 13, 13, 13, 13, // 120-129
    13, 13, 13, 13, 13, 14, 14, 14, 14, 14, // 130-139
    14, 14, 14, 14, 14, 15, 15, 15, 15, 15, // 140-149
    15, 15, 15, 15, 15, 16, 16, 16, 16, 16, // 150-159
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, // 160-169
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, // 170-179
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17, // 180-189
    18, 18, 18, 18, 18, 18, 18, 18, 18, 18, // 190-199
    18, 18, 18, 18, 18, 18, 18, 18, 18, 18, // 200-209
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, // 210-219
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, // 220-229
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, // 230-239
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, // 240-249
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, // 250-259
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, // 260-269
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, // 270-279
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, // 280-289
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, // 290-299
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, // 300-309
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, // 310-319
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, // 320-329
    25, 25, 25, 25, 25, 25, 25, 25, 25, 25, // 330-339
    25, 25, 25, 25, 25, 25, 25, 25, 25, 25, // 340-349
    25, 25, 25, 25, 25, 25, 25, 25, 25, 25, // 350-359
    25, 25, 25, 25, 25, 25, 25, 25, 25, 25, // 360-369
    25, 25, 25, 25, 25, 25, 25, 25, 25, 25, // 370-379
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, // 380-389
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, // 390-399
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, // 400-409
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, // 410-419
    27, 27, 27, 27, 27, 27, 27, 27, 27, 27, // 420-429
    27, 27, 27, 27, 27, 27, 27, 27, 27, 27, // 430-439
    27, 27, 27, 27, 27, 27, 27, 27, 27, 27, // 440-449
    27, 27, 27, 27, 27, 27, 27, 27, 27, 27, // 450-459
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28, // 460-469
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28, // 470-479
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28, // 480-489
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28, // 490-499
    29, 29, 29, 29, 29, 29, 29, 29, 29, 29, // 500-509
    29, 29, 29, 29, 29, 29, 29, 29, 29, 29, // 510-519
    29, 29, 29, 29, 29, 29, 29, 29, 29, 29, // 520-529
    29, 29, 29, 29, 29, 29, 29, 29, 29, 29, // 530-539
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, // 540-549
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, // 550-559
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, // 560-569
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, // 570-579
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, // 580-589
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, // 590-599
    31                                      // 600
};

#define TPIC8101_CHANNEL_IDX 0
#define TPIC8101_PRESCALER_IDX 3
#define TPIC8101_ADVANCED_MODE_CMD 0x71
#define TPIC8101_SDO_STATUS 0
#define TPIC8101_DEFAULT_TC 26

void initTPIC8101(void);

inline uint8_t tpic8101_send(uint8_t data)
{
    SPI.beginTransaction(tpic8101_spi_settings);
    digitalWrite(TPIC_CS_PIN, LOW);
    uint8_t response = SPI.transfer(data);
    digitalWrite(TPIC_CS_PIN, HIGH);
    SPI.endTransaction();
    return response;
}

inline uint16_t tpic8101_read(void)
{
    delayMicroseconds(50);
    tpic8101_cmd = 0b01000000 | (TPIC8101_PRESCALER_IDX << 1);
    tpic8101_msb = tpic8101_send(tpic8101_cmd);
    tpic8101_cmd = 0b11100000 | (TPIC8101_CHANNEL_IDX & 0x01);
    tpic8101_lsb = tpic8101_send(tpic8101_cmd);

    // Response: MSB = bits [7:0], LSB = bits [9:8] in top 2 bits
    return ((tpic8101_msb & 0xC0) << 2) | tpic8101_lsb;
}

inline void tpic8101_set_prescaler(uint8_t freqCode, uint8_t sdo)
{
    tpic8101_cmd = 0b01000000 | (freqCode << 1); // freqCode = 0–8 (e.g. 2 for 6 MHz)
    tpic8101_cmd &= (0x1 & sdo);
    tpic8101_send(tpic8101_cmd);
}

inline void tpic8101_set_channel(uint8_t ch)
{
    tpic8101_cmd = 0b11100000 | (ch & 0x01);
    tpic8101_send(tpic8101_cmd);
}

inline void tpic8101_set_filter(uint8_t index)
{
    tpic8101_cmd = 0b00000000 | (index & 0x3F);
    tpic8101_send(tpic8101_cmd);
}

inline void tpic8101_set_gain(uint8_t index)
{
    tpic8101_cmd = 0b10000000 | (index & 0x3F);
    tpic8101_send(tpic8101_cmd);
}

inline void tpic8101_set_timeconstant(uint8_t index)
{
    tpic8101_cmd = 0b11000000 | (index & 0x1F);
    tpic8101_send(tpic8101_cmd);
}

inline uint8_t find_integrator_index(uint32_t time_constant_us)
{
    // Handle edge cases
    if (time_constant_us <= 40)
        return 0;
    if (time_constant_us >= 600)
        return 31;

    // Direct lookup
    return tpic8101_tc_lookup[time_constant_us - 40];
}

inline void tpic8101_update(void)
{
    tpic8101_rev_delta = (int32_t)(tpic8101_last_rev - currentStatus.RPM);
    if (tpic8101_rev_delta >= 400 || tpic8101_rev_delta <= -400)
    {
        tpic8101_last_rev = currentStatus.RPM;
        //Update tau
        tpic8101_set_timeconstant(find_integrator_index((angleToTimeMicroSecPerDegree(configPage13.knockWindow) * 0x1999) >> 16));
        // Update gain
        tpic8101_set_gain(configPage13.knockGain);
        // Update freq
        tpic8101_set_filter(configPage13.knockFrequency);
    }
}

#endif