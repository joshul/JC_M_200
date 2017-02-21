/*******************************************************************************************
 File Name:

 Version: 1.00

 Discription:

 Author:juntham

 Date:

 Copyright:(c) 2008 by Buildwin, All Rights Reserved.
*******************************************************************************************/
#ifndef __TM1629_H__
#define __TM1629_H__

#include "include.h"

//#define Delay_myself	{asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");asm("nop");asm("nop");asm("nop"); asm("nop"); asm("nop");asm("nop");asm("nop");asm("nop");}

#if 0
#define TM_STB_OUTPUT    P1DIR  &= ~ BIT(1)
#define TM_STB_HIGH      P1     |=   BIT(1)
#define TM_STB_LOW       P1     &= ~ BIT(1)

#define TM_CLK_OUTPUT    P1DIR  &= ~ BIT(3)
#define TM_CLK_HIGH      P1     |=   BIT(3)
#define TM_CLK_LOW       P1     &= ~ BIT(3)

#define	TM_DIO_OUTPUT    P1DIR  &= ~ BIT(2)
#define TM_DIO_HIGH      P1     |=   BIT(2)
#define	TM_DIO_LOW       P1     &= ~ BIT(2)
#define TM_DIO_INPUT     P1DIR  |=   BIT(2)

#define TM_STB_OUTPUT    P1DIR  &= ~ BIT(5)
#define TM_STB_HIGH      P1     |=   BIT(5)
#define TM_STB_LOW       P1     &= ~ BIT(5)

#define TM_CLK_OUTPUT    P3DIR  &= ~ BIT(1)
#define TM_CLK_HIGH      P3     |=   BIT(1)
#define TM_CLK_LOW       P3     &= ~ BIT(1)

#define	TM_DIO_OUTPUT    P3DIR  &= ~ BIT(0)
#define TM_DIO_HIGH      P3     |=   BIT(0)
#define	TM_DIO_LOW       P3     &= ~ BIT(0)
#define TM_DIO_INPUT     P3DIR  |=   BIT(0)
#endif
#define TM_STB_OUTPUT    P1DIR  &= ~ BIT(2)
#define TM_STB_HIGH      P1     |=   BIT(2)
#define TM_STB_LOW       P1     &= ~ BIT(2)

#define TM_CLK_OUTPUT    P2DIR  &= ~ BIT(0)
#define TM_CLK_HIGH      P2     |=   BIT(0)
#define TM_CLK_LOW       P2     &= ~ BIT(0)

#define	TM_DIO_OUTPUT    P2DIR  &= ~ BIT(1)
#define TM_DIO_HIGH      P2     |=   BIT(1)
#define	TM_DIO_LOW       P2     &= ~ BIT(1)
#define TM_DIO_INPUT     P2DIR  |=   BIT(1)

//---------------------------------LED_DEFINE-----------------------------------------------
#if 0
#define ICON_A              0x0100
#define ICON_B              0x4000
#define ICON_C              0x0001
#define ICON_D              0x0008
#define ICON_E              0x0020
#define ICON_F              0x0200
#define ICON_G1             0x0080
#define ICON_G2             0x2000
#define ICON_H              0x0400
#define ICON_I              0x0800
#define ICON_J              0x1000
#define ICON_K              0x0010
#define ICON_L              0x0004
#define ICON_M              0x0002
#define ICON_N              0x0040
#define CHAR_MASK           0x0000
//#else
#define ICON_A              0x4000
#define ICON_B              0x0100
#define ICON_C              0x0080
#define ICON_D              0x0010
#define ICON_E              0x0001
#define ICON_F              0x2000
#define ICON_G1             0x0004
#define ICON_G2             0x0200
#define ICON_H              0x1000
#define ICON_I              0x0800
#define ICON_J              0x0400
#define ICON_K              0x0008
#define ICON_L              0x0020
#define ICON_M              0x0040
#define ICON_N              0x0002
#define CHAR_MASK           0x0000
#endif

