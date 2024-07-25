#ifndef __SPECTRUM_H
#define __SPECTRUM_H

void Spectrum_Init(void);
void Spectrum_GetValue(void);
uint8_t Spectrum_GetModeInFlag(void);
uint8_t Spectrum_GetModeOutFlag(void);
void Spectrum_Show(void);
void Spectrum_ChangeType(void);

void Spectrum_AutoSwitchChange(void);
void Spectrum_AutoSwitchShow(void);


#endif
