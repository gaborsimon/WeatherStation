//====== Header includes =======================================================
#include "Tasking.h"


//====== Private Constants =====================================================
//#define CPU_LOAD_MEASUREMENT


//====== Private Signals =======================================================
static volatile Flag L_Task_1SEC = Flag_CLEAR;


//====== Private Function Prototypes ===========================================


//====== Private Functions =====================================================


//====== Public Signals ========================================================


//====== Public Functions ======================================================

//******************************************************************************
//****** INTERRUPT HANDLER
//******************************************************************************
/* DEBUG */
static uint8  i  = 1u;
/* DEBUG */

static uint8  bit_cnt       = INIT_VALUE_UINT;
static uint8  BitArray[60u] = { 0 };

#define DCF77_PULSE_ZERO_MIN_TIME    (1875u)    /*  60ms */
#define DCF77_PULSE_ZERO_MAX_TIME    (4531u)    /* 145ms */ 
#define DCF77_PULSE_ONE_MIN_TIME     (4844u)    /* 155ms */
#define DCF77_PULSE_ONE_MAX_TIME     (7656u)    /* 245ms */

#define DCF77_START_MIN     (21u)
#define DCF77_P1            (28u)
#define DCF77_START_HOUR    (29u)
#define DCF77_P2            (35u)
#define DCF77_START_DAY     (36u)
#define DCF77_START_DAY_NUM (42u)
#define DCF77_START_MONTH   (45u)
#define DCF77_START_YEAR    (50u)
#define DCF77_P3            (58u)
#define DCF77_MINUTE_MARK   (59u)

static uint8 calc_parity(uint8 start, uint8 stop)
{
    uint8 _loop_cnt = 0u;
    uint8 _p        = 0u;
    uint8 _ret      = 1u;


    for (_loop_cnt=start; _loop_cnt<stop; _loop_cnt++)
    {
        if (BitArray[_loop_cnt] == 1u) { _p ^= 1u; }
    }
    if (_p != BitArray[stop])
    {
        _ret = 1u;
    }
    else
    {
        _ret = 0u;
    }
    
    return _ret;
}

ISR(TIMER1_COMPA_vect)
{
    uint8  _min     = INIT_VALUE_UINT;
    uint8  _h       = INIT_VALUE_UINT;
    uint8  _d       = INIT_VALUE_UINT;
    uint8  _dn      = INIT_VALUE_UINT;
    uint8  _m       = INIT_VALUE_UINT;
    uint8  _y       = INIT_VALUE_UINT;
    uint8  _error   = INIT_VALUE_UINT;

/* DEBUG */
    LCD_SetCursor(1u,i++);
    LCD_WriteChar('X');
/* DEBUG */

    if (bit_cnt == DCF77_MINUTE_MARK)
    {
        _min  = BitArray[DCF77_START_MIN];
        _min += BitArray[DCF77_START_MIN + 1u] *  2u;
        _min += BitArray[DCF77_START_MIN + 2u] *  4u;
        _min += BitArray[DCF77_START_MIN + 3u] *  8u;
        _min += BitArray[DCF77_START_MIN + 4u] * 10u;
        _min += BitArray[DCF77_START_MIN + 5u] * 20u;
        _min += BitArray[DCF77_START_MIN + 6u] * 40u;
        _error = calc_parity(DCF77_START_MIN, DCF77_P1);        

 
        _h  = BitArray[DCF77_START_HOUR];
        _h += BitArray[DCF77_START_HOUR + 1u] *  2u;
        _h += BitArray[DCF77_START_HOUR + 2u] *  4u;
        _h += BitArray[DCF77_START_HOUR + 3u] *  8u;
        _h += BitArray[DCF77_START_HOUR + 4u] * 10u;
        _h += BitArray[DCF77_START_HOUR + 5u] * 20u;
        _error = calc_parity(DCF77_START_HOUR, DCF77_P2);        

        
        _d  = BitArray[DCF77_START_DAY];
        _d += BitArray[DCF77_START_DAY + 1u] *  2u;
        _d += BitArray[DCF77_START_DAY + 2u] *  4u;
        _d += BitArray[DCF77_START_DAY + 3u] *  8u;
        _d += BitArray[DCF77_START_DAY + 4u] * 10u;
        _d += BitArray[DCF77_START_DAY + 5u] * 20u;
        
        _dn  = BitArray[DCF77_START_DAY_NUM];
        _dn += BitArray[DCF77_START_DAY_NUM + 1u] *  2u;
        _dn += BitArray[DCF77_START_DAY_NUM + 2u] *  4u;
        
        _m  = BitArray[DCF77_START_MONTH];
        _m += BitArray[DCF77_START_MONTH + 1u] *  2u;
        _m += BitArray[DCF77_START_MONTH + 2u] *  4u;
        _m += BitArray[DCF77_START_MONTH + 3u] *  8u;
        _m += BitArray[DCF77_START_MONTH + 4u] * 10u;
      
        _y  = BitArray[DCF77_START_YEAR];
        _y += BitArray[DCF77_START_YEAR + 1u] *  2u;
        _y += BitArray[DCF77_START_YEAR + 2u] *  4u;
        _y += BitArray[DCF77_START_YEAR + 3u] *  8u;
        _y += BitArray[DCF77_START_YEAR + 4u] * 10u;
        _y += BitArray[DCF77_START_YEAR + 5u] * 20u;
        _y += BitArray[DCF77_START_YEAR + 6u] * 40u;
        _y += BitArray[DCF77_START_YEAR + 7u] * 80u;
        _error = calc_parity(DCF77_START_DAY, DCF77_P3);


        if (_error == 0u)
        {
/* DEBUG */
            LCD_SetCursor(3u,2u);
            LCD_WriteInt(20u);
            LCD_WriteInt(_y);
            LCD_WriteChar('.');
            LCD_WriteInt(_m);
            LCD_WriteChar('.');
            LCD_WriteInt(_d);
            LCD_WriteChar('.');
            LCD_WriteChar(' ');
            LCD_WriteInt(_h);
            LCD_WriteChar(':');
            LCD_WriteInt(_min);
            LCD_SetCursor(4u,2u);
            LCD_WriteInt(_dn);
/* DEBUG */
        }
        else
        {
/* DEBUG */
            LCD_SetCursor(4u,2u);
            LCD_WriteInt(_error);
/* DEBUG */
            _error = 0u;
        }
    }
    bit_cnt = INIT_VALUE_UINT;
}

    
ISR(TIMER1_CAPT_vect)
{
    uint16 _pulse_width = INIT_VALUE_UINT;
    

    // Start of pulse - Falling (negative) edge
    if (0u == BIT_GET(TCCR1B, ICES1))
    {
        TCNT1 = 0u;
        
        // Set to rising edge
        BIT_SET(TCCR1B, ICES1);
    }
    // End of pulse - Rising (positive) edge
    else
    {
        // Capture the timer value
        _pulse_width = ICR1;
        // Reset the timer counter
        TCNT1     = 0u;

        // Set to falling edge
        BIT_CLR(TCCR1B, ICES1);
        
        // Captured bit is "0" (~100ms) 
        if ((DCF77_PULSE_ZERO_MIN_TIME < _pulse_width) && (_pulse_width < DCF77_PULSE_ZERO_MAX_TIME))
        {
            BitArray[bit_cnt] = 0u;
/* DEBUG */
            LCD_SetCursor(2u,10u);
            LCD_WriteString("  ");
            LCD_SetCursor(2u,10u);
            LCD_WriteInt(bit_cnt);
/* DEBUG */
            bit_cnt++;
        }
        // Captured bit is "1" (~200ms) 
        else if ((DCF77_PULSE_ONE_MIN_TIME < _pulse_width) && (_pulse_width < DCF77_PULSE_ONE_MAX_TIME))
        {
            BitArray[bit_cnt] = 1u;
/* DEBUG */
            LCD_SetCursor(2u,10u);
            LCD_WriteString("  ");
            LCD_SetCursor(2u,10u);
            LCD_WriteInt(bit_cnt);
/* DEBUG */
            bit_cnt++;
        }
        else
        {
            /* False pulse is detected */
        }
    }
}


