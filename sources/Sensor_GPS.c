//TODO: Finalize the component: perform coding guideline, create descriptions
//====== Header includes =======================================================
#include "Sensor_GPS.h"


//====== Private Constants =====================================================
#define L__SYNCH_TIMEOUT_1              ((uint16) 1u * (60u * 60u))    /* 1 hour till synch icon disappers */
#define L__SYNCH_TIMEOUT_2              ((uint16) 6u * (60u * 60u))    /* 6 hours till new synch starts */

#define L__GPS_MESSAGE_BUFFER_LENGTH    ((uint8) 200u)


//====== Private Signals =======================================================
typedef enum
{
    GPS_StateMachine_WAITING_FOR_START_CHAR = 1u,
    GPS_StateMachine_WAITING_FOR_RMC,
    GPS_StateMachine_MESSAGE_BUFFERING,
    GPS_StateMachine_MESSAGE_BUFFERING_FINISHED
} GPS_StateMachine_e;

static GPS_StateMachine_e GPS_StateMachine = GPS_StateMachine_WAITING_FOR_START_CHAR;

typedef struct
{
    uint8   hour;
    uint8   minute;
    uint8   second;
    uint16  year;
    uint8   month;
    uint8   day;
    float32 latitude;
    float32 longitude;
}GPSdata_s;

static GPSdata_s GPSdata_RMC;

static uint8 GPSmessage[L__GPS_MESSAGE_BUFFER_LENGTH];
static uint8 GPSmessagePointer = U__INIT_VALUE_UINT;


//====== Private Function Prototypes ===========================================


//====== Private Functions =====================================================


//====== Public Signals ========================================================
GPS_Status_e GPS_Status = GPS_Status_INIT;


//====== Public Functions ======================================================
void GPS_Callback_USART_RXC(void)
{
    uint8 _GPSchar = U__INIT_VALUE_UINT;


    _GPSchar = UDR;
                
    switch(GPS_StateMachine)
    {
        case GPS_StateMachine_WAITING_FOR_START_CHAR:
        {
            GPSmessagePointer = 0u;
            
            if ('$' == _GPSchar)
            {
                GPSmessage[GPSmessagePointer++] = _GPSchar;
                GPS_StateMachine = GPS_StateMachine_WAITING_FOR_RMC;
            }
        }
        break;

        case GPS_StateMachine_WAITING_FOR_RMC:
        {
            GPSmessage[GPSmessagePointer++] = _GPSchar;
            
            // Waiting for the necessary first message identifiers
            if (GPSmessagePointer == 6u)
            {
                // The desired message is received, go on with the buffering
                if ((GPSmessage[3u] == 'R') && (GPSmessage[4u] == 'M') && (GPSmessage[5u] == 'C'))
                {
                    GPS_StateMachine = GPS_StateMachine_MESSAGE_BUFFERING; 
                }
                // Not the desired message is received, start again the processing
                else
                {
                    GPS_StateMachine = GPS_StateMachine_WAITING_FOR_START_CHAR;
                }
            } 
        }
        break;

        case GPS_StateMachine_MESSAGE_BUFFERING:
        {
            GPSmessage[GPSmessagePointer++] = _GPSchar;
            
            // The desired message is received, go on with the buffering
            if ((GPSmessage[GPSmessagePointer-2u] == 13u) && (GPSmessage[GPSmessagePointer-1u] == 10u))
            {
                GPS_StateMachine = GPS_StateMachine_MESSAGE_BUFFERING_FINISHED; 
            }
        }
        break;

        case GPS_StateMachine_MESSAGE_BUFFERING_FINISHED:
        {
        }
        break;
    }
}


