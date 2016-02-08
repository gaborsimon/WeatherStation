//====== Header includes =======================================================
#include "Sensor_DCF77.h"


//====== Private Constants =====================================================
#define L_PULSE_ZERO_MIN_TIME    ((uint16) (60.0f / 0.032f))
#define L_PULSE_ZERO_MAX_TIME    ((uint16)(145.0f / 0.032f))
#define L_PULSE_ONE_MIN_TIME     ((uint16)(155.0f / 0.032f))
#define L_PULSE_ONE_MAX_TIME     ((uint16)(245.0f / 0.032f))

#define L_START_MIN     (21u)
#define L_P1            (28u)
#define L_START_HOUR    (29u)
#define L_P2            (35u)
#define L_START_DAY     (36u)
#define L_START_DAY_NUM (42u)
#define L_START_MONTH   (45u)
#define L_START_YEAR    (50u)
#define L_P3            (58u)
#define L_MINUTE_MARK   (59u)


//====== Private Signals =======================================================
static uint8  BitPos        = INIT_VALUE_UINT;
static uint8  BitArray[60u] = { 0u };


//====== Private Function Prototypes ===========================================
static Flag ParityCheck(uint8 start, uint8 stop);
static Flag SignalParityCheck(void);


//====== Private Functions =====================================================
static Flag ParityCheck(uint8 start, uint8 stop)
{
    uint8 _loop_cnt = INIT_VALUE_UINT;
    uint8 _p        = INIT_VALUE_UINT;
    Flag  _ret      = Flag_SET;


    for (_loop_cnt = start; _loop_cnt < stop; _loop_cnt++)
    {
        if (BitArray[_loop_cnt] == 1u) { _p ^= 1u; }
    }
    
    if (_p != BitArray[stop])
    {
        _ret = Flag_SET;
    }
    else
    {
        _ret = Flag_CLEAR;
    }
    
    return _ret;
}


static Flag SignalParityCheck(void)
{
    uint8  _error = Flag_SET;

    
    _error = ParityCheck(L_START_MIN, L_P1);        
    _error = ParityCheck(L_START_HOUR, L_P2);        
    _error = ParityCheck(L_START_DAY, L_P3);


    return _error;
}


static void SignalDecode(void)
{
    uint8  _min     = INIT_VALUE_UINT;
    uint8  _h       = INIT_VALUE_UINT;
    uint8  _d       = INIT_VALUE_UINT;
    uint8  _dn      = INIT_VALUE_UINT;
    uint8  _m       = INIT_VALUE_UINT;
    uint8  _y       = INIT_VALUE_UINT;
    
    
    _min  = BitArray[L_START_MIN];
    _min += BitArray[L_START_MIN + 1u] *  2u;
    _min += BitArray[L_START_MIN + 2u] *  4u;
    _min += BitArray[L_START_MIN + 3u] *  8u;
    _min += BitArray[L_START_MIN + 4u] * 10u;
    _min += BitArray[L_START_MIN + 5u] * 20u;
    _min += BitArray[L_START_MIN + 6u] * 40u;

    _h  = BitArray[L_START_HOUR];
    _h += BitArray[L_START_HOUR + 1u] *  2u;
    _h += BitArray[L_START_HOUR + 2u] *  4u;
    _h += BitArray[L_START_HOUR + 3u] *  8u;
    _h += BitArray[L_START_HOUR + 4u] * 10u;
    _h += BitArray[L_START_HOUR + 5u] * 20u;

    _d  = BitArray[L_START_DAY];
    _d += BitArray[L_START_DAY + 1u] *  2u;
    _d += BitArray[L_START_DAY + 2u] *  4u;
    _d += BitArray[L_START_DAY + 3u] *  8u;
    _d += BitArray[L_START_DAY + 4u] * 10u;
    _d += BitArray[L_START_DAY + 5u] * 20u;

    _dn  = BitArray[L_START_DAY_NUM];
    _dn += BitArray[L_START_DAY_NUM + 1u] *  2u;
    _dn += BitArray[L_START_DAY_NUM + 2u] *  4u;

    _m  = BitArray[L_START_MONTH];
    _m += BitArray[L_START_MONTH + 1u] *  2u;
    _m += BitArray[L_START_MONTH + 2u] *  4u;
    _m += BitArray[L_START_MONTH + 3u] *  8u;
    _m += BitArray[L_START_MONTH + 4u] * 10u;

    _y  = BitArray[L_START_YEAR];
    _y += BitArray[L_START_YEAR + 1u] *  2u;
    _y += BitArray[L_START_YEAR + 2u] *  4u;
    _y += BitArray[L_START_YEAR + 3u] *  8u;
    _y += BitArray[L_START_YEAR + 4u] * 10u;
    _y += BitArray[L_START_YEAR + 5u] * 20u;
    _y += BitArray[L_START_YEAR + 6u] * 40u;
    _y += BitArray[L_START_YEAR + 7u] * 80u;

    RTC_SetDate((uint16)(2000u + _y), _m, _d, _h, _min, 0u);
}

//====== Public Signals ========================================================
Flag DCF77_SyncDone = Flag_CLEAR;    


//====== Public Functions ======================================================
void DCF77_Callback_TimerOverflow(void)
{
    if ((BitPos == L_MINUTE_MARK) &&
        (Flag_CLEAR == SignalParityCheck()))
    {
        SignalDecode();
    }
    else
    {
        BitPos = INIT_VALUE_UINT;
    }
}

    
void DCF77_Callback_InputCapture(void)
{
    uint16 _pulse_width = INIT_VALUE_UINT;
    

    // Start of pulse - Falling (negative) edge
    if (0u == BIT_GET(TCCR1B, ICES1))
    {
        // Start the Timer1 from zero
        TCNT1 = 0u;
        
        if (BitPos == L_MINUTE_MARK)
        {
            BitPos = INIT_VALUE_UINT;
            TCNT2 = 0u;
            DCF77_SyncDone = Flag_SET;    
        }
        else
        {
            // Set to rising edge
            BIT_SET(TCCR1B, ICES1);
        }
    }
    // End of pulse - Rising (positive) edge
    else
    {
        // Capture the timer value
        _pulse_width = ICR1;
        // Reset the timer counter
        TCNT1 = 0u;

        // Set to falling edge
        BIT_CLR(TCCR1B, ICES1);
        
        // Captured bit is "0" (~100ms) 
        if ((L_PULSE_ZERO_MIN_TIME < _pulse_width) && (_pulse_width < L_PULSE_ZERO_MAX_TIME))
        {
            BitArray[BitPos] = 0u;
/* DEBUG */
            LCD_SetCursor(2u,19u);
            LCD_WriteString("  ");
            LCD_SetCursor(2u,19u);
            LCD_WriteInt(BitPos);
/* DEBUG */
            BitPos++;
        }
        // Captured bit is "1" (~200ms) 
        else if ((L_PULSE_ONE_MIN_TIME < _pulse_width) && (_pulse_width < L_PULSE_ONE_MAX_TIME))
        {
            BitArray[BitPos] = 1u;
/* DEBUG */
            LCD_SetCursor(2u,19u);
            LCD_WriteString("  ");
            LCD_SetCursor(2u,19u);
            LCD_WriteInt(BitPos);
/* DEBUG */
            BitPos++;
        }
        else
        {
            /* False pulse is detected */
        }
    }
}

