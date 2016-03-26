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
// DCF77 receiver
#define P_DCF77_DATA    (PD6)   // PIN 020
#define P_DCF77_CTRL    (PD5)   // PIN 019
#define DDR_DCF77       (DDRD)
#define DDR_DCF77_CTRL  (DDRD)
#define PIN_DCF77       (PIND)
#define PORT_DCF77_CTRL (PORTD)

// LCD panel
#define P_LCD_RS        (PA0)   // PIN 040
#define P_LCD_RW        (PA1)   // PIN 039
#define P_LCD_EN        (PA2)   // PIN 038
#define P_LCD_D4        (PA3)   // PIN 037
#define P_LCD_D5        (PA4)   // PIN 036
#define P_LCD_D6        (PA5)   // PIN 035
#define P_LCD_D7        (PA6)   // PIN 034
#define P_LCD_CTRL      (PB3)   // PIN 004
#define DDR_LCD         (DDRA)
#define DDR_LCD_CTRL    (DDRB)
#define PIN_LCD         (PINA)
#define PORT_LCD        (PORTA)
#define PORT_LCD_CTRL   (PORTB)

// I2C
#define P_I2C_SCL       (PC0)   // PIN 022
#define P_I2C_SDA       (PC1)   // PIN 023
#define DDR_I2C         (DDRC)
#define PIN_I2C         (PINC)
#define PORT_I2C        (PORTC)

// DHT22
#define P_DHT22_DATA    (PD4)   // PIN 018
#define P_DHT22_CTRL    (PD3)   // PIN 017
#define DDR_DHT22       (DDRD)
#define DDR_DHT22_CTRL  (DDRD)
#define PIN_DHT22       (PIND)
#define PORT_DHT22      (PORTD)
#define PORT_DHT22_CTRL (PORTD)

// Ambient Light Sensor
#define P_ALS           (PA7)   // PIN 033
#define DDR_ALS         (DDRA)
#define PIN_ALS         (PINA)
#define PORT_ALS        (PORTA)


//******************************************************************************
//****** PIN CONTROL
//******************************************************************************
#define GPIO_DIRECTION(ddr, p, dir)     ((dir)   ? BIT_SET((ddr),  (p)) : BIT_CLR((ddr),  (p)))
#define GPIO_WRITE(port, p, value)      ((value) ? BIT_SET((port), (p)) : BIT_CLR((port), (p)))
#define GPIO_READ(pin, p)               (BIT_GET((pin), (p)))

//******************************************************************************
//****** PERIPHERAL CONSTANTS
//******************************************************************************
// ADC
#define MCH_ADC_CHANNEL_0       (0u)
#define MCH_ADC_CHANNEL_1       (1u)
#define MCH_ADC_CHANNEL_2       (2u)
#define MCH_ADC_CHANNEL_3       (3u)
#define MCH_ADC_CHANNEL_4       (4u)
#define MCH_ADC_CHANNEL_5       (5u)
#define MCH_ADC_CHANNEL_6       (6u)
#define MCH_ADC_CHANNEL_7       (7u)

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

    extern void  MCH_Init_Pins(void);
    extern void  MCH_Init_Watchdog(void);
    extern void  MCH_Init_Timer0(void);
    extern void  MCH_Init_Timer1(void);
    extern void  MCH_Init_Timer2(void);
    extern void  MCH_Init_ADC(void);
    extern void  MCH_Init_I2C(uint16 scl_freq_in_khz);
    extern uint8 MCH_I2C_Start(uint8 address, uint8 read_or_write);
    extern uint8 MCH_I2C_Write(uint8 data);
    extern void  MCH_I2C_Read(uint8 *data, uint8 read_status);
    extern void  MCH_I2C_Stop(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* MICROCONTROLLERHAL_H_ */
