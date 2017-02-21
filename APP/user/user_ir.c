/*****************************************************************************
 * Module    : User
 * File      : user_ir.c
 * Author    : Hanny
 * Function  : �û���IO����
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "user_ir.h"
#include "user_key.h"


#if (HARD_IR_EN || SOFT_IR_EN || SOFT_IR_PORT_INT_EN)

#include SET_USER_PATH(CFG_IO_IR_DIR, /io/io_ir.c)

volatile  u16 ir_data;
volatile  u8 ir_state;
u16 userCode16;


__near_func u8 ir_key_get(u8 ir_data)
{
    u8 knum = IR_KEY_NUM;
    while (knum--)
    {
        if(userCode16 == IR_USER_CODE8989){
            
            if(ir_data == tbl_ir_dat[knum])return tbl_ir_val[knum];
        }
    }
    return NO_KEY;
}


#if SOFT_IR_EN
/*****************timer2��ʼ��,����ң�������ʹ��************************/
volatile type_ir ir;
#pragma vector = VECTOR_OFFSET(TIMER2_INT)
#pragma register_bank=2   //0���ȼ�(������ȼ�)������bank1 //2���ȼ�������bank2 //3���ȼ�������bank3
__interrupt __near_func void timer2_isr(void)
{
    u8 ir_cnt;
    ISR_ENTER();
    if(TMR2CON1 & BIT(6)){
        //printf("cp!");
        ir_cnt = TMR2CNTH;
        //ir_cnt <<= 8;
        //ir_cnt +=  TMR1CNTL;  //1/48000*128*(0x1A6+1) =1.128   //1/48000*128*(0x353+1)=2.272 �����۷���.
        TMR2CNTH = 0;
        TMR2CNTL = 0;
        TMR2CON1 &= ~BIT(6);
        //uart_putchar(ir_cnt);

    }else if(TMR2CON1 & BIT(7)){
        //printf("op>");
        TMR2CON1 &= ~BIT(7);
        ir_cnt = 0;   //����ж� -->ir_state=0;
    }
    //2C 13 01 01 01 01 01 01 01 01 03 03 03 03 03 03 03 03 03 01 03 01 01 01 03 01 01 03
    //01 03 03 03 01 03 3B 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10
    //(9+2.25)/(1/48000*128)/256 = 0x10.7A
    //(110-11.25)/(1/48000*128)/256 = 0x90.A74 (0x8E)�����е����,ʵ��0x8E
    //��һ���ظ����崮ʵ��0x13
    if (((ir_cnt > 0x0A) && (ir_cnt < 0x15)) || ((ir_cnt > 0x33) && (ir_cnt < 0x46)) || ((ir_cnt > 0x87) && (ir_cnt < 0x97)))
    {
       //irState = 32,�������ѵõ�����ǰ������Чʹ��irState=32��ʹirDataֵҲ���䣬��ʱ��KeyScan��ȡ
       goto EXIT_TM2;
    }

    ir.data >>= 1;               //irData��8λ����ֵ
    ir.state++;
    //uart_putchar(ir.state);
    //if(32 == ir.state){uart_putchar(ir.data>>8);uart_putchar(ir.data);}
    if (ir_cnt == 3)
    {
        ir.data |= 0x8000;       //irDataΪ32λ  // �����ǹؼ������ǰ������1���൱��irDataΪ16λ�����Σ�ͨ�������պð�����λ������ŵ����һ���ֽ�
    }
    else if (ir_cnt == 1)
    {
        goto EXIT_TM2;
    }
    else
    {
        ir.state = 0;               //�ɼ� ֻ��counter = 01 02 0B 29 62����Ϊ����Чֵ���������ὫirState��0��Ԥʾ��һ�½���IRd ID�Ŀ�ʼ
        ir.data = NO_KEY;
    }
EXIT_TM2:
    ISR_EXIT();
}


void ir_timer2_init(void)
{
    IE_TM2 = 0;
    P1DIR |= BIT(6);
    P1PUS0 &= ~BIT(6);
    P1PUS1 &= ~BIT(6);
    P1PU  |= BIT(6);//������
    TMR2CON0 = 0x74;    //Falling Edge trigger// Capture Mode //P07 //SystemClk
    TMR2CON1 = 0x37;   //OverflowInt,CaptureInt enalbe   //devide 128
    TMR2PRH = 0xA8;    //The overflow period of the timer is: Tinc-source * T1PSR * (T1PR + 1).
    TMR2PRL = 0x74;    //(1/48000000)*128*(0xA122+1) = 110 ms   //125/(1/48000*128)-1 = 0xA874
    TMR2CNTL = 0;
    TMR2CNTH = 0;
    IP0 &= ~BIT(3);    //���ȼ�2
    IPH0 |= BIT(3);    //Timer2ʹ�����ȼ���2
    IE_TM2 = 1;
}
#endif

