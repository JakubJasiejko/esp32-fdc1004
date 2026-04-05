#include "fdc1004.h"

void app_main(void)
{
    initI2C();
    FDC1004_configureMeasurement(PCA_ADDR, PCA_CH0, 0, 0, 0.0f);
    FDC1004_triggerMeasurement(PCA_ADDR, PCA_CH0, 0);
    float value_pf = FDC1004_getCapacitancePicoFarradsDifferential(PCA_ADDR, PCA_CH0, 0);
    (void)value_pf;
}
