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


//====== Public Signals ========================================================


//====== Public Functions ======================================================
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
    /* DCF77 */
    GPIO_DIRECTION  (DDR_DCF77,         P_DCF77_DATA,   INPUT);
    GPIO_DIRECTION  (DDR_DCF77_CTRL,    P_DCF77_CTRL,   OUTPUT);
    GPIO_WRITE      (PORT_DCF77_CTRL,   P_DCF77_CTRL,   HIGH);

    /* DHT22 */
    GPIO_DIRECTION  (DDR_DHT22,         P_DHT22_DATA,   OUTPUT);
    GPIO_DIRECTION  (DDR_DHT22_CTRL,    P_DHT22_CTRL,   OUTPUT);
    GPIO_WRITE      (PORT_DHT22,        P_DHT22_DATA,   HIGH);
    GPIO_WRITE      (PORT_DHT22_CTRL,   P_DHT22_CTRL,   HIGH);

    /* LCD DISPLAY */
    GPIO_DIRECTION  (DDR_LCD,           P_LCD_RS,       OUTPUT);
    GPIO_DIRECTION  (DDR_LCD,           P_LCD_RW,       OUTPUT);
    GPIO_DIRECTION  (DDR_LCD,           P_LCD_EN,       OUTPUT);
    GPIO_DIRECTION  (DDR_LCD,           P_LCD_D4,       OUTPUT);
    GPIO_DIRECTION  (DDR_LCD,           P_LCD_D5,       OUTPUT);
    GPIO_DIRECTION  (DDR_LCD,           P_LCD_D6,       OUTPUT);
    GPIO_DIRECTION  (DDR_LCD,           P_LCD_D7,       OUTPUT);
    GPIO_DIRECTION  (DDR_LCD_CTRL,      P_LCD_CTRL,     OUTPUT);
    GPIO_WRITE      (PORT_LCD_CTRL,     P_LCD_CTRL,     LOW);

    /* Ambient Light Sensor */
    GPIO_DIRECTION  (DDR_ALS,           P_ALS,          INPUT);
}


/*
 * Name: MCH_Init_Watchdog
 *
 * Description: This function initializes the Watchdog timer.
 *
 * Input: None
 *
 * Output: None
 */
/*
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
*/


/*
 * Name: MCH_Init_Timer0
 *
 * Description: This function initializes the Timer0 peripheral
 *              for PWM generation of LCD backlight.
 *
 * Input: None
 *
 * Output: None
 */
void MCH_Init_Timer0(void)
{
    // Reset control register
    TCCR0 = 0x00u;
   
    // Waveform Generation Mode: 3 - Fast PWM, update of OCR0 at BOTTOM 
    BIT_SET(TCCR0, WGM01);
    BIT_SET(TCCR0, WGM00);
    
    // Compare Output Mode: Non-inverting - clear OC0 on compare match, Set OC0 at BOTTOM
    BIT_SET(TCCR0, COM01);
    BIT_CLR(TCCR0, COM00);

#define PWM_3

#ifdef PWM_1   
    // Clock Select: CLK / 64 => PWM frequency = 488Hz 
    BIT_CLR(TCCR0, CS02);
    BIT_SET(TCCR0, CS01);
    BIT_SET(TCCR0, CS00);
#endif

#ifdef PWM_2   
    // Clock Select: CLK / 256 => PWM frequency = 122Hz 
    BIT_SET(TCCR0, CS02);
    BIT_CLR(TCCR0, CS01);
    BIT_CLR(TCCR0, CS00);
#endif

#ifdef PWM_3   
    // Clock Select: CLK / 1024 => PWM frequency = 31Hz 
    BIT_SET(TCCR0, CS02);
    BIT_CLR(TCCR0, CS01);
    BIT_SET(TCCR0, CS00);
#endif

    // Reset Timer Register
    TCNT0 = 0u;
    // Reset Output Compare Register
    OCR0 = 0u;
    
    // Output Compare Match Interrupt: Disabled
    BIT_CLR(TIMSK, OCIE0);
    // Timer Overflow Interrupt: Disabled
    BIT_CLR(TIMSK, TOIE0);
}


/*
 * Name: MCH_Init_Timer1
 *
 * Description: This function initializes the Timer1 peripheral
 *              for receiving the signal of DCF77.
 *
 * Input: None
 *
 * Output: None
 */
void MCH_Init_Timer1(void)
{
    // Reset control registers
    TCCR1A = 0x00u;
    TCCR1B = 0x00u;

    // Compare Output Mode for Channel A/B: Normal port operation, OC1A/OC1B disconnected
    BIT_CLR(TCCR1A, COM1A1);
    BIT_CLR(TCCR1A, COM1A0);
    BIT_CLR(TCCR1A, COM1B1);
    BIT_CLR(TCCR1A, COM1B0);

    // Waveform Generation Mode: CTC, update of OCR1x Immediate
    BIT_CLR(TCCR1B, WGM13);
    BIT_SET(TCCR1B, WGM12);
    BIT_CLR(TCCR1A, WGM11);
    BIT_CLR(TCCR1A, WGM10);
    
    // Input Capture Noise Canceler: Enabled
    BIT_SET(TCCR1B, ICNC1);
    
    // Input Capture Edge Select: Falling (negative) edge
    BIT_CLR(TCCR1B, ICES1);
    
    // Clock Select: CLK / 256 = 31.250kHz = 32us
    BIT_SET(TCCR1B, CS12);
    BIT_CLR(TCCR1B, CS11);
    BIT_CLR(TCCR1B, CS10);
    
    // Reset Timer Register
    TCNT1 = 0u;
    // Set Output Compare Register for Channel A to 1500ms to catch DCF77 Minute Mark
    OCR1A = 46875u;
    // Reset Output Compare Register for Channel B
    OCR1B = 0u;

    // Timer Input Capture Interrupt: Disabled
    BIT_CLR(TIMSK, TICIE1);
    // Output Compare A Match Interrupt: Disabled
    BIT_CLR(TIMSK, OCIE1A);
    // Output Compare B Match Interrupt: Disabled
    BIT_CLR(TIMSK, OCIE1B);
    // Timer Overflow Interrupt: Disabled
    BIT_CLR(TIMSK, TOIE1);
}


