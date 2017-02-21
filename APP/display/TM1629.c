#include "include.h"
#include "TM1629.h"
#include "lcd.h"
#include "task_NPCP215X.h"


#if TM1629

__no_init volatile u16 temp_1629[8];
//__no_init u16 temp_1629[8];

#if 0
#if BT_MODE_EN
extern u8 bt_status;
#endif
#endif
const u16 DIGIT_FONT[10]  = {CHAR_0, CHAR_1, CHAR_2, CHAR_3, CHAR_4, CHAR_5, CHAR_6, CHAR_7, CHAR_8, CHAR_9};
const u16 DIGIT_FONT_[10] = {CHAR_0, CHAR_1, CHAR_2_,CHAR_3_,CHAR_4_,CHAR_5_,CHAR_6_,CHAR_7,CHAR_8_,CHAR_9_};

#pragma location="IIC_CODE_API"
void WriteData(BYTE Value)
//#pragma location="LCD_1629_API"
//void WriteData(BYTE Value)
{
	BYTE i;
	for(i=0;i<8;i++)
	{
		TM_CLK_LOW;
		if(Value & 0x01)TM_DIO_HIGH;
		else            TM_DIO_LOW;
		Delay_myself;
		TM_CLK_HIGH;
		Value >>= 1;
	}
}
//#pragma location="LCD_1629_API"
//__near_func void WriteData(BYTE Value)
#pragma location="IIC_CODE_API"
void TM_INIT(void)
{
	TM_STB_OUTPUT;
	TM_CLK_OUTPUT;
	TM_DIO_OUTPUT;
	
	TM_STB_HIGH;			  //Initial state
	TM_CLK_HIGH;			  //Initial state
#if 1
	TM_STB_LOW;
	//WriteData(0x8f);			//Command 3, Display ON //10/16 PWM
	//sys_ctl.dim_level = 7;
	Duty_set();
	TM_STB_HIGH;
#endif 
    //sys_ctl.dim_level = 3;
    //Dim_Duty_set(0);////
}
#pragma location="IIC_CODE_API"
//__near_func void TM_INIT(void)
//#pragma location="LCD_1629_API"
void ShowCHAR(void)
{

    TM_STB_LOW;
	WriteData(0x40);					//Command 1, Fixed address
	TM_STB_HIGH;	
	Delay_myself;
	
	TM_STB_LOW;
	WriteData(0xc0);					//Command 2	
	#if 0
	WriteData((temp_1629[4]&0x00ff)   );//c1
	WriteData((temp_1629[4]&0xff00)>>8);
	
	WriteData((temp_1629[2]&0x00ff)   );//D3
	WriteData((temp_1629[2]&0xff00)>>8);
	
	WriteData((temp_1629[1]&0x00ff)   );//D2
	WriteData((temp_1629[1]&0xff00)>>8);
	
	WriteData((temp_1629[0]&0x00ff)	 );//D1
	WriteData((temp_1629[0]&0xff00)>>8);
	
	WriteData((temp_1629[3]&0x00ff)	 );//D4
	WriteData((temp_1629[3]&0xff00)>>8);
	
	WriteData((temp_1629[6]&0x00ff)	 );//D6
	WriteData((temp_1629[6]&0xff00)>>8);
	
	WriteData((temp_1629[5]&0x00ff)	 );//Dchar
	WriteData((temp_1629[5]&0xff00)>>8);
	
	WriteData((temp_1629[7]&0x00ff)   );//Dflash
	WriteData((temp_1629[7]&0xff00)>>8);
	#else
        
    WriteData((temp_1629[7]&0x00ff)   );//c2
	WriteData((temp_1629[7]&0xff00)>>8);
    WriteData((temp_1629[6]&0x00ff)   );//c2
	WriteData((temp_1629[6]&0xff00)>>8);  
    WriteData((temp_1629[5]&0x00ff)   );//c2
	WriteData((temp_1629[5]&0xff00)>>8);
    WriteData((temp_1629[4]&0x00ff)   );//c2
	WriteData((temp_1629[4]&0xff00)>>8);
    WriteData((temp_1629[2]&0x00ff)   );//c2
	WriteData((temp_1629[2]&0xff00)>>8);
    WriteData((temp_1629[3]&0x00ff)   );//c2
	WriteData((temp_1629[3]&0xff00)>>8);
    WriteData((temp_1629[1]&0x00ff)   );//c1
	WriteData((temp_1629[1]&0xff00)>>8);
    WriteData((temp_1629[0]&0x00ff)   );//c0
	WriteData((temp_1629[0]&0xff00)>>8);
#if 0    
    WriteData((temp_1629[0]&0x00ff)   );//c0
	WriteData((temp_1629[0]&0xff00)>>8);
	WriteData((temp_1629[1]&0x00ff)   );//c1
	WriteData((temp_1629[1]&0xff00)>>8);
	WriteData((temp_1629[2]&0x00ff)   );//c2
	WriteData((temp_1629[2]&0xff00)>>8);
	WriteData((temp_1629[4]&0x00ff)   );//c4
	WriteData((temp_1629[4]&0xff00)>>8);
	WriteData((temp_1629[7]&0x00ff)	 );//c7////rst
	WriteData((temp_1629[7]&0xff00)>>8);
	WriteData((temp_1629[5]&0x00ff)	 );//c5
	WriteData((temp_1629[5]&0xff00)>>8);
	WriteData((temp_1629[6]&0x00ff)	 );//c6///icon
	WriteData((temp_1629[6]&0xff00)>>8);
	WriteData((temp_1629[3]&0x00ff)	 );//c3
	WriteData((temp_1629[3]&0xff00)>>8);
	WriteData((temp_1629[0]&0x00ff)   );//c0
	WriteData((temp_1629[0]&0xff00)>>8);
    
#endif

	#endif
	TM_STB_HIGH;
	Delay_myself;

	TM_STB_LOW;
	//WriteData(0x8f);			//Command 3, Display ON //10/16 PWM
	WriteData(sys_ctl.TM1629_input);
	TM_STB_HIGH;
	
}
#if 0

