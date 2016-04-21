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
#define DHT22__CONTROL(ctrl) ( ((ctrl) == U__ENABLE) ? \
                               MCH__GPIO_WRITE(MCH__PORT_DHT22_CTRL, MCH__P_DHT22_CTRL, U__LOW) : \
                               MCH__GPIO_WRITE(MCH__PORT_DHT22_CTRL, MCH__P_DHT22_CTRL, U__HIGH) )


//====== Public Signals ========================================================
typedef struct
{
    float32         TemperatureValue;
    float32         HumidityValue;
    G_Qualifier_e   Qualifier;
    G_Flag_e        Updated;
} DHT22_Data_s;


#define XDHT22__TEMPERATURE_VALUE    ((float32)         DHT22_Data.TemperatureValue)
#define XDHT22__HUMIDITY_VALUE       ((float32)         DHT22_Data.HumidityValue)
#define XDHT22__DATA_QUALIFIER       ((G_Qualifier_e)   DHT22_Data.Qualifier)
#define XDHT22__DATA_UPDATED         ((G_Flag_e)        DHT22_Data.Updated)

extern DHT22_Data_s DHT22_Data;


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
