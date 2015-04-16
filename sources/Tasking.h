#ifndef TASKING_H_
#define TASKING_H_


//====== Header includes =======================================================
#include <avr/interrupt.h>

#include "typedef.h"

#include "LCDCharacters_HD44780.h"
#include "LCDDriver_HD44780.h"
#include "LCDManager.h"
#include "MicroControllerHAL.h"
#include "MoonFractionCalculator.h"
#include "RealTimeClock.h"
#include "Sensor_DHT22.h"
#include "Sensor_DS1621.h"
#include "SunTimeCalculator.h"


//====== Public Constants ======================================================


//====== Public Signals ========================================================


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void Task_Init(void);
    extern void Task_Main(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* TASKING_H_ */
