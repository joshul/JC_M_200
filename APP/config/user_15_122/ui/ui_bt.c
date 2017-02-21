/*****************************************************************************
* Module    : UI
* File      : ui_bt.c
* Author    : Hanny
* Function  : ���������û��Զ���UI��ƣ�����������ӳ������ʾ
*****************************************************************************/
#ifndef CONFIG_C

#pragma constseg="BT_RFC_TX_CONST"
IAR_CONST char at_spp0_ptr[] = "PREV"; //only the pointer form is workable to send vol into RfcommSendATCommand(), (why?) arthur
IAR_CONST char at_spp1_ptr[] = "NEXT";
#pragma constseg=default

extern u8 flag_dial;
extern u8 bt_user_feature;
void mp3res_play_btmode(u8 music_name);
bool send_hshfvol_hook(void);
void task_bt_deal_msg_10s(void);
void task_bt_deal_notice_msg(u8 msg);

void tasksb_bt_test_play(void);
void tasksb_bt_test_hsf(void);
void tasksb_bt_test_prev(void);
void tasksb_bt_test_next(void);
void tasksb_bt_test_voldown(void);
void tasksb_bt_test_volup(void);

#pragma location="TASK_BT_SEG"
void task_bt_hid_deal_msg(u8 msg)
{
    switch (msg) {
    case KU_PLAY:
        if(sys_ctl.mute_flag)break;
    //case KU_PLAY_POWER:
	//case KU_HSF:
        bt_hid_photo();
        break;

#if BT_HID_MANU
	case KL_NEXT:
        if(sys_ctl.mute_flag)break;
        //printf("kl\n");
		t_bt.hid_enable = 0;
        bt_hid_disconnect();
        if (bt_hid_wait_disconnect()) {
		    mp3res_play_btmode(RES_MP3_VOL_MAXMIN);//RES_MP3_PH_DISCONNECT
            //printf("hid nc\n");
        }
        break;
#endif

#if TASK_BTHID_EN
    case KL_PLAY:
        //�����Ͽ�����
        bt_disconnect();
        break;
#endif

    case Q_BT_CONN_DONE:                        //����������
    case Q_BT_DISCONN_DONE:                     //�����ѶϿ�
    case Q_BT_PLAY:                             //������ʼ����
    case Q_BT_STOP:                             //����ֹͣ����
    case Q_BT_INCOMING:                         //��������
    case Q_BT_CALL:                             //����ͨ��
    case Q_BT_CONN_LOST:                        //��������Զ����Ͽ�
    case Q_VOL_DOWN:                            //����������
    case Q_VOL_UP:                              //����������
        task_bt_deal_notice_msg(msg);
        break;

    case QSYSTEM_10S:
        task_bt_deal_msg_10s();
        break;

    case QSYSTEM_1S:
        deal_msg(msg);
        break;

    default:
        deal_msg(msg);
        break;
    }
}

