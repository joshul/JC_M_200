/*****************************************************************************
 * Module    : Display
 * File      : ledseg_display.c
 * Author    : Hanny
 * Function  : LED数码管屏显示函数
*****************************************************************************/
#include "include.h"
#include "task_clock.h"

#if IS_LEDSEG_DISPLAY

type_ledseg_ocx_ctl ledseg_ocx;

//控件事件
#pragma location="LEDSEG_OCX"
void ledseg_ocx_event(void)
{
    if (ledseg_ocx.show_time && ocx_show_cnt >= ledseg_ocx.show_time) {
        ledseg_ocx.status = LEDSEG_OCX_NULL;
        sys_ctl.Vol_tyle = STATE_NULL;
        sys_ctl.file_change_flag = 0;
        //sys_ctl.dim_adj_flag = 0;
    }
    if (ledseg_ocx.status != ledseg_ocx.disp_status|| ledseg_ocx.disp_flag) {
        ledseg_ocx.disp_status = ledseg_ocx.status;
        //ledseg_ocx.disp_flag=0;
    }
}

//控件清除
#pragma location="LEDSEG_OCX"
void ledseg_ocx_clear(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    ledseg_ocx.show_time = 0;
    ledseg_ocx.status = LEDSEG_OCX_NULL;
    IE_EA = ie_ea;
}

//显示数字
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_num(u16 num)
{
    ledseg_ocx.number = num;
    ledseg_ocx.status = LEDSEG_OCX_NUMBOX;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ledseg_ocx.disp_flag = 1;
    ocx_show_cnt = 0;
}
//显示音量
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_vol(void)
{
    ledseg_ocx.status = LEDSEG_OCX_VOLBOX;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
    ledseg_ocx.disp_flag = 1;
    ledseg_disp(MENU_VOL);  //马上更新显示S
}
//
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_file(void)
{
    ledseg_ocx.status = LEDSEG_OCX_FILE;
    ledseg_ocx.show_time = MENU_DISP_TIME_COUNT;
    ocx_show_cnt = 0;
    ledseg_ocx.disp_flag = 1;
    //ledseg_disp(MENU_VOL);  //马上更新显示S
}

#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_dim(void)
{
    ledseg_ocx.status = LEDSEG_OCX_DIM;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
    ledseg_ocx.disp_flag = 1;
}

#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_menu(void)
{
    ledseg_ocx.status = LEDSEG_OCX_MENUBOX;
    ledseg_ocx.show_time = MENU_DISP_TIME_COUNT;
    ocx_show_cnt = 0;
    ledseg_ocx.disp_flag = 1;
    //ledseg_disp(MENU_VOL);  //马上更新显示S
}
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_version(void)
{
    ledseg_ocx.status = LEDSEG_OCX_VERSION;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
    ledseg_ocx.disp_flag = 1;
    //ledseg_disp(MENU_VOL);  //马上更新显示S
}

//显示频道
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_chan(void)
{
    ledseg_ocx.status = LEDSEG_OCX_CHANBOX;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
    ledseg_ocx.disp_flag = 1;
}

//显示EQ
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_eq(void)
{
    ledseg_ocx.status = LEDSEG_OCX_EQ;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
    ledseg_ocx.disp_flag = 1;
}
#if 1
//显示3d
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_3d(void)
{
    ledseg_ocx.status = LEDSEG_OCX_3D;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
    ledseg_ocx.disp_flag = 1;
}
#endif
//显示mute
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_mute(void)
{
    ledseg_disp(MENU_MUTE);
    ledseg_ocx.status = LEDSEG_0CX_MUTE;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
}

//显示PLAY MODE
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_mode_play(void)
{
    ledseg_ocx.status = LEDSEG_OCX_MODE_PLAY;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
    ledseg_ocx.disp_flag = 1;
}

//显示PLAY MODE
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_stat_play(void)
{
    ledseg_ocx.status = LEDSEG_OCX_STAT_PLAY;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
    ledseg_ocx.disp_flag = 1;
}

