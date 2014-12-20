#ifndef TEMPERATUREMEASURESENSOR_H_
#define TEMPERATUREMEASURESENSOR_H_


//====== Header includes =======================================================
#include "typedef.h"
#include "utils.h"
#include "MicroControllerHAL.h"
#include "LCDDriver_HD44780.h"


//====== Public Constants ======================================================
#define TMS_SENSOR_ADDR_OUTDOOR (0x92u)


//====== Public Signals ========================================================
typedef struct
{
    sint8     Value;
    Qualifier Qualifier;
    sint8     Minimum;
    sint8     Maximum;
} Temperature;

#define XTMS_TEMPERATURE_OUT_VALUE  ((sint8)     TMS_TemperatureOut.Value)
#define XTMS_TEMPERATURE_OUT_QUAL   ((Qualifier) TMS_TemperatureOut.Qualifier)
#define XTMS_TEMPERATURE_OUT_MIN    ((sint8)     TMS_TemperatureOut.Minimum)
#define XTMS_TEMPERATURE_OUT_MAX    ((sint8)     TMS_TemperatureOut.Maximum)

extern Temperature TMS_TemperatureOut;


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void TMS_Init(void);
    extern void TMS_Refresh(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* TEMPERATUREMEASURESENSOR_H_ */
