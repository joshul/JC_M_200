/*****************************************************************************
 * Module    : Display
 * File      : led_seg.c
 * Author    : Hanny
 * Function  : LED数码管显示函数
*****************************************************************************/
#include "include.h"
#include "task.h"
#include "task_music.h"
#include "task_fm.h"
#include "task_clock.h"
#include "task_record.h"
#include "task_update_fw.h"
#include "tm1629.h"  


#if IS_LEDSEG_DISPLAY
#include "led_seg.h"

extern  __no_init volatile u16 temp_1629[8];

__no_init u8 disp_buf[5];
u16 disp_sel_num;
u8 flicker_500ms_cnt;
extern u8 eq_flag;


typedef void(*H)(void);

#pragma constseg="DISP_LEDSEG_TBL"
IAR_CONST u16 tbl_ledseg_num[10] =
{
#if 0
    LEDSEG_0, LEDSEG_1, LEDSEG_2, LEDSEG_3, LEDSEG_4,
    LEDSEG_5, LEDSEG_6, LEDSEG_7, LEDSEG_8, LEDSEG_9,
#endif
    CHAR_0, CHAR_1, CHAR_2, CHAR_3, CHAR_4,
    CHAR_5, CHAR_6, CHAR_7, CHAR_8, CHAR_9,
};

#pragma constseg="DISP_LEDSEG_TBL"
IAR_CONST u8 eq_table[7][3] =
{
    {LEDSEG_N,LEDSEG_O,LEDSEG_R},//NORMAL
    {LEDSEG_P,LEDSEG_O,LEDSEG_P},//POP
    {LEDSEG_R,LEDSEG_O,LEDSEG_C},//ROCK
    {LEDSEG_J,LEDSEG_A,LEDSEG_Z},//JAZZ
    {LEDSEG_C,LEDSEG_L,LEDSEG_A},//CLASSIC
    {LEDSEG_C,LEDSEG_U,LEDSEG_T},//COUNTRY
    {LEDSEG_D,LEDSEG_R,LEDSEG_C},//DRC
};

#pragma constseg="DISP_LEDSEG_TBL"
IAR_CONST u16 play_mode_table[6][3] =
{
    {CHAR_A,CHAR_L,CHAR_L},//全部循環
    {CHAR_O,CHAR_N,CHAR_E},//單曲循環
    {CHAR_F,CHAR_O,CHAR_D},//文件夾循環
    {CHAR_N,CHAR_O,CHAR_R},//顺序播放
    {CHAR_R,CHAR_D,CHAR_O},//隨机
    {CHAR_B,CHAR_R,CHAR_O},//浏览
#if 0
    {LEDSEG_A,LEDSEG_L,LEDSEG_L},//全部循環
    {LEDSEG_O,LEDSEG_N,LEDSEG_E},//單曲循環
    {LEDSEG_F,LEDSEG_O,LEDSEG_D},//文件夾循環
    {LEDSEG_N,LEDSEG_O,LEDSEG_R},//顺序播放
    {LEDSEG_R,LEDSEG_D,LEDSEG_O},//隨机
    {LEDSEG_B,LEDSEG_R,LEDSEG_O},//浏览
#endif    
};

#pragma constseg=default

//显示播放设备
#pragma location="DISP_LEDSEG_SEG"
void ledseg_disp_device(void)
{
#if 0
    if(task_ctl.work_sta == TASK_MUSIC || task_ctl.work_sta == TASK_MIC || task_ctl.work_sta == TASK_VOICE 
       || task_ctl.work_sta == TASK_AUX || task_ctl.work_sta == TASK_FM)
    {
        if(fs_cur_dev() == DEVICE_SDMMC) {
            //disp_buf[4] |= ICON_SD;
        } else if (fs_cur_dev() == DEVICE_UDISK) {
            //disp_buf[4] |= ICON_USB;
        }
    }
#endif    
}

//显示数字
#pragma location="DISP_LEDSEG_SEG"
void ledseg_disp_num(u16 num)
{
    temp_1629[2] = tbl_ledseg_num[num / 1000];
    temp_1629[3] = tbl_ledseg_num[(num % 1000)/100];
    temp_1629[4] = tbl_ledseg_num[(num % 100)/10];
    temp_1629[5] = tbl_ledseg_num[num % 10];    
#if 0
    disp_buf[0] = tbl_ledseg_num[num / 1000];
    disp_buf[1] = tbl_ledseg_num[(num % 1000)/100];
    disp_buf[2] = tbl_ledseg_num[(num % 100)/10];
    disp_buf[3] = tbl_ledseg_num[num % 10];
#endif    
}

