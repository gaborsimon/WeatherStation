//====== Header includes =======================================================
#include "LCDManager.h"


//====== Private Constants =====================================================
const char L_LCD_String_DOT[]           PROGMEM = ".";
const char L_LCD_String_ZERO[]          PROGMEM = "0";
const char L_LCD_String_SPACE[]         PROGMEM = " ";
const char L_LCD_String_DASH[]          PROGMEM = "-";
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
    if ((Flag_SET == FirstRun) || (Flag_SET == XRTC_TIMEDATE_NEWMONTH))
    {
        LCD_SetCursor(1u,1u);
        LCD_WriteInt(XRTC_TIMEDATE_YEAR);
        LCD_WriteStringM(L_LCD_String_DASH);
    }
    
    if ((Flag_SET == FirstRun) || (Flag_SET == XRTC_TIMEDATE_NEWMONTH))
    {
        LCD_SetCursor(1u,6u);
        if (XRTC_TIMEDATE_MONTH < 10u)
        {
            LCD_WriteStringM(L_LCD_String_ZERO);
        }
        LCD_WriteInt(XRTC_TIMEDATE_MONTH);
        LCD_WriteStringM(L_LCD_String_DASH);
    }

    if ((Flag_SET == FirstRun) || (Flag_SET == XRTC_TIMEDATE_NEWDAY))
    {
        LCD_SetCursor(1u,9u);
        if (XRTC_TIMEDATE_DAY < 10u)
        {
            LCD_WriteStringM(L_LCD_String_ZERO);
        }
        LCD_WriteInt(XRTC_TIMEDATE_DAY);
    }
    

    //**************************************************************************
    //****** TIME
    //**************************************************************************
    if ((Flag_SET == FirstRun) || (Flag_SET == XRTC_TIMEDATE_NEWHOUR))
    {
        LCD_SetCursor(1u,16u);
        if (XRTC_TIMEDATE_HOUR < 10u)
        {
            LCD_WriteStringM(L_LCD_String_SPACE);
        }
        LCD_WriteInt(XRTC_TIMEDATE_HOUR);
    }

    if (Flag_SET == FirstRun)
    {
        LCD_SetCursor(1u,18u);
        LCD_WriteChar(':');
    }

    if ((Flag_SET == FirstRun) || (Flag_SET == XRTC_TIMEDATE_NEWMINUTE))
    {
        LCD_SetCursor(1u,19u);
        if (XRTC_TIMEDATE_MINUTE < 10u)
        {
            LCD_WriteStringM(L_LCD_String_ZERO);
        }
        LCD_WriteInt(XRTC_TIMEDATE_MINUTE);
    }

    
    //**************************************************************************
    //****** SUN TIME
    //**************************************************************************
    if (Flag_SET == FirstRun)
    {
        LCD_StoreCustomChar(1u, lcd_char_sunrise);
        LCD_SetCursor(3u,1u);
        LCD_WriteCustomChar(1u);

        LCD_StoreCustomChar(2u, lcd_char_sunset);
        LCD_SetCursor(4u,1u);
        LCD_WriteCustomChar(2u);
    }
    
    if ((Flag_SET == FirstRun) || (Flag_SET == XRTC_TIMEDATE_NEWDAY))
    {
        LCD_SetCursor(3u,2u);
        if (XSTC_SUN_RISEHOUR < 10u)
        {
            LCD_WriteStringM(L_LCD_String_SPACE);
        }
        LCD_WriteInt(XSTC_SUN_RISEHOUR);
        LCD_WriteChar(':');
        if (XSTC_SUN_RISEMINUTE < 10u)
        {
            LCD_WriteStringM(L_LCD_String_ZERO);
        }
        LCD_WriteInt(XSTC_SUN_RISEMINUTE);
    
        LCD_SetCursor(4u,2u);

        if (XSTC_SUN_SETHOUR < 10u)
        {
            LCD_WriteStringM(L_LCD_String_SPACE);
        }
        LCD_WriteInt(XSTC_SUN_SETHOUR);
        LCD_WriteChar(':');
        if (XSTC_SUN_SETMINUTE < 10u)
        {
            LCD_WriteStringM(L_LCD_String_ZERO);
        }
        LCD_WriteInt(XSTC_SUN_SETMINUTE);
    }
    
    
    //**************************************************************************
    //****** MOON FRACTION
    //**************************************************************************
    if ((Flag_SET == FirstRun) || (Flag_SET == XRTC_TIMEDATE_NEWDAY))
    {
        switch (XMFC_MOON_DIRECTION)
        {
            case (MOON_WAXES):
            {
                if (XMFC_MOON_FRACTION <= 10u)
                {
                    LCD_StoreCustomChar(3u, lcd_char_moon_left_100);
                    LCD_SetCursor(3u,10u);
                    LCD_WriteCustomChar(3u);
                    LCD_StoreCustomChar(4u, lcd_char_moon_right_100);
                    LCD_SetCursor(3u,11u);
                    LCD_WriteCustomChar(4u);
                }
                else if ((XMFC_MOON_FRACTION > 10u) && (XMFC_MOON_FRACTION <= 40u))
                {
                    LCD_StoreCustomChar(3u, lcd_char_moon_left_100);
                    LCD_SetCursor(3u,10u);
                    LCD_WriteCustomChar(3u);
                    LCD_StoreCustomChar(4u, lcd_char_moon_right_75);
                    LCD_SetCursor(3u,11u);
                    LCD_WriteCustomChar(4u);
                }
                else if ((XMFC_MOON_FRACTION > 40u) && (XMFC_MOON_FRACTION <= 60u))
                {
                    LCD_StoreCustomChar(3u, lcd_char_moon_left_100);
                    LCD_SetCursor(3u,10u);
                    LCD_WriteCustomChar(3u);
                    LCD_StoreCustomChar(4u, lcd_char_moon_right_0);
                    LCD_SetCursor(3u,11u);
                    LCD_WriteCustomChar(4u);
                }
                else if ((XMFC_MOON_FRACTION > 60u) && (XMFC_MOON_FRACTION <= 90u))
                {
                    LCD_StoreCustomChar(3u, lcd_char_moon_left_25);
                    LCD_SetCursor(3u,10u);
                    LCD_WriteCustomChar(3u);
                    LCD_StoreCustomChar(4u, lcd_char_moon_right_0);
                    LCD_SetCursor(3u,11u);
                    LCD_WriteCustomChar(4u);
                }
                else if (XMFC_MOON_FRACTION > 90u)
                {
                    LCD_StoreCustomChar(3u, lcd_char_moon_left_0);
                    LCD_SetCursor(3u,10u);
                    LCD_WriteCustomChar(3u);
                    LCD_StoreCustomChar(4u, lcd_char_moon_right_0);
                    LCD_SetCursor(3u,11u);
                    LCD_WriteCustomChar(4u);
                }
                else
                {
                    // Nothing to do because Moon Fraction is between 0..100
                }
            }
            break;
            
            case (MOON_WANES):
            {
                if (XMFC_MOON_FRACTION <= 10u)
                {
                    LCD_StoreCustomChar(3u, lcd_char_moon_left_100);
                    LCD_SetCursor(3u,10u);
                    LCD_WriteCustomChar(3u);
                    LCD_StoreCustomChar(4u, lcd_char_moon_right_100);
                    LCD_SetCursor(3u,11u);
                    LCD_WriteCustomChar(4u);
                }
                else if ((XMFC_MOON_FRACTION > 10u) && (XMFC_MOON_FRACTION <= 40u))
                {
                    LCD_StoreCustomChar(3u, lcd_char_moon_left_75);
                    LCD_SetCursor(3u,10u);
                    LCD_WriteCustomChar(3u);
                    LCD_StoreCustomChar(4u, lcd_char_moon_right_100);
                    LCD_SetCursor(3u,11u);
                    LCD_WriteCustomChar(4u);
                }
                else if ((XMFC_MOON_FRACTION > 40u) && (XMFC_MOON_FRACTION <= 60u))
                {
                    LCD_StoreCustomChar(3u, lcd_char_moon_left_0);
                    LCD_SetCursor(3u,10u);
                    LCD_WriteCustomChar(3u);
                    LCD_StoreCustomChar(4u, lcd_char_moon_right_100);
                    LCD_SetCursor(3u,11u);
                    LCD_WriteCustomChar(4u);
                }
                else if ((XMFC_MOON_FRACTION > 60u) && (XMFC_MOON_FRACTION <= 90u))
                {
                    LCD_StoreCustomChar(3u, lcd_char_moon_left_0);
                    LCD_SetCursor(3u,10u);
                    LCD_WriteCustomChar(3u);
                    LCD_StoreCustomChar(4u, lcd_char_moon_right_25);
                    LCD_SetCursor(3u,11u);
                    LCD_WriteCustomChar(4u);
                }
                else if (XMFC_MOON_FRACTION > 90u)
                {
                    LCD_StoreCustomChar(3u, lcd_char_moon_left_0);
                    LCD_SetCursor(3u,10u);
                    LCD_WriteCustomChar(3u);
                    LCD_StoreCustomChar(4u, lcd_char_moon_right_0);
                    LCD_SetCursor(3u,11u);
                    LCD_WriteCustomChar(4u);
                }
                else
                {
                    // Nothing to do because Moon Fraction is between 0..100
                }
            }
            break;
            
            default:
            {
                LCD_StoreCustomChar(3u, lcd_char_moon_left_100);
                LCD_SetCursor(3u,10u);
                LCD_WriteCustomChar(3u);
                LCD_StoreCustomChar(4u, lcd_char_moon_right_100);
                LCD_SetCursor(3u,11u);
                LCD_WriteCustomChar(4u);
            }
            break;
        } 
    
        LCD_SetCursor(4u,9u);
        if ((100u > XMFC_MOON_FRACTION) && (10u < XMFC_MOON_FRACTION))
        {
            LCD_WriteStringM(L_LCD_String_SPACE);
        } 
        else if (10u > XMFC_MOON_FRACTION)
        {
            LCD_WriteStringM(L_LCD_String_SPACE);
            LCD_WriteStringM(L_LCD_String_SPACE);
        }
        else
        {
            ; // No additional character is needed. 
        }
        LCD_WriteInt(XMFC_MOON_FRACTION);
        LCD_WriteChar('%');
    }
    
    //**************************************************************************
    //****** BATTERY
    //**************************************************************************
/*
    if (Flag_SET == FirstRun)
    {
        LCD_StoreCustomChar(0u, lcd_char_battery_100);
        LCD_SetCursor(1u,20u);
        LCD_WriteCustomChar(0u);
    }
*/

    //**************************************************************************
    //****** TEMPERATURE OUTDOOR
    //**************************************************************************
/*
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
*/

    FirstRun = Flag_CLEAR;
}