#if SOFT_IR_PORT_INT_EN
//extern interput io for ir
//--------------------------------------------------------------------------
//port int + timer2 ����

volatile type_ir ir;
#pragma vector=VECTOR_OFFSET(PORT_INT)
#pragma register_bank=2
__interrupt __near_func void port_isr(void)
{
    u8 ir_cnt;
    ISR_ENTER();
    //printf("port int->");
    if(PWKPND & BIT(IR_PORT_INT_SEL&0x03))   //PORT �ж�
    {
        PWKPND &=~BIT(IR_PORT_INT_SEL&0x03);
		TMR2CON0 = 0x00;
        ir_cnt = TMR2CNTH;
        //ir_cnt <<= 8;
        //ir_cnt +=  TMR1CNTL;  //1/48000*128*(0x1A6+1) =1.128   //1/48000*128*(0x353+1)=2.272 �����۷���.
        TMR2CNTH = 0;
        TMR2CNTL = 0;
        TMR2CON0 = 0x14;
        ir.timeout_cnt = 120;
        //uart_putchar(ir_cnt);
        //2C 13 01 01 01 01 01 01 01 01 03 03 03 03 03 03 03 03 03 01 03 01 01 01 03 01 01 03
        //01 03 03 03 01 03 3B 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10 8E 10
        //(9+2.25)/(1/48000*128)/256 = 0x10.7A
        //(110-11.25)/(1/48000*128)/256 = 0x90.A74 (0x8E)�����е����,ʵ��0x8E
        //��һ���ظ����崮ʵ��0x3B
       if (((ir_cnt > 0x0A) && (ir_cnt < 0x15)) || ((ir_cnt > 0x33) && (ir_cnt < 0x46)) || ((ir_cnt > 0x87) && (ir_cnt < 0x97)))
       {
           //irState = 32,�������ѵõ�����ǰ������Чʹ��irState=32��ʹirDataֵҲ���䣬��ʱ��KeyScan��ȡ
           ISR_EXIT();
           return;
       }

       ir.data >>= 1;               //irData��8λ����ֵ
       ir.state++;
       //uart_putchar(ir.state);
       //if(32 == ir.state){uart_putchar(ir.data);}
       if (ir_cnt == 3)
       {
            ir.data |= 0x8000;       //irDataΪ32λ  // �����ǹؼ������ǰ������1���൱��irDataΪ16λ�����Σ�ͨ�������պð�����λ������ŵ����һ���ֽ�
       }
       else if (ir_cnt == 1)
       {
            ISR_EXIT();
            return;
       }
       else
      {
            ir.state = 0;               //�ɼ� ֻ��counter = 01 02 0B 29 62����Ϊ����Чֵ���������ὫirState��0��Ԥʾ��һ�½���IRd ID�Ŀ�ʼ
            ir.data = NO_KEY;
      }
    }
    ISR_EXIT();
}

void ir_port_int_init(void)
{
    //printf("ir_port_int_init chip XX = %d\n",get_chip_type());
    TMR2CON0 = 0X14;        //TM2EN_counter mode, SysClk
    TMR2CON1 = 0x07;        //no interrupt, DIV 128
    TMR2PRL = 0xF0;         //
    TMR2PRH = 0xFF;         //120/(1/48000*128)-1=0xAFC7.
    TMR2CNTL = 0;
    TMR2CNTH = 0;           //Clear CNT
    IE1 &= ~BIT(2);
    //------------------------------------
#if (IR_PORT_INT_SEL == IR_PORT_INT_P01)
    P0DIR |= BIT(1);  //P01
    P0PU |= BIT(1);
    PMUXCON1 &= ~BIT(0);
    PWKEN &= ~BIT(0);
    PWKEDGE = (PWKEDGE&0xE0)|BIT(0);  //FALL EGDE
#elif(IR_PORT_INT_SEL == IR_PORT_INT_P06)
    P0DIR |= BIT(6);  //P06
    P0PU |= BIT(6);
    PMUXCON1 &= ~BIT(1);
    PWKEN &= ~BIT(1);
    PWKEDGE = (PWKEDGE&0xE0)|BIT(1);  //FALL EGDE
#elif(IR_PORT_INT_SEL == IR_PORT_INT_P07)
    P0DIR |= BIT(7);  //P07
    P0PU |= BIT(7);
    PMUXCON1 |= BIT(7);
    PMUXCON1 |= BIT(5);
    PWKEN &= ~BIT(3);
    PWKEDGE = (PWKEDGE&0xE0)|BIT(3);  //FALL EGDE
#elif(IR_PORT_INT_SEL == IR_PORT_INT_P34)
    P3DIR |= BIT(4);  //P34
    P3PU |= BIT(4);
    PWKEN &= ~BIT(2);
    PMUXCON0 &= ~BIT(2);
    PWKEDGE = (PWKEDGE&0xE0)|BIT(2);  //FALL EGDE
#elif(IR_PORT_INT_SEL == IR_PORT_INT_P20)
    P2DIR |= BIT(0);  //P20
    P2PU |= BIT(0);
    PMUXCON2 |= BIT(0);
    PMUXCON1 &= ~BIT(7);
    PMUXCON1 &= ~BIT(5);
    PWKEN &= ~BIT(3);
    PWKEDGE = (PWKEDGE&0xE0)|BIT(3);  //FALL EGDE
#else
#error "please sel ir receiver port"
#endif
    //--------------------------------------*/
    IPH1 |= BIT(2);  //portʹ�����ȼ���2
    IE1 |= BIT(2);   //port interrupt enable
}
#endif //SOFT_IR_PORT_INT_EN

