/*****************************************************************************
 * Module    : Task
 * File      : task_poweroff.c
 * Author    : Hanny
 * Function  : �ػ�����
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "typedef.h"
#include "user_iis.h"

#pragma constseg="RTC_SEG_CONST"
IAR_CONST char str_power_down_enter[] = "power_down_enter\n";
#pragma constseg=default

#pragma location="RTC_SEG"
void power_down_enter(void)
{
    u8 wko_cfg = 0;
#if WKO_WKUP_POWERDOWN  //ʹ�������ػ���
    wko_cfg = 0x1c;  //wko pin output en, 60k pull down, pin en & input en
#endif
#if ALARM_WKUP_POWERDOWN     //BIT(5)|BIT(1) ALARM WAKE UP
     wko_cfg |=  BIT(5)|BIT(1);
#endif
    irtcc_write_cfg(WRITE_WKO, 0x00);
    asm("nop");asm("nop");asm("nop");

    WDTCON = 0;
    IE_EA = 0;
    P0DIR = 0xFF;
    P1DIR = 0xFF;
    P2DIR = 0xFF;
    P3DIR = 0xFF;
    P0PU = 0;
    P1PU = 0;
    P2PU = 0;
    P3PU = 0;
    u8 vcl = irtcc_read_cfg(READ_VCL);
    //�Ӵ���4.2v��ѹ(��ص�ѹ)����ʱҪ����BIT(1)������ػ������е����޷�������
    irtcc_write_cfg(WRITE_VCL,vcl|BIT(2)/*| BIT(1)*/ | BIT(0));  //����BIT(0),����͵�ػ���,��ѹ������3.4V���϶����ܿ���������
    delay_ms(1);

    irtcc_write_cfg(WRITE_CFG,irtcc_read_cfg(READ_CFG)|BIT(5));
    delay_ms(1);

    irtcc_write_cfg(WRITE_WKO, wko_cfg);
    asm("nop");asm("nop");asm("nop");

    PWRCON1 = 0x00;
    PWRCON2 = 0x00;
    PWRCON3 = 0x00;
    PWRCON4 = 0x00;
    PWRCON5 = 0x00;
#if SOFT_POWER_OFF_20UA
    PWRCON3 &= ~BIT(4);
    while(1){
        irtcc_write_cfg(WRITE_PWR, 0x02);
        delay_ms(1);
        WDTCON = 0x13;      //ʹ�ÿ��Ź���ÿ128ms����һ��
        irtcc_write_cfg(WRITE_CFG,irtcc_read_cfg(READ_CFG)&(~BIT(5)));
        irtcc_write_cfg(WRITE_PWR, 0x0F);  //RVDD disable
        PWRCON3 |= BIT(4);
        delay_ms(100);
    }
#elif  SOFT_POWER_OFF_3UA
    irtcc_write_cfg(WRITE_PWR, 0x02);    //VDDIO disable
    delay_ms(200);
    while(1);
#endif
}

//��DCIN��ٹػ�, ����������ػ�
#pragma location="RTC_SEG"
void power_down_dcin_check(void)
{
   // u16 wko_high_cnt = 0;
    u8 msg;
    /*WDTCON = 0;
    IE_EA = 0;
    P0DIR = 0xFF;
    P1DIR = 0xFF;
    P2DIR = 0xFF;
    P3DIR = 0xFF;
    //P0DIR &= ~BIT(1); //����ӡ
    while(irtcc_read_cfg(READ_STA) & BIT(5)){
        delay(0xFF);
    }*/
    
    flush_msg();//��հ�����Ϣ
    while(1){
        msg = get_msg();
        WATCHDOG_CLR();
        if(msg == QSYSTEM_500MS)
        {
            ledseg_disp(MENU_STANDBY);    
        }
        if(msg == KU_POWER||((msg == KL_MODE)&&(!sys_ctl.irkey))||IS_NFC_WAKEUP())
        {
            if((sys_ctl.first_power_on)&&(task_ctl.last_work!=TASK_MUSIC)){
                
                sys_ctl.first_power_on=0;
                device_change_check();
            }
            ledseg_disp(MENU_POWERON);
            delay_5ms(200);
            ledseg_disp(MENU_MODEL);
            delay_5ms(200);
    		break;   
    	}             
    }
    task_ctl.work_sta = task_ctl.last_work;
}

#if TASK_POWEROFF_EN
//����������
#pragma location="TASK_POWER"
void task_poweroff(void)
{
    
#if WARNING_POWER_OFF
    dac_enable();
    sys_unmute();
    mp3_res_play_wait(RES_MP3_POWER_OFF);
    dac_disable();
#endif
    ledseg_disp(MENU_POWEROFF);
    led_mode_off();
#if LOWER_POWER_DISPLAY_OFF
#if IS_LEDSEG_DISPLAY
    u8 i;
    for(i = 0;i < 3;i++)
    {
        u8 temp = 1;
        while(temp--){
            WATCHDOG_CLR();
            delay_5ms(200);
        }
    }
#endif
#endif
    
#if 0//SOFT_POWER_ON_OFF
    if(sys_ctl.lowpower_flag || sys_ctl.soft_poweroff_flag){
        sys_ctl.sleep_flag = 1;
        #if LED_GREEN_BLINK
            LED_GREEN_OFF();
        #endif
        #if LED_BLUE_BLINK
            LED_BLUE_OFF();
        #endif
        #if LED_STATUS_BLINK
            LED_STATUS_OFF();
        #endif

        dac_disable();
        speaker_mute();
        IE_EA = 0;
        WDTCON = 0;
        SOFT_POWER_CTL_OFF();
        while(1);
    }
#endif
#if BT_ALL_TASK
    bt_close();
#endif

#if IS_LCD_DISPLAY
    lcd_off();
#endif
#if IIS_EN
    //iis_powerdown();	// �ر�IIS
#endif

    power_down_dcin_check();
   // power_down_enter();
   // sleep_mode_enter();
}

#endif