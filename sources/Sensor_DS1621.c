//====== Header includes =======================================================
#include "Sensor_DS1621.h"


//====== Private Constants =====================================================
// Sensor read in every x second
#define L__SENSOR_READ_PERIOD_TIME_SEC (5u)

// Temperature Conversion command
#define L__COMMAND_READ_TEMPERATURE  (0xAAu)
#define L__COMMAND_READ_COUNTER      (0xA8u)
#define L__COMMAND_READ_SLOPE        (0xA9u)
#define L__COMMAND_START_CONVERT_T   (0xEEu)
#define L__COMMAND_STOP_CONVERT_T    (0x22u)
// Thermostat command
#define L__COMMAND_ACCESS_TH         (0xA1u)
#define L__COMMAND_ACCESS_TL         (0xA2u)
#define L__COMMAND_ACCESS_CONFIG     (0xACu)

#define L__CONFIG_CONV_DONE          (7u)
#define L__CONFIG_THF                (6u)
#define L__CONFIG_THL                (5u)
#define L__CONFIG_NVB                (4u)
#define L__CONFIG_POL_HIGH           (0x02u)
#define L__CONFIG_POL_LOW            (0x00u)
#define L__CONFIG_1SHOT_ON           (0x01u)
#define L__CONFIG_1SHOT_OFF          (0x00u)


//====== Private Signals =======================================================
typedef enum
{
    L_State_CUT_OFF             = 1u,
    L_State_INITED              = 2u,
    L_State_IDLE                = 3u,
    L_State_START_CONVERSION    = 4u,
    L_State_READ_TEMP           = 5u
} L_State_e;

static L_State_e L_State = L_State_CUT_OFF;


//====== Private Function Prototypes ===========================================
static void L_CalculateMinMax(void);
static void L_ReadSensor(void);


//====== Private Functions =====================================================
/*
 * Name: L_CalculateMinMax
 *
 * Description: The function calculates the minimum and the maximum values
 *              of the temperature data.
 *
 * Input: None
 *
 * Output: None
 */
static void L_CalculateMinMax(void)
{
    if (DS1621_Data.TemperatureValue < DS1621_Data.TemperatureMinimum)
    {
        DS1621_Data.TemperatureMinimum = DS1621_Data.TemperatureValue;
    }

    if (DS1621_Data.TemperatureValue > DS1621_Data.TemperatureMaximum)
    {
        DS1621_Data.TemperatureMaximum = DS1621_Data.TemperatureValue;
    }
}


/*
 * Name: L_ReadSensor
 *
 * Description: The function reads out the temperature data of DS1621 sensor
 *              via I2C communication interface. The action is performed
 *              through an internal state manchine.
 *
 * Input: None
 *
 * Output: None
 */
