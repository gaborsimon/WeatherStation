#ifndef LCDDRIVER_H_
#define LCDDRIVER_H_


//====== Header includes =======================================================
#include <stdlib.h>
//-- CPU clock define is needed for "delay.h" functions
#include "MicroControllerHAL.h"
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "typedef.h"
#include "utils.h"


//====== Public Constants ======================================================


//====== Public Signals ========================================================


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void LCD_Init(void);
    extern void LCD_SwitchOn(void);
    extern void LCD_SwitchOff(void);
    extern void LCD_Clear(void);
    extern void LCD_SetCursor(uint8 row, uint8 column);
    extern void LCD_WriteChar(uint8 character);
    extern void LCD_WriteString(const char *poi_string);
    extern void LCD_WriteStringM(const char *poi_string);
    extern void LCD_WriteInt(sint16 number);
    extern void LCD_StoreCustomChar(uint8 location, const uint8 custom_charmap[]);
    extern void LCD_WriteCustomChar(uint8 location);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* LCDDRIVER_H_ */
