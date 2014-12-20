//v1.1

//====== Header includes =======================================================
#include "MicroControllerHAL.h"


//====== Private Constants =====================================================
#define L_I2C_TWSR_STATUS_MASK      (0xF8u)

#define L_I2C_STATUS_START          (0x08u)
#define L_I2C_STATUS_REP_START      (0x10u)
#define L_I2C_STATUS_W_ADDR_ACK     (0x18u)
#define L_I2C_STATUS_W_ADDR_NOTACK  (0x20u)
#define L_I2C_STATUS_W_DATA_ACK     (0x28u)
#define L_I2C_STATUS_W_DATA_NOTACK  (0x30u)
#define L_I2C_STATUS_ARBIT_LOST     (0x38u)
#define L_I2C_STATUS_R_ADDR_ACK     (0x40u)
#define L_I2C_STATUS_R_ADDR_NOTACK  (0x48u)
#define L_I2C_STATUS_R_DATA_ACK     (0x50u)
#define L_I2C_STATUS_R_DATA_NOTACK  (0x58u)

#define L_I2C_RETRY_MAX_NUMBER      (1000u)


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
void MCH_Init_Watchdog(void)
{
    WD_RESET();

    // Watchdog Change enabled
    BIT_SET(WDTCSR, WDCE);
    // Watchdog System Reset enabled
    BIT_SET(WDTCSR, WDE);
    // Watchdog Timeout: 0110 = 128k (131072 cycle) = 1sec
    BIT_SET(WDTCSR, WDE);
    BIT_CLR(WDTCSR, WDP3);
    BIT_SET(WDTCSR, WDP2);
    BIT_SET(WDTCSR, WDP1);
    BIT_CLR(WDTCSR, WDP0);
}


/*
 * Name: MCH_Init_Timer1CHA
 *
 * Description: This function initializes the Timer1 channel "A" periphery for
 *              the main scheduler.
 *
 * Input: None
 *
 * Output: None
 */
void MCH_Init_Timer1CHA(void)
{
    // Setting TIMER1 "A" channel to 1Hz (1sec) frequency
    // Normal port operation OCnX are disconnected
    TCCR1A = 0x00u;
    TCCR1B = 0x00u;
    TCCR1C = 0x00u;
    // Prescaler = 1024
    BIT_SET(TCCR1B, CS12);
    BIT_SET(TCCR1B, CS10);
    // Turn on CTC mode
    BIT_SET(TCCR1B, WGM12);
    // 1Hz = 16MHz / [1024 x (1 + N)] => N = 15624
    OCR1A = 15624u;
    // Enable the Timer compare interrupt
    BIT_SET(TIMSK1, OCIE1A);
}


/*
 * Name: MCH_Init_Pins
 *
 * Description: This function initializes uC pins by setting them to digital or
 *              analogue / input or output.
 *
 * Input: None
 *
 * Output: None
 */
void MCH_Init_Pins(void)
{
    GPIO_PANEL_LED_OUTPUT;

    GPIO_LCD_RS_OUTPUT;
    GPIO_LCD_RW_OUTPUT;
    GPIO_LCD_EN_OUTPUT;
    GPIO_LCD_D4_OUTPUT;
    GPIO_LCD_D5_OUTPUT;
    GPIO_LCD_D6_OUTPUT;
    GPIO_LCD_D7_OUTPUT;
}


/*
 * Name: MCH_Init_I2C
 *
 * Description: This function initializes the I2C peripheral of the MCU.
 *
 * Input: clock frequency of the I2C in kHz dimension
 *
 * Output: None
 */
void MCH_Init_I2C(uint16 scl_freq_in_khz)
{
    // Enable internal pull-up resistors
    BIT_CLR(DDRD,PIN_I2C_SCL);
    BIT_SET(PORTD,PIN_I2C_SCL);
    BIT_CLR(DDRD,PIN_I2C_SDA);
    BIT_SET(PORTD,PIN_I2C_SDA);

    // Setting the Bit Rate Generator Unit
    // SCL [Hz] = CPU [Hz] / (16 + 2x(TWBR)x4^(TWPS))
    // 100 kHz => BitRate = 18 / Prescaler = 1
    TWBR = ((((uint8)(F_CPU / 1000L / scl_freq_in_khz)) - 16u) / 2u);
    BIT_CLR(TWSR, TWPS0);
    BIT_CLR(TWSR, TWPS1);

    // Switch On the interface
    BIT_SET(TWCR, TWEA);
    BIT_SET(TWCR, TWEN);
}