#define ICON_A              0x0020
#define ICON_B              0x0002
#define ICON_C              0x0004
#define ICON_D              0x0008
#define ICON_E              0x0100
#define ICON_F              0x0400
#define ICON_G              0x1000
#define ICON_H              0x4000
#define ICON_I              0x8000
#define ICON_J              0x0080
#define ICON_K              0x0040
#define ICON_L              0x0200
#define ICON_M              0x0800
#define ICON_N              0x2000
#define ICON_O              0x0001
#define ICON_P              0x0010

#define CHAR_COAX           (ICON_D)
#define CHAR_AUX          	(ICON_C)//AUX
#define CHAR_OPT            (ICON_B)//OPT
#define CHAR_3D             (ICON_A)//3D
#define CHAR_MP3            (ICON_A)//MP3
#define CHAR_WMA            (ICON_B)//WMR
#define CHAR_TV             (ICON_C)//AAC
#define CHAR_MUSIC          (ICON_D)//WAV
#define CHAR_MOVIE          (ICON_E)//FLAC
#define CHAR_POWER          (ICON_F)//POWER
#define CHAR_STOP           (ICON_I)//STOP
#define CHAR_PLAY           (ICON_H)//PLAY
#define CHAR_BT_PLAY        (ICON_G)//LOOP
#define CHAR_BT          	(ICON_F)//TF
#define CHAR_USB            (ICON_E)//USB
#define CHAR_DOT1           (ICON_J)//DOT1
#define CHAR_DOT2           (ICON_I)//DOT2


#define CHAR_MINUS          (ICON_L |ICON_M)
#define CHAR_MINUS_         (ICON_G1|ICON_G2)

#define CHAR_MASK           0x0000

#define ICON_G1             0x0004
#define ICON_G2             0x0200


#define CHAR_21             (ICON_B)
#define CHAR_51          	(ICON_C)
//#define CHAR_AUX          	(ICON_D)
#define CHAR_VIDEO          (ICON_E)



#define ICON_O2              (ICON_G2)///84
#define ICON_O3              (ICON_H)
#define ICON_O4              (ICON_I)
#define ICON_O5              (ICON_J)

#define ICON_P1              (ICON_G2)///85
#define ICON_P2              (ICON_H)
#define ICON_P3              (ICON_I)
#define ICON_P4              (ICON_J)
#define ICON_P5              (ICON_M)

#define ICON_Q2              (ICON_G2)///86
#define ICON_Q3              (ICON_H)
#define ICON_Q4              (ICON_I)
#define ICON_Q5              (ICON_J)

#define ICON_R1              (ICON_A)
#define ICON_R2              (ICON_B)
#define ICON_R3              (ICON_C)
#define ICON_R4              (ICON_D)
#define ICON_R5              (ICON_E)
     
#define ICON_S2              (ICON_F)
#define ICON_S3              (ICON_G1)
#define ICON_S4              (ICON_G2)
#define ICON_S5              (ICON_H)

#define ICON_T1              (ICON_I)
#define ICON_T2              (ICON_J)
#define ICON_T3              (ICON_M)
#define ICON_T4              (ICON_L)
#define ICON_T5              (ICON_K)

#define CHAR_PREV         (ICON_J|ICON_M|ICON_B|ICON_C|ICON_I|ICON_L) 
#define CHAR_NEXT         (ICON_H|ICON_K|ICON_E|ICON_F|ICON_I|ICON_L) 

