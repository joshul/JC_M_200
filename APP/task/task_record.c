/*****************************************************************************
 * Module    : Task
 * File      : task_record.c
 * Author    : Hanny
 * Function  : 录音任务流程
 *****************************************************************************/
#include "include.h"
#include "task_NPCP215X.h" 

#if (TASK_MIC_EN || TASK_AUX_EN)
#include "task.h"
#include "task_record.h"
#include "task_music.h"
#include "task_aux_eq.h"

void task_aux_eq_enter(void);
void task_aux_audio_ch(void);

void task_aux_eq_exit(void);
void aux_eq_continue(void);
void mp3res_play_auxeqmode(u8 music_name);
void auxeq_play_vol_music(void);

IAR_XDATA_A u8 wav_en_out_buf[WAV_OUT_BUF_SIZE] @ "WAV_ENCODE_BUF";
IAR_XDATA_A u8 wav_en_header_buf[512]           @ "WAV_EN_HEADER_BUF";
IAR_XDATA_A u8 rec_write_buf[512]               @ "WAV_EN_HEADER_BUF";
IAR_XDATA_A type_task_rec t_rec                 @ "RECORD_XDATA";

#pragma constseg="TASK_MIC_CONST"
IAR_CONST char str_show_taskmic[] = "task_mic\n";
IAR_CONST char str_show_taskaux[] = "task_aux\n";
#pragma constseg=default

#define TASK_SUB
#include SET_USER_PATH(CFG_UI_RECORD_DIR, /ui/ui_record.c)
#include "sub/tasksub_recording.c"          //录音子任务
#include "sub/tasksub_recplay.c"            //录音子任务

#pragma location="TASK_MIC_SEG"
void mp3res_play_micmode(u8 music_name)
{
    dac_enable();
    mp3_res_play_wait(music_name);
    dac_disable();
    adc_init(SEL_RECMIC);  //选择MIC录音
#if (REC_TYPE == REC_WAV_ADPCM || REC_TYPE == REC_WAV_PCM)
    wav_load_code(0x60);
#else
    mp3en_load_code(0x80);
#endif
}

#pragma location="TASK_MIC_SEG"
void aux_continue(void)
{
    dac_disable();
    t_rec.aux_mute = 0;
    task_record_init();
    if (!sys_ctl.volume) {
        amux_stop();
        sys_mute();
    }
}

#pragma location="TASK_MIC_SEG"
void mp3res_play_auxmode(u8 music_name)
{
    amux_exit();
    dac_enable();
    mp3_res_play_wait(music_name);
    aux_continue();
}

#pragma location="TASK_MIC_SEG"
void aux_play_vol_music(void)
{
    if(!sys_ctl.vol_maxmin_music_play){
        sys_ctl.vol_maxmin_music_play = 1;
        amux_exit();
        sys_unmute();
        dac_enable();
    }
    mp3_res_play_wait(RES_MP3_VOL_MAXMIN);
    flush_msg();
}

#pragma location="TASK_MIC_SEG"
void task_aux_audio_ch(void)
{
    if(task_ctl.work_sta != TASK_MIC){        
        if (task_ctl.work_sta == TASK_AUX){ 
           //AUX_LINEIN();
           ledseg_disp(MENU_AUX);
        } else if (task_ctl.work_sta == TASK_OPTI){
           //AUX_OPT();            
        } 
    }
#if 0
    if(task_ctl.work_sta != TASK_MIC){        
        if (task_ctl.work_sta == TASK_AUX){ 
           AUX_LINEIN();
           ledseg_disp(MENU_AUX);
        } else if (task_ctl.work_sta == TASK_AUX1){
           AUX_OPT();            
        } else if (task_ctl.work_sta == TASK_AUX2){
           AUX_LIN();    
        } else if (task_ctl.work_sta == TASK_AUX3){
           AUX_HD();    
        } 
    } 
#endif    
}