#pragma location="FRE_SHOW_API"
void Write_Display_Protect_(void)
{
	temp_1629[3]&= (ICON_O2|ICON_O3|ICON_O4|ICON_O5);
	temp_1629[4]&= (ICON_P1|ICON_P2|ICON_P3|ICON_P4|ICON_P5);
	temp_1629[5]&= (ICON_Q2|ICON_Q3|ICON_Q4|ICON_Q5);
	temp_1629[7]&= (ICON_R1|ICON_R2|ICON_R3|ICON_R4|ICON_R5|ICON_T1|ICON_T2|ICON_T3|ICON_T4|ICON_T5|ICON_S2|ICON_S3|ICON_S4|ICON_S5);
}
#pragma location="LCD_1629_API"
void Write_Display_Digtial_1629(s8 value)
{
	Write_Display_Protect_();
	temp_1629[6]&= ~ CHAR_MASK;
	if(value < 0) 
	{		
		value = -value;		
		if(value > 9)
		{ 
			temp_1629[4] |= (DIGIT_FONT[value/10]|CHAR_MINUS);
		}
		else 
		{	
			temp_1629[4] |= CHAR_MINUS;
		}
	}
	else 
	{
//		if(cur_menu==MENU_VOLUE)
			temp_1629[4] |= DIGIT_FONT[value/10];
		//else if(DIGIT_FONT[value/10]<1)
		//	temp_1629[4] = CHAR_MASK;
	}
	temp_1629[5] |= DIGIT_FONT[value%10];
}