//显示ALARM开关
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_alarm_status(void)
{
    ledseg_ocx.status = LEDSEG_OCX_ALARM_STATUS;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
    ledseg_ocx.disp_flag = 1;
}

//显示控件
#pragma location="LEDSEG_OCX"
void ledseg_ocx_display(void)
{
    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NUMBOX:
        ledseg_disp(MENU_NUM);
        break;
    case LEDSEG_OCX_VOLBOX:
        ledseg_disp(MENU_VOL);
        break;
    case LEDSEG_OCX_MENUBOX:
        ledseg_disp(MENU_MENU);
        break;
    case LEDSEG_OCX_CHANBOX:
        ledseg_disp(MENU_FQ_CHAN);
        break;
    case LEDSEG_OCX_EQ:
        ledseg_disp(MENU_EQ);
        break;      
    case LEDSEG_OCX_3D:
        ledseg_disp(MENU_3D);
        break;   
    case LEDSEG_0CX_MUTE:
        ledseg_disp(MENU_MUTE);
        break; 
    case LEDSEG_OCX_MODE_PLAY:
        ledseg_disp(MENU_MODE_PLAY);
        break;
    case LEDSEG_OCX_ALARM_STATUS:
        ledseg_disp(MENU_ALARM_STA);
        break;
    case LEDSEG_OCX_STAT_PLAY: 
        ledseg_disp(MENU_MODE_STOP);
        break;
    case LEDSEG_OCX_VERSION:
        ledseg_disp(MENU_CODE_VERSION);
        break;
    case LEDSEG_OCX_DIM:
        ledseg_disp(MENU_DIM);
        break;
    case LEDSEG_OCX_FILE:
        ledseg_disp(MENU_FILE);
        break;
    case LEDSEG_OCX_ERR:
        ledseg_disp(MENU_ERR);
        break;
    }
}
//显示ALARM开关
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_mix_step(void)
{
    ledseg_ocx.status = LEDSEG_OCX_MIX_STEP;
    ledseg_ocx.disp_flag = 1;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
}

//显示无设备
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_no_device(void)
{
    ledseg_disp(MENU_NO_DEVICE);
    ledseg_ocx.status = LEDSEG_OCX_NO_DEVICE;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
}

//显示删除
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_del(void)
{
    ledseg_disp(MENU_DEL);
    ledseg_ocx.status = LEDSEG_OCX_DEL;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
}

//显示磁盘满
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_full(void)
{
    ledseg_disp(MENU_FULL);
    ledseg_ocx.status = LEDSEG_OCX_FULL;
    ledseg_ocx.show_time = DISP_TIME_COUNT;
    ocx_show_cnt = 0;
}

//显示错误
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_err(void)
{
    //ledseg_disp(MENU_ERR);
    //ledseg_disp(MENU_NULL);
    ledseg_ocx.status = LEDSEG_OCX_ERR;
    ledseg_ocx.show_time = 20;//DISP_TIME_COUNT;
    ocx_show_cnt = 0;
}

//正在设置时间显示
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_setting_clock(void)
{
    if (t_clock.sub_sta == STA_SET_CLOCK) {
        ledseg_ocx.show_time = 1;
        ocx_show_cnt = 1;
        if (t_clock.setting_flag) {
            ledseg_ocx.show_time = DISP_TIME_COUNT;
            ocx_show_cnt = 0;
        }
    }
}
//正在设置闹钟显示
#pragma location="LEDSEG_OCX"
void ledseg_ocx_show_setting_alarm(void)
{
    if (t_clock.sub_sta == STA_SET_ALARM) {
        ledseg_ocx.show_time = 1;
        ocx_show_cnt = 1;
        if (t_clock.setting_flag) {
            ledseg_ocx.show_time = DISP_TIME_COUNT;
            ocx_show_cnt = 0;
        }
    }
}

//等待界面
#pragma location="DRAW_IMG_SEG1"
void draw_wait(void)
{
}

//清屏
void draw_clear(void)
{
}

//返回选中的值
#pragma location="LIST_BOX_SEG"
u8 listbox(u8 index, u8 total, u8 mode, u16 param, u8 show_time)
{
    return 0;
}

#endif