//AUX控制MUTE
#pragma location="TASK_MIC_SEG"
void aux_mute_control(void)
{
#if IIS_EN
    if (t_auxeq.mute && sys_ctl.volume) {
        t_auxeq.mute = 0;
        sys_ctl.sleep_cnt = 0xffff;
        sys_unmute();
    } else {
        t_auxeq.mute = 1;
        sys_ctl.sleep_cnt = STANDBY_TIME;
        sys_mute();
    }
#else
    if (t_rec.aux_mute) {
        t_rec.aux_mute = 0;
        sys_ctl.sleep_cnt = 0xffff;
        sys_unmute();
        amux_play();
        led_aux_play();
    } else {
        t_rec.aux_mute = 1;
        sys_ctl.sleep_cnt = STANDBY_TIME;
        amux_stop();
        sys_mute();
        led_aux_pause();
    }
#endif
}

//AUX减小音量
#pragma location="TASK_MIC_SEG"
void aux_vol_down(void)
{
    if (task_ctl.work_sta == TASK_MIC) {
        return;
    }
    user_set_volume(user_volume_dec(sys_ctl.volume));
    show_volume();

    
#if WARNING_VOL_MAXMIN
    if(!sys_ctl.volume){
#if IIS_EN
        auxeq_play_vol_music();
#else
        aux_play_vol_music();
        t_rec.aux_mute = 1;
#endif
        return;
    }
#endif


#if IIS_EN
 
    if (t_auxeq.mute && sys_ctl.volume){
        t_auxeq.mute = 0;
        sys_ctl.sleep_cnt = 0xffff;
        sys_unmute();
    }
#else
    if(sys_ctl.volume){
        t_rec.aux_mute = 0;
        sys_ctl.sleep_cnt = 0xffff;
        sys_unmute();
        amux_play();
        led_aux_play();
    }
#endif
}

//AUX增加音量
#pragma location="TASK_MIC_SEG"
void aux_vol_up(void)
{
    if (task_ctl.work_sta == TASK_MIC) {
        return;
    }
    user_set_volume(user_volume_inc(sys_ctl.volume));
    show_volume();
#if WARNING_VOL_MAXMIN
    if(sys_ctl.volume == VOLUME_MAX){
#if IIS_EN
        auxeq_play_vol_music();
#else
        aux_play_vol_music();
#endif
        return;
    }
#endif
#if IIS_EN
    if (t_auxeq.mute) {
        t_auxeq.mute = 0;
        sys_ctl.sleep_cnt = 0xffff;
        sys_unmute();
    }
#else
    t_rec.aux_mute = 0;
    sys_ctl.sleep_cnt = 0xffff;
    sys_unmute();
    amux_play();
#endif
    led_aux_play();
}

//录音控制器初始化
#pragma location="TASK_MIC_SEG"
void task_record_init(void)
{
    //关闭预分配
    fat_fs.limit_clust = 0;
    memset(&t_rec,0,sizeof(t_rec));
    t_rec.rec_sta = REC_STA_STOP;
    //录音控制器初始化
    if (task_ctl.work_sta == TASK_MIC) {
        adc_ch = 1;
        adc_init(SEL_RECMIC);
    } else {
#if AUX_RECORD_EN
        t_rec.aux_record_flag = 1;
#endif
        sys_ctl.sleep_cnt = 0xffff;
        adc_ch = ((AUX_CHANNEL_SEL & 0x80) >> 7) + 1;
        
        if(task_ctl.work_sta == TASK_AUX)
        {
            amux_init(AUX_CHANNEL_SEL);
        }
        else if((task_ctl.work_sta == TASK_COAX)||(task_ctl.work_sta == TASK_OPTI))
        {
            amux_init(FM_CHANNEL_SEL);
        }                
        adc_init(SEL_RECAUX);
#if MIC_DIRECT_OUT_EN
        mic_direct_out_enter(MIC_CHANNEL_SEL,11);
        dac_fade_in();
#endif
        amux_play();
        led_aux_play();
    }
#if (REC_TYPE == REC_WAV_ADPCM || REC_TYPE == REC_WAV_PCM)
    wav_load_code(0x60);
#else
    mp3en_load_code(0x80);
#endif
}

