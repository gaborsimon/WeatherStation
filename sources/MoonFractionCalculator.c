//====== Header includes =======================================================
#include "MoonFractionCalculator.h"


//====== Private Constants =====================================================
#define L__JAN (1u)
#define L__FEB (2u)


//====== Private Signals =======================================================


//====== Private Function Prototypes ===========================================
static float32 L_CalculateJDE(uint16 _Year, uint16 _Month, uint16 _Day);
static void    L_CalculateMoonFraction(uint16 _Year, uint16 _Month, uint16 _Day,
                                  uint8 *_Fraction, uint8 *_Direction);


//====== Private Functions =====================================================
/*
 * Name: L_CalculateJDE
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
static float32 L_CalculateJDE(uint16 _Year, uint16 _Month, uint16 _Day)
{
    float32 _Result = U__INIT_VALUE_FLOAT;
    float32 _B      = -13.0f;

    
    //****** STEP 1. ***********************************************************
    // If Month is equal to the 1st or 2nd month of the year then Month shall be
    // the 13th or 14th month of the previous year.
    if ((L__JAN == _Month) || (L__FEB == _Month))
    {
        _Year  = _Year  -  1u;
        _Month = _Month + 12u;
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
    _Result  = (float32) ((uint32) (365.25f   * ((float32) (_Year)  + 4716.0f)));
    _Result += (float32) ((uint32) ( 30.6001f * ((float32) (_Month) +    1.0f)));
    _Result += (float32) (_Day);
    _Result += _B;
    _Result -= 1524.5f;

    return _Result;
}


/*
 * Name: L_CalculateMoonFraction
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
static void L_CalculateMoonFraction(uint16 _Year, uint16 _Month, uint16 _Day,
                                  uint8 *_Fraction, uint8 *_Direction)
{
    float32 _JDE = U__INIT_VALUE_FLOAT;
    float32 _T   = U__INIT_VALUE_FLOAT;
    float32 _T2  = U__INIT_VALUE_FLOAT;
    float32 _T3  = U__INIT_VALUE_FLOAT;
    float32 _T4  = U__INIT_VALUE_FLOAT;
    float32 _D   = U__INIT_VALUE_FLOAT;
    float32 _Ms  = U__INIT_VALUE_FLOAT;
    float32 _Mm  = U__INIT_VALUE_FLOAT;
    float32 _i   = U__INIT_VALUE_FLOAT;
    float32 _k   = U__INIT_VALUE_FLOAT;

    
    //****** STEP 1. ***********************************************************
    // Calculation of "JDE"
    _JDE = L_CalculateJDE(_Year, _Month, _Day);

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
    _i -= (  6.289f * U__SIND(_Mm));
    _i += (  2.100f * U__SIND(_Ms));
    _i -= (  1.274f * U__SIND(2 * _D - _Mm));
    _i -= (  0.658f * U__SIND(2 * _D));
    _i -= (  0.214f * U__SIND(2 * _Mm));
    _i -= (  0.110f * U__SIND(_D));

    //****** STEP 7. ***********************************************************
    // Calculation of "k": (1 + cos i) / 2
    // Illuminated Fraction of the disk of the Moon
    _k = ((1.0f + U__COSD(_i)) * 0.5f);

    // Calculate the fraction in percent dimension
    *_Fraction = (uint8)(_k * 100.0f);

    //****** DIRECTION *********************************************************
    if      (U__SIND(_i) > 0.001f) { *_Direction = MFC__MOON_WAXES; }
    else if (U__SIND(_i) < 0.001f) { *_Direction = MFC__MOON_WANES; }
    else                           { *_Direction = MFC__MOON_FULL_OR_NEW; }
}


//====== Public Signals ========================================================
MFC_Moon_s MFC_Moon;


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
    L_CalculateMoonFraction(XRTC__TIMEDATE_YEAR, XRTC__TIMEDATE_MONTH, XRTC__TIMEDATE_DAY,
                          &MFC_Moon.Fraction, &MFC_Moon.Direction);
}