//�û�������Ϣ����
#pragma location="TASK_BT_SEG"
void task_bt_deal_msg(u8 msg)
{
#if WARNING_VOL_MAXMIN
    if((sys_ctl.vol_maxmin_music_flag == 1) && (sys_ctl.vol_maxmin_music_play == 1)){
        sys_ctl.vol_maxmin_music_flag = 0;
        sys_ctl.vol_maxmin_music_play = 0;
        bt_play_continue();
        if(!sys_ctl.volume) {
            sys_mute();
        }
        flush_dispensable_msg();
        return;
    }
#endif

    switch (msg) {
        
    case KU_PLAY:
    case KU_PLAY_POWER:
        if(sys_ctl.mute_flag)break;
        //��������/��ͣ
        sys_ctl.bt_playing_flag = !sys_ctl.bt_playing_flag;
        bt_pause_play();
        break;
        
    case KL_PLAY: 
        if(sys_ctl.mute_flag)break;
        bt_disconnect();
        break;
#if 0        
    case KL_PLAY:
        if(sys_ctl.irkey){
            if(sys_ctl.mute_flag)break;
            bt_disconnect();
        }else{
            task_ctl.work_sta = TASK_POWEROFF;
        }        
        break;
#endif        
 /*   case T_KEY_STOP:
        if(sys_ctl.mute_flag)break;
        //�����Ͽ�����
        bt_disconnect();
        break;*/
        
    case KU_PREV:
    case KU_PREV_VOL_DOWN:
        if(sys_ctl.mute_flag)break;
        //������һ��
        bt_prev();
        break;

    case KU_NEXT:
    case KU_NEXT_VOL_UP:
        if(sys_ctl.mute_flag)break;
        //������һ��
        bt_next();
        break;
#if 0
    case KL_PREV:
    case KH_PREV:
        if(sys_ctl.irkey)break;    
        user_set_volume(user_volume_dec(sys_ctl.volume));
        send_vol_to_iphone_hook();
        show_volume();
        sys_unmute();
        if(!sys_ctl.volume){
        sys_mute();
        }
        break;
        
//    case K_VOL_DOWN1:
//    case KL_VOL_DOWN1:
//    case KH_VOL_DOWN1:
    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_VOL_DOWN:
        //������
        user_set_volume(user_volume_dec(sys_ctl.volume));
        send_vol_to_iphone_hook();
        show_volume();
        sys_unmute();
#if WARNING_VOL_MAXMIN
        if(!sys_ctl.volume){
            sys_unmute();
            dac_fade_out();
            bt_play_vol_music();
        }
#else
        if(!sys_ctl.volume){
            sys_mute();
        }
#endif
        break;

    case KL_NEXT:
    case KH_NEXT: 
        if(sys_ctl.irkey)break;
        user_set_volume(user_volume_inc(sys_ctl.volume));
        send_vol_to_iphone_hook();
        show_volume();
        sys_unmute();
        break;
//    case K_VOL_UP1:
//    case KL_VOL_UP1:
//    case KH_VOL_UP1:    
    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_VOL_UP:
        //������
        user_set_volume(user_volume_inc(sys_ctl.volume));
        send_vol_to_iphone_hook();
        show_volume();
        sys_unmute();
#if WARNING_VOL_MAXMIN
        if(VOLUME_MAX == sys_ctl.volume){
            bt_play_vol_music();
        }
#endif
        break;

#endif
    case Q_BT_CONN_DONE:                        //����������
    case Q_BT_DISCONN_DONE:                     //�����ѶϿ�
    case Q_BT_PLAY:                             //������ʼ����        
    case Q_BT_STOP:                             //����ֹͣ����        
    case Q_BT_INCOMING:                         //��������
    case Q_BT_CALL:                             //����ͨ��
    case Q_BT_CONN_LOST:                        //��������Զ����Ͽ�
    case Q_VOL_DOWN:                            //����������
    case Q_VOL_UP:                              //����������
       if(sys_ctl.mute_flag)break;
        task_bt_deal_notice_msg(msg);
        break;
#if 0
    case KU_POWER:		
		task_ctl.work_sta = TASK_POWEROFF;
		break;
        
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
#if 0   
    case KU_BT:    
        if(sys_ctl.mute_flag)break;
        task_ctl.work_sta = TASK_BT;
        break;
        
   case KU_AUX:
        if(sys_ctl.mute_flag)break;
        task_ctl.work_sta = TASK_AUX;
        break;
#endif        
    case T_KEY_EQ: 
        user_set_eq(sys_ctl.eq_num+1);
        show_eq();
        break;  
#endif        

     //500������Ϣ
    case QSYSTEM_500MS:
        if((!sys_ctl.mute_flag)&&(ledseg_ocx.status==LEDSEG_OCX_NULL))
        {
            if(sys_ctl.bonoff&&(sys_ctl.bt_sta < BT_STA_CONNECTED))ledseg_disp(MENU_NULL);
            else                                  ledseg_disp(MENU_BT);
        }
#if 0        
        if((sys_ctl.bt_sta < BT_STA_PLAYING)&&(!sys_ctl.voice_play_flag)){
            if(sys_ctl.bt_Mute_cnt < 2)sys_ctl.bt_Mute_cnt++;
            if(sys_ctl.bt_Mute_cnt == 1){
                sys_ctl.bt_Mute_cnt = 2;
#if NPCP215X
                AudioCodec_MUTE(MUTE);
#endif                
            }
        }
        else if(!sys_ctl.mute_flag){
            if(sys_ctl.bt_Mute_cnt > 1){
#if NPCP215X
                AudioCodec_MUTE(UNMUTE);
#endif
            }
            sys_ctl.bt_Mute_cnt = 0;
        }
#endif        
#if 0        
        if((sys_ctl.bt_sta >=BT_STA_CONNECTED)&&(sys_ctl.bt_sta_change_flag))
        {
            sys_ctl.bt_sta_change_flag = 0;                              
            if(sys_ctl.bt_play_flag){
#if NPCP215X
                AudioCodec_MUTE(UNMUTE);
#endif
            }
            else {
#if NPCP215X
                AudioCodec_MUTE(MUTE);
#endif
            }               
        }
#endif   
#if 0        
        else if(sys_ctl.mute_flag)
        {
            if(sys_ctl.bonoff)show_err();       //show_err  ������������
            else    ledseg_disp(MENU_MUTE);
        }
#endif        
        break;

    case QSYSTEM_100MS:
#if 1        
        if((sys_ctl.bt_sta < BT_STA_PLAYING)&&(!sys_ctl.voice_play_flag)){
            if(sys_ctl.bt_Mute_cnt < 2)sys_ctl.bt_Mute_cnt++;
            if(sys_ctl.bt_Mute_cnt == 1){
                sys_ctl.bt_mute_flag = 2;
#if NPCP215X
                AudioCodec_MUTE(MUTE);
#endif                
            }
        }
        else if(!sys_ctl.mute_flag){
            if(sys_ctl.bt_Mute_cnt > 3){
#if NPCP215X
                AudioCodec_MUTE(UNMUTE);
#endif  
                sys_ctl.bt_Mute_cnt = 0;
                sys_ctl.bt_mute_flag = 0;
            }
            else if(sys_ctl.bt_mute_flag){
                if(sys_ctl.bt_Mute_cnt < 5)sys_ctl.bt_Mute_cnt++;
            }
            
        }
#endif 
        break;

    default:
        deal_msg(msg);
        break;
    }
}

