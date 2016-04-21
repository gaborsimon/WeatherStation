#ifndef MICROCONTROLLERHAL_H_
#define MICROCONTROLLERHAL_H_


//====== Header includes =======================================================
#include <avr/io.h>

#include "typedef.h"
#include "utils.h"


//====== Public Constants ======================================================
/******************************************************************************/
/****** PROCESSOR CONFIGURATION */
/******************************************************************************/
#define F_CPU (8000000UL)

/******************************************************************************/
/****** PIN CONFIGURATION */
/******************************************************************************/
/* DCF77 receiver */
#define MCH__P_DCF77_DATA       (PD6)   /* PIN 020 */
#define MCH__P_DCF77_CTRL       (PD5)   /* PIN 019 */
#define MCH__DDR_DCF77          (DDRD)
#define MCH__DDR_DCF77_CTRL     (DDRD)
#define MCH__PIN_DCF77          (PIND)
#define MCH__PORT_DCF77_CTRL    (PORTD)

/* LCD panel */
#define MCH__P_LCD_RS           (PA0)   /* PIN 040 */
#define MCH__P_LCD_RW           (PA1)   /* PIN 039 */
#define MCH__P_LCD_EN           (PA2)   /* PIN 038 */
#define MCH__P_LCD_D7           (PA3)   /* PIN 037 */
#define MCH__P_LCD_D6           (PA4)   /* PIN 036 */
#define MCH__P_LCD_D5           (PA5)   /* PIN 035 */
#define MCH__P_LCD_D4           (PA6)   /* PIN 034 */
#define MCH__P_LCD_CTRL         (PB3)   /* PIN 004 */
#define MCH__DDR_LCD            (DDRA)
#define MCH__DDR_LCD_CTRL       (DDRB)
#define MCH__PIN_LCD            (PINA)
#define MCH__PORT_LCD           (PORTA)
#define MCH__PORT_LCD_CTRL      (PORTB)

/* I2C */
#define MCH__P_I2C_SCL          (PC0)   /* PIN 022 */
#define MCH__P_I2C_SDA          (PC1)   /* PIN 023 */
#define MCH__DDR_I2C            (DDRC)
#define MCH__PIN_I2C            (PINC)
#define MCH__PORT_I2C           (PORTC)

/* DHT22 */
#define MCH__P_DHT22_DATA       (PD4)   /* PIN 018 */
#define MCH__P_DHT22_CTRL       (PD3)   /* PIN 017 */
#define MCH__DDR_DHT22          (DDRD)
#define MCH__DDR_DHT22_CTRL     (DDRD)
#define MCH__PIN_DHT22          (PIND)
#define MCH__PORT_DHT22         (PORTD)
#define MCH__PORT_DHT22_CTRL    (PORTD)

/* Ambient Light Sensor */
#define MCH__P_ALS              (PA7)   /* PIN 033 */
#define MCH__DDR_ALS            (DDRA)
#define MCH__PIN_ALS            (PINA)
#define MCH__PORT_ALS           (PORTA)


/******************************************************************************/
/****** PIN CONTROL */
/******************************************************************************/
#define MCH__GPIO_DIRECTION(ddr, p, dir)    ((dir)   ? U__BIT_SET((ddr),  (p)) : U__BIT_CLR((ddr),  (p)))
#define MCH__GPIO_WRITE(port, p, value)     ((value) ? U__BIT_SET((port), (p)) : U__BIT_CLR((port), (p)))
#define MCH__GPIO_READ(pin, p)              (U__BIT_GET((pin), (p)))

/******************************************************************************/
/****** PERIPHERAL CONSTANTS */
/******************************************************************************/
/* ADC */
#define MCH__ADC_CHANNEL_0       (0u)
#define MCH__ADC_CHANNEL_1       (1u)
#define MCH__ADC_CHANNEL_2       (2u)
#define MCH__ADC_CHANNEL_3       (3u)
#define MCH__ADC_CHANNEL_4       (4u)
#define MCH__ADC_CHANNEL_5       (5u)
#define MCH__ADC_CHANNEL_6       (6u)
#define MCH__ADC_CHANNEL_7       (7u)

/* I2C */
#define MCH__I2C_START_READ      (1u)
#define MCH__I2C_START_WRITE     (0u)
#define MCH__I2C_READ_PENDING    (1u)
#define MCH__I2C_READ_STOP       (2u)
#define MCH__I2C_NO_ERROR        (1u)
#define MCH__I2C_ERROR           (2u)


//====== Public Signals ========================================================


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void  MCH_InitPins(void);
    extern void  MCH_InitWatchdog(void);
    extern void  MCH_InitTimer0(void);
    extern void  MCH_InitTimer1(void);
    extern void  MCH_InitTimer2(void);
    extern void  MCH_InitADC(void);
    extern uint8 MCH_ReadADC(uint8 _Channel);
    extern void  MCH_InitI2C(uint16 _SCLFreqKHZ);
    extern uint8 MCH_I2CStart(uint8 _Address, uint8 _ReadWrite);
    extern uint8 MCH_I2CWrite(uint8 _Data);
    extern void  MCH_I2CRead(uint8 *_Data, uint8 _ReadStatus);
    extern void  MCH_I2CStop(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* MICROCONTROLLERHAL_H_ */
