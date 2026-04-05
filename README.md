# esp32-fdc1004

Biblioteka do obsługi `FDC1004` na ESP32 z gotowymi funkcjami do konfiguracji pomiaru, triggerowania i przeliczania wyniku pojemności.

## Co zawiera
- konfigurację pomiaru single-ended i differential
- automatyczny trigger i odczyt wyników
- pomocniczą logikę `CAPDAC`

## Zależności
- `esp32-i2c-tools`
- `esp32-uart-xor` dla debugowania

## Pliki
- `include/fdc1004.h`
- `src/fdc1004.c`

## Szybki start
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

Pełny przykład znajdziesz w `examples/basic_usage/basic_usage.c`.
