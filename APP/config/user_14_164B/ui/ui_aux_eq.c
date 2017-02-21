/*****************************************************************************
* Module    : UI
* File      : ui_aux_eq.c
* Author    : Hanny
* Function  : ��Դ���������û��Զ���UI��ƣ�����������ӳ������ʾ
*****************************************************************************/
#ifndef CONFIG_C

//������������ʾ����
#if IS_LEDSEG_7PIN_DISPLAY
#pragma location="TASK_AUX_SEG"
void task_aux_eq_display(void)
{
    ledseg_ocx_event();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        ledseg_disp(MENU_AUX_EQ);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}
#else
#pragma location="TASK_AUX_SEG"
void task_aux_eq_display(void)
{
}
#endif

//������Ϣ����
#pragma location="TASK_AUX_SEG"
void task_aux_eq_deal_msg(u8 msg)
{
#if WARNING_VOL_MAXMIN
    if((sys_ctl.vol_maxmin_music_flag == 1) && (sys_ctl.vol_maxmin_music_play == 1)){ //�ָ���ǰ������
        aux_eq_continue();
        sys_ctl.vol_maxmin_music_flag = 0;
        sys_ctl.vol_maxmin_music_play = 0;
        flush_msg();
        return;
    }
#endif

    switch(msg) {
    case KU_PLAY:
    case KU_PLAY_POWER:
        //����MUTE
        if (t_auxeq.mute && sys_ctl.volume) {
            t_auxeq.mute = 0;
            sys_ctl.sleep_cnt = 0xffff;
            t_auxeq.pos = 0;
            adc_start();
            sys_unmute();
        } else {
            t_auxeq.mute = 1;
            sys_ctl.sleep_cnt = STANDBY_TIME;
            adc_stop();
            sys_mute();
        }
        break;

        //�����Ӽ�
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
	case KH_PREV_VOL_DOWN:
        user_set_volume(user_volume_dec(sys_ctl.volume));
        show_volume();
#if WARNING_VOL_MAXMIN
        if(!sys_ctl.volume){
            auxeq_play_vol_music();
            t_auxeq.mute = 1;
            break;
        }
#endif
        if (t_auxeq.mute && sys_ctl.volume) {
            t_auxeq.mute = 0;
            sys_ctl.sleep_cnt = 0xffff;
            t_auxeq.pos = 0;
            adc_start();
            sys_unmute();
        }
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
	case KH_NEXT_VOL_UP:
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
#if WARNING_VOL_MAXMIN
        if(sys_ctl.volume == VOLUME_MAX){
            auxeq_play_vol_music();
            break;
        }
#endif
        if (t_auxeq.mute) {
            t_auxeq.mute = 0;
            sys_ctl.sleep_cnt = 0xffff;
            t_auxeq.pos = 0;
            adc_start();
            sys_unmute();
        }
        break;

    case KU_HSF:
        user_set_eq(sys_ctl.eq_num+1);
        show_eq();
        break;

    default:
        deal_msg(msg);
        break;
    }
}
#endif