//录音任务初始化
#pragma location="TASK_MIC_SEG"
void task_record_enter(void)
{
    //sys_ctl.input_sta_err_flag = 0;
#if IIS_EN
    if(task_ctl.work_sta == TASK_AUX||task_ctl.work_sta == TASK_OPTI||task_ctl.work_sta == TASK_COAX) 
    {
        if (task_ctl.work_sta == TASK_AUX)
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
        task_aux_eq_enter();
        //task_ctl.work_sta = TASK_AUX;       
        t_rec.rec_sta = REC_STA_STOP;
#if AUX_RECORD_EN
        t_rec.aux_record_flag = 1;
#endif
        ocx_clear();
        if(task_ctl.work_sta == TASK_AUX){
#if NPCP215X
            //Audio_Slow_Volume(UNMUTE);
            AudioCodec_MUTE(UNMUTE);
#endif 
        }      
        else if((task_ctl.work_sta == TASK_COAX)||(task_ctl.work_sta == TASK_OPTI)){
            sys_ctl.input_sta = CS8416_read_byte(0x0B);
            if(sys_ctl.input_sta == 0x40){
                sys_ctl.input_sta_err_flag = 0;
#if NPCP215X
                AudioCodec_MUTE(UNMUTE);
#endif                
            }
            else {                
                sys_ctl.input_sta_err_flag = 1;
                show_err();
            }
        }

#if IIS_TYPE == IIS_TAS5711
        TAS5711_AMP_Mute_Set(FALSE);
#endif
        sys_ctl.mode_switch_flag = 0;
        //flush_msg();//清消息       
        return;
    }
#endif
    led_mode_off();
    led_idle();
    set_sys_clk(SYS_48M);
    draw_wait();
    device_change();
    if (task_ctl.work_sta == TASK_MIC) {
        if(!record_mount_device()) {
            task_ctl.work_sta = TASK_EXIT;      //设备无效，返回主菜单
            msgbox(STR_NO_DEVICE, NULL, MSGBOX_TIME);   //未插入存储设备
            return;
        }
        printf(str_show_taskmic);
    }else {
/*#if LINEIN_DETECT_EN
        if (!device_activate(DEVICE_LINEIN)) {
            task_ctl.work_sta = TASK_EXIT;      //设备无效，返回主菜单
            msgbox(STR_NO_DEVICE, NULL, MSGBOX_TIME);   //未插入存储设备
            return;
        }
#endif*/
#if (DEBUG_UART_SEL && ((AUX_CHANNEL_SEL & 0x07) == CHANNEL_01))
        uart_disable();
        P0DIR |= BIT(1);
#endif
        printf(str_show_taskaux);
    }
#if (WARNING_TASK_MIC || WARNING_TASK_AUX)
    if (work_sta_interrupt == TASK_NULL) {
        dac_enable();
        if (task_ctl.work_sta == TASK_MIC) {
            mp3_res_play_wait(RES_MP3_RECORDING_MODE);
        } else {
            mp3_res_play_wait(RES_MP3_AUX_MODE);
        }
        dac_disable();
    }
#endif
    work_sta_interrupt = TASK_NULL;
    task_record_init();
#if IS_LCD_DISPLAY
    draw_clear();                                       //清空屏显
    t_rec.updata_flag = 1;
    // 清除"请稍候"字样
    line_clear();
    line_update(2);
    line_update(4);
#if THEME_SELECT==THEME_LCD_DEFAULT
    lcd_buf = lcd_buf2;   // lcd使用小buf，否则会花屏
#endif
#endif    
#if NPCP215X
    //Audio_Slow_Volume(UNMUTE);
    AudioCodec_MUTE(UNMUTE);
#endif
    //sys_unmute();

}

