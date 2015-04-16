//====== Header includes =======================================================
#include "LCDDriver_HD44780.h"


//====== Private Constants =====================================================
// LCD size (e.g.: 4x20)
#define L_LCD_DISPLAY_ROW ( 4u)
#define L_LCD_DISPLAY_COL (20u)

// Commands
#define L_LCD_CMD_CLEAR_DISPLAY     (0x01u)
#define L_LCD_CMD_RETURN_HOME       (0x02u)
#define L_LCD_CMD_ENTRY_MODE_SET    (0x04u)
#define L_LCD_CMD_DISPLAY_CONTROL   (0x08u)
#define L_LCD_CMD_CURSOR_SHIFT      (0x10u)
#define L_LCD_CMD_FUNCTION_SET      (0x20u)
#define L_LCD_CMD_SET_CGRAM         (0x40u)
#define L_LCD_CMD_SET_DDRAM         (0x80u)

// Entry mode set configurations
#define L_LCD_ENTRY_MODE_INCREMENT  (0x02u)
#define L_LCD_ENTRY_MODE_DECREMENT  (0x00u)

// Display control configurations
#define L_LCD_DISPLAY_ON            (0x04u)
#define L_LCD_DISPLAY_OFF           (0x00u)
#define L_LCD_CURSOR_ON             (0x02u)
#define L_LCD_CURSOR_OFF            (0x00u)
#define L_LCD_CURSOR_BLINK_ON       (0x01u)
#define L_LCD_CURSOR_BLINK_OFF      (0x00u)

// Cursor shift control configurations
#define L_LCD_DISPLAY_SHIFT         (0x08u)
#define L_LCD_CURSOR_MOVE           (0x00u)
#define L_LCD_SHIFT_TO_RIGHT        (0x04u)
#define L_LCD_SHIFT_TO_LEFT         (0x00u)

// Function set configuration
#define L_LCD_FUNCTION_8BIT_MODE    (0x10u)
#define L_LCD_FUNCTION_4BIT_MODE    (0x00u)
#define L_LCD_FUNCTION_2LINE        (0x08u)
#define L_LCD_FUNCTION_1LINE        (0x00u)
#define L_LCD_FUNCTION_5x10_DOTS    (0x04u)
#define L_LCD_FUNCTION_5x8_DOTS     (0x00u)

#define L_LCD_RS_COMMAND (0u)
#define L_LCD_RS_DATA    (1u)
#define L_LCD_RW_WRITE   (0u)
#define L_LCD_RW_READ    (1u)

#define L_COMMAND        (0u)
#define L_DATA           (1u)


//====== Private Signals =======================================================


//====== Private Function Prototypes ===========================================
static void EnableTransfer(void);
static void Send(uint8 type, uint8 package);


//====== Private Functions =====================================================
/*
 * Name: EnableTransfer
 *
 * Description: This function sends an enable pulse to the LCD controller in
 *              order to confirm a data/command transfer.
 *              Execution time: ~100us
 *
 * Input: None
 *
 * Output: None
 */
static void EnableTransfer(void)
{
    // Enable the sending
    GPIO_WRITE(PORT_LCD, P_LCD_EN, LOW);
    _delay_us(1u);

    // EN pulse shall be greater than 450ns
    GPIO_WRITE(PORT_LCD, P_LCD_EN, HIGH);
    _delay_us(1u);

    GPIO_WRITE(PORT_LCD, P_LCD_EN, LOW);

    // Commands need more than 37us to settle
    _delay_us(100u);
}


/*
 * Name: Send
 *
 * Description: This function sends the given command or data to the LCD
 *              controller in 4-bit mode.
 *              Execution time: ~200us
 *
 * Input: type - command or data
          package - sent command or data
 *
 * Output: None
 */
