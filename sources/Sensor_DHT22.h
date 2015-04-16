#ifndef SENSORDHT22_H_
#define SENSORDHT22_H_


//====== Header includes =======================================================
#include "typedef.h"
#include "utils.h"

#include "LCDDriver_HD44780.h"
#include "MicroControllerHAL.h"


//====== Public Constants ======================================================


//====== Public Signals ========================================================
typedef struct
{
    float32   TemperatureValue;
    float32   HumidityValue;
    Qualifier Qualifier;
} DHT22_data;


#define XDHT22_TEMPERATURE_VALUE    ((float32)   DHT22_Data.TemperatureValue)
#define XDHT22_HUMIDITY_VALUE       ((float32)   DHT22_Data.HumidityValue)
#define XDHT22_DATA_QUALIFIER       ((Qualifier) DHT22_Data.Qualifier)

extern DHT22_data DHT22_Data;


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void DHT22_Init(void);
    extern void DHT22_Refresh(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* SENSORDHT22_H_ */
