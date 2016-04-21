//====== Header includes =======================================================
#include "LCDManager.h"


//====== Private Constants =====================================================
#define L__POS_ROW_RX                    (2u)
#define L__POS_COL_RX                   (18u)

#define L__POS_ROW_DATE                  (1u)
#define L__POS_COL_DATE                  (1u)

#define L__POS_ROW_DAYNAME               (1u)
#define L__POS_COL_DAYNAME              (12u)

#define L__POS_ROW_TIME                  (1u)
#define L__POS_COL_TIME                 (16u)

#define L__POS_ROW_SUN_TIME_RISE         (3u)
#define L__POS_COL_SUN_TIME_RISE         (1u)
#define L__POS_ROW_SUN_TIME_SET          (4u)
#define L__POS_COL_SUN_TIME_SET          (1u)

#define L__POS_ROW_MOON_ICON             (3u)
#define L__POS_COL_MOON_ICON            (10u)
#define L__POS_ROW_MOON_PERC             (4u)
#define L__POS_COL_MOON_PERC             (9u)

#define L__POS_ROW_DS1621_TEMP           (2u)
#define L__POS_COL_DS1621_TEMP          (15u)

#define L__POS_ROW_DHT22_TEMP            (3u)
#define L__POS_COL_DHT22_TEMP           (15u)
#define L__POS_ROW_DHT22_HUM             (4u)
#define L__POS_COL_DHT22_HUM            (15u)

#define L__CUSTOM_CHAR_POS_SUNRISE       (1u)
#define L__CUSTOM_CHAR_POS_SUNSET        (2u)
#define L__CUSTOM_CHAR_POS_MOON_L        (3u)
#define L__CUSTOM_CHAR_POS_MOON_R        (4u)
#define L__CUSTOM_CHAR_POS_DCF77_RX_OK   (5u)
#define L__CUSTOM_CHAR_POS_DCF77_RX_NO   (6u)


//====== Private Signals =======================================================


//====== Private Function Prototypes ===========================================


//====== Private Functions =====================================================
void L_RefreshRxOK(void)
{
    LCD_SetCursor(L__POS_ROW_RX, L__POS_COL_RX);
    LCD_WriteString("   ");
    LCD_SetCursor(L__POS_ROW_RX, L__POS_COL_RX);
    LCD_WriteCustomChar(L__CUSTOM_CHAR_POS_DCF77_RX_OK);
}

void L_RefreshRxNO(void)
{
    LCD_SetCursor(L__POS_ROW_RX, L__POS_COL_RX);
    LCD_WriteString("   ");
    LCD_SetCursor(L__POS_ROW_RX, L__POS_COL_RX);
    LCD_WriteCustomChar(L__CUSTOM_CHAR_POS_DCF77_RX_NO);
}

void L_RefreshRxNONE(void)
{
    LCD_SetCursor(L__POS_ROW_RX, L__POS_COL_RX);
    LCD_WriteString("   ");
}

void L_RefreshYear(void)
{
    LCD_SetCursor(L__POS_ROW_DATE, L__POS_COL_DATE);
    LCD_WriteInt(XRTC__TIMEDATE_YEAR);
    LCD_WriteChar('-');
}

void L_RefreshMonth(void)
{
    LCD_SetCursor(L__POS_ROW_DATE, L__POS_COL_DATE + 5u);
    if (XRTC__TIMEDATE_MONTH < 10u)
    {
        LCD_WriteChar('0');
    }
    LCD_WriteInt(XRTC__TIMEDATE_MONTH);
    LCD_WriteChar('-');
}

void L_RefreshDay(void)
{
    LCD_SetCursor(L__POS_ROW_DATE, L__POS_COL_DATE + 8u);
    if (XRTC__TIMEDATE_DAY < 10u)
    {
        LCD_WriteChar('0');
    }
    LCD_WriteInt(XRTC__TIMEDATE_DAY);
    
    LCD_SetCursor(L__POS_ROW_DAYNAME, L__POS_COL_DAYNAME);
    
    if ((0u < XRTC__TIMEDATE_DAYNUMBER) && (XRTC__TIMEDATE_DAYNUMBER < 9u))
    {
        LCD_WriteString(XRTC__DAYNAME(XRTC__TIMEDATE_DAYNUMBER));
    }
    else
    {
        LCD_WriteString(XRTC__DAYNAME(0u));
    }
}

