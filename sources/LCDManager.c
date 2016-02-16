//====== Header includes =======================================================
#include "LCDManager.h"


//====== Private Constants =====================================================
#define L_POS_ROW_RX                 (2u)
#define L_POS_COL_RX                 (18u)

#define L_POS_ROW_DATE               (1u)
#define L_POS_COL_DATE               (1u)

#define L_POS_ROW_TIME               (1u)
#define L_POS_COL_TIME              (16u)

#define L_POS_ROW_SUN_TIME_RISE      (3u)
#define L_POS_COL_SUN_TIME_RISE      (1u)
#define L_POS_ROW_SUN_TIME_SET       (4u)
#define L_POS_COL_SUN_TIME_SET       (1u)

#define L_POS_ROW_MOON_ICON          (3u)
#define L_POS_COL_MOON_ICON         (10u)
#define L_POS_ROW_MOON_PERC          (4u)
#define L_POS_COL_MOON_PERC          (9u)

#define L_POS_ROW_DS1621_TEMP        (2u)
#define L_POS_COL_DS1621_TEMP       (15u)

#define L_POS_ROW_DHT22_TEMP         (3u)
#define L_POS_COL_DHT22_TEMP        (15u)
#define L_POS_ROW_DHT22_HUM          (4u)
#define L_POS_COL_DHT22_HUM         (15u)

#define L_CUSTOM_CHAR_POS_SUNRISE       (1u)
#define L_CUSTOM_CHAR_POS_SUNSET        (2u)
#define L_CUSTOM_CHAR_POS_MOON_L        (3u)
#define L_CUSTOM_CHAR_POS_MOON_R        (4u)
#define L_CUSTOM_CHAR_POS_DCF77_RX_OK   (5u)
#define L_CUSTOM_CHAR_POS_DCF77_RX_NO   (6u)


//====== Private Signals =======================================================


//====== Private Function Prototypes ===========================================


//====== Private Functions =====================================================
void RefreshRxOK(void)
{
    LCD_SetCursor(L_POS_ROW_RX, L_POS_COL_RX);
    LCD_WriteCustomChar(L_CUSTOM_CHAR_POS_DCF77_RX_OK);
}

void RefreshRxNO(void)
{
    LCD_SetCursor(L_POS_ROW_RX, L_POS_COL_RX);
    LCD_WriteCustomChar(L_CUSTOM_CHAR_POS_DCF77_RX_NO);
}

void RefreshYear(void)
{
    LCD_SetCursor(L_POS_ROW_DATE, L_POS_COL_DATE);
    LCD_WriteInt(XRTC_TIMEDATE_YEAR);
    LCD_WriteChar('-');
}

void RefreshMonth(void)
{
    LCD_SetCursor(L_POS_ROW_DATE, L_POS_COL_DATE + 5u);
    if (XRTC_TIMEDATE_MONTH < 10u)
    {
        LCD_WriteChar('0');
    }
    LCD_WriteInt(XRTC_TIMEDATE_MONTH);
    LCD_WriteChar('-');
}

void RefreshDay(void)
{
    LCD_SetCursor(L_POS_ROW_DATE, L_POS_COL_DATE + 8u);
    if (XRTC_TIMEDATE_DAY < 10u)
    {
        LCD_WriteChar('0');
    }
    LCD_WriteInt(XRTC_TIMEDATE_DAY);
}

void RefreshHour(void)
{
    LCD_SetCursor(L_POS_ROW_TIME, L_POS_COL_TIME);
    if (XRTC_TIMEDATE_HOUR < 10u)
    {
        LCD_WriteChar(' ');
    }
    LCD_WriteInt(XRTC_TIMEDATE_HOUR);

    LCD_SetCursor(L_POS_ROW_TIME, L_POS_COL_TIME + 2u);
    LCD_WriteChar(':');
}

void RefreshMinute(void)
{
    LCD_SetCursor(L_POS_ROW_TIME, L_POS_COL_TIME + 3u);
    if (XRTC_TIMEDATE_MINUTE < 10u)
    {
        LCD_WriteChar('0');
    }
    LCD_WriteInt(XRTC_TIMEDATE_MINUTE);
}

