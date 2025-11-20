#ifndef ADC_H_KJJ
#define ADC_H_KJJ

#include <stdint.h>

struct SensorData {
    uint16_t x;
    uint16_t y;
    uint16_t z;
    uint16_t orientation;
};

int initializeADC(void);
struct SensorData readADCValue(uint16_t orientation);

#endif
