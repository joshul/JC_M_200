#include "include.h"
#include "task.h"
#include "user_setting.h"
#include "task_STA350.h"
#include "task_NPCP215X.h"

#if ST350BW 

#define  STA350ADDERSS	    0x3a

#pragma location="STA350_DATA"
const __code u16 Audio_Sub_VolumeData[33] =
{
    0xff,0x8c,0x65,
    0x51,0x41,0x39,0x31,0x2d,0x29,0x25,0x21,0x1d,0x1b,
    0x19,0x15,0x13,0x11,0x0f,0x0f,0x0d,0x0d,0x0b,0x0b,
    0x09,0x09,0x07,0x07,0x05,0x05,0x03,0x03,0x00,0x00,//20.5db    
#if 0
    0xff,0x8c,0x87,
    0x81,0x79,0x71,0x6d,0x5d,0x4d,0x43,0x37,0x31,0x2b,
    0x27,0x21,0x21,0x1f,0x1d,0x1d,0x19,0x13,0x0f,0x0d,
    0x0b,0x09,0x07,0x07,0x05,0x05,0x03,0x03,0x01,0x00,//20.5db
 
#endif    
};

#pragma location="STA350_DATA"
const __code u16 Audio_Sub_VolumeData1[33] =
{
    0xff,0x8c,0x65,
    0x51,0x41,0x39,0x31,0x2d,0x29,0x25,0x21,0x1d,0x1b,
    0x19,0x15,0x13,0x11,0x0f,0x0f,0x0d,0x0d,0x0b,0x0b,
    0x09,0x09,0x07,0x07,0x05,0x05,0x03,0x03,0x00,0x00,//20.5db  
};

