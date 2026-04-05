# esp32-fdc1004

`FDC1004` driver for ESP32 with ready-to-use helpers for measurement configuration, triggering, and capacitance conversion.

## Features
- single-ended and differential measurement setup
- measurement triggering and result readout
- helper logic for `CAPDAC`

## Dependencies
- `esp32-i2c-tools`
- optional `esp32-uart-xor` for debug output

## Files
- `include/fdc1004.h`
- `src/fdc1004.c`

## Quick start
```c
#include "fdc1004.h"

void app_main(void)
{
    initI2C();
    FDC1004_configureMeasurement(PCA_ADDR, PCA_CH0, 0, 0, 0.0f);
    FDC1004_triggerMeasurement(PCA_ADDR, PCA_CH0, 0);
    float value_af = FDC1004_getCapacitanceAttofarads(PCA_ADDR, PCA_CH0, 0, 0.0f);
    (void)value_af;
}
```

See `examples/basic_usage/basic_usage.c` for a complete example.
