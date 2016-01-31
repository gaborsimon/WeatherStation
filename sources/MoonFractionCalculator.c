//====== Header includes =======================================================
#include "MoonFractionCalculator.h"


//====== Private Constants =====================================================
#define L_JAN (1u)
#define L_FEB (2u)


//====== Private Signals =======================================================


//====== Private Function Prototypes ===========================================
static float32 CalculateJDE(uint16 year, uint16 month, uint16 day);
static void    CalculateMoonFraction(uint16 year, uint16 month, uint16 day,
                                  uint8 *fraction, uint8 *direction);


//====== Private Functions =====================================================
/*
 * Name: CalculateJDE
 *
 * Description: This function calculates the Julian Ephemeris Day (JDE) by the
 *              given Gregorian date.
 *
 * Input: Year
 *        Month
 *        Day
 *
 * Output: JDE
 */
static float32 CalculateJDE(uint16 year, uint16 month, uint16 day)
{
    float32 _result = INIT_VALUE_FLOAT;
    float32 _B      = -13.0f;

    
    //****** STEP 1. ***********************************************************
    // If Month is equal to the 1st or 2nd month of the year then Month shall be
    // the 13th or 14th month of the previous year.
    if ((L_JAN == month) || (L_FEB == month))
    {
        year  = year  -  1u;
        month = month + 12u;
    }

    //****** STEP 2. ***********************************************************
    // Calculation of "A": INT(YEAR/100)
    // This is needed only for "B" calculation, but this is constant in our
    // application therefore not required to calculate.

    //****** STEP 3. ***********************************************************
    // Calculation of "B": 2-A+INT(A/4)
    // If the given date is between 1900.03.01 and 2100.02.28 then B=-13.
    // This program is intended to be used between these dates therefore the
    // constant value can be used instead of the continuous calculation.
    _B = -13.0f;

    //****** STEP 4. ***********************************************************
    // Calculation of "JDE": INT(365.25(YEAR+4716)) +
    //                       INT(30.6001(MONTH+1)) +
    //                       DAY +
    //                       B -
    //                       1524.5
    _result  = (float32) ((uint32) (365.25f   * ((float32) (year)  + 4716.0f)));
    _result += (float32) ((uint32) ( 30.6001f * ((float32) (month) +    1.0f)));
    _result += (float32) (day);
    _result += _B;
    _result -= 1524.5f;

    return _result;
}


/*
 * Name: CalculateMoonFraction
 *
 * Description: This function calculates the illuminated fraction (k) of the
 *              disk of the Moon depends on the selenocentric elongation of the
 *              Earth from the Sun, called the Phase Angle (i).
 *              Selenocentric means 'as seen from the center of the Moon'.
 *
 *              The used algorithm is an approximate calculation with lower
 *              accuracy but still a good result. The geocentric position of
 *              the Sun and the Moon are not needed.
 *
 *              The calculation is based on the book:
 *              Jean Meeus 'Astronomical Algorithms' First English Edition
 *              Chapter  7: Julian Day
 *              Chapter 45: Position of the Moon
 *              Chapter 46: Illuminated Fraction of the Moon's Disk
 *
 *              To test it: http://aa.usno.navy.mil/data/docs/MoonFraction.php
 *
 * Input: Year, Month, Day
 *
 * Output: Fraction  = Illuminated Fraction in % dimension
 *         Direction = Moon wanes (shrinks) or waxes (grows) or full or new
 */
