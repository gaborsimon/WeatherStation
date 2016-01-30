//====== Header includes =======================================================
#include "Sensor_DHT22.h"
#include <stdlib.h>


//====== Private Constants =====================================================
// Sensor read in every x second
#define L_SENSOR_READ_PERIODE_TIME_SEC   (5u)
#define L_DHT22_BIT_TIME_THRESHOLD      (40u)
#define L_DHT22_DATA_BIT_COUNT          (40u)

// Port macros
#define DHT22_OUTPUT    GPIO_DIRECTION(DDR_DHT22, P_DHT22_DATA, OUTPUT)
#define DHT22_INPUT     GPIO_DIRECTION(DDR_DHT22, P_DHT22_DATA, INPUT)
#define DHT22_HIGH      GPIO_WRITE(PORT_DHT22, P_DHT22_DATA, HIGH)    
#define DHT22_LOW       GPIO_WRITE(PORT_DHT22, P_DHT22_DATA, LOW)    
#define DHT22_READ      GPIO_READ(PIN_DHT22, P_DHT22_DATA) 


//====== Private Signals =======================================================
typedef enum L_Error_enum
{
    L_Error_NONE        = 1u,
    L_Error_CUT_OFF     = 2u,
    L_Error_CHECKSUM    = 3u
} L_Error_t;

typedef enum L_State_enum
{
    L_State_IDLE        = 1u,
    L_State_READ_SENSOR = 2u
} L_State_t;

static L_State_t L_State = L_State_IDLE;


//====== Private Function Prototypes ===========================================
static L_Error_t ReadSensor(void);


//====== Private Functions =====================================================
/*
 * Name: ReadSensor
 *
 * Description: The function reads out the temperature and relative humidity
*               data of DHT22 sensor via special 1-wire communication interface.
 *
 * Input: None
 *
 * Output: Error state
 */