/*
 * Name: MCH_Init_Timer2
 *
 * Description: This function initializes the Timer2 peripheral for
 *              the main scheduler for counting seconds.
 *              It has its own external watch crystal (32.768kHz) clock source.
 *
 * Input: None
 *
 * Output: None
 */
void MCH_Init_Timer2(void)
{
    // Reset control registers
    TCCR2 = 0x00u;
    ASSR  = 0x00u;

    // Waveform Generation Mode: Normal, update of OCR2 Immediate
    BIT_CLR(TCCR2, WGM21);
    BIT_CLR(TCCR2, WGM20);

    // Compare Output Mode: Normal port operation, OC2 disconnected
    BIT_CLR(TCCR2, COM21);
    BIT_CLR(TCCR2, COM20);

    // Clock Celect: CLK / 128 = 256Hz = 3.90625ms
    BIT_SET(TCCR2, CS22);
    BIT_CLR(TCCR2, CS21);
    BIT_SET(TCCR2, CS20);

    // Reset Timer Register
    TCNT2 = 0u;
    // Reset Output Compare Register
    OCR2 = 0u;

    // Asynchronous Timer2: Clocked from external Crystal Oscillator connected to TOSC1 pin
    BIT_SET(ASSR,AS2);

    // Output Compare Match Interrupt: Disabled
    BIT_CLR(TIMSK, OCIE2);
    // Timer Overflow Interrupt: Enabled
    BIT_SET(TIMSK, TOIE2);
}


/*
 * Name: MCH_Init_ADC
 *
 * Description: This function initializes the ADC peripheral of the MCU.
 *
 * Input:  None
 *
 * Output: None
 */
void MCH_Init_ADC(void)
{
    // Reset control registers
    ADMUX  = 0x00u;
    ADCSRA = 0x00u; 
    
    // Reference Selection: Internal 2.56V Voltage Reference
    BIT_SET(ADMUX, REFS1);
    BIT_SET(ADMUX, REFS0);
    
    // ADC Left Adjust Result: Enabled
    BIT_SET(ADMUX, ADLAR);
    
    // Analog Channel and Gain Selection Bits: Single mode
    BIT_CLR(ADMUX, MUX4);
    BIT_CLR(ADMUX, MUX3);
    BIT_CLR(ADMUX, MUX2);
    BIT_CLR(ADMUX, MUX1);
    BIT_CLR(ADMUX, MUX0);

    // ADC Auto Trigger: Disabled 
    BIT_CLR(ADCSRA, ADATE);
    
    // ADC Interrupt: Disabled
    BIT_CLR(ADCSRA, ADIE);
    
    // ADC Prescaler Select: CLK / 128 = 62.5kHz
    BIT_SET(ADCSRA, ADPS2);
    BIT_SET(ADCSRA, ADPS1);
    BIT_SET(ADCSRA, ADPS0);
   
    // ADC: Enabled
    BIT_SET(ADCSRA, ADEN); 
}


/*
 * Name: MCH_Read_ADC
 *
 * Description: This function performs an AD conversion with the given channel.
 *              The result is stored in 8-bit precision (lower 2 bits of 10-bit ADC are not used).
 *
 * Input:  Desired AD input channel
 *
 * Output: Result of the AD conversion with 8-bit precision
 */
uint8 MCH_Read_ADC(uint8 _channel)
{
    uint8 _ret = INIT_VALUE_UINT;


    // Only the 7 single channels can be selected
    _channel &= 0x07u;

    // Set the desired channel
    ADMUX = (ADMUX & 0xF8u) | _channel;

    // Start AD conversion
    BIT_SET(ADCSRA, ADSC);
    
    // Wait for the conversion to complete
    loop_until_bit_is_clear(ADCSRA, ADSC);

    _ret = ADCH;


    return _ret;
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
void MCH_Init_I2C(uint16 _scl_freq_in_khz)
{
    // Enable internal pull-up resistors
    BIT_CLR(DDR_I2C,  P_I2C_SCL);
    BIT_SET(PORT_I2C, P_I2C_SCL);
    BIT_CLR(DDR_I2C,  P_I2C_SDA);
    BIT_SET(PORT_I2C, P_I2C_SDA);

    // Reset control registers
    TWBR = 0x00u;
    TWCR = 0x00u;
    
    // Bit Rate Register
    // SCL [Hz] = CPU [Hz] / (16 + 2x(TWBR)x4^(TWPS))
    // 100 kHz => BitRate = 18 / Prescaler = 1
    TWBR = (uint8)((((uint8)(F_CPU / 1000UL / _scl_freq_in_khz)) - 16u) / 2u);
    
    // TWI Acknowledge: Enabled
    BIT_SET(TWCR, TWEA);
    // TWI: Enabled
    BIT_SET(TWCR, TWEN);

    // TWI Interrupt: Disabled
    BIT_CLR(TWCR, TWIE);    
   
    // TWI Prescaler: 1 
    BIT_CLR(TWSR, TWPS0);
    BIT_CLR(TWSR, TWPS1);
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