void RefreshSunTime(void)
{
    LCD_SetCursor(L_POS_ROW_SUN_TIME_RISE, L_POS_COL_SUN_TIME_RISE);
    LCD_WriteCustomChar(L_CUSTOM_CHAR_POS_SUNRISE);

    LCD_SetCursor(L_POS_ROW_SUN_TIME_SET, L_POS_COL_SUN_TIME_SET);
    LCD_WriteCustomChar(L_CUSTOM_CHAR_POS_SUNSET);

    LCD_SetCursor(L_POS_ROW_SUN_TIME_RISE, L_POS_COL_SUN_TIME_RISE + 1u);
    if (XSTC_SUN_RISEHOUR < 10u)
    {
        LCD_WriteChar(' ');
    }
    LCD_WriteInt(XSTC_SUN_RISEHOUR);
    LCD_WriteChar(':');
    if (XSTC_SUN_RISEMINUTE < 10u)
    {
        LCD_WriteChar('0');
    }
    LCD_WriteInt(XSTC_SUN_RISEMINUTE);

    LCD_SetCursor(L_POS_ROW_SUN_TIME_SET, L_POS_COL_SUN_TIME_SET + 1u);
    if (XSTC_SUN_SETHOUR < 10u)
    {
        LCD_WriteChar(' ');
    }
    LCD_WriteInt(XSTC_SUN_SETHOUR);
    LCD_WriteChar(':');
    if (XSTC_SUN_SETMINUTE < 10u)
    {
        LCD_WriteChar('0');
    }
    LCD_WriteInt(XSTC_SUN_SETMINUTE);
}

void RefreshMoonFraction(void)
{
    switch (XMFC_MOON_DIRECTION)
    {
        case (MOON_WAXES):
        {
            if (XMFC_MOON_FRACTION <= 10u)
            {
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_L, lcd_char_moon_left_100);
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_R, lcd_char_moon_right_100);
            }
            else if ((XMFC_MOON_FRACTION > 10u) && (XMFC_MOON_FRACTION <= 40u))
            {
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_L, lcd_char_moon_left_100);
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_R, lcd_char_moon_right_75);
            }
            else if ((XMFC_MOON_FRACTION > 40u) && (XMFC_MOON_FRACTION <= 60u))
            {
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_L, lcd_char_moon_left_100);
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_R, lcd_char_moon_right_0);
            }
            else if ((XMFC_MOON_FRACTION > 60u) && (XMFC_MOON_FRACTION <= 90u))
            {
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_L, lcd_char_moon_left_25);
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_R, lcd_char_moon_right_0);
            }
            else if (XMFC_MOON_FRACTION > 90u)
            {
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_L, lcd_char_moon_left_0);
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_R, lcd_char_moon_right_0);
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
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_L, lcd_char_moon_left_100);
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_R, lcd_char_moon_right_100);
            }
            else if ((XMFC_MOON_FRACTION > 10u) && (XMFC_MOON_FRACTION <= 40u))
            {
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_L, lcd_char_moon_left_75);
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_R, lcd_char_moon_right_100);
            }
            else if ((XMFC_MOON_FRACTION > 40u) && (XMFC_MOON_FRACTION <= 60u))
            {
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_L, lcd_char_moon_left_0);
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_R, lcd_char_moon_right_100);
            }
            else if ((XMFC_MOON_FRACTION > 60u) && (XMFC_MOON_FRACTION <= 90u))
            {
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_L, lcd_char_moon_left_0);
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_R, lcd_char_moon_right_25);
            }
            else if (XMFC_MOON_FRACTION > 90u)
            {
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_L, lcd_char_moon_left_0);
                LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_R, lcd_char_moon_right_0);
            }
            else
            {
                // Nothing to do because Moon Fraction is between 0..100
            }
        }
        break;

        default:
        {
            LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_L, lcd_char_moon_left_100);
            LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_MOON_R, lcd_char_moon_right_100);
        }
        break;
    } 

    LCD_SetCursor(L_POS_ROW_MOON_ICON, L_POS_COL_MOON_ICON);
    LCD_WriteCustomChar(L_CUSTOM_CHAR_POS_MOON_L);
    LCD_SetCursor(L_POS_ROW_MOON_ICON, L_POS_COL_MOON_ICON + 1u);
    LCD_WriteCustomChar(L_CUSTOM_CHAR_POS_MOON_R);

    LCD_SetCursor(L_POS_ROW_MOON_PERC, L_POS_COL_MOON_PERC);
    if ((100u > XMFC_MOON_FRACTION) && (10u < XMFC_MOON_FRACTION))
    {
        LCD_WriteChar(' ');
    } 
    else if (10u > XMFC_MOON_FRACTION)
    {
        LCD_WriteChar(' ');
        LCD_WriteChar(' ');
    }
    else
    {
        ; // No additional character is needed. 
    }
    LCD_WriteInt(XMFC_MOON_FRACTION);
    LCD_WriteChar('%');
}

