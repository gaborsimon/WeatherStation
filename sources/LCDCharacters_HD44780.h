#ifndef LCDCHARACTERS_H_
#define LCDCHARACTERS_H_


//====== Header includes =======================================================


//====== Public Constants ======================================================
#define L_LCD_CHARACTER_ROW_SIZE (8u)


/*
 *******************************************************************************
 * SUN TIME
 *******************************************************************************
 */
static const uint8 lcd_char_sunrise[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b00100,
    0b01110,
    0b11111,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00000
};

static const uint8 lcd_char_sunset[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b00000,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b11111,
    0b01110,
    0b00100
};


/*
 *******************************************************************************
 * MOON
 *******************************************************************************
 */
static const uint8 lcd_char_moon_left_100[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b00011,
    0b00111,
    0b01111,
    0b01111,
    0b01111,
    0b01111,
    0b00111,
    0b00011
};

static const uint8 lcd_char_moon_left_25[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b00011,
    0b00110,
    0b01100,
    0b01100,
    0b01100,
    0b01100,
    0b00110,
    0b00011
};

static const uint8 lcd_char_moon_left_75[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b00011,
    0b00101,
    0b01011,
    0b01011,
    0b01011,
    0b01011,
    0b00101,
    0b00011
};

static const uint8 lcd_char_moon_left_0[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b00011,
    0b00100,
    0b01000,
    0b01000,
    0b01000,
    0b01000,
    0b00100,
    0b00011
};

static const uint8 lcd_char_moon_right_100[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b11000,
    0b11100,
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11100,
    0b11000
};

static const uint8 lcd_char_moon_right_25[L_LCD_CHARACTER_ROW_SIZE] =
{
   0b11000,
   0b01100,
   0b00110,
   0b00110,
   0b00110,
   0b00110,
   0b01100,
   0b11000
};

static const uint8 lcd_char_moon_right_75[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b11000,
    0b10100,
    0b11010,
    0b11010,
    0b11010,
    0b11010,
    0b10100,
    0b11000
};

static const uint8 lcd_char_moon_right_0[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b11000,
    0b00100,
    0b00010,
    0b00010,
    0b00010,
    0b00010,
    0b00100,
    0b11000
};


/*
 *******************************************************************************
 * DCF77
 *******************************************************************************
 */
static const uint8 lcd_char_dcf77_rx_ok[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b01110,
    0b10001,
    0b00100,
    0b01010,
    0b00000,
    0b00100,
    0b00100,
    0b00100
};

static const uint8 lcd_char_dcf77_rx_no[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00100,
    0b00100,
    0b00100
};


/*
 *******************************************************************************
 * BATTERY
 *******************************************************************************
 */
/*
static const uint8 lcd_char_battery_0[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b01110,
    0b11011,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b11111
};

static const uint8 lcd_char_battery_10[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b01110,
    0b11011,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b11111,
    0b11111
};

static const uint8 lcd_char_battery_30[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b01110,
    0b11011,
    0b10001,
    0b10001,
    0b10001,
    0b11111,
    0b11111,
    0b11111
};

static const uint8 lcd_char_battery_50[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b01110,
    0b11011,
    0b10001,
    0b10001,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};

static const uint8 lcd_char_battery_70[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b01110,
    0b11011,
    0b10001,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};

static const uint8 lcd_char_battery_90[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b01110,
    0b11011,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};

static const uint8 lcd_char_battery_100[L_LCD_CHARACTER_ROW_SIZE] =
{
    0b01110,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};
*/


//====== Public Signals ========================================================


//====== Public Functions ======================================================


#endif /* LCDCHARACTERS_H_ */
