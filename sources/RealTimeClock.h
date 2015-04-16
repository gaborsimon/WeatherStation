#ifndef REALTIMECLOCK_H_
#define REALTIMECLOCK_H_


//====== Header includes =======================================================
#include "typedef.h"
#include "utils.h"


//====== Public Constants ======================================================


//====== Public Signals ========================================================
typedef struct
{
    uint8  Second;
    Flag   NewSecond;
    uint8  Minute;
    Flag   NewMinute;
    uint8  Hour;
    Flag   NewHour;
    uint8  Day;
    Flag   NewDay;
    uint8  Month ;
    Flag   NewMonth;
    uint16 Year;
    Flag   NewYear;
} TimeDate;


#define XRTC_TIMEDATE_SECOND ((uint8)  (RTC_TimeDate.Second))
#define XRTC_TIMEDATE_MINUTE ((uint8)  (RTC_TimeDate.Minute))
#define XRTC_TIMEDATE_HOUR   ((uint8)  (RTC_TimeDate.Hour))
#define XRTC_TIMEDATE_DAY    ((uint8)  (RTC_TimeDate.Day))
#define XRTC_TIMEDATE_MONTH  ((uint8)  (RTC_TimeDate.Month))
#define XRTC_TIMEDATE_YEAR   ((uint16) (RTC_TimeDate.Year))

#define XRTC_TIMEDATE_NEWSECOND  ((Flag) (RTC_TimeDate.NewSecond))
#define XRTC_TIMEDATE_NEWMINUTE  ((Flag) (RTC_TimeDate.NewMinute))
#define XRTC_TIMEDATE_NEWHOUR    ((Flag) (RTC_TimeDate.NewHour))
#define XRTC_TIMEDATE_NEWDAY     ((Flag) (RTC_TimeDate.NewDay))
#define XRTC_TIMEDATE_NEWMONTH   ((Flag) (RTC_TimeDate.NewMonth))
#define XRTC_TIMEDATE_NEWYEAR    ((Flag) (RTC_TimeDate.NewYear))

extern TimeDate RTC_TimeDate;


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void RTC_Refresh(void);
    extern void RTC_SetDate(uint16 year, uint8 month,  uint8 day,
                             uint8 hour, uint8 minute, uint8 second);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* REALTIMECLOCK_H_ */
