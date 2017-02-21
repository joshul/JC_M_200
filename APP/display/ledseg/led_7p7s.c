/*****************************************************************************
 * Module    : ledseg
 * File      : led_7p7s.c
 * Author    : Hanny
 * Function  : led_7p7s数码管显示屏驱动
 *****************************************************************************/
#include "include.h"
#include "led_seg.h"

#if IS_LEDSEG_7PIN_DISPLAY
#include "led_7p7s.h"

__no_init u8 pin_disp_buf[7];
u8 led_seg_valid[7];
IAR_XDATA_A u8 COM_7P7S;

//有效位置1
IAR_CONST u8 tbl_ledseg_valid[7] = {
#if (THEME_SELECT == THEME_LEDSEG_7PIN)
    /*PIN1 | PIN2 | PIN3 | PIN5,
    PIN0 | PIN2 | PIN3 | PIN5 | PIN6,
    PIN0 | PIN2 | PIN3 | PIN4 | PIN5 | PIN6,
    PIN0 | PIN1 | PIN2 | PIN4 | PIN6,
    PIN0 | PIN1 | PIN2 | PIN3 | PIN5,
    PIN0 | PIN1 | PIN2 | PIN3 | PIN4 | PIN6,
    PIN0 | PIN1 | PIN3 | PIN4 | PIN5,*/
    PIN1 | PIN2 | PIN3 | PIN4 | PIN5,
    PIN0 | PIN2 | PIN3 | PIN4 | PIN5 | PIN6,
    PIN0 | PIN1 | PIN3 | PIN4 | PIN5,
    PIN0 | PIN1 | PIN2 | PIN4 | PIN5,
    PIN0 | PIN1 | PIN2 | PIN3 | PIN5 | PIN6,
    PIN0 | PIN1 | PIN2 | PIN3 | PIN4 | PIN6,
    PIN0 | PIN1 | PIN3 | PIN4 | PIN5,
#else
    PIN1 | PIN2 | PIN3 | PIN4 | PIN5,
    PIN0 | PIN2 | PIN3 | PIN4 | PIN5,
    PIN0 | PIN1 | PIN3 | PIN4 | PIN5 | PIN6,
    PIN0 | PIN1 | PIN2 | PIN4 | PIN5,
    PIN0 | PIN1 | PIN2 | PIN3 | PIN5 | PIN6,
    PIN0 | PIN1 | PIN2 | PIN3 | PIN4 | PIN6,
    PIN2 | PIN4 | PIN5,
#endif
};

#include SET_USER_PATH(CFG_IO_KEY_DIR, /io/io_led_seg.c)

//led_7p7s初始化函数
/****************************************
 
****************************************/
#pragma location="DISP_LED_7P7S_SEG"
void led_7p7s_init(void)
{
    //led_7p7s_io_init();
    //P1DIR |= BIT(2);
    P3DIR |= BIT(4);
   // P2DIR |= (BIT(0)|BIT(1)|BIT(3)|BIT(4)|BIT(7));

    COM_7P7S = 0;

    my_memset((u8*)disp_buf,0x00,5);
    my_memset((u8 *)pin_disp_buf, 0x00, 7);
}

