#ifndef SUNTIMECALCULATOR_H_
#define SUNTIMECALCULATOR_H_


//====== Header includes =======================================================
#include <math.h>

#include "typedef.h"
#include "utils.h"

#include "RealTimeClock.h"
#include "LCDDriver_HD44780.h"


//====== Public Constants ======================================================


//====== Public Signals ========================================================
typedef struct
{
    uint8 RiseHour;
    uint8 RiseMinute;
    uint8 SetHour;
    uint8 SetMinute;
} Sun;


#define XSTC_SUN_RISEHOUR    ((uint8) (STC_Sun.RiseHour))
#define XSTC_SUN_RISEMINUTE  ((uint8) (STC_Sun.RiseMinute))
#define XSTC_SUN_SETHOUR     ((uint8) (STC_Sun.SetHour))
#define XSTC_SUN_SETMINUTE   ((uint8) (STC_Sun.SetMinute))


extern Sun STC_Sun;


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void STC_Refresh(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* SUNTIMECALCULATOR_H_ */
