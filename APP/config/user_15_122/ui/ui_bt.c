/*****************************************************************************
* Module    : UI
* File      : ui_bt.c
* Author    : Hanny
* Function  : 蓝牙任务用户自定义UI设计，处理按键功能映射与显示
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
        //主动断开蓝牙
        bt_disconnect();
        break;
#endif

    case Q_BT_CONN_DONE:                        //蓝牙已连接
    case Q_BT_DISCONN_DONE:                     //蓝牙已断开
    case Q_BT_PLAY:                             //蓝牙开始播放
    case Q_BT_STOP:                             //蓝牙停止播放
    case Q_BT_INCOMING:                         //蓝牙来电
    case Q_BT_CALL:                             //蓝牙通话
    case Q_BT_CONN_LOST:                        //蓝牙连接远距离断开
    case Q_VOL_DOWN:                            //蓝牙音量减
    case Q_VOL_UP:                              //蓝牙音量加
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

//用户按键消息处理
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
        //蓝牙播放/暂停
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
        //主动断开蓝牙
        bt_disconnect();
        break;*/
        
    case KU_PREV:
    case KU_PREV_VOL_DOWN:
        if(sys_ctl.mute_flag)break;
        //播放上一曲
        bt_prev();
        break;

    case KU_NEXT:
    case KU_NEXT_VOL_UP:
        if(sys_ctl.mute_flag)break;
        //播放下一曲
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
        //音量减
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
        //音量加
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
    case Q_BT_CONN_DONE:                        //蓝牙已连接
    case Q_BT_DISCONN_DONE:                     //蓝牙已断开
    case Q_BT_PLAY:                             //蓝牙开始播放        
    case Q_BT_STOP:                             //蓝牙停止播放        
    case Q_BT_INCOMING:                         //蓝牙来电
    case Q_BT_CALL:                             //蓝牙通话
    case Q_BT_CONN_LOST:                        //蓝牙连接远距离断开
    case Q_VOL_DOWN:                            //蓝牙音量减
    case Q_VOL_UP:                              //蓝牙音量加
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

     //500毫秒消息
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
            if(sys_ctl.bonoff)show_err();       //show_err  暂作灭屏功能
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
                btdec_play(); //重新淡入
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

//用户显示处理
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
 * Module    : 蓝牙 - 响铃 下的UI控制
 *****************************************************************************/
#pragma location="TASK_BT_RING_SEG"
void task_bt_ring_deal_msg(u8 msg)
{
    switch (msg) {
    case KU_HSF:
        //接听电话
        bt_pickup();
        break;

    case KL_HSF:
        //挂断电话
        bt_hangup();
        break;
    }
}

/*****************************************************************************
 * Module    : 蓝牙 - 通话 下的UI控制
 *****************************************************************************/
#pragma location="TASK_BT_CALL_SEG"
void task_bt_call_deal_msg(u8 msg)
{
    u8 call_status;
    switch (msg) {

    case KU_NEXT:   //1.手机接听跟蓝牙之间切换功能
    case KL_NEXT:
        bt_sco_switch();  //或者调用 bt_sco_disconnect/bt_sco_connect也可以实现此功能
        break;

    case KU_HSF:
        call_status = bt_get_call_status();
        //printf("c_sta: %d\n", call_status);
        if(call_status == 2) {
            bt_pickup();                //接听第2路通话
        } else {
            bt_hangup();                //挂断电话
        }
        break;

    case KL_HSF:
        call_status = bt_get_call_status();
        //printf("c_sta: %d\n", call_status);
        if(call_status == 2) {
            bt_hangup();                //拒接第2路通话
        } else if(call_status == 4) {
            bt_callswap();              //切换两路通话
        }
        break;

    case K_VOL_DOWN:
    case KL_VOL_DOWN:
    case KH_VOL_DOWN:
    case KH_PREV_VOL_DOWN:
        //音量减
        user_set_hshfvol(user_volume_dec(sys_ctl.hshf_vol));
        send_hshfvol_hook();
        show_volume();
        break;

    case K_VOL_UP:
    case KL_VOL_UP:
    case KH_VOL_UP:
    case KH_NEXT_VOL_UP:
        //音量加
        user_set_hshfvol(user_volume_inc(sys_ctl.hshf_vol));
        send_hshfvol_hook();
        show_volume();
        break;

        //手机端调系统音量
    case Q_VOL_DOWN:
        user_set_volume(user_volume_dec(sys_ctl.volume));
        show_volume();
        break;
    case Q_VOL_UP:
        user_set_volume(user_volume_inc(sys_ctl.volume));
        show_volume();
        break;

#if !BT_CALL_PRIVATE
        //手机端调通话音量
    case Q_HSHF_VOL_CHANGE:
        show_volume();
        break;
#endif
    }
}

/*****************************************************************************
 * Module    : 蓝牙 - 测试盒 下的UI控制
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

    case KU_MODE:  //测试盒下切模式
        bt_test_flag = true;
        if(sys_ctl.bt_sta >= BT_STA_INCOMING){
          adc_stop();  //关闭ADC,防止啸叫
          pcm_stop();
          adc_exit();
          Test_Mode_Flag =0;
          bt_init();   //由于某些变量没清，引起复位
        }
        task_ctl.work_sta = TASK_EXIT;
        break;

    case KU_PLAY:
    case KU_PLAY_POWER:
        //测试播放键
        tasksb_bt_test_play();
        break;

    case KU_HSF:
        //测试通话键
        tasksb_bt_test_hsf();
        break;

    case KU_PREV:
    case KU_PREV_VOL_DOWN:
        //播放上一曲
        tasksb_bt_test_prev();
        break;

    case KU_NEXT:
    case KU_NEXT_VOL_UP:
        //播放下一曲
        tasksb_bt_test_next();
        break;

    case K_VOL_DOWN:
        //测试音量减
        tasksb_bt_test_voldown();
        break;

    case K_VOL_UP:
        //测试音量加
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
 * Module    : 蓝牙防丢功能的UI控制
 *****************************************************************************/
#pragma location="TASK_BT_LOST_SEG"
void task_bt_lost_deal_msg(u8 msg)
{
    switch (msg) {

    case KU_HSF://短按HSF和MODE按键退出防丢模式
    case KU_MODE:
        task_ctl.work_sub = TASK_BT_NULL;
        break;

    case QSYSTEM_1S:      //防丢功能，1S一次提示音
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
