/*-------------------------------------------------------------------------
    
-------------------------------------------------------------------------*/
/*****************************************************************************
* Module    : UI
* File      : ui_bt.c
* Author    : Hanny
* Function  : 音乐任务用户自定义UI设计，处理按键功能映射与显示
*****************************************************************************/
#ifndef CONFIG_C
#include "task_NPCP215X.h"  
//extern void task_music_prev_dir(void);
//extern void task_music_next_dir(void);

//音乐任务下显示
#if (IS_LEDSEG_5C7S_DISPLAY || IS_LEDSEG_7PIN_DISPLAY)
#pragma location="TASK_MUSIC_SEG"
void task_music_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        if(!sys_ctl.mute_flag){   
            
            if(sys_ctl.mp3_stop_flag)
                ledseg_disp(MENU_MODE_STOP);
            else if(t_msc.pause) 
                ledseg_disp(MENU_MUSIC_PAUSE);
            else 
                ledseg_disp(MENU_MUSIC_PLAY);              
        }
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}
#else
#pragma location="TASK_MUSIC_SEG"
void task_music_display(void)
{
}
#endif

//音乐任务消息处理
#pragma location="TASK_MUSIC_SEG"
void task_music_deal_msg(u8 msg)
{
#if WARNING_VOL_MAXMIN
    if((sys_ctl.vol_maxmin_music_flag == 1) && (sys_ctl.vol_maxmin_music_play == 1)){
        sys_ctl.vol_maxmin_music_flag = 0;
        sys_ctl.vol_maxmin_music_play = 0;
        u8 pause_flag = t_msc.pause;
        music_init();
        task_music_play_init();
        music_jump(&pt);    //恢复播放
        music_play();
        t_msc.pause = pause_flag;
        if(pause_flag){   //没有暂停时才恢复播放
            music_pause();
        }
        if(!sys_ctl.volume) {
            sys_mute();
        }
        flush_msg();
        return;
    }
#endif
    if (msg == NULL) {
        return;
    }
    switch(msg) {

        //上一曲，下一曲
    case KU_PREV:
    case KU_PREV_VOL_DOWN:
        if(sys_ctl.mute_flag)break;
        if(sys_ctl.mp3_stop_flag)break;
#if AB_REPEAT_EN
        if(t_msc.repeat_flag)
        {
            if(t_msc.repeat_flag == 2)
            {
                t_msc.repeat_flag = 1;
                t_msc.disp_status_flag = 1;
            }
            else if(t_msc.repeat_flag == 3)
            {
                t_msc.repeat_flag = 1;
                t_msc.disp_status_flag = 1;
                t_msc.play_sta = PLAYER_NORMAL;
                music_get_point(&t_msc.point.pb);
                music_jump(&t_msc.point.pb);
            }
            break;
        }
#endif
        DBG_TASK_MUSIC("Prev\n");
        task_music_prev();
        break;
        
    case KU_NEXT:
    case KU_NEXT_VOL_UP:
        if(sys_ctl.mute_flag)break;
        if(sys_ctl.mp3_stop_flag)break;
#if AB_REPEAT_EN
        if(t_msc.repeat_flag == 1)
        {
            t_msc.repeat_flag = 2;
            t_msc.disp_status_flag = 1;
            task_music_set_a();//更新A点
            break;
        }
        else if(t_msc.repeat_flag == 2)
        {
            task_music_repeat_play();
            t_msc.repeat_flag = 3;
            t_msc.disp_status_flag = 1;
            break;
        }
        else if(t_msc.repeat_flag == 3)
        {
            t_msc.repeat_flag = 1;
            t_msc.disp_status_flag = 1;
            t_msc.play_sta = PLAYER_NORMAL;
            music_get_point(&t_msc.point.pb);
            music_jump(&t_msc.point.pb);
            break;
        }
#endif
        DBG_TASK_MUSIC("Next\n");
        task_music_next();
        break;
        
        //播放/暂停
    case KU_PLAY:
    case KU_PLAY_POWER:
        if(sys_ctl.mute_flag)break;
        if(sys_ctl.mp3_stop_flag)
        {                   
#if BREAK_POINT_EN
            t_msc.break_flag = 0;
            t_msc.music_num = 1;
#endif
            t_msc.file_change = 1;          //需要打开一个新文件
        }
        else
        {
#if NPCP215X
            AudioCodec_MUTE(MUTE);
#endif
            if (music_start_pause()) 
            {                
                t_msc.pause = 0;                //更新显示状态
                sys_ctl.music_play_flag = 1;
                sys_ctl.sleep_cnt = 0xffff;
#if AB_REPEAT_EN
                if (t_msc.play_sta == PLAYER_NORMAL) {
                    task_music_set_a();         //正常播放模式下，重设A点
                }
#endif
//                DBG_TASK_MUSIC("play\n");    
//                delay_5ms(200);
#if NPCP215X
                //AudioCodec_MUTE(UNMUTE);
#endif
                led_music_play();
            } 
            else 
            {   
                sys_ctl.music_play_flag = 0;
                t_msc.pause = 1;
//                sys_ctl.sleep_cnt = STANDBY_TIME;        //进入休眠模式
//                DBG_TASK_MUSIC("pause\n");
                led_idle();
#if NPCP215X
            //Audio_Slow_Volume(MUTE);
                //AudioCodec_MUTE(MUTE);
#endif
            }
        }
        t_msc.disp_status_flag = 1;
        break;
#if 0        
    case T_KEY_STOP:
        if(sys_ctl.mute_flag)break;
        sys_ctl.mp3_stop_flag = 1;
        //if (!music_start_pause())
        //    t_msc.pause = 1;
        music_stop();                               //停止播放器
        t_msc.disp_status_flag = 1;
        show_stat_play();
        break;  
#endif        
#if 0
         //长按关机
    case KL_PLAY:
        if(sys_ctl.irkey)break;
        task_ctl.work_sta = TASK_POWEROFF;
        break;
#endif
#if (HARD_IR_EN || SOFT_IR_EN)
    case T_KEY_NUM_0:
    case T_KEY_NUM_1:
    case T_KEY_NUM_2:
    case T_KEY_NUM_3:
    case T_KEY_NUM_4:
    case T_KEY_NUM_5:
    case T_KEY_NUM_6:
    case T_KEY_NUM_7:
    case T_KEY_NUM_8:
    case T_KEY_NUM_9:
        if(sys_ctl.mute_flag)break;
        task_music_select_numsic(msg - T_KEY_NUM_0);
        break;
#endif

#if 0        
        //快进快退，复读模式下不支持
    case KL_PREV:
    case KH_PREV:       
        if(sys_ctl.mp3_stop_flag)break;
        if(sys_ctl.irkey){
            if(sys_ctl.mute_flag)break;
            if (t_msc.play_sta == PLAYER_NORMAL) {
                DBG_TASK_MUSIC("P");
                t_msc.speed = 0;                            //快进快退恢复会清空变速信息
                music_fast(MUSIC_FAST_BACKWORD, 1);            //快退
#if AB_REPEAT_EN
                task_music_set_a();
#endif
            }
        }
        else{
            user_set_volume(user_volume_dec(sys_ctl.volume));
            show_volume();
            sys_unmute();
            if(!sys_ctl.volume){
            sys_mute();
            }
        }
        break;
       
    case KL_NEXT:
    case KH_NEXT:       
        if(sys_ctl.mp3_stop_flag)break;
        
        if(sys_ctl.irkey){
            if(sys_ctl.mute_flag)break;
            if (t_msc.play_sta == PLAYER_NORMAL) {
                DBG_TASK_MUSIC("N");
               
                music_fast(MUSIC_FAST_FORWORD, 1);             //快进
#if AB_REPEAT_EN
                task_music_set_a();
#endif
            }
        }
        else{
            user_set_volume(user_volume_inc(sys_ctl.volume));
            show_volume();
            sys_unmute();
        }
        break;
        
    case KLU_PREV:
    case KLU_NEXT:
        if(sys_ctl.mute_flag)break;
        if(sys_ctl.mp3_stop_flag)break;
        if (t_msc.play_sta == PLAYER_NORMAL) {
            DBG_TASK_MUSIC("E");
            music_fast_end();                           //快进快退结束
#if AB_REPEAT_EN
            task_music_set_a();                         //更新A点
#endif
            //music_set_speed(tbl_speed_order[t_msc.speed]);
          //  t_msc.speed = 0;                            //快进快退恢复会清空变速信息
           // music_set_speed(tbl_speed_order[t_msc.speed]);
        }
        break;
#endif        
#if 0         
           
    //音量加减 
//    case K_VOL_DOWN1:
//    case KL_VOL_DOWN1:
//    case KH_VOL_DOWN1:    
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KL_PREV_VOL_DOWN:
    case KH_PREV_VOL_DOWN:
        sys_ctl.Up_Down=0;
        AudioCodec_Volume_Set(sys_ctl.Up_Down);
        show_volume();
#if 0        
        user_set_volume(user_volume_dec(sys_ctl.volume));
        show_volume();
        sys_unmute();
#if WARNING_VOL_MAXMIN
        if(!sys_ctl.volume){
            sys_unmute();
            dac_fade_out();
            music_play_vol_music();
        }
#else
        if(!sys_ctl.volume){
            sys_mute();
        }
#endif
#endif
        break;

//    case K_VOL_UP1:
//    case KL_VOL_UP1:
//    case KH_VOL_UP1:
    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KL_NEXT_VOL_UP:
    case KH_NEXT_VOL_UP:
         sys_ctl.Up_Down=1; 
         AudioCodec_Volume_Set(sys_ctl.Up_Down);
         show_volume();
#if 0         
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
        sys_unmute();        
#if WARNING_VOL_MAXMIN
        if(sys_ctl.volume == VOLUME_MAX){
            music_play_vol_music();
        }
#endif
#endif
        break;

#endif

#if 0
    case KL_VOL_UP:
    case KH_VOL_UP:
        sys_ctl.Up_Down=1;
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
        AudioCodec_Volume_Set(sys_ctl.Up_Down);
#if WARNING_VOL_MAXMIN
        if((!sys_ctl.main_vol)||(30 == sys_ctl.main_vol)){
            music_play_vol_music();
        }
#endif
        break;
#endif

#if 0
#if AB_REPEAT_EN
        //AB复读
   /*case KU_SET_A:
        task_music_set_a();
        t_msc.disp_status_flag = 1;
        break;
    case KU_REPEAT:
        task_music_repeat_play();
        t_msc.disp_status_flag = 1;
        break;*/
#endif
#endif
       //短按菜单键，进入目录导航
    case KU_MODE:
        if(sys_ctl.mute_flag)break; 
#if AB_REPEAT_EN
        if(t_msc.repeat_flag)
        {
            t_msc.repeat_flag = 0;
            music_get_point(&t_msc.point.pb);
            music_jump(&t_msc.point.pb);
            t_msc.play_sta = PLAYER_NORMAL;
            t_msc.disp_status_flag = 1;
            break;
        }
#endif
        if(t_msc.mode_flag) {
            task_ctl.work_sta = TASK_EXIT;
        } 
        else {
            if (music_next_device()) {
#if BREAK_POINT_EN
                t_msc.break_flag = 1;
                task_music_get_num();
#endif
                t_msc.file_change = 1;          //需要打开一个新文件
            }
            else
            {
                task_ctl.work_sta = TASK_EXIT;
            }
        }
        t_msc.mode_flag = 1;
        break;
#if 0
    case KU_POWER: 
        task_ctl.work_sta = TASK_POWEROFF;
        break;
        
    case KL_MODE:
        if(sys_ctl.irkey)break;
        task_ctl.work_sta = TASK_POWEROFF;
        break;  
#endif        
    case KU_PLAY_MODE:
        if(sys_ctl.mute_flag)break;
        if(sys_ctl.mode_play == ITEM_LOOP_ALL)
        {
            sys_ctl.mode_play = ITEM_LOOP_RAMDOM;
        }
        else if(sys_ctl.mode_play == ITEM_LOOP_RAMDOM)
        {
            sys_ctl.mode_play = ITEM_LOOP_SINGLE;
        }
        else
        {
            sys_ctl.mode_play = ITEM_LOOP_ALL;
        }
        param_write8(PARAM_MODE_PLAY, sys_ctl.mode_play);
        show_mode_play();
        break; 
        //测试功能段
#if 0
    case T_KEY_TV:
        task_music_prev_dir();
        break;
        
    case T_KEY_MUSIC:
        task_music_next_dir();
        break;    
#endif
#if 0           
    case KU_AUX:
        if(sys_ctl.mute_flag)break;
        if(t_msc.mode_flag) {
            task_ctl.work_sta = TASK_AUX;
        } else {
            if (music_next_device()) {
#if BREAK_POINT_EN
                t_msc.break_flag = 1;
                task_music_get_num();
#endif
                t_msc.file_change = 1;          //需要打开一个新文件
            }
            else
            {
                task_ctl.work_sta = TASK_AUX;
            }
        }
        t_msc.mode_flag = 1;       
        break;
#endif
#if 0
/*    case T_KEY_EQ:
        if(sys_ctl.mute_flag)break;
        if(sys_ctl.mp3_stop_flag)break;
#if BT_SPP_EQ_EN
        user_def_eq();
#else
        user_set_eq(sys_ctl.eq_num+1);
        show_eq();
#endif
        break;*/

    case T_KEY_USBSD:
        if(sys_ctl.mute_flag)break;
        if(is_other_device()) {
            task_music_auto_next_device();
        }
        break;
#endif        
#if 0
    case KU_BT:    
		if(sys_ctl.mute_flag)break;
		task_ctl.work_sta = TASK_BT;
	    
		break;

    case T_KEY_EQ: 
        if(sys_ctl.mute_flag)break;
        user_set_eq(sys_ctl.eq_num+1);
        show_eq();
        break;
#endif
#if BT_SPP_MSC_EN
    case Q_SPP_MSC_GET_LFN:
        spp_msc_get_lfn(t_msc.music_total);
        break;

    case Q_SPP_MSC_GET_TOTAL:
        spp_tx_msc_total(t_msc.music_total);
        break;

    case Q_SPP_MSC_GET_NUM:
        spp_tx_msc_num(t_msc.music_num);
        break;
#endif

    case QSYSTEM_1S:
#if MUSIC_LRC_EN
        if (t_msc.lrc_flag && t_msc.lrc_flag != LRC_FILENAME_TIME) {
            t_msc.lrc_flag++;
            if (t_msc.lrc_flag == LRC_FILENAME_TIME) {
                if (music_lrc_first_time() <= music_get_mtime()) {
                    t_msc.lrc_change = music_get_lrc(music_lrc_first_time());
                } else {
                    t_msc.lrc_flag = LRC_FILENAME_TIME - 1;
                }
            }
        }
#endif
#if BREAK_POINT_EN
        if(!t_msc.music_voice_flag)
        {
            music_get_point(&t_msc.break_point);
            task_music_break_save();
        }
#endif
        if((sys_ctl.music_play_flag)&&(!t_msc.pause)){
            sys_ctl.music_play_flag = 0;
#if NPCP215X
            AudioCodec_MUTE(UNMUTE);
#endif
        }
        deal_msg(msg);
        break;
               
        //500毫秒消息
#if 0         
        case QSYSTEM_500MS:
            if(sys_ctl.mute_flag)
            {
                if(sys_ctl.bonoff)show_err();       //show_err  暂作灭屏功能
                else  ledseg_disp(MENU_MUTE);
            }
            break;
#endif
    default:
        deal_msg(msg);
        break;
    }
}
#endif
