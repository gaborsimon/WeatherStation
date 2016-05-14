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
static uint8 L_EndDayOfMonth(void);
static uint8 L_IsLeapYear(void);


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


//====== Public Signals ========================================================
RTC_TimeDate_s RTC_TimeDate;
const char*    RTC_DayName[8u] = {"ER","MO","TU","WE","TH","FR","SA","SU"};


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
                RTC_TimeDate.DayNumber++;
                if (8u == RTC_TimeDate.DayNumber)
                {
                    RTC_TimeDate.DayNumber = 1u;
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
 * Input: Second
 *        Minute
 *        Hour
 *        Number of the day in the week
 *        Daylight Saving Time (CET or CEST)
 *        Day
 *        Month
 *        Year
 *
 * Output: None
 */
void RTC_SetDate(uint16 _Year, uint8 _Month,  uint8 _Day,
                 uint8  _DayNumber, G_Flag_e _DST,
                 uint8  _Hour, uint8 _Minute, uint8 _Second)
{
    RTC_TimeDate.Year       = _Year;
    RTC_TimeDate.Month      = _Month;
    RTC_TimeDate.Day        = _Day;
    RTC_TimeDate.DayNumber  = _DayNumber;
    RTC_TimeDate.DST        = _DST;
    RTC_TimeDate.Hour       = _Hour;
    RTC_TimeDate.Minute     = _Minute;
    RTC_TimeDate.Second     = _Second;
}
