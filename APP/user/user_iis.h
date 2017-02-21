#ifndef _USER_IIS_H_
#define _USER_IIS_H_

#include "user_tas5711.h"

#define IIS_OUT_DISEN()         P2DIR |= BIT(2)
#define IIS_OUT_EN()            P2DIR &= ~BIT(2)

extern volatile u8 kick_flag;

void iis_conf(void);
void pll1_set(void);
void pll2_set(void);
void iis_init(void);
void iis_set_vol(u8 volume);
void iis_powerdown(void);

void iis_kick(void);
void pll2_record_set(u8 channel);
void pll2_record_exit(void);

#endif
