#ifndef MOONFRACTIONCALCULATOR_H_
#define MOONFRACTIONCALCULATOR_H_


//====== Header includes =======================================================
#include <math.h>

#include "typedef.h"
#include "utils.h"

#include "RealTimeClock.h"
#include "LCDDriver_HD44780.h"


//====== Public Constants ======================================================
#define MOON_WAXES       (1u)
#define MOON_WANES       (2u)
#define MOON_FULL_OR_NEW (3u)


//====== Public Signals ========================================================
typedef struct
{
    uint8 Fraction;
    uint8 Direction;
} Moon;


#define XMFC_GET_FRACTION   ((uint8) (MFC_Moon.Fraction))
#define XMFC_GET_DIRECTION  ((uint8) (MFC_Moon.Direction))


extern Moon MFC_Moon;


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void MFC_Refresh(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* MOONFRACTIONCALCULATOR_H_ */
