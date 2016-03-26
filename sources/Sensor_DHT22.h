#ifndef SENSORDHT22_H_
#define SENSORDHT22_H_


//====== Header includes =======================================================
#include <stdlib.h>
//-- CPU clock define is needed for "delay.h" functions
#include "MicroControllerHAL.h"
#include <util/delay.h>

#include "typedef.h"
#include "utils.h"


//====== Public Constants ======================================================
#define DHT22_CONTROL(ctrl) ( ((ctrl) == ENABLE) ? \
                              GPIO_WRITE(PORT_DHT22_CTRL, P_DHT22_CTRL, LOW) : \
                              GPIO_WRITE(PORT_DHT22_CTRL, P_DHT22_CTRL, HIGH) )


//====== Public Signals ========================================================
typedef struct
{
    float32   TemperatureValue;
    float32   HumidityValue;
    Qualifier Qualifier;
    Flag      Updated;
} DHT22_data;


#define XDHT22_TEMPERATURE_VALUE    ((float32)   DHT22_Data.TemperatureValue)
#define XDHT22_HUMIDITY_VALUE       ((float32)   DHT22_Data.HumidityValue)
#define XDHT22_DATA_QUALIFIER       ((Qualifier) DHT22_Data.Qualifier)
#define XDHT22_DATA_UPDATED         ((Flag)      DHT22_Data.Updated)

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