//开机显示
#pragma location="DISP_LEDSEG_SEG"
void menu_welcome_ledseg(void)
{
#if 0
    disp_buf[1] = LEDSEG_H;
    disp_buf[2] = LEDSEG_I;
#endif
    temp_1629[0] = CHAR_Q;
    temp_1629[1] = CHAR_S;
    temp_1629[2] = CHAR_O;
    temp_1629[3] = CHAR_N;
    temp_1629[4] = CHAR_I;
    temp_1629[5] = CHAR_C;
    temp_1629[6] = CHAR_MASK;
    temp_1629[7] = CHAR_MASK;
}

//关机显示
#pragma location="DISP_LEDSEG_SEG"
void menu_poweroff_ledseg(void)
{
    temp_1629[1] = CHAR_B;
    temp_1629[2] = CHAR_Y;
    temp_1629[3] = CHAR_E;
}

#pragma location="DISP_LEDSEG_SEG"
void menu_standby_ledseg(void)
{
    //temp_1629[1] = CHAR_B;
    //temp_1629[2] = CHAR_Y;
    //temp_1629[3] = CHAR_E;
    if(sys_ctl.bonoff)temp_1629[7] = CHAR_POWER;
    else              temp_1629[7] = CHAR_MASK;    
}

//显示机型
#pragma location="DISP_LEDSEG_SEG"
void menu_model_ledseg(void)
{
    temp_1629[1] = CHAR_M;
    temp_1629[2] = CHAR__;
    temp_1629[3] = CHAR_3;
    temp_1629[4] = CHAR_2;
    temp_1629[5] = CHAR_8;   
    temp_1629[6] = CHAR_MASK;
    temp_1629[7] = CHAR_MASK;
}
//显示机型
#pragma location="DISP_LEDSEG_SEG"
void menu_dim_ledseg(void)
{    
    temp_1629[0] = CHAR_D;
    temp_1629[1] = CHAR_I;
    temp_1629[2] = CHAR_M;
    temp_1629[3] = CHAR_MASK;
    temp_1629[4] = tbl_ledseg_num[sys_ctl.dim_level / 10];
    temp_1629[5] = tbl_ledseg_num[sys_ctl.dim_level % 10];       
}

//音量显示
#pragma location="DISP_LEDSEG_SEG"
void menu_vol_ledseg(void)
{
    s8 vol;
    switch(sys_ctl.Vol_tyle)
    {
        case STATE_MAIN:
            temp_1629[0]=CHAR_V;
            temp_1629[1]=CHAR_O;  
            temp_1629[2]=CHAR_L;
            vol = sys_ctl.main_vol;
            break;
        case STATE_TREBLE:
            temp_1629[1]=CHAR_T;
            //temp_1629[1]=CHAR_MASK;   
            vol = sys_ctl.Trable_vol;
            break;
        case STATE_SUB:
            temp_1629[0]=CHAR_S;
            temp_1629[1]=CHAR_U;  
            temp_1629[2]=CHAR_B;
            vol = sys_ctl.Sub_vol;
            break;
        case STATE_BASS:
            temp_1629[1]=CHAR_B;
            //temp_1629[1]=CHAR_MASK;   
            vol = sys_ctl.Bass_vol;
            break;
    }
    Write_Display_Digtial_1629(vol);
#if 0
    temp_1629[0] = CHAR_V;
    temp_1629[1] = CHAR_O;
    temp_1629[2] = CHAR_L;
    temp_1629[3] = CHAR__;
    temp_1629[4] = tbl_ledseg_num[sys_ctl.main_vol / 10];//tbl_ledseg_num[sys_ctl.volume / 10];sys_ctl.main_vol
    temp_1629[5] = tbl_ledseg_num[sys_ctl.main_vol % 10];     
#endif    
#if 0
    disp_buf[0] = LEDSEG_U;
    //暂时这样写，虽然封装很不好，后期再做整理
#if BT_CALL_PRIVATE
    disp_buf[2] = tbl_ledseg_num[sys_ctl.volume / 10];
    disp_buf[3] = tbl_ledseg_num[sys_ctl.volume % 10];
#else
    if (sys_ctl.bt_sta >= BT_STA_INCOMING){
        disp_buf[2] = tbl_ledseg_num[sys_ctl.hshf_vol / 10];
        disp_buf[3] = tbl_ledseg_num[sys_ctl.hshf_vol % 10];
    } else {
        disp_buf[2] = tbl_ledseg_num[sys_ctl.volume / 10];
        disp_buf[3] = tbl_ledseg_num[sys_ctl.volume % 10];
    }
#endif
#if TASK_MUSIC_EN
    if(task_ctl.work_sta == TASK_MUSIC) {
        if(t_msc.pause) {
            //disp_buf[4] |= (ICON_PAUSE | ICON_MP3);
        } else {
            //disp_buf[4] |= (ICON_PLAY | ICON_MP3);
        }
    }
#endif
    ledseg_disp_device();
#endif
}
//显示机型
#pragma location="DISP_LEDSEG_SEG"
void menu_menu_ledseg(void)
{
#if 1
    switch(sys_ctl.Vol_tyle)
    {
        case STATE_MAIN:            
            temp_1629[1]=CHAR_M;   
            temp_1629[2]=CHAR_V;
            temp_1629[3]=CHAR_O; 
            temp_1629[4]=CHAR_L; 
            break;
            
        case STATE_TREBLE:
            temp_1629[0]=CHAR_T;
            temp_1629[1]=CHAR_R;   
            temp_1629[2]=CHAR_A;
            temp_1629[3]=CHAR_B; 
            temp_1629[4]=CHAR_L;     
            temp_1629[4]=CHAR_E;
            break;
            
        case STATE_SUB:
            temp_1629[0]=CHAR_S;
            temp_1629[1]=CHAR__;   
            temp_1629[2]=CHAR_V;
            temp_1629[3]=CHAR_O; 
            temp_1629[4]=CHAR_L;             
            break;
            
        case STATE_BASS:
            temp_1629[1]=CHAR_B;   
            temp_1629[2]=CHAR_A;
            temp_1629[3]=CHAR_S; 
            temp_1629[4]=CHAR_S;   
            break;            
    } 
#endif
#if 0
    temp_1629[1] = CHAR_M;
    temp_1629[2] = CHAR__;
    temp_1629[3] = CHAR_2;
    temp_1629[4] = CHAR_0;
    temp_1629[5] = CHAR_0; 
#endif    
}

