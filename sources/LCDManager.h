#ifndef LCDMANAGER_H_
#define LCDMANAGER_H_


//====== Header includes =======================================================
#include <avr/pgmspace.h>

#include "typedef.h"

#include "LCDCharacters_HD44780.h"
#include "LCDDriver_HD44780.h"
#include "MoonFractionCalculator.h"
#include "RealTimeClock.h"
#include "Sensor_DHT22.h"
#include "Sensor_DS1621.h"
#include "SunTimeCalculator.h"


//====== Public Constants ======================================================
#define LCM_DATETIME        (1u)
#define LCM_YEAR            (2u)
#define LCM_MONTH           (3u)
#define LCM_DAY             (4u)
#define LCM_HOUR            (5u)
#define LCM_MINUTE          (6u)
#define LCM_SUNTIME         (7u)
#define LCM_MOONFRACTION    (8u)
#define LCM_DHT22           (9u)


//====== Public Signals ========================================================


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void LCM_Refresh(uint8 element);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* LCDMANAGER_H_ */