static void Send(uint8 type, uint8 package)
{
    
    // Command/Data & Write
    switch (type)
    {
        case L_COMMAND:
        {
            GPIO_WRITE(PORT_LCD, P_LCD_RS, L_LCD_RS_COMMAND);
        }
        break;
        
        case L_DATA:
        {
            GPIO_WRITE(PORT_LCD, P_LCD_RS, L_LCD_RS_DATA);
        }
        break;
    }    
    GPIO_WRITE(PORT_LCD, P_LCD_RW, L_LCD_RW_WRITE);

    // Just to be sure, set data pins to output
    GPIO_DIRECTION(DDR_LCD, P_LCD_D4, OUTPUT);
    GPIO_DIRECTION(DDR_LCD, P_LCD_D5, OUTPUT);
    GPIO_DIRECTION(DDR_LCD, P_LCD_D6, OUTPUT);
    GPIO_DIRECTION(DDR_LCD, P_LCD_D7, OUTPUT);

    // Send the MSB 4 bits
    GPIO_WRITE(PORT_LCD, P_LCD_D4, ((package >> 4u) & 0x01u));
    GPIO_WRITE(PORT_LCD, P_LCD_D5, ((package >> 5u) & 0x01u));
    GPIO_WRITE(PORT_LCD, P_LCD_D6, ((package >> 6u) & 0x01u));
    GPIO_WRITE(PORT_LCD, P_LCD_D7, ((package >> 7u) & 0x01u));

    EnableTransfer();

    // Send the LSB 4 bits
    GPIO_WRITE(PORT_LCD, P_LCD_D4, ((package >> 0u) & 0x01u));
    GPIO_WRITE(PORT_LCD, P_LCD_D5, ((package >> 1u) & 0x01u));
    GPIO_WRITE(PORT_LCD, P_LCD_D6, ((package >> 2u) & 0x01u));
    GPIO_WRITE(PORT_LCD, P_LCD_D7, ((package >> 3u) & 0x01u));

    EnableTransfer();
}


//====== Public Signals ========================================================


//====== Public Functions ======================================================
/*
 * Name: LCD_Init
 *
 * Description: This function initializes the LCD controller (panel) according
 *              to the datasheet/manual of HD44780 controller.
 *              Execution time: ~65ms
 *
 * Input: None
 *
 * Output: None
 */
void LCD_Init(void)
{
    //****** STEP 1. **********************************************************/
    // Wait more than 15ms after Vdd rises to 4.5V
    _delay_ms(50u);

    GPIO_WRITE(PORT_LCD, P_LCD_RS, LOW);
    GPIO_WRITE(PORT_LCD, P_LCD_RW, LOW);
    GPIO_WRITE(PORT_LCD, P_LCD_EN, LOW);

    //****** STEP 2. **********************************************************/
    // 4-bit mode initialization according to datasheet p46
    Send(L_COMMAND, 0x03u);
    _delay_ms(5u);
    Send(L_COMMAND, 0x03u);
    _delay_ms(5u);
    Send(L_COMMAND, 0x03u);
    _delay_us(500u);
    Send(L_COMMAND, 0x02u);

    //****** STEP 3. **********************************************************/
    // Function set: 0 0 1 DL N F 0 0
    //                      | | |------ = 0 : 5x8 dots character font
    //                      | |-------- = 1 : 2-line mode
    //                      |---------- = 0 : 4-bit mode
    Send(L_COMMAND,
         L_LCD_CMD_FUNCTION_SET   |
         L_LCD_FUNCTION_4BIT_MODE |
         L_LCD_FUNCTION_2LINE     |
         L_LCD_FUNCTION_5x8_DOTS);

    //****** STEP 4. **********************************************************/
    // Display control: 0 0 0 0 1 D C B
    //                            | | |-- = 0 : Cursor Blink OFF
    //                            | |---- = 0 : Cursor OFF
    //                            |------ = 0 : Display OFF
    Send(L_COMMAND,
         L_LCD_CMD_DISPLAY_CONTROL |
         L_LCD_DISPLAY_OFF         |
         L_LCD_CURSOR_OFF          |
         L_LCD_CURSOR_BLINK_OFF);

    //****** STEP 5. **********************************************************/
    // Display clear
    LCD_Clear();

    //****** STEP 6. **********************************************************/
    // Entry mode set: 0 0 0 0 0 0 1 I/D S
    //                                |  |-- = 0 : Cursor moves not the display
    //                                |----- = 1 : Increment cursor move
    Send(L_COMMAND,
         L_LCD_CMD_ENTRY_MODE_SET   |
         L_LCD_ENTRY_MODE_INCREMENT |
         L_LCD_CURSOR_MOVE);
}


/*
 * Name: LCD_SwitchOn
 *
 * Description: This function turns on the LCD display. Remained data - if there
 *              is any - will be shown again.
 *
 * Input: None
 *
 * Output: None
 */
