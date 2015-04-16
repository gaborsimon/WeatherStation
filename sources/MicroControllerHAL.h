#ifndef MICROCONTROLLERHAL_H_
#define MICROCONTROLLERHAL_H_


//====== Header includes =======================================================
#include <avr/io.h>

#include "typedef.h"
#include "utils.h"


//====== Public Constants ======================================================
//******************************************************************************
//****** PROCESSOR CONFIGURATION
//******************************************************************************
#define F_CPU (16000000UL)

//******************************************************************************
//****** PIN CONFIGURATION
//******************************************************************************
// Built-in LED of Arduino panel
#define P_PANEL_LED     (PB7)   // PIN 026
#define DDR_PANEL_LED   (DDRB)
#define PIN_PANEL_LED   (PINB)
#define PORT_PANEL_LED  (PORTB)

// LCD panel
#define P_LCD_RS        (PK0)   // PIN 089 :: A08
#define P_LCD_RW        (PK1)   // PIN 088 :: A09
#define P_LCD_EN        (PK2)   // PIN 087 :: A10
#define P_LCD_D4        (PK3)   // PIN 086 :: A11
#define P_LCD_D5        (PK4)   // PIN 085 :: A12
#define P_LCD_D6        (PK5)   // PIN 084 :: A13
#define P_LCD_D7        (PK6)   // PIN 083 :: A14
#define DDR_LCD         (DDRK)
#define PIN_LCD         (PINK)
#define PORT_LCD        (PORTK)

// I2C
#define P_I2C_SCL       (PD0)   // PIN 043 :: SCL 21
#define P_I2C_SDA       (PD1)   // PIN 044 :: SDA 20
#define DDR_I2C         (DDRD)
#define PIN_I2C         (PIND)
#define PORT_I2C        (PORTD)

// DHT22
#define P_DHT22_DATA    (PK7)   // PIN 082 :: A15
#define DDR_DHT22       (DDRK)
#define PIN_DHT22       (PINK)
#define PORT_DHT22      (PORTK)

//******************************************************************************
//****** PIN CONTROL
//******************************************************************************
#define GPIO_DIRECTION(ddr, p, dir)     ((dir)   ? BIT_SET(ddr,  p) : BIT_CLR(ddr,  p))
#define GPIO_WRITE(port, p, value)      ((value) ? BIT_SET(port, p) : BIT_CLR(port, p))
#define GPIO_READ(pin, p)               (BIT_GET(pin, p) >> p)

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

    extern void  MCH_Init_Watchdog(void);
    extern void  MCH_Init_Timer1CHA(void);
    extern void  MCH_Init_Pins(void);
    extern void  MCH_Init_I2C(uint16 scl_freq_in_khz);
    extern uint8 MCH_I2C_Start(uint8 address, uint8 read_or_write);
    extern uint8 MCH_I2C_Write(uint8 data);
    extern void  MCH_I2C_Read(uint8 *data, uint8 read_status);
    extern void  MCH_I2C_Stop(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* MICROCONTROLLERHAL_H_ */
