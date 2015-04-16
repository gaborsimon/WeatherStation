//====== Header includes =======================================================
#include "SunTimeCalculator.h"


//====== Private Constants =====================================================
// Center coordinates are set to Hungary
#define L_LONGITUDE ((float32) 19.0f)
#define L_LATITUDE  ((float32) 47.0f)

// GMT offset is set to Hungary
#define L_GMT_OFFSET ((uint8) 1u)

#define L_SUN_BELOW_HORIZON_DEG ((float32) -0.8333f)


//====== Private Signals =======================================================


//====== Private Function Prototypes ===========================================
static void    CalculateSunTime(uint16 year, uint16 month, uint16 day,
                                uint8 *RiseHour, uint8 *RiseMinute,
                                uint8 *SetHour,  uint8 *SetMinute);


//====== Private Functions =====================================================
/*
 * Name: CalculateSunTime
 *
 * Description: 
 *
 * Input: Year, Month, Day
 *
 * Output: 
 *        
 */
static void CalculateSunTime(uint16 year, uint16 month, uint16 day,
                             uint8 *RiseHour, uint8 *RiseMinute,
                             uint8 *SetHour,  uint8 *SetMinute)
{
    uint32  _T          = INIT_VALUE_UINT;
    float32 _w          = INIT_VALUE_FLOAT;
    float32 _e          = INIT_VALUE_FLOAT;
    float32 _M          = INIT_VALUE_FLOAT;
    float32 _oblecl     = INIT_VALUE_FLOAT;
    float32 _L          = INIT_VALUE_FLOAT;
    float32 _E          = INIT_VALUE_FLOAT;
    float32 _x          = INIT_VALUE_FLOAT;
    float32 _y          = INIT_VALUE_FLOAT;
    float32 _r          = INIT_VALUE_FLOAT;
    float32 _v          = INIT_VALUE_FLOAT;
    float32 _xe         = INIT_VALUE_FLOAT;
    float32 _ye         = INIT_VALUE_FLOAT;
    float32 _ze         = INIT_VALUE_FLOAT;
    float32 _lon        = INIT_VALUE_FLOAT;
    float32 _RA_deg     = INIT_VALUE_FLOAT;
    float32 _RA_h       = INIT_VALUE_FLOAT;
    float32 _DEC_deg    = INIT_VALUE_FLOAT;
    float32 _GMST0_deg  = INIT_VALUE_FLOAT;
    float32 _GMST0_h    = INIT_VALUE_FLOAT;
    float32 _UTs_h      = INIT_VALUE_FLOAT;
    float32 _X1         = INIT_VALUE_FLOAT;
    float32 _LHA1       = INIT_VALUE_FLOAT;
    float32 _LHA1_h     = INIT_VALUE_FLOAT;
    float32 _SunRise    = INIT_VALUE_FLOAT;
    float32 _SunSet     = INIT_VALUE_FLOAT;

    
    //****** STEP 1. ***********************************************************
    // Calculation of time scale
    _T  = (uint32)(367.0f * year);
    _T -= (uint32)((7.0f * (year + (uint32)((month + 9.0f) / 12.0f))) / 4.0f);
    _T += (uint32)((275.0f * month) / 9.0f);
    _T += (uint32)(day);
    _T -= (uint32)(730530u);

    //****** STEP 2. ***********************************************************
    // Orbital elements of the Sun
    _w = 282.9404f + 4.70935E-5f   * _T;    // [deg] longitude of perihelion
    _e = 0.016709f - 1.151E-9f     * _T;    // eccentricity
    _M = 356.0470f + 0.9856002585f * _T;    // [deg] mean anomaly
    _M = NORMALIZE(_M);                     // reduces angle between 0-360 degree

    //****** STEP 3. ***********************************************************
    // Parameters
    _oblecl = 23.4393f - 3.563E-7f * _T;    // [deg] Obliquity of the ecliptic
    _L = _M + _w;                           // [deg] Mean longitude of the Sun
    _E = _M + RAD_TO_DEG(_e * SIND(_M) * (1.0f + _e * COSD(_M)));   // [deg] Eccentric anomaly   

    // Ecliptic rectangular coordinates of the Sun
    _x = COSD(_E) - _e;
    _y = SIND(_E) * sqrt(1.0f - (_e*_e));

    // Distance r) and true anomaly (v)
    _r = sqrt(_x*_x + _y*_y);
    _v = RAD_TO_DEG(atan2(_y,_x));

    // Longitude of the Sun
    _lon = _v + _w;
    _lon = NORMALIZE(_lon);

    // Ecliptic rectangular coordinates of the Sun
    _x = _r * COSD(_lon);
    _y = _r * SIND(_lon);
    //_z = 0.0f;

    // Equatorial coordinates
    _xe = _x;
    _ye = _y * COSD(_oblecl);
    _ze = _y * SIND(_oblecl);

    //****** STEP 4. ***********************************************************
    // Sun position 
    _RA_deg  = RAD_TO_DEG(atan2(_ye,_xe));
    _RA_h    = _RA_deg / 15.0f;
    _DEC_deg = RAD_TO_DEG(atan2(_ze,sqrt(_xe*_xe + _ye*_ye)));

    //****** STEP 5. ***********************************************************
    // Sun rise/set time
    _GMST0_deg = NORMALIZE(_L + 180.0f);
    _GMST0_h   = _GMST0_deg / 15.0f;
    _UTs_h     = _RA_h - _GMST0_h - (L_LONGITUDE / 15.0f);
    _UTs_h     = _UTs_h - floor(_UTs_h / 24.0f) * 24.0f;

    _X1      = (SIND(L_SUN_BELOW_HORIZON_DEG) - SIND(L_LATITUDE) * SIND(_DEC_deg)) / (COSD(L_LATITUDE) * COSD(_DEC_deg));
    _LHA1    = RAD_TO_DEG(acos(_X1));
    _LHA1_h  = _LHA1 / 15.0f;

    _SunRise = _UTs_h - _LHA1_h;
    _SunSet  = _UTs_h + _LHA1_h;

    *RiseHour   = ((uint8)(floor(_SunRise))) + L_GMT_OFFSET;
    *RiseMinute = (uint8)(round(59.0f * (_SunRise - floor(_SunRise))));
    *SetHour    = ((uint8)(floor(_SunSet))) + L_GMT_OFFSET;
    *SetMinute  = (uint8)(round(59.0f * (_SunSet - floor(_SunSet))));
}


