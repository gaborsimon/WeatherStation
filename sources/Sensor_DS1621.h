#ifndef SENSORDS1621_H_
#define SENSORDS1621_H_


//====== Header includes =======================================================
#include "typedef.h"
#include "utils.h"

#include "LCDDriver_HD44780.h"
#include "MicroControllerHAL.h"


//====== Public Constants ======================================================
#define TMS__SENSOR_I2C_ADDR    (0x92u)


//====== Public Signals ========================================================
typedef struct
{
    float32         TemperatureValue;
    float32         TemperatureMinimum;
    float32         TemperatureMaximum;
    G_Qualifier_e   Qualifier;
} DS1621_Data_s;


#define XDS1621__TEMPERATURE_VALUE  ((float32)          DS1621_Data.TemperatureValue)
#define XDS1621__TEMPERATURE_MIN    ((float32)          DS1621_Data.TemperatureMinimum)
#define XDS1621__TEMPERATURE_MAX    ((float32)          DS1621_Data.TemperatureMaximum)
#define XDS1621__DATA_QUALIFIER     ((G_Qualifier_e)    DS1621_Data.Qualifier)

extern DS1621_Data_s DS1621_Data;


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void DS1621_Init(void);
    extern void DS1621_Refresh(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* SENSORDS1621_H_ */