//#pragma location="IIC_CODE_API"
#pragma location="LCD_1629_API"
void Write_Display_VOL(u8 style)
{
	s8 vol;
	temp_1629[0]=CHAR_MASK;
	temp_1629[1]=CHAR_MASK;
	switch(style)
	{
		#if PT2358
		case MENU_FL:
			temp_1629[0]=CHAR_F_;
			temp_1629[1]=CHAR_R_;
			temp_1629[2]=CHAR_T_;
			vol=fl_vol;
			break;
		case MENU_CEN:
			temp_1629[0]=CHAR_C;
			temp_1629[1]=CHAR_E_;
			temp_1629[2]=CHAR_N;
			vol=cen_vol;
			break;
		case MENU_SL:
			temp_1629[0]=CHAR_S_;
			temp_1629[1]=CHAR_U;
			temp_1629[2]=CHAR_R_;
			vol=sl_vol;
			break;
		case MENU_SUB:
			temp_1629[0]=CHAR_S_;
			temp_1629[1]=CHAR_U;
			temp_1629[2]=CHAR_B_;
			vol=sw_vol;
			break;
		case MENU_VOLUE:
			temp_1629[0]=CHAR_V_;
			temp_1629[1]=CHAR_O;
			temp_1629[2]=CHAR_L;
			vol=main_vol;
			break;
		#endif

		#if CS2313
		case MENU_TRE:
			temp_1629[2]=CHAR_H_;
			vol=Trable_vol;
			break;
		//case MENU_BASS:
		//	temp_1629[0]=CHAR_B_;
		//	temp_1629[1]=CHAR_A_;
		//	vol=Bass_vol;
		//	break;
		case MENU_SW:
			temp_1629[2]=CHAR_B_;
			vol=SW_vol;
			break;
		case MENU_VOLUE:
			temp_1629[0]=CHAR_V_;
			temp_1629[1]=CHAR_O;
			temp_1629[2]=CHAR_L;
			vol=main_vol;
			break;
		#endif

		default:
			break;
	}
#if (CS2313 || PT2358)
	Write_Display_Digtial_1629(vol);
#endif
}
#endif
#if 0
#if 1
#pragma location="LCD_1629_API"
void Write_Display_FL_VOL(void)
{
	temp_1629[0]=CHAR_F_;
	temp_1629[1]=CHAR_R_;
	temp_1629[2]=CHAR_T_;
	//temp[3]=CHAR_MASK;
	Write_Display_Digtial_1629(fl_vol);
	//temp_1629[6] &= ~ CHAR_MASK;
}
#pragma location="LCD_1629_API"
void Write_Display_CE_VOL(void)
{
	temp_1629[0]=CHAR_C;
	temp_1629[1]=CHAR_E_;
	temp_1629[2]=CHAR_N;
	//temp[3]=CHAR_MASK;
	Write_Display_Digtial_1629(cen_vol);
	//temp_1629[6] &= ~ CHAR_MASK;
}
#pragma location="LCD_1629_API"
void Write_Display_SL_VOL(void)
{
	temp_1629[0]=CHAR_S_;
	temp_1629[1]=CHAR_U;
	temp_1629[2]=CHAR_R_;
	//temp[3]=CHAR_MASK;
	Write_Display_Digtial_1629(sl_vol);
	//temp_1629[6] &= ~ CHAR_MASK;
}
#pragma location="LCD_1629_API"
void Write_Display_SB_VOL(void)
{
	temp_1629[0]=CHAR_S_;
	temp_1629[1]=CHAR_U;
	temp_1629[2]=CHAR_B_;
	//temp[3]=CHAR_MASK;
	Write_Display_Digtial_1629(sw_vol);
	//temp_1629[6] &= ~ CHAR_MASK;
}
#else
#pragma location="LCD_1629_API"
void Write_Display_TRE_VOL(void)
{
	temp[0]=CHAR_H_;
	temp[1]=CHAR_MASK;
	temp[2]=CHAR_MASK;
	temp[3]=CHAR_MASK;
	Write_Display_Digtial(Trable_vol);
	temp[6] &= ~ CHAR_MASK;
}
#pragma location="LCD_1629_API"
void Write_Display_SW_VOL(void)
{
	temp[0]=CHAR_S_;
	temp[1]=CHAR_B_;
	temp[2]=CHAR_MASK;
	temp[3]=CHAR_MASK;
	Write_Display_Digtial(Bass_vol);
	temp[6] &= ~ CHAR_MASK;
}
#pragma location="LCD_1629_API"
void Write_Display_BASS_VOL(void)
{
	temp[0]=CHAR_B_;
	temp[1]=CHAR_A_;
	temp[2]=CHAR_MASK;
	temp[3]=CHAR_MASK;
	Write_Display_Digtial(Sw_vol);
	temp[6] &= ~ CHAR_MASK;
}
#endif
#pragma location="LCD_1629_API"
void Write_Display_MAIN_VOL(void)
{
	temp_1629[0]=CHAR_V_;
	temp_1629[1]=CHAR_O;
	temp_1629[2]=CHAR_L;
	//temp[3]=CHAR_MASK;
	Write_Display_Digtial_1629(main_vol);
}

#pragma location="LCD_1629_API"
void Write_Display_Number(u16 num1)
{
    u8 ii;
    for(ii = 6; ii > 3; ii--)
    {
        temp_1629[ii-1] = DIGIT_FONT[num1 % 10];
        num1 /= 10;
    }
	temp_1629[2] = DIGIT_FONT_[num1 % 10];
}
#pragma location="LCD_1629_API"
void Write_Display_FM_Frequency(u16 fmFrequency)
{
    u8 ii;
	temp_1629[3] = DIGIT_FONT[fmFrequency % 10];
	fmFrequency /= 10;
    for(ii = 3; ii > 0; ii--)
    {
        temp_1629[ii-1] = DIGIT_FONT_[fmFrequency % 10];
        fmFrequency /= 10;
    }
	if(temp_1629[0]==CHAR_0)temp_1629[0]=CHAR_MASK;
}

#pragma location="FRE_SHOW_API"
void MENU_DISPLAY_MASK(void)
{
	u8 i;
    for(i=0;i<8;i++)
        temp_1629[i] = CHAR_MASK;
}

