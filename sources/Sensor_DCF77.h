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


//====== Public Signals ========================================================
#define XDCF77_SYNC_DONE ((Flag) DCF77_SyncDone)

extern Flag DCF77_SyncDone;


//====== Public Functions ======================================================
#ifdef __cplusplus
extern "C" {
#endif

    extern void DCF77_Callback_TimerOverflow(void);
    extern void DCF77_Callback_InputCapture(void);

#ifdef __cplusplus
} // extern "C"
#endif


#endif /* SENSORDCF77_H_ */