static void CalculateMoonFraction(uint16 year, uint16 month, uint16 day,
                                  uint8 *fraction, uint8 *direction)
{
    float32 _JDE = INIT_VALUE_FLOAT;
    float32 _T   = INIT_VALUE_FLOAT;
    float32 _T2  = INIT_VALUE_FLOAT;
    float32 _T3  = INIT_VALUE_FLOAT;
    float32 _T4  = INIT_VALUE_FLOAT;
    float32 _D   = INIT_VALUE_FLOAT;
    float32 _Ms  = INIT_VALUE_FLOAT;
    float32 _Mm  = INIT_VALUE_FLOAT;
    float32 _i   = INIT_VALUE_FLOAT;
    float32 _k   = INIT_VALUE_FLOAT;

    
    //****** STEP 1. ***********************************************************
    // Calculation of "JDE"
    _JDE = CalculateJDE(year, month, day);

    //****** STEP 2. ***********************************************************
    // Calculation of "T" and its power
    _T = ((_JDE - 2451545.0f) / 36525.0f);
    _T2 = (_T  * _T);
    _T3 = (_T2 * _T);
    _T4 = (_T3 * _T);

    //****** STEP 3. ***********************************************************
    // Calculation of "D": [deg] Mean Elongation of the Moon
    _D  = 297.850192f;
    _D += (445267.111403f * _T);
    _D += (    -0.001882f * _T2);
    _D += (( 1.0f /    545868.0f) * _T3);
    _D += ((-1.0f / 113065000.0f) * _T4);

    //****** STEP 4. ***********************************************************
    // Calculation of "M": [deg] Mean Anomaly of the Sun
    _Ms  = 357.529109f;
    _Ms += (35999.050290f * _T);
    _Ms += (   -0.000154f * _T2);
    _Ms += ((1.0f / 24490000.0f) * _T3);

    //****** STEP 5. ***********************************************************
    // Calculation of "M'": [deg] Mean Anomaly of the Moon
    _Mm  = 134.963411f;
    _Mm += (477198.867631f * _T);
    _Mm += (     0.008742f * _T2);
    _Mm += (( 1.0f /    69699.0f) * _T3);
    _Mm += ((-1.0f / 14712000.0f) * _T4);

    //****** STEP 6. ***********************************************************
    // Calculation of "i": [deg] Phase Angle
    _i  = (180.000f - _D);
    _i -= (  6.289f * SIND(_Mm));
    _i += (  2.100f * SIND(_Ms));
    _i -= (  1.274f * SIND(2 * _D - _Mm));
    _i -= (  0.658f * SIND(2 * _D));
    _i -= (  0.214f * SIND(2 * _Mm));
    _i -= (  0.110f * SIND(_D));

    //****** STEP 7. ***********************************************************
    // Calculation of "k": (1 + cos i) / 2
    // Illuminated Fraction of the disk of the Moon
    _k = ((1.0f + COSD(_i)) * 0.5f);

    // Calculate the fraction in percent dimension
    *fraction = (uint8)(_k * 100.0f);

    //****** DIRECTION *********************************************************
    if      (SIND(_i) > 0.001f) { *direction = MOON_WAXES; }
    else if (SIND(_i) < 0.001f) { *direction = MOON_WANES; }
    else                        { *direction = MOON_FULL_OR_NEW; }
}


//====== Public Signals ========================================================
Moon MFC_Moon;


//====== Public Functions ======================================================
/*
 * Name: MFC_Refresh
 *
 * Description: This function is the refresh of the component.
 *
 * Input: Year
 *        Month
 *        Day
 *
 * Output: Fraction  = Illuminated Fraction in % dimension
 *         Direction = Moon wanes (shrinks) or waxes (grows) or full or new
 */
void MFC_Refresh(void)
{
    static uint8 FirstRun = TRUE;

    
    // In case of the first run or new day the fraction of the moon shall be calculated
    // and the Previous Day shall be set to the current day.
    if ((TRUE == FirstRun) || (Flag_SET == XRTC_TIMEDATE_NEWDAY))
    {
        FirstRun = FALSE;

        CalculateMoonFraction(XRTC_TIMEDATE_YEAR, XRTC_TIMEDATE_MONTH, XRTC_TIMEDATE_DAY,
                              &MFC_Moon.Fraction, &MFC_Moon.Direction);
    }
}