#pragma location="FRE_SHOW_API"
void Write_Display_Work_Mode(u8 style)
{
	MENU_DISPLAY_MASK();
	switch(style)
	{
		case MENU_LINE_IN:
			temp_1629[0]=CHAR_A_;
			temp_1629[1]=CHAR_U;
			temp_1629[2]=CHAR_X_;
			temp_1629[6] = CHAR_AUX;
			break;
		//case MENU_DVD:
		//	temp_1629[0]=CHAR_A_;
		//	temp_1629[1]=CHAR_C;
		//	temp_1629[2]=CHAR_3_;
		//	temp_1629[6] = CHAR_51;
		//	break;
		case MENU_FM_FREQ:
			Write_Display_FM_Frequency(fm_freq);
			temp_1629[2]|= CHAR_DP1;
			temp_1629[6] = CHAR_MHz;
			break;
		case MENU_BT:
#if BT_MODE_EN
			temp_1629[1] = CHAR_B_;
			temp_1629[2] = CHAR_t_;
			temp_1629[6] = CHAR_BT;
			if((bt_status<BT_STATUS_CONNECTED)&&(bonoff))
			{
				temp_1629[1] = CHAR_MASK;
				temp_1629[2] = CHAR_MASK;
			}
			//if((bt_status<BT_STATUS_CONNECTED)&&(bt_first_in)&&(bonoff))temp_1629[6] = CHAR_MASK;
			if(bt_status>=BT_STATUS_CONNECTED)
			{
				if(bt_status==BT_STATUS_PLAYING)temp_1629[6] |= CHAR_PLAY;
				else temp_1629[6] |= CHAR_PAUSE;
			}
#endif
			break;
		case MENU_USB_SD:
			if(device_active==0x02)
			{
				temp_1629[0] = CHAR_U;
				temp_1629[1] = CHAR_S_;
				temp_1629[2] = CHAR_B_;
			}
			else
			{
				temp_1629[1] = CHAR_S_;
				temp_1629[2] = CHAR_D_;
			}
			temp_1629[6] = CHAR_USBSD;
			break;
	}
	if((style==MENU_BT)||(style==MENU_DVD)||(style==MENU_LINE_IN))
	{
		temp_1629[4] = DIGIT_FONT[main_vol/10];
		temp_1629[5] = DIGIT_FONT[main_vol%10];
	}
	//if(style!=MENU_DVD)
	//{
		temp_1629[6]|=	CHAR_21;
		//if(!bMode)temp_1629[6] |= CHAR_21;
		//else	  temp_1629[6] |= CHAR_51;
	//}
}
#pragma location="FRE_SHOW_API"
void Write_Display_Mode(void)
{
	#if PT2358
	if(work_mode==DVD_MODE)
	{
		temp_1629[0]=CHAR_MASK;
		temp_1629[5] &= ~ CHAR_8_;
		if(bProgic)
		{
			temp_1629[1]=CHAR_A_;
			temp_1629[2]=CHAR_C;
			temp_1629[3]=CHAR_MINUS;
			temp_1629[4]=CHAR_3;
		}
		else
		{
			temp_1629[1]=CHAR_MASK;
			temp_1629[2]=CHAR_3_;
			temp_1629[3]=CHAR_D;
			temp_1629[4]=CHAR_MASK;
		}
	}
	else
	{
		if(!bMode)
		 {
			temp_1629[6] |=   CHAR_21;
			temp_1629[6] &= ~ CHAR_51;
		 }
		else
		 {
			temp_1629[6] |=   CHAR_51;
			temp_1629[6] &= ~ CHAR_21;
		 }
	}
	#endif

	#if CS2313
	temp_1629[0]=CHAR_0;
	if(!bMode)
	{
		temp_1629[1]=CHAR_F_;
		temp_1629[2]=CHAR_F_;
	}
	else
	{
		temp_1629[1]=CHAR_N_;
		temp_1629[2]=CHAR_MASK;
	}
	temp_1629[3]=CHAR_MASK;
	temp_1629[4]=CHAR_MASK;
	temp_1629[5]=CHAR_MASK;
	//temp_1629[6]=CHAR_MASK;
	//Write_Display_Digtial_1629(main_vol);
	#endif
}
#pragma location="LCD_1629_API"
//__near_func void Write_Display_Work_Mode(u8 style)
//#pragma location="LCD_1629_API"
void Write_Display_TIME(void)
{   
	//if(!player.play_time)
	//{
		Write_Display_Protect_();
		temp_1629[0] = CHAR_MASK;
		temp_1629[1] = CHAR_MASK;
		cur_play_time.min = player.play_time / 60;
		//if (cur_play_time.min > 99)
		//{
		//	cur_play_time.min = 99;
		//}
		temp_1629[2] = DIGIT_FONT[cur_play_time.min /10];
		temp_1629[3]|= DIGIT_FONT[cur_play_time.min %10];
		cur_play_time.sec = player.play_time % 60;
		temp_1629[4]|= DIGIT_FONT[cur_play_time.sec /10];
		temp_1629[5]|= DIGIT_FONT[cur_play_time.sec %10];
		temp_1629[6] = (CHAR_USBSD|CHAR_21);
		temp_1629[3]|= CHAR_DP2_2;
		if(play_mode==REPEAT_ONE)temp_1629[6]|= CHAR_ONE;
		else                     temp_1629[6]|= CHAR_ALL;
		if(play_pause)           temp_1629[6]|= CHAR_PLAY;
		else                     temp_1629[6]|= CHAR_PAUSE;
		//if(!bMode)               temp_1629[6]|=	CHAR_21;
		//else                     temp_1629[6]|= CHAR_51;
		//temp_1629[6]|=	CHAR_21;
	//}
}
#pragma location="LCD_1629_API"
void MENU_USB_SD_REPEAT(void)
{
	temp_1629[0] = CHAR_M_;
	temp_1629[1] = CHAR_P_;
	temp_1629[2] = CHAR_MASK;
	temp_1629[6]&= ~(CHAR_ONE|CHAR_ALL);
	if(play_mode==REPEAT_ONE)
	{
		temp_1629[6]|= CHAR_ONE;
		temp_1629[3] = CHAR_0;
		temp_1629[4] = CHAR_N;
		temp_1629[5] = CHAR_E;
	}
	else					 
	{
		temp_1629[6]|= CHAR_ALL;
		temp_1629[3] = CHAR_A;
		temp_1629[4] = CHAR_L;
		temp_1629[5] = CHAR_L;
	}
}
#pragma location="LCD_1629_API"
void MENU_FILE_NUM(void)
{
	temp_1629[0] =  CHAR_MASK;
	temp_1629[1] =  CHAR_MASK;
	Write_Display_Number(fs_apiinfo.file_number);
	temp_1629[3]&=~ CHAR_DP2_2;
	temp_1629[7] =  CHAR_MASK;
	temp_1629[6] =  (CHAR_USBSD|CHAR_21|CHAR_PLAY);
	if(play_mode==REPEAT_ONE)temp_1629[6]|= CHAR_ONE;
	else					 temp_1629[6]|= CHAR_ALL;
}
#pragma location="LCD_FUN_ENCODE"
void MENU_FM_NUMBER(void)
{
	temp_1629[0] = CHAR_F_;
	temp_1629[1] = CHAR_M_;
	temp_1629[2] = CHAR_MASK;
	temp_1629[3] = (CHAR_P|CHAR_DP2_2);
	temp_1629[4] = DIGIT_FONT[fm_cur_ch/10];
	temp_1629[5] = DIGIT_FONT[fm_cur_ch%10];
	temp_1629[6]&= ~ CHAR_MHz;
	//temp_1629[2]&= ~ CHAR_DP1;
	//temp_1629[3]|=  CHAR_DP2_2;
	temp_1629[7] =  CHAR_MASK;
}
#pragma location="LCD_1629_API"
void Write_Display_Power_Mute(void)
{
	MENU_DISPLAY_MASK();
	if(work_mode==OFF_MODE)
	{
		//temp_1629[1] = CHAR_MINUS_;
		//temp_1629[2] = CHAR_MINUS_;
		temp_1629[3] = CHAR_MINUS;
		temp_1629[4] = CHAR_MINUS;
		temp_1629[5] = CHAR_MINUS;
	}
	else if(volume_mute_falg)
	{
		temp_1629[0] = CHAR_M_;
		temp_1629[1] = CHAR_U;
		temp_1629[2] = CHAR_T_;
		temp_1629[3] = CHAR_E;
	}
}