ISR(TIMER2_OVF_vect)
{
/* DEBUG */ /*
    L_Task_1SEC = Flag_SET;
*/ /* DEBUG */
}


//******************************************************************************
//****** INIT
//******************************************************************************
void Task_Init(void)
{
    DISABLE_INTERRUPT();

    MCH_Init_Timer1();
    MCH_Init_Timer2();
    MCH_Init_Pins();
    //MCH_Init_I2C(100u);

    LCD_Init();
    LCD_SwitchOn();
    LCD_Clear();

    DHT22_Init();
    //DS1621_Init();

    RTC_SetDate(2015u,4u,1u,23u,59u,50u);

    //MCH_Init_Watchdog();

    ENABLE_INTERRUPT();
}


//******************************************************************************
//****** MAIN
//******************************************************************************
void Task_Main(void)
{
#ifdef CPU_LOAD_MEASUREMENT
    static volatile uint16 timer_start = 0u;
    static volatile uint16 timer_stop  = 0u;
#endif

    for (;;)
    {
        if (Flag_SET == L_Task_1SEC)
        {

#ifdef CPU_LOAD_MEASUREMENT
            timer_start = TCNT1;
#endif

            L_Task_1SEC = Flag_CLEAR;

            RTC_Refresh();
            MFC_Refresh();
            STC_Refresh();
            DHT22_Refresh();
            //DS1621_Refresh();
            LCM_Refresh();

            
#ifdef CPU_LOAD_MEASUREMENT
//******************************************************************************
//****** CPU LOAD in ms
//******************************************************************************
            timer_stop = TCNT1;

            static volatile uint16 cpuload     = 0u;
            static volatile uint16 cpuload_max = 0u;

            LCD_SetCursor(2u,1u);
            LCD_WriteString("                    ");
            
            //cpuload = (uint16)((((float32)(timer_stop - timer_start)) / ((float32)(OCR1A))) * 10000.0f);
            cpuload = (uint16)((timer_stop - timer_start) * (256.0f / (float32)F_CPU) * 1000.0f);
            LCD_SetCursor(2u,7u);
            //LCD_WriteInt(cpuload / 100u);
            //LCD_WriteChar('.');
            //LCD_WriteInt(cpuload % 100u);
            LCD_WriteInt(cpuload);
            LCD_WriteString("ms");
			
            if (cpuload > cpuload_max)
            {
                cpuload_max = cpuload;
            }

            LCD_SetCursor(2u,13u);
            //LCD_WriteInt(cpuload_max / 100u);
            //LCD_WriteChar('.');
            //LCD_WriteInt(cpuload_max % 100u);
            LCD_WriteInt(cpuload_max);
            LCD_WriteString("ms");
//******************************************************************************
//****** CPU LOAD
//******************************************************************************
#endif

        } // Task_1SEC

    } // main end-less loop
    
} // Task_Main