void task_bt_state(void)
{
    if(t_bt.disp_bt_sta != sys_ctl.bt_sta)
    {
        t_bt.disp_bt_sta = sys_ctl.bt_sta;
        t_bt.disp_dev_bat = 0xff;
        //WARNING_MSG("State: %02x\n", BT_STATE);
        switch (t_bt.disp_bt_sta)
        {
        case BT_STA_IDLE:
            sys_ctl.bt_playing_flag = 0;
            led_bt_idle();
            //SWUart_TX("\nNC\n");	
            break;
        case BT_STA_CONNECTED:
            sys_ctl.bt_playing_flag = 0;
            led_bt_connect();
            //SWUart_TX("\nCO\n");
            break;
        case BT_STA_PLAYING:
            sys_ctl.bt_playing_flag = 1;
            led_bt_play();
            if(!dac_e.digit){
                btdec_play(); //���µ���
            }
            //SWUart_TX("\nPL\n");	
            break;
        case BT_STA_OUTGOING:	
            sys_ctl.bt_playing_flag = 1;
            led_bt_play();
            //SWUart_TX("\nOT\n");
            break;
        case BT_STA_PHONE:
            sys_ctl.bt_playing_flag = 1;
            led_bt_play();
            //SWUart_TX("\nIN\n");
            break;
        }
    }
}

//�û���ʾ����
#if (IS_LEDSEG_5C7S_DISPLAY || IS_LEDSEG_7PIN_DISPLAY)
#pragma location="TASK_BT_SEG"
void task_bt_display(void)
{
    ledseg_ocx_event();
    task_bt_state();

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
        if((!sys_ctl.mute_flag)&&(sys_ctl.bt_sta >= BT_STA_CONNECTED))ledseg_disp(MENU_BT);
        break;
    default:
        ledseg_ocx_display();
        break;
    }
}
#else
#pragma location="TASK_BT_SEG"
void task_bt_display(void)
{
	 task_bt_state();
}
#endif

/*****************************************************************************
 * Module    : ���� - ���� �µ�UI����
 *****************************************************************************/
#pragma location="TASK_BT_RING_SEG"
void task_bt_ring_deal_msg(u8 msg)
{
    switch (msg) {
    case KU_HSF:
        //�����绰
        bt_pickup();
        break;

    case KL_HSF:
        //�Ҷϵ绰
        bt_hangup();
        break;
    }
}

/*****************************************************************************
 * Module    : ���� - ͨ�� �µ�UI����
 *****************************************************************************/