#pragma location="LCD_1629_API"
void MENU_SELECT_MP3_FM_NUMBER(void)
{
	if(work_mode==MUSIC_PLAY_MODE)
	{
		temp_1629[0] = CHAR_MASK;
		temp_1629[1] = CHAR_MASK;
		temp_1629[2] = CHAR_0;
		temp_1629[3] = DIGIT_FONT[m_Couunt3%10];
	}
	else 
	{
		temp_1629[0] = CHAR_F_;
		temp_1629[1] = CHAR_M_;
		temp_1629[2] = CHAR_MASK;
		temp_1629[3] = (CHAR_P|CHAR_DP2_2);
		temp_1629[6]&= ~ CHAR_MHz;
		//temp_1629[2]&= ~ CHAR_DP1;
		//temp_1629[3]|=	 CHAR_DP2_2;
		temp_1629[7] =	 CHAR_MASK;
	}
	temp_1629[4] = DIGIT_FONT[m_Couunt2%10];
	temp_1629[5] = DIGIT_FONT[m_Couunt1%10];
}
#pragma location="LCD_1629_API"
//#pragma location="LED_SCAN"
//__near_func void MENU_DISPLAY_FLASH(void)
void MENU_DISPLAY_FLASH(void)
{
	temp_1629[3]&=~ (ICON_O2|ICON_O3|ICON_O4|ICON_O5);
	temp_1629[4]&=~ (ICON_P1|ICON_P2|ICON_P3|ICON_P4|ICON_P5);
	temp_1629[5]&=~ (ICON_Q2|ICON_Q3|ICON_Q4|ICON_Q5);
	temp_1629[7] =  CHAR_MASK;
	///*
	if(Flash_Count == 2)
	{
		temp_1629[3]|= (ICON_O2);
		temp_1629[4]|= (ICON_P1);
		temp_1629[5]|= (ICON_Q2);
		temp_1629[7] = ICON_R1|ICON_T1;
	}
	else if(Flash_Count == 3)
	{
		temp_1629[3]|= (ICON_O2);
		temp_1629[4]|= (ICON_P1|ICON_P2|ICON_P3);
		temp_1629[5]|= (ICON_Q2);
		temp_1629[7] = ICON_R1|ICON_R2|ICON_T1|ICON_T2|ICON_T3;
	}//break;
	else if(Flash_Count == 4)
	{
		temp_1629[3]|= (ICON_O2|ICON_O3);
		temp_1629[4]|= (ICON_P1|ICON_P2);
		temp_1629[5]|= (ICON_Q2|ICON_Q3);
		temp_1629[7] = ICON_R1|ICON_R2|ICON_R3|ICON_R4|ICON_T1|ICON_T2|ICON_S2|ICON_S3;
	}//break;
	else if(Flash_Count == 5)
	{
		temp_1629[3]|= (ICON_O2|ICON_O3);
		temp_1629[4]|= (ICON_P1|ICON_P2|ICON_P3|ICON_P4);
		temp_1629[5]|= (ICON_Q2);
		temp_1629[7] = ICON_R1|ICON_R2|ICON_R3|ICON_T1|ICON_T2|ICON_T3|ICON_S2|ICON_S3;
	}//break;
	else if(Flash_Count == 6)
	{
		temp_1629[3]|= (ICON_O2|ICON_O3|ICON_O4);
		temp_1629[4]|= (ICON_P1|ICON_P2|ICON_P3);
		temp_1629[5]|= (ICON_Q2|ICON_Q3|ICON_Q4|ICON_Q5);
		temp_1629[7] = ICON_R1|ICON_R2|ICON_R3|ICON_T1|ICON_T2|ICON_T3|ICON_S2|ICON_S3|ICON_S4;
	}//break;
	else if(Flash_Count == 7)
	{
		temp_1629[3]|= (ICON_O2|ICON_O3|ICON_O4);
		temp_1629[4]|= (ICON_P1|ICON_P2|ICON_P3|ICON_P4|ICON_P5);
		temp_1629[5]|= (ICON_Q2|ICON_Q3|ICON_Q4);
		temp_1629[7] = ICON_R1|ICON_R2|ICON_R3|ICON_R4|ICON_R5|ICON_T1|ICON_T2|ICON_T3|ICON_T4|ICON_S2|ICON_S3;
	}//break;
	else if(Flash_Count == 8)
	{
		temp_1629[3]|= (ICON_O2|ICON_O3|ICON_O4|ICON_O5);
		temp_1629[4]|= (ICON_P1|ICON_P2|ICON_P3|ICON_P4|ICON_P5);
		temp_1629[5]|= (ICON_Q2|ICON_Q3|ICON_Q4|ICON_Q5);
		temp_1629[7] = ICON_R1|ICON_R2|ICON_R3|ICON_R4|ICON_R5|ICON_T1|ICON_T2|ICON_T3|ICON_T4|ICON_T5|ICON_S2|ICON_S3|ICON_S4|ICON_S5;
	}//break;
	//*/
}
#pragma location="LCD_1629_API"
//#pragma location="IIC_CODE_API"
//#pragma location="LED_SCAN"
//__near_func void MENU_POWER_FLASH(void)
void MENU_POWER_FLASH(void)
{
	temp_1629[3]&=~ (ICON_O2|ICON_O3|ICON_O4|ICON_O5);
	temp_1629[4]&=~ (ICON_P1|ICON_P2|ICON_P3|ICON_P4|ICON_P5);
	temp_1629[5]&=~ (ICON_Q2|ICON_Q3|ICON_Q4|ICON_Q5);
	temp_1629[7] =  CHAR_MASK;
	if(Power_Flash == 1)
	{
		temp_1629[4]|= ICON_P1;
		temp_1629[7] = ICON_R1|ICON_T1;
	}
	else if(Power_Flash == 2)
	{
		temp_1629[3]|= (ICON_O2);
		temp_1629[4]|= (ICON_P1|ICON_P2);
		temp_1629[5]|= (ICON_Q2);
		temp_1629[7] = ICON_R1|ICON_R2|ICON_T1|ICON_T2|ICON_S2;
	}//break;
	else if(Power_Flash == 3)
	{
		temp_1629[3]|= (ICON_O2|ICON_O3);
		temp_1629[4]|= (ICON_P1|ICON_P2|ICON_P3);
		temp_1629[5]|= (ICON_Q2|ICON_Q3);
		temp_1629[7] = ICON_R1|ICON_R2|ICON_R3|ICON_T1|ICON_T2|ICON_T3|ICON_S2|ICON_S3;
	}//break;
	else if(Power_Flash == 4)
	{
		temp_1629[3]|= (ICON_O2|ICON_O3|ICON_O4);
		temp_1629[4]|= (ICON_P1|ICON_P2|ICON_P3|ICON_P4);
		temp_1629[5]|= (ICON_Q2|ICON_Q3|ICON_Q4);
		temp_1629[7] = ICON_R1|ICON_R2|ICON_R3|ICON_R4|ICON_T1|ICON_T2|ICON_T3|ICON_T4|ICON_S2|ICON_S3|ICON_S4;
	}//break;
	else if(Power_Flash == 5)
	{
		temp_1629[3]|= (ICON_O2|ICON_O3|ICON_O4|ICON_O5);
		temp_1629[4]|= (ICON_P1|ICON_P2|ICON_P3|ICON_P4|ICON_P5);
		temp_1629[5]|= (ICON_Q2|ICON_Q3|ICON_Q4|ICON_Q5);
		temp_1629[7] = ICON_R1|ICON_R2|ICON_R3|ICON_R4|ICON_R5|ICON_T1|ICON_T2|ICON_T3|ICON_T4|ICON_T5|ICON_S2|ICON_S3|ICON_S4|ICON_S5;
	}//break;
}

