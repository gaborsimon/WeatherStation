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
#define DCF77_CONTROL(ctrl) ( ((ctrl) == ENABLE) ? \
                              GPIO_WRITE(PORT_DCF77_CTRL, P_DCF77_CTRL, LOW) : \
                              GPIO_WRITE(PORT_DCF77_CTRL, P_DCF77_CTRL, HIGH) )


//====== Public Signals ========================================================
#define XDCF77_SYNC_DONE ((Flag) DCF77_SyncDone)

extern Flag DCF77_SyncDone;
extern uint8 BitPos; 

//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void DCF77_Callback_TimerOverflow(void);
    extern void DCF77_Callback_InputCapture(void);
    extern void DCF77_Receiving(uint8 _control);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* SENSORDCF77_H_ */