//根据7脚LED的真值表进行映射（每段输出高的IO记为COM端 7*7）COM:0~6  SEG:0~6
//#pragma location="DISP_LED_7P7S_SEG"
void led_7p7s_value_set(void)
{
    u8 ie0, ie1;
    bool ie_ea = IE_EA;
    IE_EA = 0;          //关总中断，保护好IE0/IE1后再打开
    ie0 = IE0;          //保存IE0
    ie1 = IE1;          //保存IE1
    IE0 = 0;            //关掉IE0
    IE1 &= BIT(6);      //关掉IE1，除了UART中断(因为这段处理较长，不能一直关IE_EA，否则会阻塞蓝牙UART中断)
    IE_EA = ie_ea;      //恢复总中断

    my_memset((u8 *)pin_disp_buf, 0x00, 7);

/*********** SEG ****************** COM ******** SEG(out)  *******/
#if (THEME_SELECT == THEME_LEDSEG_7PIN)

    if(disp_buf[0] & LED_BIT_A)     pin_disp_buf[0] |= PIN1; //设为1，扫描时对应此SEG位设为输出(FM)
    if(disp_buf[0] & LED_BIT_B)     pin_disp_buf[0] |= PIN2;
    if(disp_buf[0] & LED_BIT_C)     pin_disp_buf[3] |= PIN0;
    if(disp_buf[0] & LED_BIT_D)     pin_disp_buf[4] |= PIN0;
    if(disp_buf[0] & LED_BIT_E)     pin_disp_buf[0] |= PIN3;
    if(disp_buf[0] & LED_BIT_F)     pin_disp_buf[1] |= PIN0;
    if(disp_buf[0] & LED_BIT_G)     pin_disp_buf[2] |= PIN0;
    

    if(disp_buf[1] & LED_BIT_A)     pin_disp_buf[1] |= PIN2;
    if(disp_buf[1] & LED_BIT_B)     pin_disp_buf[1] |= PIN3;
    if(disp_buf[1] & LED_BIT_C)     pin_disp_buf[4] |= PIN1;
    if(disp_buf[1] & LED_BIT_D)     pin_disp_buf[1] |= PIN5;
    if(disp_buf[1] & LED_BIT_E)     pin_disp_buf[1] |= PIN4;
    if(disp_buf[1] & LED_BIT_F)     pin_disp_buf[2] |= PIN1;
    if(disp_buf[1] & LED_BIT_G)     pin_disp_buf[3] |= PIN1;
    

    if(disp_buf[2] & LED_BIT_A)     pin_disp_buf[4] |= PIN3;
    if(disp_buf[2] & LED_BIT_B)     pin_disp_buf[2] |= PIN4;
    if(disp_buf[2] & LED_BIT_C)     pin_disp_buf[3] |= PIN4;
    if(disp_buf[2] & LED_BIT_D)     pin_disp_buf[5] |= PIN0;
    if(disp_buf[2] & LED_BIT_E)     pin_disp_buf[5] |= PIN2;
    if(disp_buf[2] & LED_BIT_F)     pin_disp_buf[3] |= PIN2;
    if(disp_buf[2] & LED_BIT_G)     pin_disp_buf[4] |= PIN2;
    

    if(disp_buf[3] & LED_BIT_A)     pin_disp_buf[6] |= PIN5;
    if(disp_buf[3] & LED_BIT_B)     pin_disp_buf[5] |= PIN6;
    if(disp_buf[3] & LED_BIT_C)     pin_disp_buf[4] |= PIN5;
    if(disp_buf[3] & LED_BIT_D)     pin_disp_buf[5] |= PIN3;
    if(disp_buf[3] & LED_BIT_E)     pin_disp_buf[3] |= PIN5;
    if(disp_buf[3] & LED_BIT_F)     pin_disp_buf[5] |= PIN4;
    if(disp_buf[3] & LED_BIT_G)     pin_disp_buf[4] |= PIN6;
    

    
    if(disp_buf[4] & ICON_FM)       pin_disp_buf[5] |= PIN1;
   
    if(disp_buf[4] & ICON_OPT)      pin_disp_buf[0] |= PIN4;
    if(disp_buf[4] & ICON_DOT)      pin_disp_buf[6] |= PIN4;
    if(disp_buf[4] & ICON_DOT2)     pin_disp_buf[2] |= PIN3;
    if(disp_buf[4] & ICON_AUX){
        pin_disp_buf[0] |= PIN5;
        pin_disp_buf[1] |= PIN6;
    }
    if(disp_buf[4] & ICON_BT) {      
        pin_disp_buf[2] |= PIN5;
        pin_disp_buf[6] |= PIN1;
    }

    /*if(disp_buf[4] & ICON_PLAY)     pin_disp_buf[0] |= PIN5;
    if(disp_buf[4] & ICON_PAUSE)    pin_disp_buf[2] |= PIN5;
    if(disp_buf[4] & ICON_USB)      pin_disp_buf[5] |= PIN1;
    if(disp_buf[4] & ICON_SD)       pin_disp_buf[0] |= PIN4;
    if(disp_buf[4] & ICON_DOT2)     pin_disp_buf[2] |= PIN3;
    if(disp_buf[4] & ICON_FM) {
        pin_disp_buf[2] |= PIN6;
        pin_disp_buf[6] |= PIN4;
    }
    if(disp_buf[4] & ICON_MP3)    pin_disp_buf[6] |= PIN2;*/

#elif (THEME_SELECT == THEME_LEDSEG_7PIN_MHZ)

    if(disp_buf[0] & LED_BIT_A)     pin_disp_buf[0] |= PIN1; //设为1，扫描时对应此SEG位设为输出(MHZ)
    if(disp_buf[0] & LED_BIT_B)     pin_disp_buf[0] |= PIN2;
    if(disp_buf[0] & LED_BIT_C)     pin_disp_buf[3] |= PIN0;
    if(disp_buf[0] & LED_BIT_D)     pin_disp_buf[4] |= PIN0;
    if(disp_buf[0] & LED_BIT_E)     pin_disp_buf[0] |= PIN3;
    if(disp_buf[0] & LED_BIT_F)     pin_disp_buf[1] |= PIN0;
    if(disp_buf[0] & LED_BIT_G)     pin_disp_buf[2] |= PIN0;

    if(disp_buf[1] & LED_BIT_A)     pin_disp_buf[1] |= PIN2;
    if(disp_buf[1] & LED_BIT_B)     pin_disp_buf[1] |= PIN3;
    if(disp_buf[1] & LED_BIT_C)     pin_disp_buf[4] |= PIN1;
    if(disp_buf[1] & LED_BIT_D)     pin_disp_buf[1] |= PIN5;
    if(disp_buf[1] & LED_BIT_E)     pin_disp_buf[1] |= PIN4;
    if(disp_buf[1] & LED_BIT_F)     pin_disp_buf[2] |= PIN1;
    if(disp_buf[1] & LED_BIT_G)     pin_disp_buf[3] |= PIN1;

    if(disp_buf[2] & LED_BIT_A)     pin_disp_buf[4] |= PIN3;
    if(disp_buf[2] & LED_BIT_B)     pin_disp_buf[2] |= PIN4;
    if(disp_buf[2] & LED_BIT_C)     pin_disp_buf[3] |= PIN4;
    if(disp_buf[2] & LED_BIT_D)     pin_disp_buf[5] |= PIN0;
    if(disp_buf[2] & LED_BIT_E)     pin_disp_buf[5] |= PIN2;
    if(disp_buf[2] & LED_BIT_F)     pin_disp_buf[3] |= PIN2;
    if(disp_buf[2] & LED_BIT_G)     pin_disp_buf[4] |= PIN2;

    if(disp_buf[3] & LED_BIT_A)     pin_disp_buf[6] |= PIN5;
    if(disp_buf[3] & LED_BIT_B)     pin_disp_buf[5] |= PIN6;
    if(disp_buf[3] & LED_BIT_C)     pin_disp_buf[4] |= PIN5;
    if(disp_buf[3] & LED_BIT_D)     pin_disp_buf[5] |= PIN3;
    if(disp_buf[3] & LED_BIT_E)     pin_disp_buf[3] |= PIN5;
    if(disp_buf[3] & LED_BIT_F)     pin_disp_buf[5] |= PIN4;
    if(disp_buf[3] & LED_BIT_G)     pin_disp_buf[4] |= PIN6;

    if(disp_buf[4] & ICON_PLAY)     pin_disp_buf[0] |= PIN5;
    if(disp_buf[4] & ICON_PAUSE)    pin_disp_buf[2] |= PIN5;
    if(disp_buf[4] & ICON_USB)      pin_disp_buf[5] |= PIN1;
    if(disp_buf[4] & ICON_SD)       pin_disp_buf[0] |= PIN4;
    if(disp_buf[4] & ICON_DOT2)     pin_disp_buf[2] |= PIN3;
    if(disp_buf[4] & ICON_FM) {
        pin_disp_buf[2] |= PIN6;
        pin_disp_buf[6] |= PIN4;
    }
    if(disp_buf[4] & ICON_MP3)    pin_disp_buf[6] |= PIN2;
#endif

    IE0 = ie0;          //先恢复IE0，此时会关IE_EA
    IE1 = ie1;          //再恢复IE1
    IE_EA = ie_ea;      //恢复总中断
}