/*
 * Name: MCH_I2C_Start
 *
 * Description: This function starts a read or write I2C package.
 *
 * Input: address of the I2C peripheral
 *        type of package (read or write)
 *
 * Output: status of the I2C action (ERROR or NO_ERROR)
 */
uint8 MCH_I2C_Start(uint8 address, uint8 read_or_write)
{
    uint8 _status               = INIT_VALUE_UINT;
    uint8 _res                  = MCH_I2C_ERROR;
    static uint16 _loop_counter = INIT_VALUE_UINT;

    
    address |= read_or_write;

    // Repeat start 1000 times if it is needed
    for (_loop_counter = 0u; _loop_counter < L_I2C_RETRY_MAX_NUMBER; _loop_counter++)
    {
        // Send START condition
        BIT_SET(TWCR, TWSTA);
        BIT_SET(TWCR, TWINT);
        BIT_SET(TWCR, TWEA);
        BIT_SET(TWCR, TWEN);
        
        loop_until_bit_is_set(TWCR,TWINT);

        _status = TWSR & L_I2C_TWSR_STATUS_MASK;
        
        if ((L_I2C_STATUS_START     == _status) ||
            (L_I2C_STATUS_REP_START == _status))
        {
            TWDR = address;
            
            BIT_CLR(TWCR, TWSTA);
            BIT_SET(TWCR, TWINT);
            BIT_SET(TWCR, TWEA);
            BIT_SET(TWCR, TWEN);
            
            loop_until_bit_is_set(TWCR,TWINT);
            
            _status = TWSR & L_I2C_TWSR_STATUS_MASK;
            
            if ( ((MCH_I2C_START_WRITE      == read_or_write) &&
                  (L_I2C_STATUS_W_ADDR_ACK  == _status)) ||
                 ((MCH_I2C_START_READ       == read_or_write) &&
                  (L_I2C_STATUS_R_ADDR_ACK  == _status)) )
            {
                return MCH_I2C_NO_ERROR;
            }
            else
            {
                _res = MCH_I2C_ERROR;
            }

            _loop_counter = 2000u;
        }
    }

    _res = MCH_I2C_ERROR;

    return _res;
}


/*
 * Name: MCH_I2C_Write
 *
 * Description: This function writes a byte to the I2C peripheral.
 *
 * Input: 8-bit data
 *
 * Output: status of the I2C action (ERROR or NO_ERROR)
 */
uint8 MCH_I2C_Write(uint8 data)
{
    uint8 _status   = INIT_VALUE_UINT;
    uint8 _res      = MCH_I2C_ERROR;

    
    TWDR = data;
    
    BIT_SET(TWCR, TWINT);
    BIT_SET(TWCR, TWEA);
    BIT_SET(TWCR, TWEN);
    
    loop_until_bit_is_set(TWCR, TWINT);

    _status = TWSR & L_I2C_TWSR_STATUS_MASK;
    
    if (L_I2C_STATUS_W_DATA_ACK == _status)
    {
        _res = MCH_I2C_NO_ERROR;
    }
    else
    {
        _res = MCH_I2C_ERROR;
    }

    return _res;
}


/*
 * Name: MCH_I2C_Read
 *
 * Description: This function reads a data byte from the I2C peripheral.
 *
 * Input: pointer to the 8-bit data
 *        status of the read command (PENDING or STOP)
 *
 * Output: status of the I2C action (ERROR or NO_ERROR)
 */
void MCH_I2C_Read(uint8 *data, uint8 read_status)
{
    switch (read_status)
    {
        case MCH_I2C_READ_PENDING:
        {
            BIT_SET(TWCR, TWEA);
        }
        break;

        case MCH_I2C_READ_STOP:
        {
            BIT_CLR(TWCR, TWEA);
        }
        break;
        
        default:
        {
            /* Nothing to do */
        }
        break;
    }
    BIT_SET(TWCR, TWINT);
    BIT_SET(TWCR, TWEN);

    loop_until_bit_is_set(TWCR, TWINT);

    *data = ((sint8) TWDR);
}


/*
 * Name: MCH_I2C_Stop
 *
 * Description: This function stops the communication with I2C peripheral.
 *
 * Input: None
 *
 * Output: None
 */
void MCH_I2C_Stop(void)
{
    BIT_SET(TWCR, TWSTO);
    BIT_SET(TWCR, TWEA);
    BIT_SET(TWCR, TWINT);
    BIT_SET(TWCR, TWEN);

    loop_until_bit_is_clear(TWCR, TWSTO);
}
