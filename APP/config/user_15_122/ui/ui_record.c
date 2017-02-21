/*****************************************************************************
* Module    : UI
* File      : ui_record.c
* Author    : Hanny
* Function  : ¼�������û��Զ���UI��ƣ�����������ӳ������ʾ
*****************************************************************************/
#ifndef CONFIG_C
#include "task_CS8416.h"

//¼��������ʾ����
#if (IS_LEDSEG_5C7S_DISPLAY || IS_LEDSEG_7PIN_DISPLAY)
#pragma location="TASK_MIC_SEG"
void task_record_display(void)
{
    
    ledseg_ocx_event();
    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        if(!sys_ctl.mute_flag){
            if (task_ctl.work_sta == TASK_MIC) 
            {
                ledseg_disp(MENU_MIC);
            } 
            else if (task_ctl.work_sta == TASK_AUX)
            {
                ledseg_disp(MENU_AUX);
            } 
            else if (task_ctl.work_sta == TASK_COAX)
            {
                ledseg_disp(MENU_AUX_COAX);
            }  
            else if (task_ctl.work_sta == TASK_OPTI)
            {
                ledseg_disp(MENU_AUX_OPTI);
            } 
#if 0            
            else if (task_ctl.work_sta == TASK_AUX2){
                ledseg_disp(MENU_AUX_LIN);
            } else if (task_ctl.work_sta == TASK_AUX3){
                ledseg_disp(MENU_AUX_HD);
            }
#endif            
        }
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}

//¼����������ʾ����
#pragma location="TASK_RECORD_SEG"
void tasksub_record_display(void)
{
    //ledseg_ocx_event();
    if (ledseg_ocx.show_time && ocx_show_cnt >= ledseg_ocx.show_time) {
        ledseg_ocx.status = LEDSEG_OCX_NULL;
    }
    if (ledseg_ocx.status != ledseg_ocx.disp_status) {
        ledseg_ocx.disp_status = ledseg_ocx.status;
    }

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        ledseg_disp(MENU_RECORDING);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}
#else
#pragma location="TASK_MIC_SEG"
void task_record_display(void)
{
}
#pragma location="TASK_RECORD_SEG"
void tasksub_record_display(void)
{
}
#endif

