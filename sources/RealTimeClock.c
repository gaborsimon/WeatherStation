//====== Header includes =======================================================
#include "RealTimeClock.h"


//====== Private Constants =====================================================
#define L_FEB_IN_LEAP_YEAR (0u)
#define L_FEB              (2u)


static const uint8 L_DayNumberOfMonth[13] =
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
static uint8 EndDayOfMonth(void);
static uint8 IsLeapYear(void);


//====== Private Functions =====================================================
/*
 * Name: EndDayOfMonth
 *
 * Description: This function calculates the last day of the current month.
 *              It considers the leap year for February.
 *
 * Input: Month
 *
 * Output: Last day number of the current month
 */
static uint8 EndDayOfMonth(void)
{
    uint8 _result = INIT_VALUE_UINT;

    
    // If there is February and Leap Year then February contains 29 days
    if ((L_FEB == RTC_TimeDate.Month) && (IsLeapYear()))
    {
        _result = L_DayNumberOfMonth[L_FEB_IN_LEAP_YEAR];
    }
    else
    {
        _result = L_DayNumberOfMonth[RTC_TimeDate.Month];
    }

    return _result;
}


/*
 * Name: IsLeapYear
 *
 * Description: This function calculates whether the given year is a leap
 *              year or not.
 *
 * Input: Year
 *
 * Output: TRUE or FALSE
 */
static uint8 IsLeapYear(void)
{
    uint8 _result = FALSE;

    if      ((RTC_TimeDate.Year % 400u) == 0u) { _result = TRUE;  }
    else if ((RTC_TimeDate.Year % 100u) == 0u) { _result = FALSE; }
    else if ((RTC_TimeDate.Year %   4u) == 0u) { _result = TRUE;  }
    else                                       { _result = FALSE; }

    return _result;
}


//====== Public Signals ========================================================
TimeDate RTC_TimeDate;
const char* RTC_DayName[8u] = {"ER","MO","TU","WE","TH","FR","SA","SU"};


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
                if ((EndDayOfMonth() + 1) == RTC_TimeDate.Day)
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
 *         Minute
 *         Hour
 *         Day
 *         Month
 *         Year
 *
 * Output: None
 */
void RTC_SetDate(uint16 year, uint8 month,  uint8 day,
                 uint8  daynumber,
                 uint8  hour, uint8 minute, uint8 second)
{
    RTC_TimeDate.Year       = year;
    RTC_TimeDate.Month      = month;
    RTC_TimeDate.Day        = day;
    RTC_TimeDate.DayNumber  = daynumber;
    RTC_TimeDate.Hour       = hour;
    RTC_TimeDate.Minute     = minute;
    RTC_TimeDate.Second     = second;
}
