/*
* @file fdc1004.h
* @author inż. Jakub Jasiejko
* @date 2025-03-30
* @brief Interfejs dla czujnika pojemnościowego FDC1004.
*
* Definiuje funkcje i stałe do konfiguracji i odczytu danych z czujnika FDC1004
* przez magistralę I2C z obsługą multipleksera PCA9548.
*/

 

 #ifndef FDC1004_H
 #define FDC1004_H
 
 #include "i2c.h"
 #include "uart.h"
 #include <stdint.h>
 #include <math.h>

 #define FDC1004_ADDRESS      0x50
 #define FDC1004_TRIGGER_REG  0x0C
 #define FDC_DEBUG

 #ifdef __cplusplus
 extern "C" {
 #endif
 
 /**
  * @brief Oblicza wartość CAPDAC w oparciu o offset (w pF).
  *
  * @param offset Offset w pikofaradach (max ~97.625 pF)
  * @return CAPDAC (0–31) jako liczba całkowita
  */
 uint8_t calculateCapdac(float offset);
 
 /**
  * @brief Konfiguruje pojedyńczy pomiar na FDC1004.
  *
  * @param PCAaddr Adres multipleksera PCA9548
  * @param pcaChannel Maska kanału PCA9548 do aktywacji
  * @param meas Numer kanału MEAS (0–3)
  * @param cin Wejście pomiarowe CIN (0–3)
  * @param offset Offset pojemności (pF) dla CAPDAC
  */
 void FDC1004_configureMeasurement(uint8_t PCAaddr, uint8_t pcaChannel, uint8_t meas, uint8_t cin, float offset);
 
 /**
  * @brief Konfiguruje pomiar różnicowy (dwa wejścia CIN).
  *
  * @param PCAaddr Adres multipleksera PCA9548
  * @param pcaChannel Maska kanału PCA9548 do aktywacji
  * @param meas Numer kanału MEAS (0–3)
  * @param cin1 Wejście dodatnie
  * @param cin2 Wejście ujemne
  */
 void FDC1004_configureDifferencialMeasurement(uint8_t PCAaddr, uint8_t pcaChannel, uint8_t meas, uint8_t cin1, uint8_t cin2);
 
 /**
  * @brief Wyzwala pojedynczy pomiar dla wskazanego kanału.
  *
  * @param PCAaddr Adres multipleksera PCA9548
  * @param pcaChannel Maska kanału PCA9548 do aktywacji
  * @param meas Numer kanału MEAS (0–3)
  */
 void FDC1004_triggerMeasurement(uint8_t PCAaddr, uint8_t pcaChannel, uint8_t meas);
 
 /**
  * @brief Odczytuje wynik pomiaru (single-ended) i przelicza na femtofarady.
  *
  * @param pcaAddr Adres multipleksera PCA9548
  * @param pcaChannel Kanał do aktywacji
  * @param meas Numer kanału MEAS (0–3)
  * @param offset Offset CAPDAC w pF
  * @return Pojemność w femtofaradach (uint32_t)
  */
  float FDC1004_getCapacitanceAttofarads(uint8_t pcaAddr, uint8_t pcaChannel, uint8_t meas, float offset_pf);
 
 /**
  * @brief Odczytuje wartość pomiaru różnicowego i zwraca wynik w pikofaradach (pF).
  * 
  * Używa surowych danych MSB/LSB z czujnika, konwertuje z formatu Q17.6
  * (24-bit ze znakiem), obcina do 24-bit, wykonuje korektę znaku i przelicza
  * na rzeczywiste jednostki pikofaradów.
  *
  * @param pcaAddr Adres multipleksera PCA9548
  * @param pcaChannel Kanał PCA9548
  * @param meas Kanał pomiarowy (0–3)
  * @return Wartość w pikofaradach (float)
  */
 
  float FDC1004_getCapacitancePicoFarradsDifferential(uint8_t pcaAddr, uint8_t pcaChannel, uint8_t meas);
 

  float FDC1004_getCapacitanceWithAutoCapdac(uint8_t pcaAddr, uint8_t pcaChannel, uint8_t meas, uint8_t cin);


 #ifdef __cplusplus
 }
 #endif
 
 #endif // FDC1004_H
 
