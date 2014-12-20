// v3.1

//====== Header includes =======================================================
#include "LCDManager.h"


//====== Private Constants =====================================================
const char L_LCD_String_DOT[]           PROGMEM = ".";
const char L_LCD_String_ZERO[]          PROGMEM = "0";
const char L_LCD_String_SPACE[]         PROGMEM = " ";
const char L_LCD_String_NOT_AVAILABLE[] PROGMEM = "--";


//====== Private Signals =======================================================


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
 * Name: LCM_Refresh
 *
 * Description: This is the main function that is responsible for putting
 *              the information into the LCD display.
 *
 * Input: None
 *
 * Output: None
 */
void LCM_Refresh(void)
{
    static Flag FirstRun = Flag_SET;

    
    //**************************************************************************
    //****** DATE
    //**************************************************************************
    if ((Flag_SET == FirstRun) || (Flag_SET == XRTC_GET_NEWMONTH))
    {
        LCD_SetCursor(1u,1u);
        if (XRTC_GET_MONTH < 10u)
        {
            LCD_WriteStringM(L_LCD_String_ZERO);
        }
        LCD_WriteInt(XRTC_GET_MONTH);
        LCD_WriteStringM(L_LCD_String_DOT);
    }

    if ((Flag_SET == FirstRun) || (Flag_SET == XRTC_GET_NEWDAY))
    {
        LCD_SetCursor(1u,4u);
        if (XRTC_GET_DAY < 10u)
        {
            LCD_WriteStringM(L_LCD_String_ZERO);
        }
        LCD_WriteInt(XRTC_GET_DAY);
        LCD_WriteStringM(L_LCD_String_DOT);
    }

    //**************************************************************************
    //****** TIME
    //**************************************************************************
    if ((Flag_SET == FirstRun) || (Flag_SET == XRTC_GET_NEWHOUR))
    {
        LCD_SetCursor(1u,9u);
        if (XRTC_GET_HOUR < 10u)
        {
            LCD_WriteStringM(L_LCD_String_ZERO);
        }
        LCD_WriteInt(XRTC_GET_HOUR);
    }

    if (Flag_SET == FirstRun)
    {
        LCD_SetCursor(1u,11u);
        LCD_WriteChar(':');
    }

    if ((Flag_SET == FirstRun) || (Flag_SET == XRTC_GET_NEWMINUTE))
    {
        LCD_SetCursor(1u,12u);
        if (XRTC_GET_MINUTE < 10u)
        {
            LCD_WriteStringM(L_LCD_String_ZERO);
        }
        LCD_WriteInt(XRTC_GET_MINUTE);
    }

    //**************************************************************************
    //****** BATTERY
    //**************************************************************************
    if (Flag_SET == FirstRun)
    {
        LCD_StoreCustomChar(0u, lcd_char_battery_100);
        LCD_SetCursor(1u,20u);
        LCD_WriteCustomChar(0u);
    }

    //**************************************************************************
    //****** TEMPERATURE OUTDOOR
    //**************************************************************************
    if (Flag_SET == FirstRun)
    {
        LCD_StoreCustomChar(1u, lcd_char_temp_min);
        LCD_SetCursor(3u,15u);
        LCD_WriteCustomChar(1u);

        LCD_StoreCustomChar(2u, lcd_char_temp_max);
        LCD_SetCursor(3u,20u);
        LCD_WriteCustomChar(2u);
    }

    if (Signal_RELIABLE == XTMS_TEMPERATURE_OUT_QUAL)
    {
        LCD_SetCursor(3u,8u);
        if (XTMS_TEMPERATURE_OUT_VALUE >= 10)
        {
            LCD_WriteStringM(L_LCD_String_SPACE);
        }
        else if ((XTMS_TEMPERATURE_OUT_VALUE < 10) &&
                 (XTMS_TEMPERATURE_OUT_VALUE >= 0))
        {
            LCD_WriteStringM(L_LCD_String_SPACE);
            LCD_WriteStringM(L_LCD_String_SPACE);
        }
        else if ((XTMS_TEMPERATURE_OUT_VALUE < 0) &&
                 (XTMS_TEMPERATURE_OUT_VALUE >= -9))
        {
            LCD_WriteStringM(L_LCD_String_SPACE);
        }
        else
        {
            // Nothing to do
            ;
        }

        LCD_WriteInt(XTMS_TEMPERATURE_OUT_VALUE);

        
        LCD_SetCursor(3u,12u);
        if (XTMS_TEMPERATURE_OUT_MIN >= 10)
        {
            LCD_WriteStringM(L_LCD_String_SPACE);
        }
        else if ((XTMS_TEMPERATURE_OUT_MIN < 10) &&
                 (XTMS_TEMPERATURE_OUT_MIN >= 0))
        {
            LCD_WriteStringM(L_LCD_String_SPACE);
            LCD_WriteStringM(L_LCD_String_SPACE);
        }
        else if ((XTMS_TEMPERATURE_OUT_MIN < 0) &&
                 (XTMS_TEMPERATURE_OUT_MIN >= -9))
        {
            LCD_WriteStringM(L_LCD_String_SPACE);
        }
        else
        {
            // Nothing to do
            ;
        }

        LCD_WriteInt(XTMS_TEMPERATURE_OUT_MIN);


        LCD_SetCursor(3u,17u);
        if (XTMS_TEMPERATURE_OUT_MAX >= 10)
        {
            LCD_WriteStringM(L_LCD_String_SPACE);
        }
        else if ((XTMS_TEMPERATURE_OUT_MAX < 10) &&
                 (XTMS_TEMPERATURE_OUT_MAX >= 0))
        {
            LCD_WriteStringM(L_LCD_String_SPACE);
            LCD_WriteStringM(L_LCD_String_SPACE);
        }
        else if ((XTMS_TEMPERATURE_OUT_MAX < 0) &&
                 (XTMS_TEMPERATURE_OUT_MAX >= -9))
        {
            LCD_WriteStringM(L_LCD_String_SPACE);
        }
        else
        {
            // Nothing to do
            ;
        }

        LCD_WriteInt(XTMS_TEMPERATURE_OUT_MAX);
    }
    else
    {
        LCD_SetCursor(3u,9u);
        LCD_WriteStringM(L_LCD_String_NOT_AVAILABLE);

        LCD_SetCursor(3u,13u);
        LCD_WriteStringM(L_LCD_String_NOT_AVAILABLE);

        LCD_SetCursor(3u,18u);
        LCD_WriteStringM(L_LCD_String_NOT_AVAILABLE);
    }

    FirstRun = Flag_CLEAR;
}