static L_Error_t ReadSensor(void)
{
    uint8  _bitTime      = INIT_VALUE_UINT;
    uint8  _loopCounter  = INIT_VALUE_UINT;
    uint8  _calculatedCS = INIT_VALUE_UINT;
    uint16 _dataRH       = INIT_VALUE_UINT;
    uint16 _dataT        = INIT_VALUE_UINT;
    uint8  _dataCS       = INIT_VALUE_UINT;


    //****** STEP 1. **********************************************************/
    // Send activate signal
    DHT22_OUTPUT;
    DHT22_HIGH;
    _delay_ms(1u);
    DHT22_LOW;
    _delay_ms(10u);
    DHT22_HIGH;
    _delay_us(40u);
    DHT22_INPUT;

    //****** STEP 2. **********************************************************/
    // Find the start of the ACK signal
    _bitTime = INIT_VALUE_UINT;
    do
    {
        if (_bitTime > 50u)
        {
            return L_Error_CUT_OFF;
        }
        _bitTime++;
        _delay_us(1u);
    } while(DHT22_READ == HIGH);

    //****** STEP 3. **********************************************************/
    // Response signal: 80us LOW
    _bitTime = INIT_VALUE_UINT;
    do
    {
        if (_bitTime > 150u)
        {
            return L_Error_CUT_OFF;
        }
        _bitTime++;
        _delay_us(1u);
    } while(DHT22_READ == LOW);
    
    // Response signal: 80us HIGH
    _bitTime = INIT_VALUE_UINT;
    do
    {
        if (_bitTime > 150u)
        {
            return L_Error_CUT_OFF;
        }
        _bitTime++;
        _delay_us(1u);
    } while(DHT22_READ == HIGH);
    
    //****** STEP 4. **********************************************************/
    // Read the 40 bit data stream
    for(_loopCounter = INIT_VALUE_UINT; _loopCounter < L_DHT22_DATA_BIT_COUNT; _loopCounter++)
    {
        // Measure the width of the data pulse
        _bitTime = INIT_VALUE_UINT;
        do
        {
            if (_bitTime > 150u)
            {
                return L_Error_CUT_OFF;
            }
            _bitTime++;
            _delay_us(1u);
        } while(DHT22_READ == LOW);

        // Measure the width of the data pulse
        _bitTime = INIT_VALUE_UINT;
        do
        {
            if (_bitTime > 150u)
            {
                return L_Error_CUT_OFF;
            }
            _bitTime++;
            _delay_us(1u);
        } while(DHT22_READ == HIGH);

        // Identify the bit values
        if (_bitTime > L_DHT22_BIT_TIME_THRESHOLD)
        {
            // Relative Humidity
            if (_loopCounter < 16u)
            {
                _dataRH |= (1u << (15u - _loopCounter));
            }
            // Temperature
            else if ((_loopCounter > 15u) && (_loopCounter < 32u))
            {
                _dataT |= (1u << (31u - _loopCounter));
            }
            // Check Sum
            else if ((_loopCounter > 31u) && (_loopCounter < 40u))
            {
                _dataCS |= (1u << (39u - _loopCounter));
            }
        }
    }

    //****** STEP 5. **********************************************************/
    // Release the bus - idle state
    DHT22_OUTPUT;
    DHT22_HIGH;

    //****** STEP 6. **********************************************************/
    // Calculate the Check Sum
    _calculatedCS += ((uint8)((_dataRH & 0xFF00u) >> 8u));    
    _calculatedCS += ((uint8) (_dataRH & 0x00FFu));    
    _calculatedCS += ((uint8)((_dataT  & 0xFF00u) >> 8u));    
    _calculatedCS += ((uint8) (_dataT  & 0x00FFu));    

    //****** STEP 7. **********************************************************/
    // Convert the raw data into physical values
    if(_dataCS == _calculatedCS) 
    {
        DHT22_Data.HumidityValue    = ((float32)(_dataRH / 10.0f));
        DHT22_Data.TemperatureValue = ((float32)((_dataT & 0x7FFFu) / 10.0f));

        if(_dataT & 0x8000)
        {
            DHT22_Data.TemperatureValue *= -1.0f;
        }

        return L_Error_NONE;
    }
    else
    {
        return L_Error_CHECKSUM;
    }
}


//====== Public Signals ========================================================
DHT22_data DHT22_Data;


//====== Public Functions ======================================================
/*
 * Name: DHT22_Init
 *
 * Description: The function initializes the global variables.
 *
 * Input: None
 *
 * Output: None
 */
void DHT22_Init(void)
{
    DHT22_Data.Qualifier        = Signal_NOT_RELIABLE;
    DHT22_Data.TemperatureValue = INIT_VALUE_FLOAT;
    DHT22_Data.HumidityValue    = INIT_VALUE_FLOAT;
}


/*
 * Name: DHT22_Refresh
 *
 * Description:  The function performs periodically the sensor read action
 *               trough an internal state machine.
 *
 * Input: None
 *
 * Output: None
 */
void DHT22_Refresh(void)
{
    uint8        _res = INIT_VALUE_UINT;
    static uint8 tick = INIT_VALUE_UINT;

    
    switch (L_State)
    {
        case L_State_IDLE:
        {
            if ((L_SENSOR_READ_PERIODE_TIME_SEC - 1u) == tick)
            {
                tick = INIT_VALUE_UINT;
                L_State = L_State_READ_SENSOR;
            }
            tick++;
        }
        break;
        
        case L_State_READ_SENSOR:
        {
            _res = ReadSensor();

            if (_res == L_Error_NONE)
            {
                DHT22_Data.Qualifier = Signal_RELIABLE; 
            }
            else
            {
                DHT22_Data.Qualifier = Signal_NOT_RELIABLE; 
            }
            
            L_State = L_State_IDLE;
        }
        break;
        
        default:
        {
            DHT22_Data.Qualifier = Signal_NOT_RELIABLE; 
            L_State = L_State_IDLE;
        }
        break;
    }
}