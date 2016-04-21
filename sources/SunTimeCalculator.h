#ifndef SUNTIMECALCULATOR_H_
#define SUNTIMECALCULATOR_H_


//====== Header includes =======================================================
#include <math.h>

#include "typedef.h"
#include "utils.h"

#include "LCDDriver_HD44780.h"
#include "RealTimeClock.h"


//====== Public Constants ======================================================


//====== Public Signals ========================================================
typedef struct
{
    uint8 RiseHour;
    uint8 RiseMinute;
    uint8 SetHour;
    uint8 SetMinute;
} STC_Sun_s;


#define XSTC__SUN_RISEHOUR    ((uint8) (STC_Sun.RiseHour))
#define XSTC__SUN_RISEMINUTE  ((uint8) (STC_Sun.RiseMinute))
#define XSTC__SUN_SETHOUR     ((uint8) (STC_Sun.SetHour))
#define XSTC__SUN_SETMINUTE   ((uint8) (STC_Sun.SetMinute))

extern STC_Sun_s STC_Sun;


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void STC_Refresh(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* SUNTIMECALCULATOR_H_ */