#define CHAR_0            (ICON_A|ICON_C|ICON_D|ICON_E|ICON_G|ICON_H|ICON_I|ICON_P)   
#define CHAR_1            (ICON_J|ICON_O)
#define CHAR_2            (ICON_A|ICON_C|ICON_E|ICON_G|ICON_L|ICON_M)   
#define CHAR_3            (ICON_A|ICON_C|ICON_D|ICON_E|ICON_L|ICON_M)
#define CHAR_4            (ICON_C|ICON_D|ICON_H|ICON_L|ICON_M)
#define CHAR_5            (ICON_A|ICON_D|ICON_E|ICON_H|ICON_L|ICON_M)
#define CHAR_6            (ICON_A|ICON_D|ICON_E|ICON_G|ICON_H|ICON_L|ICON_M)
#define CHAR_7            (ICON_A|ICON_C|ICON_D)
#define CHAR_8            (ICON_A|ICON_C|ICON_D|ICON_E|ICON_G|ICON_H|ICON_L|ICON_M)
#define CHAR_9            (ICON_A|ICON_C|ICON_D|ICON_E|ICON_H|ICON_L|ICON_M)

#define CHAR_2_           (ICON_A|ICON_B|ICON_D|ICON_E|ICON_G1|ICON_G2)    
#define CHAR_3_           (ICON_A|ICON_B|ICON_C|ICON_D|ICON_G1|ICON_G2)
#define CHAR_4_           (ICON_B|ICON_C|ICON_G1|ICON_G2|ICON_F)
#define CHAR_5_           (ICON_A|ICON_C|ICON_D|ICON_F|ICON_G1|ICON_G2)
#define CHAR_6_           (ICON_A|ICON_C|ICON_D|ICON_E|ICON_F|ICON_G1|ICON_G2)
#define CHAR_8_           (ICON_A|ICON_B|ICON_C|ICON_D|ICON_E|ICON_F|ICON_G1|ICON_G2)
#define CHAR_9_           (ICON_A|ICON_B|ICON_C|ICON_D|ICON_F|ICON_G1|ICON_G2)

#define CHAR_A_            (ICON_A|ICON_B|ICON_C|ICON_E|ICON_F|ICON_G1|ICON_G2)
#define CHAR_B_            (ICON_C|ICON_D|ICON_E|ICON_F|ICON_G1|ICON_G2)
#define CHAR_D_            (ICON_B|ICON_C|ICON_D|ICON_E|ICON_G1|ICON_G2)
#define CHAR_E_            (ICON_A|ICON_D|ICON_E|ICON_F|ICON_G1|ICON_G2)
#define CHAR_F_            (ICON_A|ICON_E|ICON_G1|ICON_G2|ICON_F)
#define CHAR_G_            (ICON_A|ICON_B|ICON_C|ICON_D|ICON_F|ICON_G1|ICON_G2)
#define CHAR_H_            (ICON_B|ICON_C|ICON_E|ICON_G1|ICON_G2|ICON_F)
#define CHAR_K_            (ICON_E|ICON_G1|ICON_G2|ICON_F|ICON_J|ICON_M)
#define CHAR_M_            (ICON_B|ICON_C|ICON_E|ICON_F|ICON_H|ICON_J)
#define CHAR_N_            (ICON_A|ICON_B|ICON_C|ICON_E|ICON_F)//(ICON_B|ICON_C|ICON_E|ICON_F|ICON_H|ICON_M)
#define CHAR_P_            (ICON_A|ICON_B|ICON_E|ICON_F|ICON_G1|ICON_G2)
#define CHAR_Q_            (ICON_A|ICON_B|ICON_C|ICON_F|ICON_G1|ICON_G2)
#define CHAR_R_            (ICON_A|ICON_B|ICON_E|ICON_F|ICON_G1|ICON_G2|ICON_M)
#define CHAR_S_            (ICON_A|ICON_C|ICON_D|ICON_F|ICON_G1|ICON_G2)
#define CHAR_t_            (ICON_D|ICON_E|ICON_F|ICON_G1|ICON_G2)
#define CHAR_T_            (ICON_A|ICON_I|ICON_L)
#define CHAR_V_            (ICON_E|ICON_F|ICON_J|ICON_K)
#define CHAR_W_            (ICON_B|ICON_C|ICON_E|ICON_F|ICON_K|ICON_M)
#define CHAR_X_            (ICON_H|ICON_J|ICON_K|ICON_M)
#define CHAR_Y_            (ICON_H|ICON_J|ICON_L)
#define CHAR_Z_            (ICON_A|ICON_D|ICON_J|ICON_K)