//FM频率显示
#pragma location="DISP_LEDSEG_SEG"
void menu_freq_ledseg(void)
{
    ledseg_disp_num(t_fm.freq / 10);
#if 0    
    if(disp_buf[0] == LEDSEG_0) {
        disp_buf[0] = LEDSEG_OFF;
    }
    disp_buf[4] |= ICON_FM;
#if FM_RECORD_EN
    if (t_fm.fm_sta == FM_REC_PAUSE) {
        disp_buf[4] |= ICON_PAUSE;
    }
    if (device_is_online(DEVICE_SDMMC) || device_is_online(DEVICE_UDISK)) {
        ledseg_disp_device();
    }
#endif
#endif
}

//FM频道显示
#pragma location="DISP_LEDSEG_SEG"
void menu_freq_chan_ledseg(void)
{
    ledseg_disp_num(t_fm.ch_sel);
    temp_1629[2] = CHAR_MASK;
    temp_1629[3] = CHAR_P;
}

//输入数字显示
#pragma location="DISP_LEDSEG_SEG"
void menu_numsel_ledseg(void)
{
    ledseg_disp_num(disp_sel_num);
    ledseg_disp_device();
}

//音乐播放状态显示
#pragma location="DISP_LEDSEG_SEG"
void menu_play_ledseg(void)
{
    temp_1629[2] = tbl_ledseg_num[t_msc.cur_time.min / 10];
    temp_1629[3] = tbl_ledseg_num[t_msc.cur_time.min % 10];
    temp_1629[4] = tbl_ledseg_num[t_msc.cur_time.sec / 10];
    temp_1629[5] = tbl_ledseg_num[t_msc.cur_time.sec % 10];   
    
    temp_1629[6] |= CHAR_USB|CHAR_PLAY;
    temp_1629[7] |= CHAR_MP3|CHAR_DOT2;
    
#if 0
    if(0/*(sys_ctl.mute_flag) && (flicker_500ms_cnt % 2)*/) {    //MUTE时闪烁
        disp_buf[0] = LEDSEG_OFF;
        disp_buf[1] = LEDSEG_OFF;
        disp_buf[2] = LEDSEG_OFF;
        disp_buf[3] = LEDSEG_OFF;
        disp_buf[4] = LEDSEG_OFF;
    }
    else

    {
    
        disp_buf[0] = tbl_ledseg_num[t_msc.cur_time.min / 10];
        disp_buf[1] = tbl_ledseg_num[t_msc.cur_time.min % 10];
        disp_buf[2] = tbl_ledseg_num[t_msc.cur_time.sec / 10];
        disp_buf[3] = tbl_ledseg_num[t_msc.cur_time.sec % 10];
        //disp_buf[4] |= ICON_PLAY | ICON_MP3 | ICON_DOT2;
        disp_buf[4] |= ICON_DOT2;
        ledseg_disp_device();
    }
#endif

}