#pragma location="LCD_1629_API"
void MENU_BT_CLEAR_DISPLAY(void)
{
	temp_1629[0] = CHAR_C;
	temp_1629[1] = CHAR_L;
	temp_1629[2] = CHAR_E_;
	temp_1629[3] = CHAR_MASK;
	temp_1629[4] = CHAR_MASK;
	temp_1629[5] = CHAR_MASK;
}

#pragma location="FRE_SHOW_API"
void Write_Display_BT_PREVNEXT(U8 STYTE)
{
	if(STYTE==MENU_BT_PREV)
	{
		temp_1629[0] = CHAR_PREV;
		temp_1629[1] = CHAR_PREV;
		temp_1629[2] = CHAR_PREV;
	}
	else
	{
		temp_1629[0] = CHAR_NEXT;
		temp_1629[1] = CHAR_NEXT;
		temp_1629[2] = CHAR_NEXT;
	}
	//temp_1629[3] = CHAR_MASK;
	//temp_1629[4] = CHAR_MASK;
	//temp_1629[5] = CHAR_MASK;
}

#pragma location="LCD_1629_API"
void MENU_EQ_DISPLAY(void)
{
	temp_1629[0] = CHAR_MASK;
	temp_1629[1] = CHAR_MASK;
	temp_1629[2] = CHAR_MASK;
	temp_1629[3] = CHAR_E;
	temp_1629[4] = CHAR_Q;
	//temp_1629[5] = CHAR_MASK;
	if(work_mode==MUSIC_PLAY_MODE)
	{
		temp_1629[0] = CHAR_M_;
		temp_1629[1] = CHAR_P_;
	}
	else if(work_mode==FM_MODE)
	{
		temp_1629[0] = CHAR_F_;
		temp_1629[1] = CHAR_M_;
	}
	if(eq_mode_court==0)
	{
		temp_1629[5] = CHAR_0;
	}
	else if(eq_mode_court==1)
	{
		temp_1629[5] = CHAR_1;
	}
	else if(eq_mode_court==2)
	{
		temp_1629[5] = CHAR_2;
	}
	else if(eq_mode_court==3)
	{
		temp_1629[5] = CHAR_3;
	}
	else 
	{
		temp_1629[5] = CHAR_4;
	}
}