//¼��������Ϣ����
#pragma location="TASK_MIC_SEG"
void task_record_deal_msg(u8 msg)
{
    
#if WARNING_VOL_MAXMIN
    if((sys_ctl.vol_maxmin_music_flag == 1) && (sys_ctl.vol_maxmin_music_play == 1) \
       && (task_ctl.work_sta == TASK_AUX)) //�ָ���ǰ������
    {
#if IIS_EN
        aux_eq_continue();
#else
        aux_continue();
#endif
        sys_ctl.vol_maxmin_music_flag = 0;
        sys_ctl.vol_maxmin_music_play = 0;
        flush_msg();
        return;
    }
#endif
    switch(msg) {
        //���Ż���ͣ
    
        //��ʼ¼��
    case KU_HSF:
        if(sys_ctl.mute_flag)break;
        if (!t_rec.aux_record_flag && (task_ctl.work_sta != TASK_MIC)) {
            break;
        }
        if (device_is_online(DEVICE_SDMMC) || device_is_online(DEVICE_UDISK)) {
            tasksub_recording();
            if(t_rec.play_flag) {
                tasksub_recplay();//���Ÿո�¼�µ��ļ�
#if IIS_EN && AUX_RECORD_EN
                if (task_ctl.work_sta == TASK_MIC)
#endif
                task_record_init();
            }
        } else {
            show_no_device();
        }
        break;
#if 0        
    //���������ݼ�
    case KL_NEXT:
    case KH_NEXT:
        if(sys_ctl.irkey)break;
        //aux_vol_down();
        
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
        sys_unmute();
        
#if WARNING_VOL_MAXMIN
        if(sys_ctl.volume == VOLUME_MAX){
            music_play_vol_music();
        }
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
        //aux_vol_up();
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
        sys_unmute();
        
#if WARNING_VOL_MAXMIN
        if(sys_ctl.volume == VOLUME_MAX){
            music_play_vol_music();
        }
#endif
        break;
        
    //���������ݼ�
    case KL_PREV:
    case KH_PREV:
        if(sys_ctl.irkey)break;
         //aux_vol_down();
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
        break;
//    case K_VOL_DOWN1:
//    case KL_VOL_DOWN1:
//    case KH_VOL_DOWN1:
        
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KL_PREV_VOL_DOWN:
    case KH_PREV_VOL_DOWN:
        //aux_vol_down();  
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
        break;

     //�����ػ�
    case KL_PLAY:
         if(sys_ctl.irkey)break;
         task_ctl.work_sta = TASK_POWEROFF;
         break;   

    case KU_POWER:
//		if(sys_ctl.mute_flag)break;
		task_ctl.work_sta = TASK_POWEROFF;
		break;
#endif        
#if 0   
    case T_KEY_USBSD:
        if(sys_ctl.mute_flag)break;
		if(device_is_online(DEVICE_UDISK))
		{
			fs_sel(DEVICE_UDISK);
			task_ctl.work_sta = TASK_MUSIC;
		}
		else if(device_is_online(DEVICE_SDMMC))
		{
			fs_sel(DEVICE_SDMMC);
			task_ctl.work_sta = TASK_MUSIC;
		}
		break;
#endif        
#if 0        
    case KU_BT:           
		if(sys_ctl.mute_flag)break;
		task_ctl.work_sta = TASK_BT;	    
		break;

    case KU_AUX:
        if(sys_ctl.mute_flag)break;
        if(task_ctl.work_sta == TASK_AUX3){
            if (device_is_online(DEVICE_SDMMC) || device_is_online(DEVICE_UDISK)){
                task_ctl.work_sta = TASK_MUSIC;               
            }
            else{
                task_ctl.work_sta = TASK_AUX;
            }
            
        }
        else {
            task_ctl.work_sta = TASK_EXIT;
        }
        break;
#endif 

    case QSYSTEM_1S:
        if((task_ctl.work_sta ==TASK_COAX)||(task_ctl.work_sta ==TASK_OPTI))
        { 
            sys_ctl.input_sta = CS8416_read_byte(0x0B);
            if(sys_ctl.input_sta == 0x40){
                if(sys_ctl.input_sta_err_flag){
                    sys_ctl.input_sta_err_flag = 0;
#if NPCP215X
                    AudioCodec_MUTE(UNMUTE);
#endif                     
                }                                       
            }
            else {              
                if(!sys_ctl.input_sta_err_flag){
                    sys_ctl.input_sta_err_flag = 1;
#if NPCP215X
                    AudioCodec_MUTE(MUTE);
#endif
                    show_err();
                }                 
            }
        }
        break;
        
#if 0        
	//500������Ϣ
	case QSYSTEM_500MS:
		if(sys_ctl.mute_flag)
		{
			if(sys_ctl.bonoff)show_err();		//show_err  ������������
			else  ledseg_disp(MENU_MUTE);
		}
		break;
#endif     
        //���ù�����Ϣ����
    default:
        deal_msg(msg);
        break;
    }
}

//����¼����������Ϣ����
#pragma location="TASK_RECORD_SEG"
void tasksub_record_deal_msg(u8 msg)
{
    switch(msg) {
        //��ͣ¼��
    case KU_PLAY:
    case KU_PLAY_POWER:
        task_record_pause();
        break;

        //ֹͣ¼��
    case KU_MODE:
    case KU_HSF:
        t_rec.play_flag = 1;
        task_record_stop();
        break;

    case QSYSTEM_1S:
        t_rec.rec_time++;                           //¼����ʱ
        if (t_rec.sync_cnt++ < REC_SYNC_CNT) {
            break;
        }
        t_rec.sync_cnt = 0;                     //ͬ��
        if (!sync_file()) {
            //printf("file sync Fail!\n");
            adc_stop();
        }
        break;
        //���ù�����Ϣ����
    default:
        break;
    }
}

//¼������������Ϣ����
#pragma location="TASK_MUSIC_SEG"
void tasksub_recplay_deal_msg(u8 msg)
{
    switch(msg) {
    case KU_PLAY:
    case KU_PLAY_POWER:
    case KU_MODE:
        music_stop();
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_VOL_UP:
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
#if (HARD_IR_EN || SOFT_IR_EN || SOFT_IR_PORT_INT_EN)
        if(sys_ctl.volume) {
            sys_unmute();
        }
#endif
        break;

    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_VOL_DOWN:
        user_set_volume(user_volume_dec(sys_ctl.volume));
        show_volume();
#if (HARD_IR_EN || SOFT_IR_EN || SOFT_IR_PORT_INT_EN)
        if(sys_ctl.volume) {
            sys_unmute();
        }
#endif
        break;
    }
}
#endif