#define CHAR_A            (ICON_A|ICON_C|ICON_D|ICON_G|ICON_H|ICON_L|ICON_M)
#define CHAR_B            (ICON_A|ICON_C|ICON_D|ICON_E|ICON_J|ICON_O|ICON_M)
#define CHAR_C            (ICON_A|ICON_E|ICON_G|ICON_H)
#define CHAR_D            (ICON_A|ICON_C|ICON_D|ICON_E|ICON_J|ICON_O)
#define CHAR_E            (ICON_A|ICON_E|ICON_G|ICON_H|ICON_L|ICON_M)
#define CHAR_F            (ICON_A|ICON_G|ICON_H|ICON_L|ICON_M)
#define CHAR_G            (ICON_A|ICON_C|ICON_D|ICON_E|ICON_J)
#define CHAR_H            (ICON_C|ICON_D|ICON_G|ICON_H|ICON_L|ICON_M)
#define CHAR_I            (ICON_A|ICON_E|ICON_J|ICON_O)
#define CHAR_J            (ICON_A|ICON_C|ICON_D|ICON_P)
//#define CHAR_K            (ICON_A|ICON_B|ICON_C|ICON_D|ICON_E|ICON_G|ICON_F)
#define CHAR_L            (ICON_E|ICON_G|ICON_H)
#define CHAR_M            (ICON_C|ICON_D|ICON_G|ICON_H|ICON_I|ICON_K|ICON_O)
#define CHAR_N            (ICON_C|ICON_D|ICON_G|ICON_H|ICON_I|ICON_P)
#define CHAR_O            (ICON_A|ICON_C|ICON_D|ICON_E|ICON_G|ICON_H)
#define CHAR_P            (ICON_A|ICON_C|ICON_G|ICON_H|ICON_L|ICON_M)
#define CHAR_Q            (ICON_A|ICON_C|ICON_D|ICON_E|ICON_G|ICON_H|ICON_P)
#define CHAR_R            (ICON_A|ICON_C|ICON_G|ICON_H|ICON_L|ICON_M|ICON_P)
#define CHAR_S            (ICON_A|ICON_D|ICON_E|ICON_H|ICON_L|ICON_M)
#define CHAR_T            (ICON_A|ICON_J|ICON_O)
#define CHAR_U            (ICON_C|ICON_D|ICON_E|ICON_G|ICON_H)
#define CHAR_V            (ICON_G|ICON_H|ICON_N|ICON_K)
//#define CHAR_W            (ICON_A|ICON_B|ICON_C|ICON_D|ICON_E|ICON_F|ICON_G)
#define CHAR_X            (ICON_I|ICON_N|ICON_K|ICON_P)
#define CHAR_Y            (ICON_K|ICON_I|ICON_O)
//#define CHAR_Z            (ICON_A|ICON_B|ICON_C|ICON_D|ICON_E|ICON_F|ICON_G)
#define CHAR__            (ICON_L|ICON_M)

//extern  void DispMenu_1629(u8 menu_number);
enum _MENU_TYPE
{
    MENU_LINE_IN,
};

extern  void TM_INIT(void);
//extern __near_func void DispMenu_1629(u8 menu_number);//@"IIC_CODE_API"  //ÃÓ≥‰…®√Ëbuf
extern  void ShowCHAR(void);
extern  void DispMenu_1629(u8 menu_number);//@"IIC_CODE_API"  //ÃÓ≥‰…®√Ëbuf
extern  void MENU_DISPLAY_MASK(void);
extern  void Write_Display_Digtial_1629(s8 value);
void Duty_set(void);

#endif