void L_RefreshHour(void)
{
    LCD_SetCursor(L__POS_ROW_TIME, L__POS_COL_TIME);
    if (XRTC__TIMEDATE_HOUR < 10u)
    {
        LCD_WriteChar(' ');
    }
    LCD_WriteInt(XRTC__TIMEDATE_HOUR);

    LCD_SetCursor(L__POS_ROW_TIME, L__POS_COL_TIME + 2u);
    LCD_WriteChar(':');
}

void L_RefreshMinute(void)
{
    LCD_SetCursor(L__POS_ROW_TIME, L__POS_COL_TIME + 3u);
    if (XRTC__TIMEDATE_MINUTE < 10u)
    {
        LCD_WriteChar('0');
    }
    LCD_WriteInt(XRTC__TIMEDATE_MINUTE);
}

void L_RefreshSunTime(void)
{
    LCD_SetCursor(L__POS_ROW_SUN_TIME_RISE, L__POS_COL_SUN_TIME_RISE);
    LCD_WriteCustomChar(L__CUSTOM_CHAR_POS_SUNRISE);

    LCD_SetCursor(L__POS_ROW_SUN_TIME_SET, L__POS_COL_SUN_TIME_SET);
    LCD_WriteCustomChar(L__CUSTOM_CHAR_POS_SUNSET);

    LCD_SetCursor(L__POS_ROW_SUN_TIME_RISE, L__POS_COL_SUN_TIME_RISE + 1u);
    if (XSTC__SUN_RISEHOUR < 10u)
    {
        LCD_WriteChar(' ');
    }
    LCD_WriteInt(XSTC__SUN_RISEHOUR);
    LCD_WriteChar(':');
    if (XSTC__SUN_RISEMINUTE < 10u)
    {
        LCD_WriteChar('0');
    }
    LCD_WriteInt(XSTC__SUN_RISEMINUTE);

    LCD_SetCursor(L__POS_ROW_SUN_TIME_SET, L__POS_COL_SUN_TIME_SET + 1u);
    if (XSTC__SUN_SETHOUR < 10u)
    {
        LCD_WriteChar(' ');
    }
    LCD_WriteInt(XSTC__SUN_SETHOUR);
    LCD_WriteChar(':');
    if (XSTC__SUN_SETMINUTE < 10u)
    {
        LCD_WriteChar('0');
    }
    LCD_WriteInt(XSTC__SUN_SETMINUTE);
}