#pragma location="TASK_BT_CALL_SEG"
void task_bt_call_deal_msg(u8 msg)
{
    u8 call_status;
    switch (msg) {

    case KU_NEXT:   //1.�ֻ�����������֮���л�����
    case KL_NEXT:
        bt_sco_switch();  //���ߵ��� bt_sco_disconnect/bt_sco_connectҲ����ʵ�ִ˹���
        break;

    case KU_HSF:
        call_status = bt_get_call_status();
        //printf("c_sta: %d\n", call_status);
        if(call_status == 2) {
            bt_pickup();                //������2·ͨ��
        } else {
            bt_hangup();                //�Ҷϵ绰
        }
        break;

    case KL_HSF:
        call_status = bt_get_call_status();
        //printf("c_sta: %d\n", call_status);
        if(call_status == 2) {
            bt_hangup();                //�ܽӵ�2·ͨ��
        } else if(call_status == 4) {
            bt_callswap();              //�л���·ͨ��
        }
        break;

    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_VOL_DOWN:
        //������
        user_set_hshfvol(user_volume_dec(sys_ctl.hshf_vol));
        send_hshfvol_hook();
        show_volume();
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_VOL_UP:
        //������
        user_set_hshfvol(user_volume_inc(sys_ctl.hshf_vol));
        send_hshfvol_hook();
        show_volume();
        break;

        //�ֻ��˵�ϵͳ����
    case Q_VOL_DOWN:
        user_set_volume(user_volume_dec(sys_ctl.volume));
        show_volume();
        break;
    case Q_VOL_UP:
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
        break;

#if !BT_CALL_PRIVATE
        //�ֻ��˵�ͨ������
    case Q_HSHF_VOL_CHANGE:
        show_volume();
        break;
#endif
    }
}

/*****************************************************************************
 * Module    : ���� - ���Ժ� �µ�UI����
 *****************************************************************************/
#pragma location="TASK_BT_CALL_SEG"
void task_bt_test_deal_msg(u8 msg)
{
#if TASK_BTHID_EN
    if (task_hid_flag) {
        if ((msg != KU_MODE) && (msg != KU_PLAY) && (msg != KL_PLAY_POWER)) {
            return;
        }
    }
#endif
    switch (msg) {

    case KU_MODE:  //���Ժ�����ģʽ
        bt_test_flag = true;
        if(sys_ctl.bt_sta >= BT_STA_INCOMING){
          adc_stop();  //�ر�ADC,��ֹХ��
          pcm_stop();
          adc_exit();
          Test_Mode_Flag =0;
          bt_init();   //����ĳЩ����û�壬����λ
        }
        task_ctl.work_sta = TASK_EXIT;
        break;

    case KU_PLAY:
    case KU_PLAY_POWER:
        //���Բ��ż�
        tasksb_bt_test_play();
        break;

    case KU_HSF:
        //����ͨ����
        tasksb_bt_test_hsf();
        break;

    case KU_PREV:
    case KU_PREV_VOL_DOWN:
        //������һ��
        tasksb_bt_test_prev();
        break;

    case KU_NEXT:
    case KU_NEXT_VOL_UP:
        //������һ��
        tasksb_bt_test_next();
        break;

    case K_VOL_DOWN:
        //����������
        tasksb_bt_test_voldown();
        break;

    case K_VOL_UP:
        //����������
        tasksb_bt_test_volup();
        break;

#if(SOFT_POWER_ON_OFF)
    case KL_PLAY_POWER:
        mp3res_play_btmode(RES_MP3_POWER_OFF);
        power_down_enter();
        break;
#endif
    }
}

/*****************************************************************************
 * Module    : �����������ܵ�UI����
 *****************************************************************************/
#pragma location="TASK_BT_LOST_SEG"
void task_bt_lost_deal_msg(u8 msg)
{
    switch (msg) {

    case KU_HSF://�̰�HSF��MODE�����˳�����ģʽ
    case KU_MODE:
        task_ctl.work_sub = TASK_BT_NULL;
        break;

    case QSYSTEM_1S:      //�������ܣ�1Sһ����ʾ��
        //printf("bt_lost 1S warning\n");
        mp3res_play_btmode(RES_MP3_BT_DISCONNECT);
        break;

#if(SOFT_POWER_ON_OFF)
    case KL_PLAY_POWER:
        mp3res_play_btmode(RES_MP3_POWER_OFF);
        power_down_enter();
        break;
#endif
    }
}
#endif
