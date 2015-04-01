//====== Header includes =======================================================
#include "Tasking.h"


//====== Private Constants =====================================================
#define CPU_LOAD_MEASUREMENT


//====== Private Signals =======================================================
static volatile Flag L_Task_1SEC = Flag_CLEAR;
static volatile Flag L_LED       = Flag_CLEAR;


//====== Private Function Prototypes ===========================================


//====== Private Functions =====================================================
/*
 * Name:
 *
 * Description:
 *
 * Input:
 *
 * Output:
 */


//====== Public Signals ========================================================


//====== Public Functions ======================================================
/*
 * Name:
 *
 * Description:
 *
 * Input:
 *
 * Output:
 */

ISR(TIMER1_COMPA_vect)
{
    L_Task_1SEC = Flag_SET;
}


void Task_Init(void)
{
    DISABLE_INTERRUPT();

    MCH_Init_Timer1CHA();
    MCH_Init_Pins();
    MCH_Init_I2C(100u);

    LCD_Init();
    LCD_SwitchOn();
    LCD_Clear();

    TMS_Init();

    GPIO_PANEL_LED_OFF;

    RTC_SetDate(2015u,4u,1u,23u,59u,50u);

    //MCH_Init_Watchdog();

    ENABLE_INTERRUPT();
}


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
            
            // Heartbeat on panel
            if (Flag_CLEAR == L_LED)
            {
                L_LED = Flag_SET;
                GPIO_PANEL_LED_ON;
            }
            else if (Flag_SET == L_LED)
            {
                L_LED = Flag_CLEAR;
                GPIO_PANEL_LED_OFF;
            }
            //---------------------------- Heartbeat on panel

            RTC_Refresh();
            MFC_Refresh();
            STC_Refresh();
            //TMS_Refresh();
            LCM_Refresh();

            
#ifdef CPU_LOAD_MEASUREMENT
            timer_stop = TCNT1;
#endif
//******************************************************************************
//****** CPU LOAD
//******************************************************************************
#ifdef CPU_LOAD_MEASUREMENT
            static volatile uint16 cpuload     = 0u;
            static volatile uint16 cpuload_max = 0u;

            LCD_SetCursor(2u,10u);
            LCD_WriteString("           ");
            
            //cpuload = (uint16)((((float32)(timer_stop - timer_start)) / ((float32)(OCR1A))) * 10000.0f);
            cpuload = (uint16)((timer_stop - timer_start) * 0.000064f * 1000.0f);
            LCD_SetCursor(2u,10u);
            //LCD_WriteInt(cpuload / 100u);
            //LCD_WriteChar('.');
            //LCD_WriteInt(cpuload % 100u);
            LCD_WriteInt(cpuload);

            if (cpuload > cpuload_max)
            {
                cpuload_max = cpuload;
            }

            LCD_SetCursor(2u,15u);
            //LCD_WriteInt(cpuload_max / 100u);
            //LCD_WriteChar('.');
            //LCD_WriteInt(cpuload_max % 100u);
            LCD_WriteInt(cpuload_max);
#endif // CPU_LOAD_MEASUREMENT
//******************************************************************************
//****** CPU LOAD
//******************************************************************************
        } // Task_1SEC

    } // main end-less loop
    
} // Task_Main
