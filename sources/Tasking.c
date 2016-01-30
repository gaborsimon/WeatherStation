//====== Header includes =======================================================
#include "Tasking.h"


//====== Private Constants =====================================================
//#define CPU_LOAD_MEASUREMENT


//====== Private Signals =======================================================
static volatile Flag L_Task_1SEC = Flag_CLEAR;
static volatile Flag L_LED       = Flag_CLEAR;


//====== Private Function Prototypes ===========================================


//====== Private Functions =====================================================


//====== Public Signals ========================================================


//====== Public Functions ======================================================

//******************************************************************************
//****** INTERRUPT HANDLER
//******************************************************************************
ISR(TIMER1_COMPA_vect)
{
    L_Task_1SEC = Flag_SET;
}


//******************************************************************************
//****** INIT
//******************************************************************************
void Task_Init(void)
{
    DISABLE_INTERRUPT();

    MCH_Init_Timer1CHA();
    MCH_Init_Pins();
    MCH_Init_I2C(100u);

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
            timer_stop = TCNT1;
#endif
//******************************************************************************
//****** CPU LOAD in ms
//******************************************************************************
#ifdef CPU_LOAD_MEASUREMENT
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
#endif // CPU_LOAD_MEASUREMENT
//******************************************************************************
//****** CPU LOAD
//******************************************************************************
        } // Task_1SEC

    } // main end-less loop
    
} // Task_Main
