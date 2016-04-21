//====== Header includes =======================================================
#include "Sensor_DCF77.h"


//====== Private Constants =====================================================
#define L__PULSE_ZERO_MIN_TIME    ((uint16) (60.0f / 0.032f))
#define L__PULSE_ZERO_MAX_TIME    ((uint16)(145.0f / 0.032f))
#define L__PULSE_ONE_MIN_TIME     ((uint16)(155.0f / 0.032f))
#define L__PULSE_ONE_MAX_TIME     ((uint16)(245.0f / 0.032f))

#define L__START_MIN     (21u)
#define L__P1            (28u)
#define L__START_HOUR    (29u)
#define L__P2            (35u)
#define L__START_DAY     (36u)
#define L__START_DAY_NUM (42u)
#define L__START_MONTH   (45u)
#define L__START_YEAR    (50u)
#define L__P3            (58u)
#define L__MINUTE_MARK   (59u)


//====== Private Signals =======================================================
static uint8    L_BitPos        = U__INIT_VALUE_UINT;
static uint8    L_BitArray[60u] = { U__LOW };
static G_Flag_e L_DecodeDone    = Flag_CLEAR;    


//====== Private Function Prototypes ===========================================
static G_Flag_e L_ParityCheck(uint8 _StartPos, uint8 _StopPos);
static G_Flag_e L_SignalParityCheck(void);


//====== Private Functions =====================================================
static G_Flag_e L_ParityCheck(uint8 _StartPos, uint8 _StopPos)
{
    uint8    _LoopCnt  = U__INIT_VALUE_UINT;
    uint8    _P        = U__INIT_VALUE_UINT;
    G_Flag_e _Ret      = Flag_SET;


    for (_LoopCnt = _StartPos; _LoopCnt < _StopPos; _LoopCnt++)
    {
        if (L_BitArray[_LoopCnt] == U__HIGH) { _P ^= 1u; }
    }
    
    if (_P != L_BitArray[_StopPos])
    {
        _Ret = Flag_SET;
    }
    else
    {
        _Ret = Flag_CLEAR;
    }
   

    return _Ret;
}


static G_Flag_e L_SignalParityCheck(void)
{
    uint8  _Error = Flag_SET;

    
    _Error = L_ParityCheck(L__START_MIN, L__P1);        
    _Error = L_ParityCheck(L__START_HOUR, L__P2);        
    _Error = L_ParityCheck(L__START_DAY, L__P3);


    return _Error;
}


static void L_SignalDecode(void)
{
    uint8  _min     = U__INIT_VALUE_UINT;
    uint8  _h       = U__INIT_VALUE_UINT;
    uint8  _d       = U__INIT_VALUE_UINT;
    uint8  _dn      = U__INIT_VALUE_UINT;
    uint8  _m       = U__INIT_VALUE_UINT;
    uint8  _y       = U__INIT_VALUE_UINT;
    
    
    _min  = L_BitArray[L__START_MIN];
    _min += L_BitArray[L__START_MIN + 1u] *  2u;
    _min += L_BitArray[L__START_MIN + 2u] *  4u;
    _min += L_BitArray[L__START_MIN + 3u] *  8u;
    _min += L_BitArray[L__START_MIN + 4u] * 10u;
    _min += L_BitArray[L__START_MIN + 5u] * 20u;
    _min += L_BitArray[L__START_MIN + 6u] * 40u;

    _h  = L_BitArray[L__START_HOUR];
    _h += L_BitArray[L__START_HOUR + 1u] *  2u;
    _h += L_BitArray[L__START_HOUR + 2u] *  4u;
    _h += L_BitArray[L__START_HOUR + 3u] *  8u;
    _h += L_BitArray[L__START_HOUR + 4u] * 10u;
    _h += L_BitArray[L__START_HOUR + 5u] * 20u;

    _d  = L_BitArray[L__START_DAY];
    _d += L_BitArray[L__START_DAY + 1u] *  2u;
    _d += L_BitArray[L__START_DAY + 2u] *  4u;
    _d += L_BitArray[L__START_DAY + 3u] *  8u;
    _d += L_BitArray[L__START_DAY + 4u] * 10u;
    _d += L_BitArray[L__START_DAY + 5u] * 20u;

    _dn  = L_BitArray[L__START_DAY_NUM];
    _dn += L_BitArray[L__START_DAY_NUM + 1u] *  2u;
    _dn += L_BitArray[L__START_DAY_NUM + 2u] *  4u;

    _m  = L_BitArray[L__START_MONTH];
    _m += L_BitArray[L__START_MONTH + 1u] *  2u;
    _m += L_BitArray[L__START_MONTH + 2u] *  4u;
    _m += L_BitArray[L__START_MONTH + 3u] *  8u;
    _m += L_BitArray[L__START_MONTH + 4u] * 10u;

    _y  = L_BitArray[L__START_YEAR];
    _y += L_BitArray[L__START_YEAR + 1u] *  2u;
    _y += L_BitArray[L__START_YEAR + 2u] *  4u;
    _y += L_BitArray[L__START_YEAR + 3u] *  8u;
    _y += L_BitArray[L__START_YEAR + 4u] * 10u;
    _y += L_BitArray[L__START_YEAR + 5u] * 20u;
    _y += L_BitArray[L__START_YEAR + 6u] * 40u;
    _y += L_BitArray[L__START_YEAR + 7u] * 80u;

    RTC_SetDate((uint16)(2000u + _y), _m, _d, _dn, _h, _min, 0u);

    L_DecodeDone = Flag_SET;
}