#if HARD_IR_EN

#pragma location="INIT_SEG_1"
void ir_hardware_init(void)
{
#if (SYS_CRY == CRY_2M || SYS_CRY == CRY_32K)
    #if 0
    CLKCON2  =  (CLKCON2 &0xF8)|0x06; //0x06, IR Select 32K OSC
    PCON0 &= ~BIT(3);  //IR clk_en
    P3DIR |= BIT(3);
    P3PU |= BIT(3);
    P3PD &=~ BIT(3);
    IRCON0 = 0x21;  //0x21:P33 0x01:P07 0x11:P16  0x31:P40  recv pin  //ir enable
    IRCON1 = 0x5e;
    IRCON1 = 0x28;
    IRCON1 = 0x02;
    IRCON1 = 0x04;
    IRCON1 = 0x09;
    #else
    CLKCON2  =  (CLKCON2 &0xF8)|0x03; //0x03, IR Select 1M OSC
    CLKCON2 &=~BIT(2);

    PCON0 &= ~BIT(3);  //IR clk_en
    P0DIR |= BIT(7);
    P0PU |= BIT(7);
    P0PD &=~ BIT(7);
    IRCON0 = 0x01;  //0x21:P33 0x01:P07 0x11:P16  0x31:P40  recv pin  //ir enable
    IRCON1 = 0x9C;
    IRCON1 = 0x50;
    IRCON1 = 0x04;
    IRCON1 = 0x08;
    IRCON1 = 0x11;
    #endif
#else
    #error "NO CRY_32K must sel SOFT_IR_EN"
    /*
    CLKCON2  =  (CLKCON2 &0xF8)|0x00;     //0x00 1M//0x04 32K  //IR CLK Select PLL 1M
    PCON0 &= ~BIT(3);  //IR clk_en
    P3DIR |= BIT(3);
    P3PU |= BIT(3);
    P3PD &=~ BIT(3);
    IRCON0 = 0x21; //0x21:P33 0x01:P07 0x11:P16  0x31:P40  recv pin  //ir enable
    IRCON1 = 0x9C;
    IRCON1 = 0x50;
    IRCON1 = 0x04;
    IRCON1 = 0x08;
    IRCON1 = 0x11;
    */
#endif
}

//#pragma location="IR_SEG"
__near_func u8 ir_hardware_scan(void)
{
    static u8 rep_cnt = 0;
    if (IRCON0 & BIT(2)) {
        rep_cnt = 0;
        IRCON0 &= ~BIT(2);
        ir_data = IRDAT2;   //IRDAT0��IRDAT1���û��룬������Ա���Ը����Լ����������ж�����
        //printf("%02x\n",ir_data);
        userCode16 = IRDAT1;
		userCode16 = (userCode16 << 8) | IRDAT0;
    }
    if (rep_cnt < 23) {
        rep_cnt++;
    } else {                //����110 msû�к��ⰴ��
        ir_data = 0xff;        //PLZ make sure ir_table[irData] is NO_KEY
        rep_cnt = 30;       //time out
    }

    return ir_key_get(ir_data & 0xff);
}

#endif //HARD_IR_EN

#endif //(HARD_IR_EN || SOFT_IR_EN)