static void L_ReadSensor(void)
{
    uint8        _Data1         = U__INIT_VALUE_UINT;
    uint8        _Data2         = U__INIT_VALUE_UINT;
    uint8        _CountRemain   = U__INIT_VALUE_UINT;
    uint8        _CountPerC     = U__INIT_VALUE_UINT;
    uint8        _Res           = MCH__I2C_ERROR;
    float32      _Temp          = U__INIT_VALUE_FLOAT;
    static uint8 L_TickCounter  = U__INIT_VALUE_UINT;

    
    switch (L_State)
    {
        case L_State_INITED:
        {
            L_State = L_State_IDLE;
        }
        break;

        case L_State_IDLE:
        {
            if ((L__SENSOR_READ_PERIOD_TIME_SEC - 2u) == L_TickCounter)
            {
                L_TickCounter = 0u;
                L_State = L_State_START_CONVERSION;
            }
            L_TickCounter++;
        }
        break;

        case L_State_START_CONVERSION:
        {
            _Res = MCH_I2CStart(TMS__SENSOR_I2C_ADDR, MCH__I2C_START_WRITE);
            
            if (MCH__I2C_NO_ERROR == _Res)
            {
                _Res = MCH_I2CWrite(L__COMMAND_START_CONVERT_T);
                
                if (MCH__I2C_NO_ERROR == _Res)
                {
                    MCH_I2CStop();
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
            _Res = MCH_I2CStart(TMS__SENSOR_I2C_ADDR, MCH__I2C_START_WRITE);
            
            if (MCH__I2C_NO_ERROR == _Res)
            {
                _Res = MCH_I2CWrite(L__COMMAND_READ_TEMPERATURE);
                
                if (MCH__I2C_NO_ERROR == _Res)
                {
                    _Res = MCH_I2CStart(TMS__SENSOR_I2C_ADDR, MCH__I2C_START_READ);
                    
                    if (MCH__I2C_NO_ERROR == _Res)
                    {
                        MCH_I2CRead(&_Data1, MCH__I2C_READ_PENDING);
                        MCH_I2CRead(&_Data2, MCH__I2C_READ_STOP);
                        MCH_I2CStop();

                        _Res = MCH_I2CStart(TMS__SENSOR_I2C_ADDR, MCH__I2C_START_WRITE);
                        
                        if (MCH__I2C_NO_ERROR == _Res)
                        {
                            _Res = MCH_I2CWrite(L__COMMAND_READ_COUNTER);
                            
                            if (MCH__I2C_NO_ERROR == _Res)
                            {
                                _Res = MCH_I2CStart(TMS__SENSOR_I2C_ADDR, MCH__I2C_START_READ);
                                
                                if (MCH__I2C_NO_ERROR == _Res)
                                {
                                    MCH_I2CRead(&_CountRemain, MCH__I2C_READ_STOP);
                                    MCH_I2CStop();
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

                        _Res = MCH_I2CStart(TMS__SENSOR_I2C_ADDR, MCH__I2C_START_WRITE);
                        
                        if (MCH__I2C_NO_ERROR == _Res)
                        {
                            _Res = MCH_I2CWrite(L__COMMAND_READ_SLOPE);
                            
                            if (MCH__I2C_NO_ERROR == _Res)
                            {
                                _Res = MCH_I2CStart(TMS__SENSOR_I2C_ADDR, MCH__I2C_START_READ);
                                
                                if (MCH__I2C_NO_ERROR == _Res)
                                {
                                    MCH_I2CRead(&_CountPerC, MCH__I2C_READ_STOP);
                                    MCH_I2CStop();
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

                        _Temp = ((float32) (_Data1)) - 0.25f + ((((float32) (_CountPerC)) - ((float32) (_CountRemain))) / ((float32)(_CountPerC)));
                        
                        DS1621_Data.TemperatureValue = _Temp;
                        DS1621_Data.Qualifier        = Signal_RELIABLE;

                        L_CalculateMinMax();

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
            DS1621_Data.Qualifier = Signal_NOT_RELIABLE;
            DS1621_Init();
        }
        break;

        default:
        {
            DS1621_Data.Qualifier = Signal_NOT_RELIABLE;
            L_State = L_State_CUT_OFF;
        }
        break;
    }
}


//====== Public Signals ========================================================
DS1621_Data_s DS1621_Data;


//====== Public Functions ======================================================
/*
 * Name: DS1621_Init
 *
 * Description: The function initializes the global variables and the sensor
 *              via I2C communication interface.
 *
 * Input: None
 *
 * Output: None
 */
void DS1621_Init()
{
    uint8 _Res = MCH__I2C_ERROR;

    
    // Temperature signal initialization
    DS1621_Data.Qualifier            = Signal_NOT_RELIABLE;
    DS1621_Data.TemperatureValue     = U__INIT_VALUE_FLOAT;
    DS1621_Data.TemperatureMinimum   =  99.0f;
    DS1621_Data.TemperatureMaximum   = -99.0f;

    L_State = L_State_CUT_OFF;

    // DS1621 sensor initialization
    // Low polarity
    // 1 Shot mode - temperature conversion for trigger
    _Res = MCH_I2CStart(TMS__SENSOR_I2C_ADDR, MCH__I2C_START_WRITE);

    if (MCH__I2C_NO_ERROR == _Res)
    {
        _Res = MCH_I2CWrite(L__COMMAND_ACCESS_CONFIG);

        if (MCH__I2C_NO_ERROR == _Res)
        {
            _Res = MCH_I2CWrite(L__CONFIG_POL_LOW | L__CONFIG_1SHOT_ON);

            if (MCH__I2C_NO_ERROR == _Res)
            {
                MCH_I2CStop();
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


/*
 * Name: DS1621_Refresh
 *
 * Description: The function performs periodically the sensor read action.
 *
 * Input: None
 *
 * Output: None
 */
void DS1621_Refresh(void)
{
    L_ReadSensor();
}
