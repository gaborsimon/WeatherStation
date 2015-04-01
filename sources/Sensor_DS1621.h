#ifndef TEMPERATUREMEASURESENSOR_H_
#define TEMPERATUREMEASURESENSOR_H_


//====== Header includes =======================================================
#include "typedef.h"
#include "utils.h"
#include "MicroControllerHAL.h"
#include "LCDDriver_HD44780.h"


//====== Public Constants ======================================================
#define TMS_SENSOR_I2C_ADDR (0x92u)


//====== Public Signals ========================================================
typedef struct
{
    sint8     Value;
    Qualifier Qualifier;
    sint8     Minimum;
    sint8     Maximum;
} Temperature;

#define XTMS_TEMPERATURE_VALUE  ((sint8)     TMS_Temperature.Value)
#define XTMS_TEMPERATURE_QUAL   ((Qualifier) TMS_Temperature.Qualifier)
#define XTMS_TEMPERATURE_MIN    ((sint8)     TMS_Temperature.Minimum)
#define XTMS_TEMPERATURE_MAX    ((sint8)     TMS_Temperature.Maximum)

extern Temperature TMS_Temperature;


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
