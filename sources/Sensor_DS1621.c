//====== Header includes =======================================================
#include "Sensor_DS1621.h"
#include <stdlib.h>

//====== Private Constants =====================================================

// Temperature conversion in every x second
#define L_TEMP_CONV_PERIODE_TIME_SEC (3u)

// Temperature Conversion command
#define L_COMMAND_READ_TEMPERATURE  (0xAAu)
#define L_COMMAND_READ_COUNTER      (0xA8u)
#define L_COMMAND_READ_SLOPE        (0xA9u)
#define L_COMMAND_START_CONVERT_T   (0xEEu)
#define L_COMMAND_STOP_CONVERT_T    (0x22u)
// Thermostat command
#define L_COMMAND_ACCESS_TH         (0xA1u)
#define L_COMMAND_ACCESS_TL         (0xA2u)
#define L_COMMAND_ACCESS_CONFIG     (0xACu)

#define L_CONFIG_CONV_DONE          (7u)
#define L_CONFIG_THF                (6u)
#define L_CONFIG_THL                (5u)
#define L_CONFIG_NVB                (4u)
#define L_CONFIG_POL_HIGH           (0x02u)
#define L_CONFIG_POL_LOW            (0x00u)
#define L_CONFIG_1SHOT_ON           (0x01u)
#define L_CONFIG_1SHOT_OFF          (0x00u)


//====== Private Signals =======================================================
typedef enum L_State_enum
{
    L_State_CUT_OFF             = 1u,
    L_State_INITED              = 2u,
    L_State_IDLE                = 3u,
    L_State_START_CONVERSION    = 4u,
    L_State_READ_TEMP           = 5u
} L_State_t;

static L_State_t L_State = L_State_CUT_OFF;


//====== Private Function Prototypes ===========================================
static void CalculateMinMax(void);
static void ReadTemperature(void);


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
static void CalculateMinMax(void)
{
    if (TMS_Temperature.Value < TMS_Temperature.Minimum)
    {
        TMS_Temperature.Minimum = TMS_Temperature.Value;
    }

    if (TMS_Temperature.Value > TMS_Temperature.Maximum)
    {
        TMS_Temperature.Maximum = TMS_Temperature.Value;
    }
}


/*
 * Name:
 *
 * Description:
 *
 * Input:
 *
 * Output:
 */
