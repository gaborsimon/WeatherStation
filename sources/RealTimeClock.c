//====== Header includes =======================================================
#include "RealTimeClock.h"


//====== Private Constants =====================================================
#define L__FEB_IN_LEAP_YEAR (0u)
#define L__FEB              (2u)

static const uint8 L_DayNumberOfMonth[13u] =
{
    29u,    // February in a leap Year
    31u,    // January
    28u,    // February in a not leap year
    31u,    // March
    30u,    // April
    31u,    // May
    30u,    // June
    31u,    // July
    31u,    // August
    30u,    // September
    31u,    // October
    30u,    // November
    31u     // December
};


//====== Private Signals =======================================================


//====== Private Function Prototypes ===========================================
static uint8    L_EndDayOfMonth(void);
static uint8    L_IsLeapYear(void);
static uint8    L_DayOfWeek(uint16 _Year, uint8 _Month, uint8 _Day);
static G_Flag_e L_DaylightSavingTime(uint8 _Month, uint8 _Day, uint8 _DoW);


//====== Private Functions =====================================================
/*
 * Name: L_EndDayOfMonth
 *
 * Description: This function calculates the last day of the current month.
 *              It considers the leap year for February.
 *
 * Input: Month
 *
 * Output: Last day number of the current month
 */
static uint8 L_EndDayOfMonth(void)
{
    uint8 _Result = U__INIT_VALUE_UINT;

    
    // If there is February and Leap Year then February contains 29 days
    if ((L__FEB == RTC_TimeDate.Month) && (L_IsLeapYear()))
    {
        _Result = L_DayNumberOfMonth[L__FEB_IN_LEAP_YEAR];
    }
    else
    {
        _Result = L_DayNumberOfMonth[RTC_TimeDate.Month];
    }


    return _Result;
}


/*
 * Name: L_IsLeapYear
 *
 * Description: This function calculates whether the given year is a leap
 *              year or not.
 *
 * Input: Year
 *
 * Output: TRUE or FALSE
 */
static uint8 L_IsLeapYear(void)
{
    uint8 _Result = U__FALSE;


    if      ((RTC_TimeDate.Year % 400u) == 0u) { _Result = U__TRUE;  }
    else if ((RTC_TimeDate.Year % 100u) == 0u) { _Result = U__FALSE; }
    else if ((RTC_TimeDate.Year %   4u) == 0u) { _Result = U__TRUE;  }
    else                                       { _Result = U__FALSE; }


    return _Result;
}


/*
 * Name: L_DayOfWeek
 *
 * Description: This function calculates the day number of the week.
 *              The method is based on Sakamoto's Algorithm.
 *
 * Input: Year
 *        Month
 *        Day
 *
 * Output: Number of the day of week (0 = Sunday, 6 = Saturday)
 */
static uint8 L_DayOfWeek(uint16 _Year, uint8 _Month, uint8 _Day)
{
    static uint8 t[12u] = {0u, 3u, 2u, 5u, 0u, 3u, 5u, 1u, 4u, 6u, 2u, 4u};
           uint8 _DoW   = U__INIT_VALUE_UINT;


    _Year -= _Month < 3u;
    _DoW = (uint8)((_Year + _Year/4u - _Year/100u + _Year/400u + t[_Month - 1u] + _Day) % 7u);


    return _DoW;
}


/*
 * Name: L_DaylightSavingTime
 *
 * Description: This function calculates whether it is a DTS time or not.
 *
 * Input: Month
 *        Day
 *        Day of Week
 *
 * Output: DST flag (SET = DST, CLEAR = not DST)
 */
static G_Flag_e L_DaylightSavingTime(uint8 _Month, uint8 _Day, uint8 _DoW)
{
    static G_Flag_e _DST = Flag_CLEAR;


    if ((_Month < 3u) || (10u < _Month))
    {
        _DST = Flag_CLEAR;
    }
    else if ((3u < _Month) && (_Month < 10u))
    {
        _DST = Flag_SET;
    }
    else if (3u == _Month)
    {
        if ((_Day - _DoW) >= 25u)
        {
            _DST = Flag_SET;
        }
        else
        {
            _DST = Flag_CLEAR;
        }
    }  
    else if (10u == _Month)
    {
        if ((_Day - _DoW) < 25u)
        {
            _DST = Flag_SET;
        }
        else
        {
            _DST = Flag_CLEAR;
        }
    }


    return _DST;
}


//====== Public Signals ========================================================
RTC_TimeDate_s RTC_TimeDate;
const char*    RTC_DayName[8u] = {"SU","MO","TU","WE","TH","FR","SA","ER"};


