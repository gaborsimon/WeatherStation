#ifndef REALTIMECLOCK_H_
#define REALTIMECLOCK_H_


//====== Header includes =======================================================
#include "typedef.h"
#include "utils.h"


//====== Public Constants ======================================================


//====== Public Signals ========================================================
typedef struct
{
    uint16      Year;
    G_Flag_e    NewYear;
    uint8       Month ;
    G_Flag_e    NewMonth;
    uint8       Day;
    G_Flag_e    NewDay;
    uint8       DayNumber;
    G_Flag_e    DST;
    uint8       Hour;
    G_Flag_e    NewHour;
    uint8       Minute;
    G_Flag_e    NewMinute;
    uint8       Second;
    G_Flag_e    NewSecond;
} RTC_TimeDate_s;

#define XRTC__TIMEDATE_YEAR         ((uint16)   (RTC_TimeDate.Year))
#define XRTC__TIMEDATE_MONTH        ((uint8)    (RTC_TimeDate.Month))
#define XRTC__TIMEDATE_DAY          ((uint8)    (RTC_TimeDate.Day))
#define XRTC__TIMEDATE_DAYNUMBER    ((uint8)    (RTC_TimeDate.DayNumber))
#define XRTC__TIMEDATE_DST          ((G_Flag_e) (RTC_TimeDate.DST))
#define XRTC__TIMEDATE_HOUR         ((uint8)    (RTC_TimeDate.Hour))
#define XRTC__TIMEDATE_MINUTE       ((uint8)    (RTC_TimeDate.Minute))
#define XRTC__TIMEDATE_SECOND       ((uint8)    (RTC_TimeDate.Second))

#define XRTC__TIMEDATE_NEWYEAR      ((G_Flag_e) (RTC_TimeDate.NewYear))
#define XRTC__TIMEDATE_NEWMONTH     ((G_Flag_e) (RTC_TimeDate.NewMonth))
#define XRTC__TIMEDATE_NEWDAY       ((G_Flag_e) (RTC_TimeDate.NewDay))
#define XRTC__TIMEDATE_NEWHOUR      ((G_Flag_e) (RTC_TimeDate.NewHour))
#define XRTC__TIMEDATE_NEWMINUTE    ((G_Flag_e) (RTC_TimeDate.NewMinute))
#define XRTC__TIMEDATE_NEWSECOND    ((G_Flag_e) (RTC_TimeDate.NewSecond))

#define XRTC__DAYNAME(x)            ((const char*)(RTC_DayName[(x)]))

extern RTC_TimeDate_s   RTC_TimeDate;
extern const char*      RTC_DayName[8u];


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void RTC_Refresh(void);
    extern void RTC_SetDate(uint16 _Year, uint8 _Month,  uint8 _Day,
                             uint8 _DayNumber, G_Flag_e _DST,
                             uint8 _Hour, uint8 _Minute, uint8 _Second);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* REALTIMECLOCK_H_ */