//退出录音任务
#pragma location="TASK_MIC_SEG"
void task_record_exit(void)
{
    sys_ctl.input_sta = 0;
    sys_ctl.input_sta_err_flag = 0;
#if NPCP215X
    //Audio_Slow_Volume(MUTE);
    AudioCodec_MUTE(MUTE);
#endif
    //sys_mute();   
#if 0
#if IIS_TYPE == IIS_TAS5711
    TAS5711_AMP_Mute_Set(TRUE);
#endif
#endif
    if((t_rec.rec_sta == REC_STA_PAUSE) && t_rec.file_open) {
        task_record_stop();
    }
#if IIS_EN
    if(work_sta_interrupt == TASK_AUX) {
        task_aux_eq_exit();
        work_sta_interrupt = TASK_NULL;
        return;
    }
#endif  
	adc_exit();
    amux_exit();
    adc_ch = 1;   
#if 0    
    if(sys_ctl.mute_flag) { //防止暂停退出时mute住
        sys_unmute();
    }
#endif
#if (DEBUG_UART_SEL && ((AUX_CHANNEL_SEL & 0x07) == CHANNEL_01))
    P0DIR &= ~BIT(1);
    uart_enable();
#endif
#if MODE_CHANGE_BLUE_ON
    led_mode_on();
#endif
#if MIC_DIRECT_OUT_EN
    mic_direct_out_exit();
#endif
#if IS_LCD_DISPLAY
    lcd_buf = lcd_buf1;   // 退出时，lcd恢复使用大buf
#endif
    
}

//录音任务事务处理
#pragma location="TASK_MIC_SEG"
void task_record_event(void)
{
    WATCHDOG_CLR();
    comm_event();
    //检查是否有设备需要初始化
    if (device_change()) {
        if(t_rec.rec_sta == REC_STA_PAUSE) {
            t_rec.play_flag = 0;
            task_record_stop();
			return;
        }
    }

    if (task_ctl.work_sta == TASK_MIC) {
        if (!device_is_actived(DEVICE_SDMMC) && !device_is_actived(DEVICE_UDISK)) {
            task_ctl.work_sta = TASK_EXIT;          //设备无效，退出
            msgbox(STR_NO_DEVICE, NULL, MSGBOX_TIME);   //未插入存储设备
        }
    } else if (task_ctl.work_sta == TASK_AUX) {
/*#if LINEIN_DETECT_EN
        if (!device_activate(DEVICE_LINEIN)) {
            task_ctl.work_sta = TASK_EXIT;
        }
#endif*/
    }

#if AUX_AUTO_STANDBY_EN || IR_HARD_POWERDOWN_EN
    if((((!sys_ctl.sleep_cnt) && BT_IS_SLEEP) || sys_ctl.ir_power_flag) && !sys_ctl.lowpower_flag){
        if (task_ctl.work_sta == TASK_AUX) {
            task_hold();
            t_rec.aux_mute = 0;
            sys_ctl.sleep_cnt = 0xffff;
            led_aux_play();
            if (sys_ctl.volume) {
                sys_unmute();
                amux_play();
            }
        }
    }
#endif

#if WARNING_LOWPOWER
    if(sys_ctl.lowpower_warning_flag == 1){
        sys_ctl.lowpower_warning_flag = 2;
        if (task_ctl.work_sta == TASK_MIC) {
            mp3res_play_micmode(RES_MP3_PLEASE_CHARGE);
        } else {
#if IIS_EN
            mp3res_play_auxeqmode(RES_MP3_PLEASE_CHARGE);
#else
            mp3res_play_auxmode(RES_MP3_PLEASE_CHARGE);
#endif
        }
    }
#endif
}

//录音任务
#pragma location="TASK_RECORD_SEG"
void task_record(void)
{
    u8 record_source = task_ctl.work_sta;
    task_record_enter();
    while (task_ctl.work_sta == record_source) {
        u8 msg = get_msg();
        task_record_event();
        task_record_deal_msg(msg);
        task_record_display();
    }
    if((record_source == TASK_AUX)||(record_source == TASK_OPTI)||(record_source == TASK_COAX)){
        work_sta_interrupt = TASK_AUX;
    }        
    task_record_exit();
}
#endif
