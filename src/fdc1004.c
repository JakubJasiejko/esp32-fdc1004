/**
 * @file fdc1004.c
 * @author inż. Jakub Jasiejko
 * @date 2025-03-30
 * @brief Obsługa czujnika pojemnościowego FDC1004 z wykorzystaniem I2C i multipleksera PCA9548.
 *
 * Plik zawiera funkcje do konfiguracji i odczytu danych z czujnika FDC1004.
 * Obsługuje ustawianie kanałów pomiarowych, offsetów CAPDAC, wyzwalanie pomiarów
 * oraz przeliczanie wyniku na wartość pojemności w jednostkach fizycznych.
 */

 #include "fdc1004.h"
 #include "i2c.h"
 #include "uart.h"
 #include <stdio.h>
 #include <string.h>
 #include "esp_timer.h"
 #include "esp_rom_sys.h"
 #include <math.h>

 
 /**
  * @brief Oblicza wartość CAPDAC w oparciu o offset (w pF).
  *
  * Funkcja przelicza offset pojemności na wartość CAPDAC zgodnie z dokumentacją FDC1004.
  * CAPDAC może kompensować pojemności w krokach co 3.125 pF do 97.625 pF.
  *
  * @param offset Offset w pikofaradach (maksymalnie ok. 97.625 pF)
  * @return Wartość CAPDAC (0-31)
  */
 uint8_t calculateCapdac(float offset) {
     float capdac_float = offset / 3.125f;
     if (capdac_float > 31.0f) capdac_float = 31.0f;
     return (uint8_t)capdac_float;
 }
 
 /**
  * @brief Konfiguruje pomiar typu single-ended na czujniku FDC1004.
  *
  * Ustawia wybrane wejście pomiarowe CIN, aktywuje CAPDAC oraz zapisuje konfigurację
  * do odpowiedniego rejestru MEASx.
  *
  * @param PCAaddr Adres multipleksera PCA9548
  * @param pcaChannel Kanał PCA9548 do aktywacji
  * @param meas Numer kanału pomiarowego (0-3)
  * @param cin Numer wejścia CIN (0-3)
  * @param offset Offset pojemności (w pF)
  */
 void FDC1004_configureMeasurement(uint8_t PCAaddr, uint8_t pcaChannel, uint8_t meas, uint8_t cin, float offset) {
     uint8_t reg = 0x08 + meas;
     uint8_t capdac = calculateCapdac(offset);
     uint16_t configData = 0;
     configData |= (cin << 13);
     configData |= (0b100 << 10);
     configData |= (capdac << 5);
 
     pca9548channel(PCAaddr, pcaChannel);
     if (i2c_write16(FDC1004_ADDRESS, reg, configData) == 0x0000) {
         return;
     }
 
     errorOnUART(0xCC);
 }
 
 /**
  * @brief Konfiguruje pomiar różnicowy pomiędzy dwoma wejściami CIN.
  *
  * Używane, gdy wymagany jest pomiar różnicowy pomiędzy dwoma wejściami.
  * CAPDAC nie jest aktywowany w trybie różnicowym.
  *
  * @param PCAaddr Adres multipleksera PCA9548
  * @param pcaChannel Kanał PCA9548
  * @param meas Numer kanału MEAS (0-3)
  * @param cin1 Wejście dodatnie
  * @param cin2 Wejście ujemne
  */
 void FDC1004_configureDifferencialMeasurement(uint8_t PCAaddr, uint8_t pcaChannel, uint8_t meas, uint8_t cin1, uint8_t cin2) {
     uint8_t reg = 0x08 + meas;
     uint16_t configData = 0;
     configData |= (cin1 << 13);
     configData |= (cin2 << 10);
     configData |= (0b00000 << 5);
 
     pca9548channel(PCAaddr, pcaChannel);
     if (i2c_write16(FDC1004_ADDRESS, reg, configData) == 0x0000) {
         return;
     }
 
     errorOnUART(0xCC);
 }
 
 /**
  * @brief Wyzwala pojedynczy pomiar na wskazanym kanale pomiarowym.
  *
  * Funkcja ustawia rejestr triggera czujnika FDC1004, aby rozpoczać konwersję.
  *
  * @param PCAaddr Adres multipleksera PCA9548
  * @param pcaChannel Kanał PCA9548
  * @param meas Numer kanału MEAS (0-3)
  */
 void FDC1004_triggerMeasurement(uint8_t PCAaddr, uint8_t pcaChannel, uint8_t meas) {
     uint16_t trigger_data = 0;
     trigger_data |= (0b11 << 10);
     trigger_data |= (0 << 8);
     trigger_data |= (1 << (7 - meas));
 
     pca9548channel(PCAaddr, pcaChannel);
     if (i2c_write16(FDC1004_ADDRESS, FDC1004_TRIGGER_REG, trigger_data) == 0x0000) {
         return;
     }
 }
 
 /**
  * @brief Odczytuje wartość pomiaru single-ended i przelicza na femtofarady.
  *
  * Dane są odczytywane z rejestrów MSB i LSB, przeliczane zgodnie z empiricznym współczynnikiem 457.
  *
  * @param pcaAddr Adres multipleksera PCA9548
  * @param pcaChannel Kanał PCA9548
  * @param meas Numer kanału MEAS (0-3)
  * @param offset Wartość CAPDAC (w pF)
  * @return Pojemność w femtofaradach (uint32_t)
  */
  float FDC1004_getCapacitanceAttofarads(uint8_t pcaAddr, uint8_t pcaChannel, uint8_t meas, float offset_pf) {
    uint8_t msbReg = meas * 2;
    uint8_t lsbReg = msbReg + 1;

    pca9548channel(pcaAddr, pcaChannel);

    int64_t start_time = esp_timer_get_time();
    const int64_t timeout_us = 1000000;

    while (esp_timer_get_time() - start_time < timeout_us) {
        uint16_t status = i2c_read16(FDC1004_ADDRESS, FDC1004_TRIGGER_REG);
        if (status != 0xFFFF && (status & (1 << (3 - meas)))) break;
        esp_rom_delay_us(500);
    }

    uint16_t msb = i2c_read16(FDC1004_ADDRESS, msbReg);
    uint16_t lsb = i2c_read16(FDC1004_ADDRESS, lsbReg);

    if (msb == 0xAAAA || lsb == 0xAAAA) {
        errorOnUART(0xDE);
        return 0;
    }

    int32_t rawData = ((uint32_t)msb << 16 | lsb) >> 8;  // Q17.6 format, 24-bit
    if (rawData & 0x800000) {
        rawData = -((~rawData + 1) & 0xFFFFFF);  // 2's complement
    }

    // Przeliczenie na attofarady: 1 LSB = 1.953125 aF
    float capacitance_af = rawData * 1.953125f;

    // Dodanie offsetu (przekazanego w pikofaradach, trzeba przeliczyć na aF)
    capacitance_af += offset_pf * 1000000.0f;

    return capacitance_af;
}

 
 /**
  * @brief Odczytuje wartość pomiaru różnicowego i przelicza ją na pikofarady.
  *
  * Format danych czujnika to Q17.6 (24-bit signed). Funkcja wykonuje obcięcie,
  * rozszerzenie znaku, przeliczenie do float i skalowanie.
  *
  * @param pcaAddr Adres multipleksera PCA9548
  * @param pcaChannel Kanał PCA9548
  * @param meas Numer kanału MEAS (0-3)
  * @return Wartość pojemności w pikofaradach (float)
  */
 float FDC1004_getCapacitancePicoFarradsDifferential(uint8_t pcaAddr, uint8_t pcaChannel, uint8_t meas) {
     uint8_t msbReg = meas * 2;
     uint8_t lsbReg = msbReg + 1;
 
     pca9548channel(pcaAddr, pcaChannel);
 
     int64_t start_time = esp_timer_get_time();
     const int64_t timeout_us = 1000000;
 
     while (esp_timer_get_time() - start_time < timeout_us) {
         uint16_t status = i2c_read16(FDC1004_ADDRESS, FDC1004_TRIGGER_REG);
         if (status != 0xFFFF && (status & (1 << (3 - meas)))) break;
         esp_rom_delay_us(500);
     }
 
     uint16_t msb = i2c_read16(FDC1004_ADDRESS, msbReg);
     uint16_t lsb = i2c_read16(FDC1004_ADDRESS, lsbReg);
 
     if (msb == 0xAAAA || lsb == 0xAAAA) {
         errorOnUART(0xDE);
         return 0;
     }
 
     float data = 0;
     uint32_t rawData = ((uint32_t)msb << 16) | lsb;
     rawData = (rawData >> 8) & 0xFFFFFF;
 
     if (rawData & 0x800000) {
         data = -1 * ((float)((~rawData + 1) & 0xFFFFFF));
     } else {
         data = (float)rawData;
     }
 
     data = (data / 524288.0f); //Dzielenie przez 2^19
 
     #ifdef FDC_DEBUG
     uart_write_bytes(UART_PORT, "MSB bin: ", 9);
     binaryDebug(msb);
     uart_write_bytes(UART_PORT, "\r\n", 2);
 
     uart_write_bytes(UART_PORT, "LSB bin: ", 9);
     binaryDebug(lsb);
     uart_write_bytes(UART_PORT, "\r\n", 2);
 
     char debugBuf[128];
     snprintf(debugBuf, sizeof(debugBuf),
              "[FDC1004 DEBUG] MSB: 0x%04X, LSB: 0x%04X, RAW: 0x%06lX, pF: %.3f\r\n",
              msb, lsb, (unsigned long)(rawData & 0xFFFFFF), data);
     uart_write_bytes(UART_PORT, debugBuf, strlen(debugBuf));
     #endif
 
     return data;
 }
 

 float FDC1004_getCapacitanceWithAutoCapdac(uint8_t pcaAddr, uint8_t pcaChannel, uint8_t meas, uint8_t cin) {
    float offset_pf = 0.0f;
    uint8_t capdac = 0;
    const uint8_t max_capdac = 31;

    while (1) {
        FDC1004_configureMeasurement(pcaAddr, pcaChannel, meas, cin, offset_pf);
        vTaskDelay(pdMS_TO_TICKS(10));
        FDC1004_triggerMeasurement(pcaAddr, pcaChannel, meas);
        vTaskDelay(pdMS_TO_TICKS(10));

        float capacitance_af = FDC1004_getCapacitanceAttofarads(pcaAddr, pcaChannel, meas, offset_pf);

        // 15 pF = 15,000,000 aF
        if ((capacitance_af >= (offset_pf + 15.0f) * 1000000.0f || capacitance_af <= 0.0f) && capdac < max_capdac) {
            capdac++;
            offset_pf = capdac * 3.125f;
            continue;
        }

#ifdef FDC_DEBUG
        char debugBuf[128];
        snprintf(debugBuf, sizeof(debugBuf),
                 "[AUTO CAPDAC] aF: %.0f, CAPDAC: %d\r\n",
                 capacitance_af, capdac);
        uart_write_bytes(UART_PORT, debugBuf, strlen(debugBuf));
#endif

        return capacitance_af;
    }
}