void L_RefreshMoonFraction(void)
{
    switch (XMFC__MOON_DIRECTION)
    {
        case (MFC__MOON_WAXES):
        {
            if (XMFC__MOON_FRACTION <= 10u)
            {
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_L, LCD_CharMoonLeft100);
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_R, LCD_CharMoonRight100);
            }
            else if ((10u < XMFC__MOON_FRACTION) && (XMFC__MOON_FRACTION <= 40u))
            {
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_L, LCD_CharMoonLeft100);
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_R, LCD_CharMoonRight75);
            }
            else if ((40u < XMFC__MOON_FRACTION) && (XMFC__MOON_FRACTION <= 60u))
            {
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_L, LCD_CharMoonLeft100);
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_R, LCD_CharMoonRight0);
            }
            else if ((60u < XMFC__MOON_FRACTION) && (XMFC__MOON_FRACTION <= 90u))
            {
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_L, LCD_CharMoonLeft25);
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_R, LCD_CharMoonRight0);
            }
            else if (90u < XMFC__MOON_FRACTION)
            {
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_L, LCD_CharMoonLeft0);
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_R, LCD_CharMoonRight0);
            }
            else
            {
                // Nothing to do because Moon Fraction is between 0..100
            }
        }
        break;

        case (MFC__MOON_WANES):
        {
            if (XMFC__MOON_FRACTION <= 10u)
            {
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_L, LCD_CharMoonLeft100);
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_R, LCD_CharMoonRight100);
            }
            else if ((10u < XMFC__MOON_FRACTION) && (XMFC__MOON_FRACTION <= 40u))
            {
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_L, LCD_CharMoonLeft75);
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_R, LCD_CharMoonRight100);
            }
            else if ((40u < XMFC__MOON_FRACTION) && (XMFC__MOON_FRACTION <= 60u))
            {
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_L, LCD_CharMoonLeft0);
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_R, LCD_CharMoonRight100);
            }
            else if ((60u < XMFC__MOON_FRACTION) && (XMFC__MOON_FRACTION <= 90u))
            {
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_L, LCD_CharMoonLeft0);
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_R, LCD_CharMoonRight25);
            }
            else if (90u < XMFC__MOON_FRACTION)
            {
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_L, LCD_CharMoonLeft0);
                LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_R, LCD_CharMoonRight0);
            }
            else
            {
                // Nothing to do because Moon Fraction is between 0..100
            }
        }
        break;

        default:
        {
            LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_L, LCD_CharMoonLeft100);
            LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_MOON_R, LCD_CharMoonRight100);
        }
        break;
    } 

    LCD_SetCursor(L__POS_ROW_MOON_ICON, L__POS_COL_MOON_ICON);
    LCD_WriteCustomChar(L__CUSTOM_CHAR_POS_MOON_L);
    LCD_SetCursor(L__POS_ROW_MOON_ICON, L__POS_COL_MOON_ICON + 1u);
    LCD_WriteCustomChar(L__CUSTOM_CHAR_POS_MOON_R);

    LCD_SetCursor(L__POS_ROW_MOON_PERC, L__POS_COL_MOON_PERC);
    if ((10u < XMFC__MOON_FRACTION) && (XMFC__MOON_FRACTION < 100u))
    {
        LCD_WriteChar(' ');
    } 
    else if (XMFC__MOON_FRACTION < 10u)
    {
        LCD_WriteChar(' ');
        LCD_WriteChar(' ');
    }
    else
    {
        ; // No additional character is needed. 
    }
    LCD_WriteInt(XMFC__MOON_FRACTION);
    LCD_WriteChar('%');
}

void L_RefreshDHT22(void)
{
    //**************************************************************************
    //****** TEMPERATURE - DHT22
    //**************************************************************************
    if (Signal_RELIABLE == XDHT22__DATA_QUALIFIER)
    {
        LCD_SetCursor(L__POS_ROW_DHT22_TEMP, L__POS_COL_DHT22_TEMP);
        if (10.0f <= XDHT22__TEMPERATURE_VALUE)
        {
            LCD_WriteChar(' ');
        }
        else if ((0.0f <= XDHT22__TEMPERATURE_VALUE) && (XDHT22__TEMPERATURE_VALUE < 10.0f))
        {
            LCD_WriteChar(' ');
            LCD_WriteChar(' ');
        }
        else if ((-10.0f < XDHT22__TEMPERATURE_VALUE) && (XDHT22__TEMPERATURE_VALUE < 0.0f))
        {
            LCD_WriteChar(' ');
        }
        else
        {
            // Nothing to do
            ;
        }

        LCD_WriteInt((sint8)XDHT22__TEMPERATURE_VALUE);
        LCD_WriteChar('.');
        LCD_WriteInt((uint8)((XDHT22__TEMPERATURE_VALUE - ((sint8)XDHT22__TEMPERATURE_VALUE)) * 10.0f));
        LCD_WriteChar('C');
    }
    else
    {
        LCD_SetCursor(L__POS_ROW_DHT22_TEMP, L__POS_COL_DHT22_TEMP);
        LCD_WriteString("  -- C");
    }


    //**************************************************************************
    //****** HUMIDITY - DHT22
    //**************************************************************************
    if (Signal_RELIABLE == XDHT22__DATA_QUALIFIER)
    {
        LCD_SetCursor(L__POS_ROW_DHT22_HUM, L__POS_COL_DHT22_HUM);
        if (10.0f <= XDHT22__HUMIDITY_VALUE)
        {
            LCD_WriteChar(' ');
        }
        else if ((0.0f <= XDHT22__HUMIDITY_VALUE) && (XDHT22__HUMIDITY_VALUE < 10.0f))
        {
            LCD_WriteChar(' ');
            LCD_WriteChar(' ');
        }
        else if ((-10.0f < XDHT22__HUMIDITY_VALUE) && (XDHT22__HUMIDITY_VALUE < 0.0f))
        {
            LCD_WriteChar(' ');
        }
        else
        {
            // Nothing to do
            ;
        }

        LCD_WriteInt((sint8)XDHT22__HUMIDITY_VALUE);
        LCD_WriteChar('.');
        LCD_WriteInt((uint8)((XDHT22__HUMIDITY_VALUE - ((sint8)XDHT22__HUMIDITY_VALUE)) * 10.0f));
        LCD_WriteChar('%');
    }
    else
    {
        LCD_SetCursor(L__POS_ROW_DHT22_HUM, L__POS_COL_DHT22_HUM);
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

    LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_SUNRISE, LCD_CharSunRise);
    LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_SUNSET, LCD_CharSunSet);
    LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_DCF77_RX_OK, LCD_CharDCF77RxOK);
    LCD_StoreCustomChar(L__CUSTOM_CHAR_POS_DCF77_RX_NO, LCD_CharDCF77RxNO);
}


