#ifndef MOONFRACTIONCALCULATOR_H_
#define MOONFRACTIONCALCULATOR_H_


//====== Header includes =======================================================
#include <math.h>

#include "typedef.h"
#include "utils.h"

#include "LCDDriver_HD44780.h"
#include "RealTimeClock.h"


//====== Public Constants ======================================================
#define MFC__MOON_WAXES         (1u)
#define MFC__MOON_WANES         (2u)
#define MFC__MOON_FULL_OR_NEW   (3u)


//====== Public Signals ========================================================
typedef struct
{
    uint8 Fraction;
    uint8 Direction;
} MFC_Moon_s;


#define XMFC__MOON_FRACTION     ((uint8) (MFC_Moon.Fraction))
#define XMFC__MOON_DIRECTION    ((uint8) (MFC_Moon.Direction))


extern MFC_Moon_s MFC_Moon;


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void MFC_Refresh(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* MOONFRACTIONCALCULATOR_H_ */