#pragma location="DISP_LEDSEG_SCAN"
__near_func bool led_7p7s_reuse(void)
{
    bool res = false;

#if (LINEIN_DETECT_EN && LINEIN_SEG_REUSE)
    LINEIN_SEG_ENTER_DETECT();
#endif
#if (EAR_DETECT_EN && EAR_SEG_REUSE)
    EAR_SEG_ENTER_DETECT();
#endif			
    delay(20);  //SYSCLK_24M时,20约为20 uS
#if (LINEIN_DETECT_EN && LINEIN_SEG_REUSE)
    linein_detect();
#endif
#if (EAR_DETECT_EN && EAR_SEG_REUSE)
    earphone_detect();
#endif
#if (LINEIN_DETECT_EN && LINEIN_SEG_REUSE)
    LINEIN_SEG_EXIT_DETECT();
#endif
#if (EAR_DETECT_EN && EAR_SEG_REUSE)
    EAR_SEG_EXIT_DETECT();
#endif		
#if ((LINEIN_DETECT_EN && LINEIN_SEG_REUSE) || (EAR_DETECT_EN && EAR_SEG_REUSE))
    res = true;
#endif	

    return res;
}

/****************************************
 P34   P27   P24   P23   P21   P20   P12
  |     |     |     |     |     |     |
  1     2     3     4     5     6     7
****************************************/
#pragma location="DISP_LEDSEG_SCAN"
__near_func void led_7p7s_scan(void)
{
    static u8 count = 0;
    u8 dis_seg = 0,dir_temp,out_temp,dir_temp2,out_temp2,dir_temp3,out_temp3;
    out_temp = P2 & 0x64;      //把所用到的脚全部置0 (并保存没用到的P2脚原来的电平)
    out_temp2 = P1 & 0xFB;
    out_temp3 = P3 & 0xEF;
    dir_temp = 0;//P2DIR | 0x9B;   //把所用到的脚全部设为输入 (并保存没用到的P2脚原来的方向)
    dir_temp2 = P1DIR | 0x04;
    dir_temp3 = P3DIR | 0x10;

    while (1) {
        if (count > 6) {
            count = 0;
            COM_7P7S++;
        }
        if(COM_7P7S > 6) {
            COM_7P7S = 0;
            if(led_7p7s_reuse())
                return;
        }
        if (tbl_ledseg_valid[COM_7P7S] & BIT(count)) {         //节约CPU，只扫描有效的SEG段
            break;
        }
        count++;
    }

    dis_seg = pin_disp_buf[COM_7P7S] & BIT(count);

    dir_temp &= ~(((dis_seg & 0x02)<<6) | ((dis_seg & 0x04)<<2) | (dis_seg & 0x08) | ((dis_seg & 0x10)>>3) | ((dis_seg & 0x20)>>5));  //把要显示的段设为输出 (前面已经把所有脚置0)
    dir_temp2 &= ~((dis_seg & 0x40)>>4);         //p12
    dir_temp3 &= ~((dis_seg & 0x01)<<4);         //p34

    if(COM_7P7S == 0)
    {
        dir_temp3 &= ~BIT(4);
        out_temp3 |= BIT(4);
    }
    else if(COM_7P7S == 1)
    {
        dir_temp &= ~BIT(7);
        out_temp |= BIT(7);
    }
    else if(COM_7P7S == 2)
    {
        dir_temp &= ~BIT(4);
        out_temp |= BIT(4);
    }
    else if(COM_7P7S == 3)
    {
        dir_temp &= ~BIT(3);
        out_temp |= BIT(3);
    }
    else if(COM_7P7S == 4)
    {
        dir_temp &= ~BIT(1);
        out_temp |= BIT(1);
    }
    else if(COM_7P7S == 5)
    {
        dir_temp &= ~BIT(0);
        out_temp |= BIT(0);
    }
    else if(COM_7P7S == 6)
    {
        dir_temp2 &= ~BIT(2);
        out_temp2 |= BIT(2);
    }
//    P2 = out_temp;
//    P2DIR = dir_temp;
//    P1 = out_temp2;
//    P1DIR = dir_temp2;
    P3 = out_temp3;
    P3DIR = dir_temp3;

    count++;        //扫描下一SEG
}



#endif
