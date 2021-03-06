/*  -*-  mode: c; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4; coding: utf-8  -*-  */
/************************************************************************************
**                                                                                 **
**                               mcHF QRP Transceiver                              **
**                             K Atanassov - M0NKA 2014                            **
**                                                                                 **
**---------------------------------------------------------------------------------**
**                                                                                 **
**  File name:                                                                     **
**  Description:                                                                   **
**  Last Modified:                                                                 **
**  Licence:		CC BY-NC-SA 3.0                                                **
************************************************************************************/

#ifndef __HW_SI570
#define __HW_SI570


typedef struct {
    uint8_t hsdiv;
    uint8_t n1;
    float64_t fdco;
    float64_t rfreq;
} Si570_FreqConfig;

typedef struct OscillatorState
{
    Si570_FreqConfig    cur_config;

    float 				fxtal;

    uint8_t				regs[6];

    float				fout;		// contains startup frequency info of Si570

    unsigned short		si570_address;

    uint8_t				base_reg;

    bool                present; // is a working Si570 present?
} OscillatorState;

extern __IO OscillatorState os;



typedef enum
{
    SI570_OK = 0, // tuning ok
    SI570_TUNE_LIMITED, // tuning to freq close to desired freq, still ok
    SI570_TUNE_IMPOSSIBLE, // did not tune, tune freq unknown
    SI570_I2C_ERROR, // could not talk to Si570, tune freq unknown
    SI570_ERROR_VERIFY, // register do not match, tune freq unknown
    SI570_LARGE_STEP, // did not tune, just checking

} Si570_ResultCodes;

// -------------------------------------------------------------------------------------
// Exports
// ------------------



Si570_ResultCodes 	Si570_SetFrequency(ulong freq, int calib, int temp_factor, uchar test);

uchar   Si570_ResetConfiguration();
void 	Si570_CalculateStartupFrequency();
float   Si570_GetStartupFrequency();
uint8_t Si570_GeTI2CAddress();

uchar   Si570_InitExternalTempSensor();
uchar   Si570_ReadExternalTempSensor(int32_t *temp);

inline bool   Si570_IsPresent() { return os.present == true; }


#endif