//====== Public Functions ======================================================
/*
 * Name: RTC_Refresh
 *
 * Description: This function calculates the time and date based on the second
 *              counting.
 *
 * Input: None
 *
 * Output: Second
 *         Minute
 *         Hour
 *         Day
 *         Month
 *         Year
 */
void RTC_Refresh(void)
{
    //**************************************************************************
    //****** Reset the "new" flags
    //**************************************************************************
    RTC_TimeDate.NewSecond = Flag_CLEAR;
    RTC_TimeDate.NewMinute = Flag_CLEAR;
    RTC_TimeDate.NewHour   = Flag_CLEAR;
    RTC_TimeDate.NewDay    = Flag_CLEAR;
    RTC_TimeDate.NewMonth  = Flag_CLEAR;
    RTC_TimeDate.NewYear   = Flag_CLEAR;

    //**************************************************************************
    //****** Counting the seconds
    //**************************************************************************
    RTC_TimeDate.Second++;
    RTC_TimeDate.NewSecond = Flag_SET;

    //**************************************************************************
    //****** Counting the minutes
    //**************************************************************************
    if (60u == RTC_TimeDate.Second)
    {
        RTC_TimeDate.Second = 0u;
        RTC_TimeDate.Minute++;
        RTC_TimeDate.NewMinute = Flag_SET;

        //**********************************************************************
        //****** Counting the hours
        //**********************************************************************
        if (60u == RTC_TimeDate.Minute)
        {
            RTC_TimeDate.Minute = 0u;
            RTC_TimeDate.Hour++;
            RTC_TimeDate.NewHour = Flag_SET;

            //******************************************************************
            //****** Counting the days
            //******************************************************************
            if (24u == RTC_TimeDate.Hour)
            {
                RTC_TimeDate.Hour = 0u;
                RTC_TimeDate.Day++;
                RTC_TimeDate.DayOfWeek++;
                if (7u == RTC_TimeDate.DayOfWeek)
                {
                    RTC_TimeDate.DayOfWeek = 0u;
                }
                RTC_TimeDate.NewDay = Flag_SET;

                //**************************************************************
                //****** Counting the months
                //**************************************************************
                if ((L_EndDayOfMonth() + 1) == RTC_TimeDate.Day)
                {
                    RTC_TimeDate.Day = 1u;
                    RTC_TimeDate.Month++;
                    RTC_TimeDate.NewMonth = Flag_SET;

                    //**********************************************************
                    //****** Counting the years
                    //**********************************************************
                    if (13u == RTC_TimeDate.Month)
                    {
                        RTC_TimeDate.Month = 1u;
                        RTC_TimeDate.Year++;
                        RTC_TimeDate.NewYear = Flag_SET;
                    } // Year
                } // Month
            } // Day
        } // Minute
    } // Second
}


/*
 * Name: RTC_SetDate
 *
 * Description: This function sets the time and date.
 *
 * Input: Year
 *        Month
 *        Day
 *        Hour
 *        Minute
 *        Second
 *
 * Output: None
 */
void RTC_SetDate(uint16 _Year, uint8 _Month,  uint8 _Day,
                 uint8  _Hour, uint8 _Minute, uint8 _Second)
{
    static uint8    _DoW = U__INIT_VALUE_UINT;
    static G_Flag_e _DST = Flag_CLEAR;
   

    _DoW = L_DayOfWeek(_Year, _Month, _Day);
    _DST = L_DaylightSavingTime(_Month, _Day, _DoW);
    
    RTC_TimeDate.Year       = _Year;
    RTC_TimeDate.Month      = _Month;
    RTC_TimeDate.Day        = _Day;
    RTC_TimeDate.DayOfWeek  = _DoW;
    RTC_TimeDate.DST        = _DST;
    RTC_TimeDate.Hour       = _Hour;
    RTC_TimeDate.Minute     = _Minute;
    RTC_TimeDate.Second     = _Second;
   
    // Local (Hungary) Time Zone Offset correction according to DST
    if (Flag_SET == RTC_TimeDate.DST)
    {
        RTC_TimeDate.Hour += 2u;
    }
    else
    {
        RTC_TimeDate.Hour += 1u;
    }

    // Hour correction at Midnight due to DST offset
    if (24u == RTC_TimeDate.Hour)
    {
        RTC_TimeDate.Hour = 0u;
    }
    else if (25u == RTC_TimeDate.Hour)
    {
        RTC_TimeDate.Hour = 1u;
    }
}