//音乐暂停状态显示
#pragma location="DISP_LEDSEG_SEG"
void menu_pause_ledseg(void)
{
    temp_1629[2] = tbl_ledseg_num[t_msc.cur_time.min / 10];
    temp_1629[3] = tbl_ledseg_num[t_msc.cur_time.min % 10];
    temp_1629[4] = tbl_ledseg_num[t_msc.cur_time.sec / 10];
    temp_1629[5] = tbl_ledseg_num[t_msc.cur_time.sec % 10];   
    
    temp_1629[6] = CHAR_USB|CHAR_STOP;
    temp_1629[7] |= CHAR_MP3|CHAR_DOT2;
#if 0    
        disp_buf[0] = tbl_ledseg_num[t_msc.cur_time.min / 10];
        disp_buf[1] = tbl_ledseg_num[t_msc.cur_time.min % 10];
        disp_buf[2] = tbl_ledseg_num[t_msc.cur_time.sec / 10];
        disp_buf[3] = tbl_ledseg_num[t_msc.cur_time.sec % 10];
        //disp_buf[4] |= ICON_PLAY | ICON_MP3 | ICON_DOT2;
        disp_buf[4] |= ICON_DOT2;
        ledseg_disp_device();
    ledseg_disp_device();
#endif    
}

//数字及设备显示
#pragma location="DISP_LEDSEG_SEG"
void menu_num_ledseg(void)
{
    u16 num = (ledseg_ocx.number > 9999)? 9999 : ledseg_ocx.number;
    ledseg_disp_num(num);
    ledseg_disp_device();
}

//AUX模式显示
#pragma location="DISP_LEDSEG_SEG"
void menu_aux_ledseg(void)
{
    temp_1629[2] = CHAR_A;
    temp_1629[3] = CHAR_U;
    temp_1629[4] = CHAR_X;
    temp_1629[6] = CHAR_AUX; 
#if 0
    disp_buf[0] = LEDSEG_OFF;
    disp_buf[1] = LEDSEG_A;
    disp_buf[2] = LEDSEG_U;
    disp_buf[3] = LEDSEG_H;
    disp_buf[4] = ICON_AUX;
    //disp_buf[4] = ICON_K1;
    /*if (t_rec.rec_sta == REC_STA_PAUSE) {
        disp_buf[4] |= ICON_PAUSE;
    }*/
#if AUX_RECORD_EN
    ledseg_disp_device();
    if (!device_is_online(DEVICE_SDMMC) && !device_is_online(DEVICE_UDISK)) {
        disp_buf[4] = ICON_AUX;
    }
#endif
#endif
}

#pragma location="DISP_LEDSEG_SEG"
void menu_aux_opt_ledseg(void)
{
    temp_1629[1] = CHAR_O;
    temp_1629[2] = CHAR_P;
    temp_1629[3] = CHAR_T;
    temp_1629[4] = CHAR_I;
    //temp_1629[6] = CHAR_OPT;
}

#pragma location="DISP_LEDSEG_SEG"
void menu_aux_coax_ledseg(void)
{
    temp_1629[1] = CHAR_C;
    temp_1629[2] = CHAR_O;
    temp_1629[3] = CHAR_A;
    temp_1629[4] = CHAR_X;
    temp_1629[6] = CHAR_COAX;
   
}
#pragma location="DISP_LEDSEG_SEG"
void menu_aux_hd_ledseg(void)
{
    disp_buf[0] = LEDSEG_OFF;
    disp_buf[1] = LEDSEG_H;
    disp_buf[2] = LEDSEG_D;
    disp_buf[3] = LEDSEG_OFF;
    disp_buf[4] = ICON_AUX;
   
}

//AUX EQ模式显示
#pragma location="DISP_LEDSEG_SEG"
void menu_aux_eq_ledseg(void)
{
    disp_buf[0] = LEDSEG_L;
    disp_buf[1] = LEDSEG_I;
    disp_buf[2] = LEDSEG_N;
    disp_buf[3] = LEDSEG_E;
    disp_buf[4] = ICON_AUX;
}

//扩音器模式显示
#pragma location="DISP_LEDSEG_SEG"
void menu_speaker_ledseg(void)
{
    disp_buf[0] = LEDSEG_O;
    disp_buf[1] = LEDSEG__;
    disp_buf[2] = LEDSEG__;
    disp_buf[3] = LEDSEG_O;
}

//USB DEVICE模式显示
#pragma location="DISP_LEDSEG_SEG"
void menu_pc_ledseg(void)
{
    disp_buf[1] = LEDSEG_P;
    disp_buf[2] = LEDSEG_C;
}

