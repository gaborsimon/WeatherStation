//v0.3

//====== Header includes =======================================================
#include "Tasking.h"


//====== Private Constants =====================================================


//====== Private Signals =======================================================
static volatile Flag L_TaskTrigger = Flag_CLEAR;
static volatile Flag L_LED         = Flag_CLEAR;


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
    L_TaskTrigger = Flag_SET;
}


void Task_Init(void)
{
    DISABLE_INTERRUPT();

    //MCH_Init_Watchdog();

    MCH_Init_Timer1CHA();
    MCH_Init_Pins();
    MCH_Init_I2C(100u);

    LCD_Init();
    LCD_SwitchOn();
    LCD_Clear();

    TMS_Init();

    GPIO_PANEL_LED_OFF;

    _delay_ms(2000u);

    RTC_SetDate(2013u,8u,16u,23u,59u,50u);

    ENABLE_INTERRUPT();
}


void Task_Main(void)
{
    static volatile uint16 timer_start = 0u;
    static volatile uint16 timer_stop  = 0u;

    for (;;)
    {

        //WD_RESET();
        if (Flag_SET == L_TaskTrigger)
        {
            timer_start = TCNT1;

            L_TaskTrigger = Flag_CLEAR;
            
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
            TMS_Refresh();

            LCM_Refresh();

            timer_stop = TCNT1;

//******************************************************************************
//****** CPU LOAD
//******************************************************************************
            static volatile uint16 cpuload     = 0u;
            static volatile uint16 cpuload_max = 0u;

            LCD_SetCursor(4u,1u);
            LCD_WriteString("                    ");

            cpuload = (uint16)((((float32)(timer_stop - timer_start)) / ((float32)(OCR1A))) * 10000.0f);
            LCD_SetCursor(4u,1u);
            LCD_WriteInt(cpuload / 100u);
            LCD_WriteChar('.');
            LCD_WriteInt(cpuload % 100u);


            if (cpuload > cpuload_max)
            {
                cpuload_max = cpuload;
            }

            LCD_SetCursor(4u,10u);
            LCD_WriteInt(cpuload_max / 100u);
            LCD_WriteChar('.');
            LCD_WriteInt(cpuload_max % 100u);
//******************************************************************************
//****** CPU LOAD
//******************************************************************************
        } // 1sec
        //WD_RESET();

    } // main end-less loop
    
}
