//====== Header includes =======================================================
#include "SunTimeCalculator.h"


//====== Private Constants =====================================================
// Center coordinates are set to Hungary
#define L__LONGITUDE ((float32) 19.0f)
#define L__LATITUDE  ((float32) 47.0f)

// GMT offset is set to Hungary
#define L__GMT_OFFSET ((uint8) 1u)

#define L__SUN_BELOW_HORIZON_DEG ((float32) -0.8333f)


//====== Private Signals =======================================================


//====== Private Function Prototypes ===========================================
static void  L_CalculateSunTime(uint16 _Year, uint16 _Month, uint16 _Day,
                                uint8 *_RiseHour, uint8 *_RiseMinute,
                                uint8 *_SetHour,  uint8 *_SetMinute);


//====== Private Functions =====================================================
/*
 * Name: L_CalculateSunTime
 *
 * Description: 
 *
 * Input: Year, Month, Day
 *
 * Output: 
 *        
 */
static void L_CalculateSunTime(uint16 _Year, uint16 _Month, uint16 _Day,
                             uint8 *_RiseHour, uint8 *_RiseMinute,
                             uint8 *_SetHour,  uint8 *_SetMinute)
{
    uint32  _T          = U__INIT_VALUE_UINT;
    float32 _w          = U__INIT_VALUE_FLOAT;
    float32 _e          = U__INIT_VALUE_FLOAT;
    float32 _M          = U__INIT_VALUE_FLOAT;
    float32 _oblecl     = U__INIT_VALUE_FLOAT;
    float32 _L          = U__INIT_VALUE_FLOAT;
    float32 _E          = U__INIT_VALUE_FLOAT;
    float32 _x          = U__INIT_VALUE_FLOAT;
    float32 _y          = U__INIT_VALUE_FLOAT;
    float32 _r          = U__INIT_VALUE_FLOAT;
    float32 _v          = U__INIT_VALUE_FLOAT;
    float32 _xe         = U__INIT_VALUE_FLOAT;
    float32 _ye         = U__INIT_VALUE_FLOAT;
    float32 _ze         = U__INIT_VALUE_FLOAT;
    float32 _lon        = U__INIT_VALUE_FLOAT;
    float32 _RA_deg     = U__INIT_VALUE_FLOAT;
    float32 _RA_h       = U__INIT_VALUE_FLOAT;
    float32 _DEC_deg    = U__INIT_VALUE_FLOAT;
    float32 _GMST0_deg  = U__INIT_VALUE_FLOAT;
    float32 _GMST0_h    = U__INIT_VALUE_FLOAT;
    float32 _UTs_h      = U__INIT_VALUE_FLOAT;
    float32 _X1         = U__INIT_VALUE_FLOAT;
    float32 _LHA1       = U__INIT_VALUE_FLOAT;
    float32 _LHA1_h     = U__INIT_VALUE_FLOAT;
    float32 _SunRise    = U__INIT_VALUE_FLOAT;
    float32 _SunSet     = U__INIT_VALUE_FLOAT;

    
    //****** STEP 1. ***********************************************************
    // Calculation of time scale
    _T  = (uint32)(367.0f * _Year);
    _T -= (uint32)((7.0f * (_Year + (uint32)((_Month + 9.0f) / 12.0f))) / 4.0f);
    _T += (uint32)((275.0f * _Month) / 9.0f);
    _T += (uint32)(_Day);
    _T -= (uint32)(730530u);

    //****** STEP 2. ***********************************************************
    // Orbital elements of the Sun
    _w = 282.9404f + 4.70935E-5f   * _T;    // [deg] longitude of perihelion
    _e = 0.016709f - 1.151E-9f     * _T;    // eccentricity
    _M = 356.0470f + 0.9856002585f * _T;    // [deg] mean anomaly
    _M = U__NORMALIZE(_M);                  // reduces angle between 0-360 degree

    //****** STEP 3. ***********************************************************
    // Parameters
    _oblecl = 23.4393f - 3.563E-7f * _T;    // [deg] Obliquity of the ecliptic
    _L = _M + _w;                           // [deg] Mean longitude of the Sun
    _E = _M + U__RAD_TO_DEG(_e * U__SIND(_M) * (1.0f + _e * U__COSD(_M)));   // [deg] Eccentric anomaly   

    // Ecliptic rectangular coordinates of the Sun
    _x = U__COSD(_E) - _e;
    _y = U__SIND(_E) * sqrt(1.0f - (_e*_e));

    // Distance r) and true anomaly (v)
    _r = sqrt(_x*_x + _y*_y);
    _v = U__RAD_TO_DEG(atan2(_y,_x));

    // Longitude of the Sun
    _lon = _v + _w;
    _lon = U__NORMALIZE(_lon);

    // Ecliptic rectangular coordinates of the Sun
    _x = _r * U__COSD(_lon);
    _y = _r * U__SIND(_lon);
    //_z = 0.0f;

    // Equatorial coordinates
    _xe = _x;
    _ye = _y * U__COSD(_oblecl);
    _ze = _y * U__SIND(_oblecl);

    //****** STEP 4. ***********************************************************
    // Sun position 
    _RA_deg  = U__RAD_TO_DEG(atan2(_ye,_xe));
    _RA_h    = _RA_deg / 15.0f;
    _DEC_deg = U__RAD_TO_DEG(atan2(_ze,sqrt(_xe*_xe + _ye*_ye)));

    //****** STEP 5. ***********************************************************
    // Sun rise/set time
    _GMST0_deg = U__NORMALIZE(_L + 180.0f);
    _GMST0_h   = _GMST0_deg / 15.0f;
    _UTs_h     = _RA_h - _GMST0_h - (L__LONGITUDE / 15.0f);
    _UTs_h     = _UTs_h - floor(_UTs_h / 24.0f) * 24.0f;

    _X1      = (U__SIND(L__SUN_BELOW_HORIZON_DEG) - U__SIND(L__LATITUDE) * U__SIND(_DEC_deg)) / (U__COSD(L__LATITUDE) * U__COSD(_DEC_deg));
    _LHA1    = U__RAD_TO_DEG(acos(_X1));
    _LHA1_h  = _LHA1 / 15.0f;

    _SunRise = _UTs_h - _LHA1_h;
    _SunSet  = _UTs_h + _LHA1_h;

    *_RiseHour   = ((uint8)(floor(_SunRise))) + L__GMT_OFFSET;
    *_RiseMinute = (uint8)(round(59.0f * (_SunRise - floor(_SunRise))));
    *_SetHour    = ((uint8)(floor(_SunSet))) + L__GMT_OFFSET;
    *_SetMinute  = (uint8)(round(59.0f * (_SunSet - floor(_SunSet))));
}


//====== Public Signals ========================================================
STC_Sun_s STC_Sun;


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
    L_CalculateSunTime(XRTC__TIMEDATE_YEAR, XRTC__TIMEDATE_MONTH, XRTC__TIMEDATE_DAY,
                     &STC_Sun.RiseHour, &STC_Sun.RiseMinute,
                     &STC_Sun.SetHour,  &STC_Sun.SetMinute);
}
