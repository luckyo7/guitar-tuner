#include <driver/i2s.h>

#ifndef I2S_H
#define I2S_H
#define I2S_PORT I2S_NUM_0

void i2s_install();
void i2s_setpin();
void i2s_loop();

bool checkMovingAverage(float currentFrequency, int movSize,
                        float previousFrequencies[], float maxError);

void appendFrequency(float currentFrequency, float previousFrequencies[],
                     int movSize);
#endif
