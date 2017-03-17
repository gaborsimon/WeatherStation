#ifndef LCDMANAGER_H_
#define LCDMANAGER_H_


//====== Header includes =======================================================
#include <avr/pgmspace.h>

#include "typedef.h"

#include "LCDCharacters_HD44780.h"
#include "LCDDriver_HD44780.h"
#include "MicroControllerHAL.h"
#include "MoonFractionCalculator.h"
#include "RealTimeClock.h"
#include "Sensor_GPS.h"
#include "Sensor_DHT22.h"
#include "Sensor_DS1621.h"
#include "SunTimeCalculator.h"


//====== Public Constants ======================================================
#define LCM__DATETIME        (1u)
#define LCM__YEAR            (2u)
#define LCM__MONTH           (3u)
#define LCM__DAY             (4u)
#define LCM__HOUR            (5u)
#define LCM__MINUTE          (6u)
#define LCM__SUNTIME         (7u)
#define LCM__MOONFRACTION    (8u)
#define LCM__DHT22           (9u)
#define LCM__RX_OK          (10u)
#define LCM__RX_NO          (11u)
#define LCM__RX_NONE        (12u)

#define LCM__CONTROL_METHOD_STATIC      (1u)
#define LCM__CONTROL_METHOD_ADAPTIVE    (2u)


//====== Public Signals ========================================================


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void LCM_Init(void);
    extern void LCM_Refresh(uint8 _Element);
    extern void LCM_BackLightControl(uint8 _ControlMethod, uint8 _PWMDutyCycle);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* LCDMANAGER_H_ */
