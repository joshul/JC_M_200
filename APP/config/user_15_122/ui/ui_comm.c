/*****************************************************************************
* Module    : UI
* File      : ui_comm.c
* Author    : Hanny
* Function  : 任务公供用户自定义UI设计，处理按键功能映射与显示
*****************************************************************************/
#ifndef CONFIG_C

//#include "user_tas5711.h"
#include "task_NPCP215X.h"  
#include "TM1629.h"

extern void music_file_num_set(bool Up_Down);

extern u8 eq_flag;

//所有任务的公共消息处理函数
__near_func void deal_msg(u8 msg)
{
    bool Up_Down = 0;
    switch (msg) {       
    case KU_MODE:
        if(sys_ctl.mute_flag)break;
        sys_ctl.mode_switch_flag = 1;
        task_ctl.work_sta = TASK_EXIT;
        break;
        
    case KL_MODE:
        //if(sys_ctl.irkey)break;
        if(sys_ctl.mute_flag)break;
        show_version();
        break;
        
    case KU_POWER: 
        task_ctl.work_sta = TASK_POWEROFF;
        break;
        
    case KU_MENU:
        if(sys_ctl.mute_flag)break;
        sys_ctl.Vol_tyle++;
        if(task_ctl.work_sta == TASK_MUSIC){
            if(sys_ctl.Vol_tyle > STATE_FILE)sys_ctl.Vol_tyle = STATE_MAIN;
            if(sys_ctl.Vol_tyle == STATE_FILE)show_file();
            else show_menu();
            break;
        }
        else 
        {
            if(sys_ctl.Vol_tyle > STATE_TREBLE)sys_ctl.Vol_tyle = STATE_MAIN;
        }
        show_menu();
        break;
      
    case K_VOL_UP:    
    case KL_VOL_UP:
    case KH_VOL_UP:
        Up_Down = 1;
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:           
        if(!sys_ctl.Vol_tyle)sys_ctl.Vol_tyle = STATE_MAIN;
        if(sys_ctl.Vol_tyle ==STATE_FILE)
        {
            music_file_num_set(Up_Down);
            show_file();
            break;
        }        
        AudioCodec_Volume_Set(sys_ctl.Vol_tyle,Up_Down);
        show_volume();       
        break;  
        
    case T_KEY_BT:  
        if(sys_ctl.mute_flag)break;
        if(task_ctl.work_sta == TASK_BT)break;
        task_ctl.work_sta = TASK_BT;
        break;
        
    case T_KEY_DMI:
        if(sys_ctl.mute_flag)break;       
        sys_ctl.dim_level++;
        if(sys_ctl.dim_level > 3)sys_ctl.dim_level = 1;        
        Duty_set();
        show_dim();
        break;
        
#if NPCP215X     
    case T_KEY_TV:
        if(sys_ctl.mute_flag)break; 
        if(sys_ctl.Eq_tyle == STATUS_TV)break;
        else sys_ctl.Eq_tyle = STATUS_TV;
        sys_ctl.m3d_Mode = 0;
        AudioCodec_Eq_Set(sys_ctl.Eq_tyle);
        sys_param_write8(PARAM_EQ, sys_ctl.Eq_tyle);
        show_eq();
        break;
        
    case T_KEY_MUSIC:
        if(sys_ctl.mute_flag)break;
        if(sys_ctl.Eq_tyle == STATUS_MUSIC)break;
        sys_ctl.Eq_tyle = STATUS_MUSIC;
        sys_ctl.m3d_Mode = 0;
        AudioCodec_Eq_Set(sys_ctl.Eq_tyle);
        sys_param_write8(PARAM_EQ, sys_ctl.Eq_tyle);
        show_eq();
        break;
        
    case T_KEY_MOVIE:  
        if(sys_ctl.mute_flag)break;
        if(sys_ctl.Eq_tyle == STATUS_MOVIE)break;
        else sys_ctl.Eq_tyle = STATUS_MOVIE;
        sys_ctl.m3d_Mode = 0;
        AudioCodec_Eq_Set(sys_ctl.Eq_tyle);
        sys_param_write8(PARAM_EQ, sys_ctl.Eq_tyle);
        show_eq();
        break;
        
    case T_KEY_3D:   
        if(sys_ctl.mute_flag)break;
        sys_ctl.m3d_Mode = !sys_ctl.m3d_Mode;   
        AudioCodec_3d_Set(sys_ctl.m3d_Mode);
        sys_param_write8(PARAM_3D, sys_ctl.m3d_Mode);
        show_3d();
        break;    
        
#endif
    case Q_BT_CONN_REQ:
#if TASK_BT_EN
        task_ctl.work_sta = TASK_BT;
#endif
        break;
#if 0
        //长按菜单键，退出到主菜单
    case KL_MENU:
#endif
#if BT_CALL_PRIVATE
    case KU_HSF:
#else
    case KL_HSF:
#endif
        if (sys_ctl.bt_sta >= BT_STA_INCOMING)
        {
            break;      //通话过程中与测试模式不切模式
        } else
        {
#if MODE_CHANGE_BLUE_ON
            led_mode_on();
#endif
            task_ctl.work_sta = TASK_EXIT;
            break;
        }
#if 0    
#if IIS_EN
    case T_KEY_EQ:
        eq_flag++;
        if(eq_flag > 4)eq_flag = 1;
        TAS5731M_EQ_Set();
        show_eq();
        break;
#endif
#endif
#if(SOFT_POWER_ON_OFF) //使用软开关机功能
    case KL_POWER:
    case KL_PLAY_POWER:
    #if BT_AUTO_PLAY
        if(sys_ctl.bt_sta == BT_STA_PLAYING){
            sys_ctl.bt_auto_play_flag = 1;
            sys_param_write8(PARAM_BT_AUTO_PLAY, sys_ctl.bt_auto_play_flag);
            param_write(Link_BdAddr, PARAM_REMOTE_DEV_ADDR, 6);
        }
        else{
            sys_ctl.bt_auto_play_flag = 0;
            sys_param_write8(PARAM_BT_AUTO_PLAY, sys_ctl.bt_auto_play_flag);
        }
    #endif
        sys_ctl.soft_poweroff_flag = 1;
        task_ctl.work_sta = TASK_POWEROFF;
        break;
#endif

#if IR_HARD_POWERDOWN_EN
    case KU_POWER:
        sys_ctl.ir_power_flag = 1;
        //printf("IR_POWER_DOWN\n");
        break;
#endif

    //1秒消息
    case QSYSTEM_1S:

#if ALARM_EN
        task_alarm_check();
#endif
#if FM_TIME_EN
        task_fm_check();
#endif
#if TASK_HOLD_EN
        task_sleep_check();
#endif
        //task_backlight_check();
/*
        task_poweroff_check();

#if LED_STATUS_BLINK
        if(sys_ctl.led_status_blink_flag){  //低电时状态灯慢闪
            LED_STATUS_TOG();
        }
#endif*/
        break;

#if (HARD_IR_EN || SOFT_IR_EN || SOFT_IR_PORT_INT_EN)
    case T_KEY_MUTE:
#if FM_VOUT_THROUGH_MCU
        if(sys_ctl.mute_flag){
            sys_unmute();
            ocx_clear();
#if NPCP215X

            AudioCodec_MUTE(UNMUTE);
#endif            

        } else
        {
#if NPCP215X

            AudioCodec_MUTE(MUTE);
#endif        
            sys_mute();
            //ledseg_disp(MENU_MUTE);
            show_mute();
        }
        break;
#else
        if(sys_ctl.mute_flag){
            if(task_ctl.work_sta == TASK_FM) {
                sys_ctl.mute_flag = 0;
                speaker_unmute();//FM模式下若不经过主控输出则直接mute功放即可
            } else {
                sys_unmute();
            }
        } else{
            if(task_ctl.work_sta == TASK_FM) {
                sys_ctl.mute_flag = 1;
                speaker_mute();
            } else {
                sys_mute();
            }
        }
        break;
#endif
#endif
    }
}

#endif