void MENU_BT_ONONLINE_DISPLAY(void)
{
	temp_1629[1] = CHAR_MASK;
	temp_1629[2] = CHAR_MASK;
}

#if 0
#pragma location="LED_SCAN"
__near_func void DispMenu_1629(u8 menu_number)//@"IIC_CODE_API"  //ÃÓ≥‰…®√Ëbuf
//#pragma location="LCD_621_SEG_API"
//void DispMenu_1629(u8 menu_number)
{
	switch (menu_number)
	{
		case MENU_VOLUE:	  
			Write_Display_VOL(MENU_VOLUE);
			break;		  
#if PT2358
		case MENU_FL:	
			Write_Display_VOL(MENU_FL);
			break;
			
		case MENU_SL:	
			Write_Display_VOL(MENU_SL);
			break;
			
		case MENU_CEN:		
			Write_Display_VOL(MENU_CEN);
			break;

		case MENU_SUB:	
			Write_Display_VOL(MENU_SUB);
			break;
#endif
			
#if CS2313
		case MENU_SW:   
			Write_Display_VOL(MENU_SW);
			break;

		case MENU_BASS:
			Write_Display_VOL(MENU_BASS);
			break;

		case MENU_TRE:	  
			Write_Display_VOL(MENU_TRE);
			break;
#endif
		case MENU_USB_SD:
			Write_Display_Work_Mode(MENU_USB_SD);
			break;

		case MENU_LINE_IN:   
			Write_Display_Work_Mode(MENU_LINE_IN);
			break;
			
		case MENU_DVD:	 
			Write_Display_Work_Mode(MENU_DVD);
			break;

		case MENU_FM_FREQ:
			Write_Display_Work_Mode(MENU_FM_FREQ);
			break;

		case MENU_BT:
			Write_Display_Work_Mode(MENU_BT);
			break;

		case MENU_BT_NEXT:
			Write_Display_BT_PREVNEXT(MENU_BT_NEXT);
			break;
			
		case MENU_BT_PREV:
			Write_Display_BT_PREVNEXT(MENU_BT_PREV);
			break;

		case MENU_PLAY_NUM:
			MENU_FILE_NUM();
			break;

		case MENU_PAUSE:
		case MENU_PLAY:
			if(!volume_mute_falg)Write_Display_TIME();
			break;

		case MENU_SELECT_NUMBER:
			MENU_SELECT_MP3_FM_NUMBER();
			break;
			
		case MENU_PLAY_MODE:
			MENU_USB_SD_REPEAT();
			break;
			
		case MENU_FM_CH:
			MENU_FM_NUMBER();
			break;

		case MENU_POWER_MUTE:
			Write_Display_Power_Mute();
			break;

		case MENU_MODE:
			Write_Display_Mode();
			break;

		case MENU_FLASH:
			MENU_DISPLAY_FLASH();
			break;

		case MENU_POWER_ON_FLASH:
			MENU_POWER_FLASH();
			break;
			
		case MENU_BT_CLEAR:
			MENU_BT_CLEAR_DISPLAY();
			break;

		case MENU_EQ:
			MENU_EQ_DISPLAY();
			break;
			
		case BT_MENU_NULL:
			MENU_BT_ONONLINE_DISPLAY();
			break;
			
		default:
			break;
	}
	ShowCHAR();
}
#endif