//====== Public Signals ========================================================
DCF77_Status_e DCF77_Status = DCF77_Status_INIT;


//====== Public Functions ======================================================
void DCF77_Callback_TimerOverflow(void)
{
    // This is the end of the minute
    // Start parity check and decode the received bits
    if ((L__MINUTE_MARK == L_BitPos) &&
        (Flag_CLEAR    == L_SignalParityCheck()))
    {
        L_SignalDecode();
    }
    // There was some error, reset the receiving procedure 
    else
    {
        L_BitPos = U__INIT_VALUE_UINT;
    }
}

    
void DCF77_Callback_InputCapture(void)
{
    uint16 _PulseWidth = U__INIT_VALUE_UINT;
    

    // Start of pulse - Falling (negative) edge
    if (U__LOW == U__BIT_GET(TCCR1B, ICES1))
    {
        // Start the Timer1 from zero
        TCNT1 = 0u;
        
        // Decoding was OK
        if (Flag_SET == L_DecodeDone)
        {
            L_DecodeDone = Flag_CLEAR;
            
            // Reset main scheduler Timer Register
            TCNT2 = 0u;
            
            // Turn off the receiving
            DCF77_Receiving(U__DISABLE);
            
            // Synchronization has been finished
            DCF77_Status = DCF77_Status_SYNCH_DONE;  
        }
        // Normal receiving
        else
        {
            // Set to rising edge
            U__BIT_SET(TCCR1B, ICES1);
        }
    }
    // End of pulse - Rising (positive) edge
    else
    {
        // Capture the timer value
        _PulseWidth = ICR1;
        
        // Reset Timer Register
        TCNT1 = 0u;

        // Set to falling edge
        U__BIT_CLR(TCCR1B, ICES1);
        
        // Captured bit is "0" (~100ms) 
        if ((L__PULSE_ZERO_MIN_TIME < _PulseWidth) && (_PulseWidth < L__PULSE_ZERO_MAX_TIME))
        {
            L_BitArray[L_BitPos] = U__LOW;
            L_BitPos++;
        }
        // Captured bit is "1" (~200ms) 
        else if ((L__PULSE_ONE_MIN_TIME < _PulseWidth) && (_PulseWidth < L__PULSE_ONE_MAX_TIME))
        {
            L_BitArray[L_BitPos] = U__HIGH;
            L_BitPos++;
        }
        else
        {
            /* False pulse is detected */
        }
    }
}


void DCF77_Receiving(uint8 _Control)
{
    switch (_Control)
    {
        case U__ENABLE:
        {
            // Reset decoder array
            L_BitPos = U__INIT_VALUE_UINT;
        
            // Module turned on
            DCF77__CONTROL(U__ENABLE);

            // Reset Timer Register
            TCNT1 = 0u;
            
            // Timer1 turned on
            // Clock Select: CLK / 256 = 31.250kHz = 32us
            U__BIT_SET(TCCR1B, CS12);
            U__BIT_CLR(TCCR1B, CS11);
            U__BIT_CLR(TCCR1B, CS10);
    
            // Output Compare Interrupt: Enabled
            U__BIT_SET(TIMSK, OCIE1A);
            // Timer Input Capture Interrupt: Enabled
            U__BIT_SET(TIMSK, TICIE1);
        }
        break;

        case U__DISABLE:
        {
            // Module turned off
            DCF77__CONTROL(U__DISABLE);
    
            // Timer1 turned off
            U__BIT_CLR(TCCR1B, CS12);
            U__BIT_CLR(TCCR1B, CS11);
            U__BIT_CLR(TCCR1B, CS10);
    
            // Output Compare Interrupt: Disabled
            U__BIT_CLR(TIMSK, OCIE1A);
            // Timer Input Capture Interrupt: Disabled
            U__BIT_CLR(TIMSK, TICIE1);
        }
        break;
    }
}


void DCF77_Refresh(void)
{
    static uint8  L_SymbolFlipFlag = U__LOW; 
    static uint16 L_TickCounter = U__INIT_VALUE_UINT; 


    switch (DCF77_Status)
    {
        case DCF77_Status_INIT:
        {
            L_SymbolFlipFlag = U__LOW;
            L_TickCounter = U__INIT_VALUE_UINT;
            DCF77_Receiving(U__ENABLE);
            DCF77_Status = DCF77_Status_SYNCH_ONGOING;
        }
        break;
        
        case DCF77_Status_SYNCH_ONGOING:
        {
            if (U__HIGH == L_SymbolFlipFlag)
            {
                L_SymbolFlipFlag = U__LOW;
                LCM_Refresh(LCM__RX_OK);
            }
            else
            {
                L_SymbolFlipFlag = U__HIGH;
                LCM_Refresh(LCM__RX_NO);
            }

            if (L_BitPos < 100u)
            {
                if (L_BitPos < 10u)
                {
                    LCD_WriteChar(' ');   
                }
                LCD_WriteInt(L_BitPos);   
            }
            else
            {
                LCD_WriteString("ER");
            }
        }
        break;
        
        case DCF77_Status_SYNCH_DONE:
        {
            DCF77_Status = DCF77_Status_SYNCHRONIZED;
        }
        break;

        case DCF77_Status_SYNCHRONIZED:
        {
            L_TickCounter++;

            if (10u == L_TickCounter)
            {
                LCM_Refresh(LCM__RX_NONE);
            }

            if (20u == L_TickCounter)
            {
                DCF77_Status = DCF77_Status_INIT;
            }
        }
        break;
    }
}
