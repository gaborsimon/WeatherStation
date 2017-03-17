#ifndef SENSORGPS_H_
#define SENSORGPS_H_


//====== Header includes =======================================================
#include "typedef.h"
#include "utils.h"

#include "LCDCharacters_HD44780.h"
#include "LCDManager.h"
#include "MicroControllerHAL.h"
#include "RealTimeClock.h"
#include "SunTimeCalculator.h"


//====== Public Constants ======================================================
#define GPS__CONTROL(ctrl) ( ((ctrl) == U__ENABLE) ? \
                             MCH__GPIO_WRITE(MCH__PORT_GPS_CTRL, MCH__P_GPS_CTRL, U__LOW) : \
                             MCH__GPIO_WRITE(MCH__PORT_GPS_CTRL, MCH__P_GPS_CTRL, U__HIGH) )


//====== Public Signals ========================================================
typedef enum
{
    GPS_Status_INIT           = 1u,
    GPS_Status_SYNCH_ONGOING,
    GPS_Status_SYNCH_DONE,
    GPS_Status_SYNCHRONIZED
} GPS_Status_e;

#define XGPS__STATUS    ((GPS_Status_e) GPS_Status)

extern GPS_Status_e GPS_Status;


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void GPS_Callback_USART_RXC(void);
    extern void GPS_Refresh(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* SENSORGPS_H_ */
