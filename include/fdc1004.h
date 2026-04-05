/**
 * @file fdc1004.h
 * @author Jakub Jasiejko
 * @date 2025-03-30
 * @brief Public interface for the FDC1004 capacitive sensor driver.
 *
 * Provides configuration and data acquisition helpers for the FDC1004 over I2C,
 * including optional PCA9548 channel selection.
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
  * @brief Converts a capacitance offset in pF to the CAPDAC register value.
  *
  * @param offset Offset in picofarads (maximum about 97.625 pF)
  * @return CAPDAC code in the range 0..31
  */
 uint8_t calculateCapdac(float offset);
 
 /**
  * @brief Configures a single-ended FDC1004 measurement.
  *
  * @param PCAaddr Address of the PCA9548 multiplexer
  * @param pcaChannel PCA9548 channel mask to enable
  * @param meas Measurement slot number (0..3)
  * @param cin Input channel CIN (0..3)
  * @param offset Capacitance offset in pF used for CAPDAC compensation
  */
 void FDC1004_configureMeasurement(uint8_t PCAaddr, uint8_t pcaChannel, uint8_t meas, uint8_t cin, float offset);
 
 /**
  * @brief Configures a differential measurement using two CIN inputs.
  *
  * @param PCAaddr Address of the PCA9548 multiplexer
  * @param pcaChannel PCA9548 channel mask to enable
  * @param meas Measurement slot number (0..3)
  * @param cin1 Positive input channel
  * @param cin2 Negative input channel
  */
 void FDC1004_configureDifferencialMeasurement(uint8_t PCAaddr, uint8_t pcaChannel, uint8_t meas, uint8_t cin1, uint8_t cin2);
 
 /**
  * @brief Triggers a single conversion for the selected measurement slot.
  *
  * @param PCAaddr Address of the PCA9548 multiplexer
  * @param pcaChannel PCA9548 channel mask to enable
  * @param meas Measurement slot number (0..3)
  */
 void FDC1004_triggerMeasurement(uint8_t PCAaddr, uint8_t pcaChannel, uint8_t meas);
 
 /**
  * @brief Reads a single-ended measurement result and converts it to attofarads.
  *
  * @param pcaAddr Address of the PCA9548 multiplexer
  * @param pcaChannel PCA9548 channel to enable
  * @param meas Measurement slot number (0..3)
  * @param offset_pf CAPDAC offset in picofarads
  * @return Capacitance value in attofarads
  */
  float FDC1004_getCapacitanceAttofarads(uint8_t pcaAddr, uint8_t pcaChannel, uint8_t meas, float offset_pf);
 
 /**
  * @brief Reads a differential measurement result and converts it to picofarads.
  * 
  * Uses raw MSB/LSB data from the sensor, converts the Q17.6 signed 24-bit
  * value and scales it to physical units.
  *
  * @param pcaAddr Address of the PCA9548 multiplexer
  * @param pcaChannel PCA9548 channel to enable
  * @param meas Measurement slot number (0..3)
  * @return Capacitance value in picofarads
  */
 
  float FDC1004_getCapacitancePicoFarradsDifferential(uint8_t pcaAddr, uint8_t pcaChannel, uint8_t meas);
 
 /**
  * @brief Automatically adjusts CAPDAC and returns the measured capacitance.
  *
  * @param pcaAddr Address of the PCA9548 multiplexer
  * @param pcaChannel PCA9548 channel to enable
  * @param meas Measurement slot number (0..3)
  * @param cin Input channel CIN (0..3)
  * @return Capacitance value in attofarads
  */
  float FDC1004_getCapacitanceWithAutoCapdac(uint8_t pcaAddr, uint8_t pcaChannel, uint8_t meas, uint8_t cin);


 #ifdef __cplusplus
 }
 #endif
 
 #endif // FDC1004_H
 