void RefreshDHT22(void)
{
    //**************************************************************************
    //****** TEMPERATURE - DHT22
    //**************************************************************************
    if (Signal_RELIABLE == XDHT22_DATA_QUALIFIER)
    {
        LCD_SetCursor(L_POS_ROW_DHT22_TEMP, L_POS_COL_DHT22_TEMP);
        if (XDHT22_TEMPERATURE_VALUE >= 10.0f)
        {
            LCD_WriteChar(' ');
        }
        else if ((XDHT22_TEMPERATURE_VALUE < 10.0f) &&
                 (XDHT22_TEMPERATURE_VALUE >= 0.0f))
        {
            LCD_WriteChar(' ');
            LCD_WriteChar(' ');
        }
        else if ((XDHT22_TEMPERATURE_VALUE < 0.0f) &&
                 (XDHT22_TEMPERATURE_VALUE < -10.0f))
        {
            LCD_WriteChar(' ');
        }
        else
        {
            // Nothing to do
            ;
        }

        LCD_WriteInt((sint8) XDHT22_TEMPERATURE_VALUE);
        LCD_WriteChar('.');
        LCD_WriteInt((uint8)((XDHT22_TEMPERATURE_VALUE - ((sint8)XDHT22_TEMPERATURE_VALUE)) * 10.0f));
        LCD_WriteChar('C');
    }
    else
    {
        LCD_SetCursor(L_POS_ROW_DHT22_TEMP, L_POS_COL_DHT22_TEMP);
        LCD_WriteString("  -- C");
    }


    //**************************************************************************
    //****** HUMIDITY - DHT22
    //**************************************************************************
    if (Signal_RELIABLE == XDHT22_DATA_QUALIFIER)
    {
        LCD_SetCursor(L_POS_ROW_DHT22_HUM, L_POS_COL_DHT22_HUM);
        if (XDHT22_HUMIDITY_VALUE >= 10.0f)
        {
            LCD_WriteChar(' ');
        }
        else if ((XDHT22_HUMIDITY_VALUE < 10.0f) &&
                 (XDHT22_HUMIDITY_VALUE >= 0.0f))
        {
            LCD_WriteChar(' ');
            LCD_WriteChar(' ');
        }
        else if ((XDHT22_HUMIDITY_VALUE < 0.0f) &&
                 (XDHT22_HUMIDITY_VALUE < -10.0f))
        {
            LCD_WriteChar(' ');
        }
        else
        {
            // Nothing to do
            ;
        }

        LCD_WriteInt((sint8) XDHT22_HUMIDITY_VALUE);
        LCD_WriteChar('.');
        LCD_WriteInt((uint8)((XDHT22_HUMIDITY_VALUE - ((sint8)XDHT22_HUMIDITY_VALUE)) * 10.0f));
        LCD_WriteChar('%');
    }
    else
    {
        LCD_SetCursor(L_POS_ROW_DHT22_HUM, L_POS_COL_DHT22_HUM);
        LCD_WriteString("  -- %");
    }
}


//====== Public Signals ========================================================


//====== Public Functions ======================================================
/*
 * Name: LCM_Init
 *
 * Description: This is the initialization function that set the custom
 *              characters and init and clear the LCD.
 *
 * Input: None
 *
 * Output: None
 */
void LCM_Init(void)
{
    LCD_Init();
    LCD_SwitchOn();
    LCD_Clear();

    LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_SUNRISE, lcd_char_sunrise);
    LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_SUNSET, lcd_char_sunset);
    LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_DCF77_RX_OK, lcd_char_dcf77_rx_ok);
    LCD_StoreCustomChar(L_CUSTOM_CHAR_POS_DCF77_RX_NO, lcd_char_dcf77_rx_no);
}


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
void LCM_Refresh(uint8 element)
{
    switch (element)
    {
        case LCM_RX_OK:
        {
            RefreshRxOK();
        }
        break;

        case LCM_RX_NO:
        {
            RefreshRxNO();
        }
        break;

        case LCM_DATETIME:
        {
            RefreshYear();
            RefreshMonth();
            RefreshDay();

            RefreshHour();
            RefreshMinute();
        }
        break;

        case LCM_YEAR:
        {
            RefreshYear();
        }
        break;


        case LCM_MONTH:
        {
            RefreshMonth();
        }
        break;

        case LCM_DAY:
        {
            RefreshDay();
        }
        break;

        case LCM_HOUR:
        {
            RefreshHour();
        }
        break;
        
        case LCM_MINUTE:
        {
            RefreshMinute();
        }
        break;
        
        case LCM_SUNTIME:
        {
            RefreshSunTime();
        }
        break;
        
        case LCM_MOONFRACTION:
        {
            RefreshMoonFraction();
        }
        break;
        
        case LCM_DHT22:
        {
            RefreshDHT22();
        }
        break;
    } 
}
