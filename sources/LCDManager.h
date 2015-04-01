#ifndef LCDMANAGER_H_
#define LCDMANAGER_H_


//====== Header includes =======================================================
#include <avr/pgmspace.h>

#include "typedef.h"
#include "LCDCharacters_HD44780.h"
#include "LCDDriver_HD44780.h"
#include "MoonFractionCalculator.h"
#include "RealTimeClock.h"
#include "Sensor_DS1621.h"
#include "SunTimeCalculator.h"


//====== Public Constants ======================================================


//====== Public Signals ========================================================


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void LCM_Refresh(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* LCDMANAGER_H_ */
