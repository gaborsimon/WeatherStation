//====== Header includes =======================================================
#include "Tasking.h"


//====== Private Constants =====================================================
//#define CPU_LOAD_MEASUREMENT
#define L_DHT22_READ_PERIOD_SEC     (10u)


//====== Private Signals =======================================================
static volatile Flag L_Task_1SEC = Flag_CLEAR;


//====== Private Function Prototypes ===========================================


//====== Private Functions =====================================================


//====== Public Signals ========================================================


//====== Public Functions ======================================================

//******************************************************************************
//****** INTERRUPT HANDLER
//******************************************************************************
ISR(TIMER1_COMPA_vect)
{
    DCF77_Callback_TimerOverflow();
}

    
ISR(TIMER1_CAPT_vect)
{
    DCF77_Callback_InputCapture();
}


ISR(TIMER2_OVF_vect)
{
    L_Task_1SEC = Flag_SET;
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

    LCM_Init();

    DHT22_Init();
    //DS1621_Init();

    RTC_SetDate(2000u,1u,1u,1u,0u,0u,0u);

    //MCH_Init_Watchdog();

    ENABLE_INTERRUPT();
}


//******************************************************************************
//****** MAIN
//******************************************************************************
void Task_Main(void)
{
    static Flag  FirstRun       = Flag_SET;
    static uint8 DHT22Counter   = INIT_VALUE_UINT;

#ifdef CPU_LOAD_MEASUREMENT
    static volatile uint16 timer_start = 0u;
    static volatile uint16 timer_stop  = 0u;
#endif

    for (;;)
    {
        if (Flag_SET == XDCF77_SYNC_DONE)
        {
            DCF77_SyncDone = Flag_CLEAR;
            FirstRun       = Flag_SET;
    
            LCM_Refresh(LCM_RX_OK);
            LCM_Refresh(LCM_DATETIME);
        }
        
        if (Flag_SET == L_Task_1SEC)
        {

#ifdef CPU_LOAD_MEASUREMENT
            timer_start = TCNT1;
#endif

            L_Task_1SEC = Flag_CLEAR;
            DHT22Counter++;

            RTC_Refresh();

            // LCD refresh, only the relevant data has to be updated 
            if (L_DHT22_READ_PERIOD_SEC == DHT22Counter)
            {
                DHT22Counter = INIT_VALUE_UINT;
                DHT22_Refresh();
                LCM_Refresh(LCM_DHT22);
            }

            if ((Flag_SET == FirstRun) || (Flag_SET == XRTC_TIMEDATE_NEWMINUTE))
            {
                LCM_Refresh(LCM_MINUTE);
            }
            
            if ((Flag_SET == FirstRun) || (Flag_SET == XRTC_TIMEDATE_NEWHOUR))
            {
                LCM_Refresh(LCM_HOUR);
            }
                
            if ((Flag_SET == FirstRun) || (Flag_SET == XRTC_TIMEDATE_NEWDAY))
            {
                STC_Refresh();
                MFC_Refresh();
                LCM_Refresh(LCM_DAY);
                LCM_Refresh(LCM_SUNTIME);
                LCM_Refresh(LCM_MOONFRACTION);
            }
            
            if ((Flag_SET == FirstRun) || (Flag_SET == XRTC_TIMEDATE_NEWMONTH))
            {
                LCM_Refresh(LCM_MONTH);
            }
   
            if ((Flag_SET == FirstRun) || (Flag_SET == XRTC_TIMEDATE_NEWYEAR))
            {
                LCM_Refresh(LCM_YEAR);
            }

            FirstRun = Flag_CLEAR;            
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