#if 1
#pragma location="STA350_DATA"
const __code u8 sta350_ommands[214] =
{    
    0x00, 0x63, //## Configuration Register A
    0x01, 0x80, //## Configuration Register B
    0x02, 0x97, //## Configuration Register C
    0x03, 0x40, //## Configuration Register D
    0x04, 0xC2, //## Configuration Register E
    0x05, 0xdf, //## Configuration Register F
    0x06, 0x10, //## Master Mute
    0x07, 0x16, //## Master Volume    
    0x08, 0x66, //## Channel 1 Volume //-3dB
    0x09, 0x66, //## Channel 2 Volume //-3dB
    0x0A, 0x66, //## Channel 3 Volume //-3dB
    0x0B, 0x80, //## Audio Preset Register 1
    0x0C, 0x00, //## Audio Preset Register 2
    0x0E, 0x00, //## Channel 1 Config
    0x0F, 0x40, //## Channel 2 Config
    0x10, 0x90, //## Channel 3 Config
    0x11, 0x77, //## Bass and Treble Tone Control
    0x12, 0x69, //## Limiter 1 Attack/Release Rate
    0x13, 0x69, //## Limiter 1 Attack/Release Threshold
    0x14, 0x6A, //## Limiter 2 Attack/Release Rate
    0x15, 0x69, //## Limiter 2 Attack/Release Threshold
    0x16, 0x00, //## Coefficient Address Bits 5..0
    0x17, 0x00, //## Coefficient b1 Data Bits 23..16
    0x18, 0x00, //## Coefficient b1 Data Bits 15..8
    0x19, 0x00, //## Coefficient b1 Data Bits 7..0
    0x1A, 0x00, //## Coefficient b2 Data Bits 23..16
    0x1B, 0x00, //## Coefficient b2 Data Bits 15..8
    0x1C, 0x00, //## Coefficient b2 Data Bits 7..0
    0x1D, 0x00, //## Coefficient a1 Data Bits 23..16
    0x1E, 0x00, //## Coefficient a1 Data Bits 15..8
    0x1F, 0x00, //## Coefficient a1 Data Bits 7..0
    0x20, 0x00, //## Coefficient a2 Data Bits 23..16
    0x21, 0x00, //## Coefficient a2 Data Bits 15..8
    0x22, 0x00, //## Coefficient a2 Data Bits 7..0
    0x23, 0x00, //## Coefficient b0 Data Bits 23..16
    0x24, 0x00, //## Coefficient b0 Data Bits 15..8
    0x25, 0x00, //## Coefficient b0 Data Bits 7..0
    0x26, 0x00, //## Coefficient Write Control
    0x27, 0x1A, //## Max Power Correction Config Msb
    0x28, 0xC0, //## Max Power Correction Config Lsb
    0x29, 0xF3, //## Variable Distortion Compensation Msb
    0x2A, 0x33, //## Variable Distortion Compensation Lsb
    0x2B, 0x00, //## Fault Detect Recovery Bits Msb
    0x2C, 0x0C, //## Fault Detect Recovery Bits Lsb
    0x2D, 0x7F, //## Device Status
    0x31, 0x00, //## EQ Coeff and DRC Configuration
    0x32, 0x30, //## Limiter 1 Ext.Attack Threshold
    0x33, 0x30, //## Limiter 1 Ext.Release Threshold
    0x34, 0x30, //## Limiter 2 Ext.Attack Threshold
    0x35, 0x30, //## Limiter 2 Ext.Release Threshold
    0x36, 0x00, //## Extended Configuraton Register
    0x37, 0x00, //## EQ Soft Volume Config (fade-in)
    0x38, 0x00, //## EQ Soft Volume Config (fade-out)
    0x39, 0x01, //## DRC RMS Filter Coefficient C0 Bits 25..16
    0x3A, 0xEE, //## DRC RMS Filter Coefficient C0 Bits 15..8
    0x3B, 0xFF, //## DRC RMS Filter Coefficient C0 Bits 7..0
    0x3C, 0x7E, //## DRC RMS Filter Coefficient C1 Bits 25..16
    0x3D, 0xC0, //## DRC RMS Filter Coefficient C1 Bits 15..8
    0x3E, 0x26, //## DRC RMS Filter Coefficient C1 Bits 7..0
    0x3F, 0x00, //## Extended Volume Resolution
    0x48, 0x00, //## Noise Shaping
    0x49, 0x00, //## Extended Coef Range 4-1
    0x4A, 0x00, //## Extended Coef Range 7-5
    0x4B, 0x04, //## Miscellaneous 1
    0x4C, 0x00, //## Miscellaneous 2  
#if 0       
    0x31, 0x01,  //Coef[032h] = 7FFFFFh ## Coefficient 0x32 - Channel 1 Pre-scale
    0x16, 0x32,
    0x17, 0x7f,
    0x18, 0xff,
    0x19, 0xff,
    0x26, 0x01,

    0x31, 0x01,  //Coef[033h] = 7FFFFFh ## Coefficient 0x33 - Channel 2 Pre-scale
    0x16, 0x33,
    0x17, 0x7f,
    0x18, 0xff,
    0x19, 0xff,
    0x26, 0x01,
    
    0x31, 0x01, //Coef[034h] = 7FFFFFh ## Coefficient 0x35 - Channel 2 Post-scale
    0x16, 0x34,
    0x17, 0x7f,
    0x18, 0xff,
    0x19, 0xff,
    0x26, 0x01,
    
    0x31, 0x01, //Coef[035h] = 7FFFFFh ## Coefficient 0x35 - Channel 2 Post-scale
    0x16, 0x35,
    0x17, 0x7f,
    0x18, 0xff,
    0x19, 0xff,
    0x26, 0x01,
    
    0x31, 0x01, //Coef[036h] = 7FFFFFh ## Coefficient 0x36 - Channel 3 Post-scale 
    0x16, 0x36,
    0x17, 0x7f,
    0x18, 0xff,
    0x19, 0xff,
    0x26, 0x01,
#endif    
    0x31, 0x01, //Coef[037h] = 5A9DF7h ## Coefficient 0x37 - Twarn/Oc Limit
    0x16, 0x37,
    0x17, 0x5A,
    0x18, 0x9D,
    0x19, 0xF7,
    0x26, 0x01,
    
    0x31, 0x01, //Coef[038h] = 000000h ## Coefficient 0x38 - Channel 1 Mix 1
    0x16, 0x38,
    0x17, 0x00,
    0x18, 0x00,
    0x19, 0x00,
    0x26, 0x01,

    0x31, 0x01, //Coef[039h] = 7FFFFFh ## Coefficient 0x39 - Channel 1 Mix 2
    0x16, 0x39,
    0x17, 0x7f,
    0x18, 0xff,
    0x19, 0xff,
    0x26, 0x01,
    
    0x31, 0x01, //Coef[03Ah] = 7FFFFFh ## Coefficient 0x3A - Channel 2 Mix 1
    0x16, 0x3a,
    0x17, 0x7f,
    0x18, 0xff,
    0x19, 0xff,
    0x26, 0x01,
    
    0x31, 0x01, //Coef[03Bh] = 000000h ## Coefficient 0x3B - Channel 2 Mix 2
    0x16, 0x3b, 
    0x17, 0x00,
    0x18, 0x00,
    0x19, 0x00,
    0x26, 0x01,

    0x31, 0x01, //Coef[03Ch] = 200000h ## Coefficient 0x3C - Channel 3 Mix 1
    0x16, 0x3c,
    0x17, 0x80,
    0x18, 0x00,
    0x19, 0x00,
    0x26, 0x01,
    
    0x31, 0x01, //Coef[03Dh] = 200000h ## Coefficient 0x3D - Channel 3 Mix 2
    0x16, 0x3d,
    0x17, 0x80,
    0x18, 0x00,
    0x19, 0x00,
    0x26, 0x01,
};
#endif