void LCD_SwitchOn(void)
{
    Send(L_COMMAND, L_LCD_CMD_DISPLAY_CONTROL | L_LCD_DISPLAY_ON);
}


/*
 * Name: LCD_SwitchOff
 *
 * Description: This function turns off the LCD display. Data remains after it
 *              will be turned on.
 *
 * Input: None
 *
 * Output: None
 */
void LCD_SwitchOff(void)
{
    Send(L_COMMAND, L_LCD_CMD_DISPLAY_CONTROL | L_LCD_DISPLAY_OFF);
}


/*
 * Name: LCD_Clear
 *
 * Description: This function clears the LCD display and sets the cursor
 *              to home position.
 *
 * Input: None
 *
 * Output: None
 */
void LCD_Clear(void)
{
    Send(L_COMMAND, L_LCD_CMD_CLEAR_DISPLAY);
    _delay_ms(2u);
}


/*
 * Name: LCD_SetCursor
 *
 * Description: This function sets the cursor to the given position.
 *              If the given position is out of the configured size than they
 *              will be set to the maximum (saturation).
 *              Function support currently only 4x20 display.
 *
 * Input: Row    - counted from 1
 *        Column - counted from 1
 *
 * Output: None
 */
void LCD_SetCursor(uint8 row, uint8 column)
{
    uint8 _position_address = INIT_VALUE_UINT;

    
    // Saturation of row and column
    if (row    > L_LCD_DISPLAY_ROW) { row    = L_LCD_DISPLAY_ROW; }
    if (column > L_LCD_DISPLAY_COL) { column = L_LCD_DISPLAY_COL; }

    // Setting the row DDRAM address
    switch (row)
    {
      case 1u: { row = 0x00u; } break;
      case 2u: { row = 0x40u; } break;
      case 3u: { row = 0x14u; } break;
      case 4u: { row = 0x54u; } break;
    }

    // Setting the column DDRAM address
    column--;

    // Calculation of the DDRAM address
    _position_address = row + column;

    // Sending the DDRAM address to the LCD controller
    Send(L_COMMAND, L_LCD_CMD_SET_DDRAM | _position_address);
}


/*
 * Name: LCD_WriteChar
 *
 * Description: This function writes only one ASCII character to the display.
 *
 * Input: ASCII Character
 *
 * Output: None
 */
void LCD_WriteChar(uint8 character)
{
    Send(L_DATA, character);
}


/*
 * Name: LCD_WriteString
 *
 * Description: This function writes an ASCII string to the display.
 *
 * Input: ASCII String
 *
 * Output: None
 */
void LCD_WriteString(const char *poi_string)
{
    while(*poi_string)
    {
        LCD_WriteChar(*poi_string);
        poi_string++;
    }
}


void LCD_WriteStringM(const char *poi_string)
{
    while(pgm_read_byte(poi_string))
    {
        LCD_WriteChar(pgm_read_byte(poi_string));
        poi_string++;
    }
}


/*
 * Name: LCD_WriteInt
 *
 * Description: This function writes a signed integer number to the display.
 *
 * Input: Number (-32768 .. 32767)
 *
 * Output: None
 */
void LCD_WriteInt(sint16 number)
{
    char _string[L_LCD_DISPLAY_COL];

    
    itoa(number, _string, 10);
    LCD_WriteString(_string);
}


/*
 * Name: LCD_StoreCustomChar
 *
 * Description: This function stores a predefined custom character into the
 *              given CGRAM address of the LCD controller.
 *              CGRAM can be 0..7.
 *
 * Input: Location of character (0..7)
 *        Predefined character map array
 *
 * Output: None
 */
void LCD_StoreCustomChar(uint8 location, const uint8 custom_charmap[])
{
    uint8 _loop_counter = INIT_VALUE_UINT;

    
    location &= 0x07u;

    Send(L_COMMAND, L_LCD_CMD_SET_CGRAM | (location << 3u));

    for (_loop_counter = 0u; _loop_counter < 8u; _loop_counter++)
    {
        Send(L_DATA, custom_charmap[_loop_counter]);
    }
}


/*
 * Name: LCD_WriteCustomChar
 *
 * Description: This function writes a stored custom character to the display
 *              addressed by the CGRAM location (0..7).
 *
 * Input: CGRAM location (0..7)
 *
 * Output: None
 */
void LCD_WriteCustomChar(uint8 location)
{
    Send(L_DATA, location);
}
