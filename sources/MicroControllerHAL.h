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
#define F_CPU (8000000UL)

//******************************************************************************
//****** PIN CONFIGURATION
//******************************************************************************

// LCD panel
#define P_LCD_RS        (PA0)   // PIN 040
#define P_LCD_RW        (PA1)   // PIN 039
#define P_LCD_EN        (PA2)   // PIN 038
#define P_LCD_D4        (PA3)   // PIN 037
#define P_LCD_D5        (PA4)   // PIN 036
#define P_LCD_D6        (PA5)   // PIN 035
#define P_LCD_D7        (PA6)   // PIN 034
#define DDR_LCD         (DDRA)
#define PIN_LCD         (PINA)
#define PORT_LCD        (PORTA)

// I2C
#define P_I2C_SCL       (PC0)   // PIN 022
#define P_I2C_SDA       (PC1)   // PIN 023
#define DDR_I2C         (DDRC)
#define PIN_I2C         (PINC)
#define PORT_I2C        (PORTC)

// DHT22
#define P_DHT22_DATA    (PA7)   // PIN 033
#define DDR_DHT22       (DDRA)
#define PIN_DHT22       (PINA)
#define PORT_DHT22      (PORTA)

//******************************************************************************
//****** PIN CONTROL
//******************************************************************************
#define GPIO_DIRECTION(ddr, p, dir)     ((dir)   ? BIT_SET(ddr,  p) : BIT_CLR(ddr,  p))
#define GPIO_WRITE(port, p, value)      ((value) ? BIT_SET(port, p) : BIT_CLR(port, p))
#define GPIO_READ(pin, p)               (BIT_GET(pin, p))

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
    extern void  MCH_Init_Timer1(void);
    extern void  MCH_Init_Timer2(void);
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