//蓝牙模式显示
#pragma location="DISP_LEDSEG_SEG"
void menu_bt_ledseg(void)
{
#if TASK_BTHID_EN
    if(task_hid_flag) {
        disp_buf[1] = LEDSEG_H;
        disp_buf[2] = LEDSEG_I;
        disp_buf[3] = LEDSEG_D;
    } else
#endif
    {
        temp_1629[2] = CHAR_B;
        temp_1629[3] = CHAR_T;
        temp_1629[6] |= CHAR_BT;
#if 0    
        temp_1629[1] = CHAR_B;
        temp_1629[2] = CHAR_U;
        temp_1629[3] = CHAR_L;    
        temp_1629[4] = CHAR_E;        
        temp_1629[6] |= CHAR_BT;
#endif 
        if(sys_ctl.bt_sta >= BT_STA_CONNECTED){
            if (sys_ctl.bt_playing_flag) {
                temp_1629[6] |= CHAR_PLAY;
            } else {
                temp_1629[6] |= CHAR_STOP; 
            }
        }        
#if 0
        if(sys_ctl.bt_sta >= BT_STA_CONNECTED)
        {
            if(sys_ctl.bt_sta == BT_STA_PLAYING)temp_1629[6] |= CHAR_STOP;
            else temp_1629[6] |= CHAR_PLAY;    
        }
#endif        
#if 0    
        disp_buf[0] = LEDSEG_B;
        disp_buf[1] = LEDSEG_L;
        disp_buf[2] = LEDSEG_U;
        disp_buf[3] = LEDSEG_E;
        disp_buf[4] = ICON_BT;
#endif        
#if BT_HID_MANU
        if(t_bt.hid_enable){
            disp_buf[4] = ICON_DOT2;
        }else{
            disp_buf[4] = ICON_NULL;
        }
#endif
    }
}

//RTC模式闪烁控制
#pragma location="DISP_LEDSEG_SEG"
void menu_rtc_time_blick_ledseg(void)
{
#if 0
    if(t_clock.sub_sta == STA_SHOW_CLOCK) {
        disp_buf[4] &= ~ICON_DOT2;
    }
    else if((t_clock.sub_sta == STA_SET_CLOCK) || (t_clock.sub_sta == STA_SET_ALARM)) {
        if (ledseg_ocx.show_time && ocx_show_cnt >= ledseg_ocx.show_time) {
            if(t_clock.set_sta == STA_SET_TIME_HOUR) {
                disp_buf[0] = LEDSEG_OFF;
                disp_buf[1] = LEDSEG_OFF;
            }
            else if(STA_SET_TIME_MIN == t_clock.set_sta) {
                disp_buf[2] = LEDSEG_OFF;
                disp_buf[3] = LEDSEG_OFF;
            }
        }
    }
#endif    
}

//RTC模式显示
#pragma location="DISP_LEDSEG_SEG"
void menu_rtc_time_ledseg(void)
{
#if 0
    disp_buf[0] = tbl_ledseg_num[t_time.hour / 10];
    disp_buf[1] = tbl_ledseg_num[t_time.hour % 10];
    disp_buf[2] = tbl_ledseg_num[t_time.minute / 10];
    disp_buf[3] = tbl_ledseg_num[t_time.minute % 10];
    disp_buf[4] |= ICON_DOT2;

    if(sys_ctl.alarm_switch){
        disp_buf[4] |= ICON_PLAY;
    }else{
        disp_buf[4] &= ~ICON_PLAY;
    }

    if(task_ctl.work_sta == TASK_SHOW_TIME) {
        if(flicker_500ms_cnt % 2) {
            disp_buf[4] &= ~ICON_DOT2;
        }
    } else { //SetTime or SetAlarm
        if(flicker_500ms_cnt % 2) {
            menu_rtc_time_blick_ledseg();
        }
    }
#endif    
}

//闹钟模式显示
#pragma location="DISP_LEDSEG_SEG"
void menu_alarm_ledseg(void)
{
#if 0
    disp_buf[0] = tbl_ledseg_num[sys_ctl.alarm_hour / 10];
    disp_buf[1] = tbl_ledseg_num[sys_ctl.alarm_hour % 10];
    disp_buf[2] = tbl_ledseg_num[sys_ctl.alarm_min / 10];
    disp_buf[3] = tbl_ledseg_num[sys_ctl.alarm_min % 10];
    disp_buf[4] |= ICON_DOT2;
#endif    
}

//闹钟开关状态显示
#pragma location="DISP_LEDSEG_SEG"
void menu_alarm_status_ledseg(void)
{
#if 0
    if(sys_ctl.alarm_switch == 0) {
        disp_buf[1] = LEDSEG_O;
        disp_buf[2] = LEDSEG_F;
        disp_buf[3] = LEDSEG_F;
    }
    else if(sys_ctl.alarm_switch == 1) {
        disp_buf[1] = LEDSEG_O;
        disp_buf[2] = LEDSEG_N;
    }
#endif    
}

