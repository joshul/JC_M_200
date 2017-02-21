/*****************************************************************************
 * Module    : IO
 * File      : io_hold.c
* Author     : Hanny
 * Function  : ����task_hold���˳�task_hold��io�������
 ******************************************************************************/
#ifndef CONFIG_C
#pragma location="TASK_HOLD_SEG"
void task_hold_io_sleep(void)
{
    P2DIR |= 0xef;  //�����õ������Ŷ���Ϊ����
#if (LEDSEG_7PIN_SCAN_MODE == SCAN_MODE_COM)
    P2PD &= ~0xef;
#endif
}

#pragma location="TASK_HOLD_SEG"
void task_hold_io_reset(void)
{
    //user_io_init();
    //ledseg_init();
}

#endif