//====== Public Signals ========================================================
Sun STC_Sun;


//====== Public Functions ======================================================
/*
 * Name: STC_Refresh
 *
 * Description: This function is the refresh of the component.
 *
 * Input: Year
 *        Month
 *        Day
 *
 * Output: 
 *         
 */
void STC_Refresh(void)
{
    static Flag FirstRun = Flag_SET;

    
    // In case of the first run the sun time shall be calculated
    // and the Previous Day shall be set to the current day.
    if (Flag_SET == FirstRun)
    {
        FirstRun = Flag_CLEAR;

        CalculateSunTime(XRTC_TIMEDATE_YEAR, XRTC_TIMEDATE_MONTH, XRTC_TIMEDATE_DAY,
                         &STC_Sun.RiseHour, &STC_Sun.RiseMinute,
                         &STC_Sun.SetHour,  &STC_Sun.SetMinute);
    }
    // In case of a new day the sun time shall be calculated.
    else if (Flag_SET == XRTC_TIMEDATE_NEWDAY)
    {
        CalculateSunTime(XRTC_TIMEDATE_YEAR, XRTC_TIMEDATE_MONTH, XRTC_TIMEDATE_DAY,
                         &STC_Sun.RiseHour, &STC_Sun.RiseMinute,
                         &STC_Sun.SetHour,  &STC_Sun.SetMinute);
    }
}