//汤姆猫模式显示
#pragma location="DISP_LEDSEG_SEG"
void menu_tomcat_ledseg(void)
{
#if 0
    disp_buf[1] = LEDSEG_C;
    disp_buf[2] = LEDSEG_A;
    disp_buf[3] = LEDSEG_T;
#endif    
}

//音乐EQ效果显示
#pragma location="DISP_LEDSEG_SEG"
void menu_eq_ledseg(void)
{
#if 1
    //sys_ctl.Eq_tyle = STATUS_MUSIC; 
    switch(sys_ctl.Eq_tyle)
    {
        case STATUS_TV:
            temp_1629[0] = CHAR_D;
            temp_1629[1] = CHAR_S;
            temp_1629[2] = CHAR_P;
            temp_1629[3] = CHAR__;
            temp_1629[4] = CHAR_1;
            temp_1629[7] |= CHAR_TV;
            break;
            
        case STATUS_MUSIC:
            temp_1629[0] = CHAR_D;
            temp_1629[1] = CHAR_S;
            temp_1629[2] = CHAR_P;
            temp_1629[3] = CHAR__;
            temp_1629[4] = CHAR_2;
            temp_1629[7] |= CHAR_MUSIC;
            break;
            
        case STATUS_MOVIE:
            temp_1629[0] = CHAR_D;
            temp_1629[1] = CHAR_S;
            temp_1629[2] = CHAR_P;
            temp_1629[3] = CHAR__;
            temp_1629[4] = CHAR_3;
            temp_1629[7] |= CHAR_MOVIE;
            break;
    }
    //ledseg_disp_device();
#endif    
#if 0
#if IIS_EN
    disp_buf[1] = LEDSEG_E;
    disp_buf[2] = LEDSEG_Q;
    disp_buf[3] = tbl_ledseg_num[eq_flag]; 
//#elif !EQ_DISPLAY_NUM
#else
    disp_buf[1] = eq_table[sys_ctl.eq_num][0];
    disp_buf[2] = eq_table[sys_ctl.eq_num][1];
    disp_buf[3] = eq_table[sys_ctl.eq_num][2];
/*#else
    disp_buf[1] = LEDSEG_E;
    disp_buf[2] = LEDSEG_Q;
    disp_buf[3] = tbl_ledseg_num[sys_ctl.eq_num];*/
#endif
    ledseg_disp_device();
#endif
}

#pragma location="DISP_LEDSEG_SEG"
void menu_3d_ledseg(void)
{
#if 1
    temp_1629[0] = CHAR_3;
    temp_1629[1] = CHAR_D;
    if(!sys_ctl.m3d_Mode)
    {
        temp_1629[3] = CHAR_O;
        temp_1629[4] = CHAR_F;
        temp_1629[5] = CHAR_F;
    }
    else
    {
        temp_1629[3] = CHAR_O;
        temp_1629[4] = CHAR_N;
    }    
    temp_1629[6] = CHAR_3D;
#endif    
}

//音乐播放模式显示
#pragma location="DISP_LEDSEG_SEG"
void menu_mode_play_ledseg(void)
{
    temp_1629[1] = play_mode_table[sys_ctl.mode_play][0];
    temp_1629[2] = play_mode_table[sys_ctl.mode_play][1];
    temp_1629[3] = play_mode_table[sys_ctl.mode_play][2];
    //ledseg_disp_device();
}

#pragma location="DISP_LEDSEG_SEG"
void menu_mode_stop_ledseg(void)
{
#if 0
    disp_buf[0] = LEDSEG_S;
    disp_buf[1] = LEDSEG_T;
    disp_buf[2] = LEDSEG_O;
    disp_buf[3] = LEDSEG_P;
#endif    
}

//清屏显示
#pragma location="DISP_LEDSEG_SEG"
void ledseg_disp_buf_clr(void)
{
//    memset((u8*)disp_buf,0x00,5);
    memset((u16*)temp_1629,0x0000,8);
}

//MIC录音模式显示
#pragma location="DISP_LEDSEG_SEG"
void menu_mic_ledseg(void)
{
#if 0
    disp_buf[1] = LEDSEG_R;
    disp_buf[2] = LEDSEG_E;
    disp_buf[3] = LEDSEG_C;
    if (t_rec.rec_sta == REC_STA_PAUSE) {
        disp_buf[4] |= ICON_PAUSE;
    }
    ledseg_disp_device();
#endif    
}