#pragma location="CS8416_IIC_API"
void Audio_out_I2C_Start(void)
{
	ST350_DATA_OUTPUT;
	ST350_CLK_OUTPUT;
	ST350_DATA_HIGH;
	ST350_CLK_HIGH;
    delay(30);
	ST350_DATA_LOW;
    delay(30);
	ST350_CLK_LOW;
	//Delay_myself;
	//TA5731_DATA_HIGH;
}
#pragma location="CS8416_IIC_API"
void Audio_out_I2C_SendByte(u8 Data)
{
	u8 i;
	
    //delay(30);
	ST350_DATA_OUTPUT;	
	for(i = 0;i < 8;i++)
	{
		if(Data & 0x80) ST350_DATA_HIGH;
		else			ST350_DATA_LOW;
		delay(30);
		ST350_CLK_HIGH;
		delay(30);
		Data <<= 1;
		ST350_CLK_LOW;
	}
}

#pragma location="CS8416_IIC_API"
bool Audio_out_ReceiveACK(void)
{
    bool Ackflag = false;
	
	ST350_DATA_INPUT;
    delay(30);
	ST350_CLK_HIGH;
    delay(30);
	
	if(ST350_DATA) 
    {   
        ST350_DATA_OUTPUT;
		Ackflag = 1;
    }
	else 
		Ackflag = 0;
	
	ST350_CLK_LOW;
		
    delay(30);

	return Ackflag;
}

#pragma location="CS8416_IIC_API"
void Audio_out_I2C_Stop(void)
{
	ST350_DATA_OUTPUT;
	ST350_DATA_LOW;
    delay(30);
	ST350_CLK_HIGH;
    delay(30);
	ST350_DATA_HIGH;
    delay(30);
}

#pragma location="CS8416_IIC_API"
void Audio_out_I2C_WriteByte(u8 adr,u8 volume)	//const 
{
    Audio_out_I2C_Start();
    Audio_out_I2C_SendByte(STA350ADDERSS);
    Audio_out_ReceiveACK();
    Audio_out_I2C_SendByte(adr);
    Audio_out_ReceiveACK();
    Audio_out_I2C_SendByte(volume);
    Audio_out_ReceiveACK();
    Audio_out_I2C_Stop();
}

#pragma location="CS8416_IIC_API"
void sta350_Init(void)
{ 
    u8 i;
    for(i = 0 ;i< sizeof(sta350_ommands);i++)
    {     
        Audio_out_I2C_Start();                //IICÆô¶¯
        Audio_out_I2C_SendByte(STA350ADDERSS);
        Audio_out_ReceiveACK();
        Audio_out_I2C_SendByte(sta350_ommands[i]);
        Audio_out_ReceiveACK();
        Audio_out_I2C_SendByte(sta350_ommands[++i]);
        Audio_out_ReceiveACK();
        Audio_out_I2C_Stop();     // Stop and return         
    }                       
}

#pragma location="CS8416_IIC_API"
void Set_Sub_Volume(u8 volume)	  
{
    Audio_out_I2C_WriteByte(0x07,Audio_Sub_VolumeData[volume]);
#if 0
    if((task_ctl.work_sta == TASK_MUSIC)||(task_ctl.work_sta == TASK_BT))
    {
        Audio_out_I2C_WriteByte(0x07,Audio_Sub_VolumeData[volume]); 
    }
    else 
    {
        Audio_out_I2C_WriteByte(0x07,Audio_Sub_VolumeData1[volume]); 
    }
#endif    
}

#endif
