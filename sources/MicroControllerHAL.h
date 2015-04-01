#ifndef MICROCONTROLLERHAL_H_
#define MICROCONTROLLERHAL_H_


//====== Header includes =======================================================
#include <avr/io.h>

#include "typedef.h"
#include "utils.h"


//====== Public Constants ======================================================
#define F_CPU (16000000UL)

//******************************************************************************
//****** PIN CONFIGURATION
//******************************************************************************
// Built-in LED of Arduino panel
#define PIN_PANEL_LED   (PB7) // PIN 026

// LCD panel
#define PIN_LCD_RS      (PK0) // PIN 089 :: A08
#define PIN_LCD_RW      (PK1) // PIN 088 :: A09
#define PIN_LCD_EN      (PK2) // PIN 087 :: A10
#define PIN_LCD_D4      (PK3) // PIN 086 :: A11
#define PIN_LCD_D5      (PK4) // PIN 085 :: A12
#define PIN_LCD_D6      (PK5) // PIN 084 :: A13
#define PIN_LCD_D7      (PK6) // PIN 083 :: A14

// I2C
#define PIN_I2C_SCL     (PD0) // PIN 043 :: SCL 21
#define PIN_I2C_SDA     (PD1) // PIN 044 :: SDA 20

//******************************************************************************
//****** PIN CONTROL
//******************************************************************************
// Built-in LED of Arduino panel
#define GPIO_PANEL_LED_OUTPUT   (BIT_SET(DDRB,  PIN_PANEL_LED))
#define GPIO_PANEL_LED_ON       (BIT_SET(PORTB, PIN_PANEL_LED))
#define GPIO_PANEL_LED_OFF      (BIT_CLR(PORTB, PIN_PANEL_LED))

// LCD panel
#define GPIO_LCD_RS_OUTPUT      (BIT_SET(DDRK,PIN_LCD_RS))
#define GPIO_LCD_RW_OUTPUT      (BIT_SET(DDRK,PIN_LCD_RW))
#define GPIO_LCD_EN_OUTPUT      (BIT_SET(DDRK,PIN_LCD_EN))
#define GPIO_LCD_D4_OUTPUT      (BIT_SET(DDRK,PIN_LCD_D4))
#define GPIO_LCD_D5_OUTPUT      (BIT_SET(DDRK,PIN_LCD_D5))
#define GPIO_LCD_D6_OUTPUT      (BIT_SET(DDRK,PIN_LCD_D6))
#define GPIO_LCD_D7_OUTPUT      (BIT_SET(DDRK,PIN_LCD_D7))

#define GPIO_PIN_WRITE_LCD(pin, value)  ((value) ? BIT_SET(PORTK, (pin)) : BIT_CLR(PORTK, (pin)))

//******************************************************************************
//****** PERIPHERAL CONSTANTS
//******************************************************************************
// I2C
#define MCH_I2C_START_READ      (1u)
#define MCH_I2C_START_WRITE     (0u)
#define MCH_I2C_READ_PENDING    (1u)
#define MCH_I2C_READ_STOP       (2u)
#define MCH_I2C_NO_ERROR        (1u)
#define MCH_I2C_ERROR           (2u)


//====== Public Signals ========================================================


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void MCH_Init_Watchdog(void);
    extern void MCH_Init_Timer1CHA(void);
    extern void MCH_Init_Pins(void);
    extern void MCH_Init_I2C(uint16 scl_freq_in_khz);
    extern uint8 MCH_I2C_Start(uint8 address, uint8 read_or_write);
    extern uint8 MCH_I2C_Write(uint8 data);
    extern void MCH_I2C_Read(uint8 *data, uint8 read_status);
    extern void MCH_I2C_Stop(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* MICROCONTROLLERHAL_H_ */