//MIC录音中显示
#pragma location="DISP_LEDSEG_SEG"
void menu_recording_ledseg(void)
{
#if 0
    u8 min, sec;
    u32 rec_time=0;

#if (TASK_MIC_EN || (TASK_AUX_EN && AUX_RECORD_EN))
    rec_time = t_rec.rec_time;
#endif
#if (TASK_FM_EN && FM_RECORD_EN)
    if (task_ctl.work_sta == TASK_FM) {
        rec_time = t_fm_rec.rec_time;
    }
#endif

    if(rec_time < 3600) {
        min =rec_time/60;
        sec = rec_time%60;
        disp_buf[4] = ICON_DOT2;
    } else {
        min = (rec_time/60)/60;
        sec = (rec_time/60)%60;
    }

    disp_buf[0] = tbl_ledseg_num[min / 10];
    disp_buf[1] = tbl_ledseg_num[min % 10];
    disp_buf[2] = tbl_ledseg_num[sec / 10];
    disp_buf[3] = tbl_ledseg_num[sec % 10];

    if(flicker_500ms_cnt % 2) {
        if(fs_cur_dev() == DEVICE_SDMMC) {
            disp_buf[4] |= ICON_SD | ICON_DOT2;
        } else if (fs_cur_dev() == DEVICE_UDISK) {
            disp_buf[4] |= ICON_USB | ICON_DOT2;
        }
    }
#endif    
}

//无设备显示
#pragma location="DISP_LEDSEG_SEG"
void menu_no_device_ledseg(void)
{
#if 0
    disp_buf[1] = LEDSEG_N;
    disp_buf[2] = LEDSEG_O;
#endif    
}

//删除显示
#pragma location="DISP_LEDSEG_SEG"
void menu_del_ledseg(void)
{
#if 0
    disp_buf[1] = LEDSEG_D;
    disp_buf[2] = LEDSEG_E;
    disp_buf[3] = LEDSEG_L;
#endif    
}
#pragma location="DISP_LEDSEG_SEG"
void menu_code_version_ledseg(void)
{
    temp_1629[2] = CHAR_V;
    temp_1629[3] = CHAR__;     
    temp_1629[4] = CHAR_1;
    temp_1629[5] = CHAR_5;
}

#pragma location="DISP_LEDSEG_SEG"
void menu_file_ledseg(void)
{
    temp_1629[0]=CHAR_F; 
    temp_1629[1]=CHAR_I;   
    temp_1629[2]=CHAR_L;
    temp_1629[3]=CHAR_E; 

    temp_1629[4]=tbl_ledseg_num[t_msc.dir_num / 10]; 
    temp_1629[5]=tbl_ledseg_num[t_msc.dir_num % 10];
    temp_1629[6] = CHAR_USB;    
}

//磁盘满显示
#pragma location="DISP_LEDSEG_SEG"
void menu_full_ledseg(void)
{
#if 0
    disp_buf[0] = LEDSEG_F;
    disp_buf[1] = LEDSEG_U;
    disp_buf[2] = LEDSEG_L;
    disp_buf[3] = LEDSEG_L;
#endif    
}

//出错显示
#pragma location="DISP_LEDSEG_SEG"
void menu_err_ledseg(void)
{
    temp_1629[2] = CHAR_E;
    temp_1629[3] = CHAR_R;
    temp_1629[4] = CHAR_R;    
}

//mute显示
#pragma location="DISP_LEDSEG_SEG"
void menu_mute_ledseg(void)
{
    temp_1629[1] = CHAR_M;
    temp_1629[2] = CHAR_U;
    temp_1629[3] = CHAR_T;
    temp_1629[4] = CHAR_E;
}
//灭屏
#pragma location="DISP_LEDSEG_SEG"
void menu_null_ledseg(void)
{
    temp_1629[2] = CHAR_B;
    temp_1629[3] = CHAR_T;
    temp_1629[6] = CHAR_MASK;
#if 0    
    disp_buf[0] = LEDSEG_OFF;
    disp_buf[1] = LEDSEG_OFF;
    disp_buf[2] = LEDSEG_OFF;
    disp_buf[3] = LEDSEG_OFF;
#endif    
}
#pragma location="DISP_LEDSEG_SEG"
void menu_usbsd_ledseg(void)
{
    disp_buf[1] = LEDSEG_U;
    disp_buf[2] = LEDSEG_S;
    disp_buf[3] = LEDSEG_B;
}
#pragma location="DISP_LEDSEG_SEG"
void menu_tf_ledseg(void)
{
    disp_buf[1] = LEDSEG_T;
    disp_buf[2] = LEDSEG_F;
}
#pragma location="DISP_LEDSEG_SEG"
void menu_lode_ledseg(void)
{
    temp_1629[1] = CHAR_L;
    temp_1629[2] = CHAR_O;
    temp_1629[3] = CHAR_D;
    temp_1629[4] = CHAR_E;
    
    temp_1629[6] = CHAR_USB;
//    temp_1629[7] = CHAR_E;
}

