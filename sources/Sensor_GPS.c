//TODO: Finalize the component: perform coding guideline, create descriptions
//====== Header includes =======================================================
#include "Sensor_GPS.h"


//====== Private Constants =====================================================
#define L__SYNCH_TIMEOUT_1              ((uint16) 1u * (60u * 60u))    /* 1 hour till synch icon disappers */
#define L__SYNCH_TIMEOUT_2              ((uint16) 6u * (60u * 60u))    /* 6 hours till new synch starts */

#define L__GPS_MESSAGE_BUFFER_LENGTH    ((uint8) 200u)


//====== Private Signals =======================================================
static G_Flag_e L_DecodeDone    = Flag_CLEAR;    

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
    static uint8  L_SymbolFlipFlag  = U__LOW; 
    static uint16 L_TickCounter     = U__INIT_VALUE_UINT; 
    
    static uint8  i = 0u;
    static uint8  GPSdata_RMC_index = 0u;

static uint8 j = 0u;
static uint8 _col = 1u, _row = 1u;

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
            
            GPSdata_RMC_index = 0u;
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
            for (i=0u; i<L__GPS_MESSAGE_BUFFER_LENGTH; i++)
            {
                if (',' == GPSmessage[i])
                {
                    GPSdata_RMC_index++;   

                    switch (GPSdata_RMC_index)
                    {
                        case 1u:
                        {
                            GPSdata_RMC.hour   = (GPSmessage[i+1u] - 48u) * 10u + (GPSmessage[i+2u] - 48u);
                            GPSdata_RMC.minute = (GPSmessage[i+3u] - 48u) * 10u + (GPSmessage[i+4u] - 48u);
                            GPSdata_RMC.second = (GPSmessage[i+5u] - 48u) * 10u + (GPSmessage[i+6u] - 48u);
                        }
                        break;

                        case 3u:
                        {
                            GPSdata_RMC.latitude  = (float32)(((GPSmessage[i+1u] - 48u) * 10u) + (GPSmessage[i+2u] - 48u));
                            GPSdata_RMC.latitude += (float32)(((GPSmessage[i+3u] - 48u) * 10u) + (GPSmessage[i+4u] - 48u)) / 60.0;
                        }
                        break;

                        case 5u:
                        {
                            GPSdata_RMC.longitude  = (float32)(((GPSmessage[i+1u] - 48u) * 100u) + ((GPSmessage[i+2u] - 48u) * 10u) + (GPSmessage[i+3u] - 48u));
                            GPSdata_RMC.longitude += (float32)(((GPSmessage[i+4u] - 48u) *  10u) + (GPSmessage[i+5u] - 48u)) / 60.0;
                        }
                        break;

                        case 9u:
                        {
                            GPSdata_RMC.day   = (GPSmessage[i+1u] - 48u) * 10u + (GPSmessage[i+2u] - 48u);
                            GPSdata_RMC.month = (GPSmessage[i+3u] - 48u) * 10u + (GPSmessage[i+4u] - 48u);
                            GPSdata_RMC.year  = 2000u + (GPSmessage[i+5u] - 48u) * 10u + (GPSmessage[i+6u] - 48u);
                        }
                        break;
                    }
                }
                GPSmessage[i] = U__INIT_VALUE_UINT;
            }
       
// TODO: Move to the RTC module
// Sakamoto's methods
// 0 = Sunday, 6 = Saturday
            static uint8 t[12] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
            uint16 y,m,d,dow, dst;

            y = GPSdata_RMC.year;
            m = GPSdata_RMC.month;
            d = GPSdata_RMC.day;

            y -= m < 3;
            dow = (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
            if (dow == 0u) dow = 7u;

            if ((m  < 3u) || (m > 10u)) dst = 0u;
            if ((m  > 3u) && (m < 10u)) dst = 1u;
            if ((m == 3u) && ((d - dow) >= 25u)) dst = 1u;
            else dst = 0u;
            if ((m == 10u) && ((d - dow) < 25u)) dst = 1u;
            else dst = 0u;

            if (dst == 1u) GPSdata_RMC.hour += 2u;
            else GPSdata_RMC.hour += 1u;

// End of calculation

            RTC_SetDate(GPSdata_RMC.year,GPSdata_RMC.month, GPSdata_RMC.day, dow, dst, GPSdata_RMC.hour, GPSdata_RMC.minute, GPSdata_RMC.second);
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
