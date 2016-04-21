#ifndef SENSORDCF77_H_
#define SENSORDCF77_H_


//====== Header includes =======================================================
#include "typedef.h"
#include "utils.h"

#include "LCDCharacters_HD44780.h"
#include "LCDManager.h"
#include "MicroControllerHAL.h"
#include "RealTimeClock.h"


//====== Public Constants ======================================================
#define DCF77__CONTROL(ctrl) ( ((ctrl) == U__ENABLE) ? \
                               MCH__GPIO_WRITE(MCH__PORT_DCF77_CTRL, MCH__P_DCF77_CTRL, U__LOW) : \
                               MCH__GPIO_WRITE(MCH__PORT_DCF77_CTRL, MCH__P_DCF77_CTRL, U__HIGH) )


//====== Public Signals ========================================================
typedef enum
{
    DCF77_Status_INIT           = 1u,
    DCF77_Status_SYNCH_ONGOING,
    DCF77_Status_SYNCH_DONE,
    DCF77_Status_SYNCHRONIZED
} DCF77_Status_e;

#define XDCF77__STATUS    ((DCF77_Status_e) DCF77_Status)

extern DCF77_Status_e DCF77_Status;

//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void DCF77_Callback_TimerOverflow(void);
    extern void DCF77_Callback_InputCapture(void);
    extern void DCF77_Receiving(uint8 _Control);
    extern void DCF77_Refresh(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* SENSORDCF77_H_ */