static void ReadTemperature(void)
{
    uint8        _data1         = INIT_VALUE_UINT;
    uint8        _data2         = INIT_VALUE_UINT;
    uint8        _CountRemain   = INIT_VALUE_UINT;
    uint8        _CountPerC     = INIT_VALUE_UINT;
    uint8        _res           = MCH_I2C_ERROR;
    float32      _temp          = INIT_VALUE_FLOAT;
    static uint8 tick           = INIT_VALUE_UINT;

    
    switch (L_State)
    {
        case L_State_INITED:
        {
            L_State = L_State_IDLE;
        }
        break;

        case L_State_IDLE:
        {
            if ((L_TEMP_CONV_PERIODE_TIME_SEC - 2u) == tick)
            {
                tick = 0u;
                L_State = L_State_START_CONVERSION;
            }
            tick++;
        }
        break;

        case L_State_START_CONVERSION:
        {
            _res = MCH_I2C_Start(TMS_SENSOR_I2C_ADDR, MCH_I2C_START_WRITE);
            
            if (MCH_I2C_NO_ERROR == _res)
            {
                _res = MCH_I2C_Write(L_COMMAND_START_CONVERT_T);
                
                if (MCH_I2C_NO_ERROR == _res)
                {
                    MCH_I2C_Stop();
                    L_State = L_State_READ_TEMP;
                }
                else
                {
                    L_State = L_State_CUT_OFF;
                }
            }
            else
            {
                L_State = L_State_CUT_OFF;
            }
        }
        break;

        case L_State_READ_TEMP:
        {
            _res = MCH_I2C_Start(TMS_SENSOR_I2C_ADDR, MCH_I2C_START_WRITE);
            
            if (MCH_I2C_NO_ERROR == _res)
            {
                _res = MCH_I2C_Write(L_COMMAND_READ_TEMPERATURE);
                
                if (MCH_I2C_NO_ERROR == _res)
                {
                    _res = MCH_I2C_Start(TMS_SENSOR_I2C_ADDR, MCH_I2C_START_READ);
                    
                    if (MCH_I2C_NO_ERROR == _res)
                    {
                        MCH_I2C_Read(&_data1, MCH_I2C_READ_PENDING);
                        MCH_I2C_Read(&_data2, MCH_I2C_READ_STOP);
                        MCH_I2C_Stop();

                        _res = MCH_I2C_Start(TMS_SENSOR_I2C_ADDR, MCH_I2C_START_WRITE);
                        
                        if (MCH_I2C_NO_ERROR == _res)
                        {
                            _res = MCH_I2C_Write(L_COMMAND_READ_COUNTER);
                            
                            if (MCH_I2C_NO_ERROR == _res)
                            {
                                _res = MCH_I2C_Start(TMS_SENSOR_I2C_ADDR, MCH_I2C_START_READ);
                                
                                if (MCH_I2C_NO_ERROR == _res)
                                {
                                    MCH_I2C_Read(&_CountRemain, MCH_I2C_READ_STOP);
                                    MCH_I2C_Stop();
                                }
                                else
                                {
                                    L_State = L_State_CUT_OFF;
                                }
                            }
                            else
                            {
                                L_State = L_State_CUT_OFF;
                            }
                        }
                        else
                        {
                            L_State = L_State_CUT_OFF;
                        }

                        _res = MCH_I2C_Start(TMS_SENSOR_I2C_ADDR, MCH_I2C_START_WRITE);
                        
                        if (MCH_I2C_NO_ERROR == _res)
                        {
                            _res = MCH_I2C_Write(L_COMMAND_READ_SLOPE);
                            
                            if (MCH_I2C_NO_ERROR == _res)
                            {
                                _res = MCH_I2C_Start(TMS_SENSOR_I2C_ADDR, MCH_I2C_START_READ);
                                
                                if (MCH_I2C_NO_ERROR == _res)
                                {
                                    MCH_I2C_Read(&_CountPerC, MCH_I2C_READ_STOP);
                                    MCH_I2C_Stop();
                                }
                                else
                                {
                                    L_State = L_State_CUT_OFF;
                                }
                            }
                            else
                            {
                                L_State = L_State_CUT_OFF;
                            }
                        }
                        else
                        {
                            L_State = L_State_CUT_OFF;
                        }

                        TMS_Temperature.Value = ((sint8) _data1);
                        TMS_Temperature.Qualifier = Signal_RELIABLE;


                        _temp = ((float32) (_data1)) - 0.25f + ((((float32) (_CountPerC)) - ((float32) (_CountRemain))) / ((float32)(_CountPerC)));
                        LCD_SetCursor(3u, 2u);
                        LCD_WriteInt((sint16) _temp);
                        LCD_WriteChar('.');
                        LCD_WriteInt((sint16) ((_temp - ((sint16)_temp)) * 10.0f));

                        CalculateMinMax();

                        L_State = L_State_IDLE;
                    }
                    else
                    {
                        L_State = L_State_CUT_OFF;
                    }
                }
                else
                {
                    L_State = L_State_CUT_OFF;
                }
            }
            else
            {
                L_State = L_State_CUT_OFF;
            }
        }
        break;

        case L_State_CUT_OFF:
        {
            TMS_Temperature.Qualifier = Signal_NOT_RELIABLE;
            TMS_Init();
        }
        break;

        default:
        {
            TMS_Temperature.Qualifier = Signal_NOT_RELIABLE;
            L_State = L_State_CUT_OFF;
        }
        break;
    }
}


//====== Public Signals ========================================================
Temperature TMS_Temperature;


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
void TMS_Init()
{
    uint8 _res = MCH_I2C_ERROR;

    
    // Temperature signal initialization
    TMS_Temperature.Value     =   0;
    TMS_Temperature.Qualifier = Signal_NOT_RELIABLE;
    TMS_Temperature.Minimum   =  99;
    TMS_Temperature.Maximum   = -99;

    L_State = L_State_CUT_OFF;

    // DS1621 sensor initialization
    // Low polarity
    // 1 Shot mode - temperature conversion for trigger
    _res = MCH_I2C_Start(TMS_SENSOR_I2C_ADDR, MCH_I2C_START_WRITE);

    if (MCH_I2C_NO_ERROR == _res)
    {
        _res = MCH_I2C_Write(L_COMMAND_ACCESS_CONFIG);

        if (MCH_I2C_NO_ERROR == _res)
        {
            _res = MCH_I2C_Write(L_CONFIG_POL_LOW | L_CONFIG_1SHOT_ON);

            if (MCH_I2C_NO_ERROR == _res)
            {
                MCH_I2C_Stop();
                L_State = L_State_INITED;
            }
            else
            {
                L_State = L_State_CUT_OFF;
            }
        }
        else
        {
            L_State = L_State_CUT_OFF;
        }
    }
    else
    {
        L_State = L_State_CUT_OFF;
    }
}

void TMS_Refresh(void)
{
    ReadTemperature();
}