#endif
#pragma location="LCD_1629_API"
void Write_Display_Digtial_1629(s8 value)
{
	if(value < 0) 
	{		
		value = -value;
		temp_1629[2] = CHAR_MINUS;
        temp_1629[3] = DIGIT_FONT[value/10];
		temp_1629[4] = DIGIT_FONT[value%10];
	}
	else 
	{
	    if((sys_ctl.Vol_tyle == STATE_MAIN)||(sys_ctl.Vol_tyle == STATE_SUB))
        {   
    		temp_1629[4] = DIGIT_FONT[value/10];
    		temp_1629[5] = DIGIT_FONT[value%10];
        }
        else
        {
            temp_1629[3] = DIGIT_FONT[value/10];
    		temp_1629[4] = DIGIT_FONT[value%10];    
        }
	}
    
}
//
#pragma location="LCD_1629_API"
void MENU_DISPLAY_MASK(void)
{
	u8 i;
    for(i=0;i<8;i++)
        temp_1629[i] = CHAR_MASK;
}

#pragma location="LCD_1629_API"
void Duty_set(void)
{
    
    u8 dim_level,set_level;   
    if(sys_ctl.dim_level ==3)
    {
        set_level=7;
    }
    else if(sys_ctl.dim_level ==2)
    {
        set_level=2;
    }
    else if(sys_ctl.dim_level ==1)
    {
        set_level=0;
    }
    dim_level = sys_ctl.dim_level+1;
    sys_param_write8(PARAM_DIM, dim_level);
    sys_ctl.TM1629_input = (set_level & 0x07)|0x88;
#if 0    
    if(!sys_ctl.dim_level)TM1629_input = 0x80;    
    else TM1629_input = (set_level & 0x07)|0x88;
#endif   
	TM_STB_LOW;
	WriteData(sys_ctl.TM1629_input);     
	TM_STB_HIGH;
       
}
#if 0
#pragma location="LCD_1629_API"
void Dim_Duty_set(bool Up_Down)
{  
    if(Up_Down)
    {
        sys_ctl.dim_level++;
        if(sys_ctl.dim_level > 2)sys_ctl.dim_level = 2;
    }
    else
    {
        sys_ctl.dim_level--;
        if(sys_ctl.dim_level < 0)sys_ctl.dim_level = 0;
    }   
    Duty_set();
}
#endif
#pragma location="LCD_1629_API"
void Write_Display_Work_Mode(u8 style)
{
	MENU_DISPLAY_MASK();
	//memset((u16*)temp_1629,0x0000,8);
	switch(style)
	{
		case MENU_LINE_IN:
            
            temp_1629[0]  = CHAR_H;
            temp_1629[1]  = CHAR_I;          
			break;
	}
	
}

#pragma location="LCD_1629_API"
void DispMenu_1629(u8 menu_number)//@"IIC_CODE_API"  //ÃÓ≥‰…®√Ëbuf
{
	switch (menu_number)
	{
		case MENU_LINE_IN:   
			Write_Display_Work_Mode(MENU_LINE_IN);
			break;		  
			
		default:
			break;
	}
	ShowCHAR();
}

#endif
