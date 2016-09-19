//====== Header includes =======================================================
#include "Tasking.h"


//====== Private Constants =====================================================
//#define L__CPU_LOAD_MEASUREMENT


//====== Private Signals =======================================================
static volatile G_Flag_e L_Task1SEC = Flag_CLEAR;


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
    L_Task1SEC = Flag_SET;
}


//******************************************************************************
//****** INIT
//******************************************************************************
void Task_Init(void)
{
    MCH__INTERRUPT_DISABLED();

    MCH_InitPins();
    MCH_InitTimer0();
    MCH_InitTimer1();
    MCH_InitTimer2();
    MCH_InitADC(); 
    //MCH_InitI2C(100u);
    MCH_InitWatchdog();
    MCH_InitSleepMode();

    LCM_Init();
    DHT22_Init();
    //DS1621_Init();

    RTC_SetDate(2000u,1u,1u,1u,Flag_CLEAR,0u,0u,0u);

    MCH__INTERRUPT_ENABLED();
}


//******************************************************************************
//****** MAIN
//******************************************************************************
void Task_Main(void)
{
    static G_Flag_e L_FirstRun = Flag_SET;

#ifdef L__CPU_LOAD_MEASUREMENT
    static volatile uint16 L_TimerStart = U__INIT_VALUE_UINT;
    static volatile uint16 L_TimerStop  = U__INIT_VALUE_UINT;
#endif

    for (;;)
    {

/******************************************************************/
/****** Event Triggered Jobs */
/******************************************************************/
        if (DCF77_Status_SYNCH_DONE == XDCF77__STATUS)
        {
            DCF77_Refresh();
            
            LCM_Refresh(LCM__RX_OK);
            LCM_Refresh(LCM__DATETIME);
            
            L_FirstRun = Flag_SET;
        }
        
/******************************************************************/
/****** Time Triggered Jobs */
/******************************************************************/
        if (Flag_SET == L_Task1SEC)
        {

#ifdef L__CPU_LOAD_MEASUREMENT
            L_TimerStart = TCNT1;
#endif
            
            L_Task1SEC = Flag_CLEAR;

            RTC_Refresh();
            DCF77_Refresh();
            DHT22_Refresh();
            LCM_BackLightControl(LCM__CONTROL_METHOD_ADAPTIVE, 0u);

            if (Flag_SET == XDHT22__DATA_UPDATED)
            {
                LCM_Refresh(LCM__DHT22);
            }
            
            if ((Flag_SET == L_FirstRun) || (Flag_SET == XRTC__TIMEDATE_NEWMINUTE))
            {
                LCM_Refresh(LCM__MINUTE);
            }
            
            if ((Flag_SET == L_FirstRun) || (Flag_SET == XRTC__TIMEDATE_NEWHOUR))
            {
                LCM_Refresh(LCM__HOUR);
            }
                
            if ((Flag_SET == L_FirstRun) || (Flag_SET == XRTC__TIMEDATE_NEWDAY))
            {
                STC_Refresh();
                MFC_Refresh();
                LCM_Refresh(LCM__DAY);
                LCM_Refresh(LCM__SUNTIME);
                LCM_Refresh(LCM__MOONFRACTION);
            }
            
            if ((Flag_SET == L_FirstRun) || (Flag_SET == XRTC__TIMEDATE_NEWMONTH))
            {
                LCM_Refresh(LCM__MONTH);
            }
   
            if ((Flag_SET == L_FirstRun) || (Flag_SET == XRTC__TIMEDATE_NEWYEAR))
            {
                LCM_Refresh(LCM__YEAR);
            }

            if (Flag_SET == L_FirstRun)
            {
                L_FirstRun = Flag_CLEAR;
            }            

#ifdef L__CPU_LOAD_MEASUREMENT
//******************************************************************************
//****** CPU LOAD in ms
//******************************************************************************
            L_TimerStop = TCNT1;

            static volatile uint16 L_CPULoad    = U__INIT_VALUE_UINT;
            static volatile uint16 L_CPULoadMax = U__INIT_VALUE_UINT;

            LCD_SetCursor(2u,1u);
            LCD_WriteString("                    ");
            
            //L_CPULoad = (uint16)((((float32)(L_TimerStop - L_TimerStart)) / ((float32)(OCR1A))) * 10000.0f);
            L_CPULoad = (uint16)((L_TimerStop - L_TimerStart) * (256.0f / (float32)F_CPU) * 1000.0f);
            LCD_SetCursor(2u,7u);
            //LCD_WriteInt(L_CPULoad / 100u);
            //LCD_WriteChar('.');
            //LCD_WriteInt(L_CPULoad % 100u);
            LCD_WriteInt(L_CPULoad);
            LCD_WriteString("ms");
            
            if (L_CPULoad > L_CPULoadMax)
            {
                L_CPULoadMax = L_CPULoad;
            }

            LCD_SetCursor(2u,13u);
            //LCD_WriteInt(L_CPULoadMax / 100u);
            //LCD_WriteChar('.');
            //LCD_WriteInt(L_CPULoadMax % 100u);
            LCD_WriteInt(L_CPULoadMax);
            LCD_WriteString("ms");
//******************************************************************************
//****** CPU LOAD
//******************************************************************************
#endif

            MCH__SLEEP();

        } // Task_1SEC

        MCH__WD_RESET();

    } // main end-less loop
    
} // Task_Main