#pragma location="DISP_LEDSEG_SEG"
void menu_input_mask(void)
{
    switch(task_ctl.work_sta)
    {
        case TASK_AUX:
            temp_1629[6] |= CHAR_AUX;
            break;
            
        case TASK_COAX:
            temp_1629[6] |= CHAR_COAX;
            break;   
            
        case TASK_OPTI:
            temp_1629[6] |= CHAR_OPT;
            break;   
            
        case TASK_MUSIC:
            temp_1629[6] |= CHAR_USB;
            temp_1629[7] |= CHAR_MP3;            
            if(sys_ctl.mode_play == ITEM_LOOP_RAMDOM)
            {
                if(sys_ctl.bonoff_1s)temp_1629[6] |= CHAR_BT_PLAY;
            }            
            else if(sys_ctl.mode_play == ITEM_LOOP_SINGLE)
            {
                temp_1629[6] |= CHAR_BT_PLAY;
            }          
            break; 
            
        case TASK_BT:
            temp_1629[6] |= CHAR_BT;
            break;    
    }
    if(task_ctl.work_sta != TASK_POWEROFF)
    {
        switch(sys_ctl.Eq_tyle)
        {     
            case STATUS_TV:                
                temp_1629[7] |= CHAR_TV;
                break;
                
            case STATUS_MUSIC:                
                temp_1629[7] |= CHAR_MUSIC;
                break;
                
            case STATUS_MOVIE:              
                temp_1629[7] |= CHAR_MOVIE;
                break;          
        }
    }
}


#pragma constseg=DISP_LEDSEG_CONST
const H menu_ledseg[] =
{
    menu_welcome_ledseg,
    menu_model_ledseg,    
    menu_vol_ledseg,
    menu_dim_ledseg,
    menu_menu_ledseg,
#if TASK_FM_EN
    menu_freq_ledseg,
    menu_freq_chan_ledseg,
#else
    0,
    0,
#endif
    menu_numsel_ledseg,
#if TASK_MUSIC_EN
    menu_play_ledseg,
#else
    0,
#endif
    menu_pause_ledseg,
    menu_num_ledseg,
#if TASK_AUX_EN
    menu_aux_ledseg,
#else
    0,
#endif
    menu_aux_coax_ledseg,

    menu_aux_opt_ledseg,
//    menu_aux_lin_ledseg,
//    menu_aux_hd_ledseg,

    menu_aux_eq_ledseg,
    menu_speaker_ledseg,
    menu_pc_ledseg,
#if TASK_BT_EN
    menu_bt_ledseg,
#else
    0,
#endif

    menu_usbsd_ledseg,
    menu_tf_ledseg,
    menu_lode_ledseg,
#if TASK_RTC_EN
    menu_rtc_time_ledseg,
#else
    0,
#endif
#if ALARM_EN
    menu_alarm_ledseg,
    menu_alarm_status_ledseg,
#else
    0,
    0,
#endif
    menu_eq_ledseg,
    menu_3d_ledseg,
    menu_mode_play_ledseg,
    
    menu_mode_stop_ledseg,
    
#if TASK_MIC_EN
    menu_mic_ledseg,
#else
    0,
#endif
#if (TASK_MIC_EN || (TASK_AUX_EN && AUX_RECORD_EN) || (TASK_FM_EN && FM_RECORD_EN))
    menu_recording_ledseg,
#else
    0,
#endif
#if TASK_TOMCAT_EN
    menu_tomcat_ledseg,
#else
    0,
#endif
    menu_poweroff_ledseg,
    menu_standby_ledseg,
    menu_no_device_ledseg,
    menu_del_ledseg,
    menu_code_version_ledseg,
    menu_file_ledseg,
    menu_full_ledseg,
    menu_err_ledseg,
    menu_null_ledseg,
    menu_mute_ledseg,
    
};
#pragma constseg=default

#pragma location="DISP_LEDSEG_SEG"
void ledseg_disp (u8 menu_number)
{
#if 0
    void (*p)(void);
    ledseg_disp_buf_clr();
    p = menu_ledseg[menu_number];
    (*p)();
    ledseg_value_set();
#endif
    void (*p)(void);
    //ledseg_disp_buf_clr();
    MENU_DISPLAY_MASK();
    menu_input_mask();
    p = menu_ledseg[menu_number];
    (*p)();
    ShowCHAR();    
}

#endif