/*
 * Name: LCM_Refresh
 *
 * Description: This is the main function that is responsible for putting
 *              the information into the LCD display.
 *
 * Input: element name that has to be updated 
 *
 * Output: None
 */
void LCM_Refresh(uint8 _Element)
{
    switch (_Element)
    {
        case LCM__RX_OK:
        {
            L_RefreshRxOK();
        }
        break;

        case LCM__RX_NO:
        {
            L_RefreshRxNO();
        }
        break;

        case LCM__RX_NONE:
        {
            L_RefreshRxNONE();
        }
        break;

        case LCM__DATETIME:
        {
            L_RefreshYear();
            L_RefreshMonth();
            L_RefreshDay();

            L_RefreshHour();
            L_RefreshMinute();
        }
        break;

        case LCM__YEAR:
        {
            L_RefreshYear();
        }
        break;


        case LCM__MONTH:
        {
            L_RefreshMonth();
        }
        break;

        case LCM__DAY:
        {
            L_RefreshDay();
        }
        break;

        case LCM__HOUR:
        {
            L_RefreshHour();
        }
        break;
        
        case LCM__MINUTE:
        {
            L_RefreshMinute();
        }
        break;
        
        case LCM__SUNTIME:
        {
            L_RefreshSunTime();
        }
        break;
        
        case LCM__MOONFRACTION:
        {
            L_RefreshMoonFraction();
        }
        break;
        
        case LCM__DHT22:
        {
            L_RefreshDHT22();
        }
        break;
    } 
}


/*
 * Name: LCM_BackLightControl
 *
 * Description: This function is used for LCD backlight control via PWM.
 *              The brightness can be set statically by direct control or
 *              dinamically by ambient light sensor.
 *
 * Input: method of the control (direct or adaptive control)
 *
 * Output: None
 */
void LCM_BackLightControl(uint8 _ControlMethod, uint8 _PWMDutyCycle)
{
    uint8 _AmbientLight = U__INIT_VALUE_UINT;


    if (LCM__CONTROL_METHOD_ADAPTIVE == _ControlMethod)
    {
        //#define DEMO

#ifndef DEMO
        if (DCF77_Status_SYNCHRONIZED == XDCF77__STATUS)
        {
#endif
            _AmbientLight = MCH_ReadADC(MCH__ADC_CHANNEL_7);

            if                                  (_AmbientLight < 100u)      { _PWMDutyCycle =  40u; }
            else if ((100u <= _AmbientLight) && (_AmbientLight < 150u))     { _PWMDutyCycle =  60u; }
            else if ((150u <= _AmbientLight) && (_AmbientLight < 200u))     { _PWMDutyCycle =  80u; }
            else if  (200u <= _AmbientLight)                                { _PWMDutyCycle = 100u; }
#ifndef DEMO
        }
        else
        {
            _PWMDutyCycle = 0u;
        }
#endif

    }
    else if (LCM__CONTROL_METHOD_STATIC == _ControlMethod)
    {
        /* No additional logic is needed */
    }

    OCR0 = (uint8)(_PWMDutyCycle / 100.0f * 255.0f);

/* DEBUG */
    _AmbientLight = MCH_ReadADC(MCH__ADC_CHANNEL_7);
    LCD_SetCursor(2u,1u);
    LCD_WriteString("          ");
    LCD_SetCursor(2u,1u);
    LCD_WriteInt(_AmbientLight);
    LCD_SetCursor(2u,5u);
    LCD_WriteInt(OCR0);
/* DEBUG */
}