void GPS_Refresh(void)
{
    static uint8  L_SymbolFlipFlag    = U__LOW; 
    static uint16 L_TickCounter       = U__INIT_VALUE_UINT; 
    static uint8  L_BufferCounter     = U__INIT_VALUE_UINT;
    static uint8  L_GPSdata_RMC_index = U__INIT_VALUE_UINT;


//TODO: Remove debug code part in final version
/* DEBUG */
/*
    LCD_SetCursor(2u,15u);
    LCD_WriteInt(GPS_Status);
*/
/* DEBUG */
    switch (GPS_Status)
    {
        case GPS_Status_INIT:
        {
            L_SymbolFlipFlag = U__LOW;
            L_TickCounter = U__INIT_VALUE_UINT;
            
            L_GPSdata_RMC_index = 0u;
            GPS_StateMachine = GPS_StateMachine_WAITING_FOR_START_CHAR;
            GPS__CONTROL(U__ENABLE);
            GPS_Status = GPS_Status_SYNCH_ONGOING;
        }
        break;
        
        case GPS_Status_SYNCH_ONGOING:
        {
            if ('A' == GPSmessage[17u])
            {
                GPS__CONTROL(U__DISABLE);
                GPS_Status = GPS_Status_SYNCH_DONE;
            }
            else
            {
                GPS_StateMachine = GPS_StateMachine_WAITING_FOR_START_CHAR;
            
                if (U__HIGH == L_SymbolFlipFlag)
                {
                    L_SymbolFlipFlag = U__LOW;
                    LCM_Refresh(LCM__RX_OK);
                }
                else
                {
                    L_SymbolFlipFlag = U__HIGH;
                    LCM_Refresh(LCM__RX_NO);
                }
            }
        }
        break;
        
        case GPS_Status_SYNCH_DONE:
        {    
            for (L_BufferCounter = 0u; L_BufferCounter < L__GPS_MESSAGE_BUFFER_LENGTH; L_BufferCounter++)
            {
                if (',' == GPSmessage[L_BufferCounter])
                {
                    L_GPSdata_RMC_index++;   

                    switch (L_GPSdata_RMC_index)
                    {
                        case 1u:
                        {
                            GPSdata_RMC.hour   = (GPSmessage[L_BufferCounter+1u] - 48u) * 10u + (GPSmessage[L_BufferCounter+2u] - 48u);
                            GPSdata_RMC.minute = (GPSmessage[L_BufferCounter+3u] - 48u) * 10u + (GPSmessage[L_BufferCounter+4u] - 48u);
                            GPSdata_RMC.second = (GPSmessage[L_BufferCounter+5u] - 48u) * 10u + (GPSmessage[L_BufferCounter+6u] - 48u);
                        }
                        break;

                        case 3u:
                        {
                            GPSdata_RMC.latitude  = (float32)(((GPSmessage[L_BufferCounter+1u] - 48u) * 10u) + (GPSmessage[L_BufferCounter+2u] - 48u));
                            GPSdata_RMC.latitude += (float32)(((GPSmessage[L_BufferCounter+3u] - 48u) * 10u) + (GPSmessage[L_BufferCounter+4u] - 48u)) / 60.0;
                        }
                        break;

                        case 5u:
                        {
                            GPSdata_RMC.longitude  = (float32)(((GPSmessage[L_BufferCounter+1u] - 48u) * 100u) + ((GPSmessage[L_BufferCounter+2u] - 48u) * 10u) + (GPSmessage[L_BufferCounter+3u] - 48u));
                            GPSdata_RMC.longitude += (float32)(((GPSmessage[L_BufferCounter+4u] - 48u) *  10u) + (GPSmessage[L_BufferCounter+5u] - 48u)) / 60.0;
                        }
                        break;

                        case 9u:
                        {
                            GPSdata_RMC.day   = (GPSmessage[L_BufferCounter+1u] - 48u) * 10u + (GPSmessage[L_BufferCounter+2u] - 48u);
                            GPSdata_RMC.month = (GPSmessage[L_BufferCounter+3u] - 48u) * 10u + (GPSmessage[L_BufferCounter+4u] - 48u);
                            GPSdata_RMC.year  = 2000u + (GPSmessage[L_BufferCounter+5u] - 48u) * 10u + (GPSmessage[L_BufferCounter+6u] - 48u);
                        }
                        break;
                    }
                }
                GPSmessage[L_BufferCounter] = U__INIT_VALUE_UINT;
            }

            RTC_SetDate(GPSdata_RMC.year,GPSdata_RMC.month, GPSdata_RMC.day, GPSdata_RMC.hour, GPSdata_RMC.minute, GPSdata_RMC.second);
            STC_SetCoordinate(GPSdata_RMC.latitude, GPSdata_RMC.longitude);
            
            GPS_Status = GPS_Status_SYNCHRONIZED;
        }
        break;

        case GPS_Status_SYNCHRONIZED:
        {
            L_TickCounter++;

            if (L__SYNCH_TIMEOUT_1 == L_TickCounter)
            {
                LCM_Refresh(LCM__RX_NONE);
            }

            if (L__SYNCH_TIMEOUT_2 == L_TickCounter)
            {
                GPS_Status = GPS_Status_INIT;
            }
        }
        break;
    }
}
