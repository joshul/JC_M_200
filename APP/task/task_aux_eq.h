/*****************************************************************************
 * Module    : Task
 * File      : task_aux_eq.h
 * Author    : Hanny
 * Function  : 音源输入任务相关定义
*****************************************************************************/
#ifndef TASK_AUX_EQ_H
#define TASK_AUX_EQ_H

typedef struct {
    u8  mute;            //播放与MUTE状态
    u8  pos;
    u16 aux_input;
} type_task_auxeq;

extern type_task_auxeq t_auxeq;

#define AUX_EQ_SAMPLES         0x100

__near_func void dac_direct_out(u16 adr, u16 samples, u8 ch);
bool auxeq_load_code(u16 addr);
void auxeq_play_vol_music(void);
void aux_eq_continue(void);
void auxeq_mute_control(void);
void auxeq_vol_down(void);
void auxeq_vol_up(void);

#endif
