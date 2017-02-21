#include "include.h"
#include "task.h"
#include "user_setting.h"
#include "task_CS8416.h"
#include "task_NPCP215X.h"

#if CS8416

#pragma location="CS8416_IIC_DATA3"
const __code u8 cs8416_ommands[9] =
{
    0x00,
    0x00,
    0xA0,
    0x00,
    0x00,
    0x80,
    0xA4,
    0x10,
    0x00,
};

#pragma location="CS8416_IIC_API"
void Audio_in_I2C_Start(void)
{
	CS8416_DATA_OUTPUT;
	CS8416_CLK_OUTPUT;
	CS8416_DATA_HIGH;
	CS8416_CLK_HIGH;
    delay(30);
	CS8416_DATA_LOW;
    delay(30);
	CS8416_CLK_LOW;
	//Delay_myself;
	//TA5731_DATA_HIGH;
}

#pragma location="CS8416_IIC_API"
void Audio_in_I2C_SendByte(u8 Data)
{
	u8 i;
	
    //delay(30);
	CS8416_DATA_OUTPUT;	
	for(i = 0;i < 8;i++)
	{
		if(Data & 0x80) CS8416_DATA_HIGH;
		else			CS8416_DATA_LOW;
		delay(30);
		CS8416_CLK_HIGH;
		delay(30);
		Data <<= 1;
		CS8416_CLK_LOW;
	}
}
#pragma location="CS8416_IIC_API"
u8 Audio_in_I2C_GetByte()
{
    u8 i;
    u8 val = 0;
    CS8416_DATA_INPUT;
    for(i=8; i!=0; i--){
        delay(30);
        CS8416_CLK_HIGH;
        delay(30);
        val <<= 1;
        if(CS8416_DATA_IS_HIGH()){
            val |= BIT(0);
        }
        CS8416_CLK_LOW;
    }
    return val;
}

#if 0
//IIC总线接收1byte
#pragma location="DSP_IIC"
u8 audio_getbyte(void)
{
    u8 i;
    u8 val = 0;
    AUDIO_DATA_IN();
    for (i=8; i!=0; i--) {
        audio_delay_2();
        AUDIO_CLK_HIGH();
        audio_delay();
        val <<= 1;
        if (AUDIO_DATA_IS_HIGH()) {
            val |= BIT(0);
        }
        AUDIO_CLK_LOW();
    }
    return val;
}
#endif
#pragma location="CS8416_IIC_API"
bool Audio_in_ReceiveACK(void)
{
    bool Ackflag = false;
	
	CS8416_DATA_INPUT;
    delay(30);
	CS8416_CLK_HIGH;
    delay(30);
	
	if(CS8416_DATA) 
    {   
        CS8416_DATA_OUTPUT;
		Ackflag = 1;
    }
	else 
		Ackflag = 0;
	
	CS8416_CLK_LOW;
		
    delay(30);

	return Ackflag;
}

#pragma location="CS8416_IIC_API"
void Audio_in_I2C_Stop(void)
{
	CS8416_DATA_OUTPUT;
	CS8416_DATA_LOW;
    delay(30);
	CS8416_CLK_HIGH;
    delay(30);
	CS8416_DATA_HIGH;
    delay(30);
}

#pragma location="CS8416_IIC_API"
void cs8416_Init(void)
{ 
    u8 i;
    //发起始信号
    Audio_in_I2C_Start();   
    //写从地址
    Audio_in_I2C_SendByte(CS8416ADDERSS);
    Audio_in_ReceiveACK();
    // 写数据    
    for( i = 0 ;i< sizeof(cs8416_ommands);i++)
    {
        Audio_in_I2C_SendByte(cs8416_ommands[i]);
        Audio_in_ReceiveACK();
    }    
    //发送停止信号 
    Audio_in_I2C_Stop();     // Stop and return        
}

#pragma location="CS8416_IIC_API"
void CS8416_write_byte(u8 adr, u8 value)
{
    //发起始信号
    Audio_in_I2C_Start();
    //写从地址
    Audio_in_I2C_SendByte(adr);
    Audio_in_ReceiveACK();
    //写数据
    Audio_in_I2C_SendByte(value);
    Audio_in_ReceiveACK();
    Audio_in_I2C_Stop();     // Stop and return 
}

#pragma location="CS8416_IIC_API"
u8 CS8416_read_byte(u8 adr)
{
    u8 I2CReadResult;
    //发起始信号
    Audio_in_I2C_Start();
    //写从地址
    Audio_in_I2C_SendByte(0x28);
    Audio_in_ReceiveACK();
    Audio_in_I2C_SendByte(adr);
    Audio_in_ReceiveACK();
    Audio_in_I2C_Stop();
    
    //发起始信号
    Audio_in_I2C_Start();
    //写数据
    Audio_in_I2C_SendByte(0x29);
    Audio_in_ReceiveACK();
    //发起始信号
    I2CReadResult = Audio_in_I2C_GetByte();
    Audio_in_ReceiveACK();
    Audio_in_I2C_Stop();
    delay_5ms(1);
    return I2CReadResult;
}
#if 0
void Get_CS8416_err_sta(void)
{
    u8 sta = CS8416_read_byte(0x0B);
    if(sta&BIT(6)){
        CS8416_write_byte(0x12,0x12);    
    }
    else {
        CS8416_write_byte(0x11,0x11);
    }
    //printf("sta = 0x%X\n",sta);
}

void get_5711_err_sta(void)
{
    u8 sta = tas5711_read_byte(0x02);
    if(sta){
        tas5711_write_byte(0x02,0x00);
    }
    printf("sta = 0x%X\n",sta);

}

u8 tas5711_read_byte(u8 adr)
{
    u8 I2CReadResult;

    audio_start();                //I2C启动

#if A_SEL
    audio_sendbyte(0x36);         //写命令
#else
    audio_sendbyte(0x34);         //写命令
#endif
    audio_getack();

    audio_sendbyte(adr);         //写地址
    audio_getack();

    audio_start();                    //写转为读命令，需要再次启动I2C

#if A_SEL
    audio_sendbyte(0x36 | 0x01);         //读命令
#else
    audio_sendbyte(0x34 | 0x01);         //读命令
#endif
    audio_getack();

    I2CReadResult = audio_getbyte();
    audio_sendnack();                      //发送ack命令

    audio_stop();                     //I2C停止
    delay_5ms(1);

    return I2CReadResult;
}
#endif
#pragma location="CS8416_IIC_API"
void CS8416_in_choose(bool ch)
{
    Audio_in_I2C_Start(); 
    Audio_in_I2C_SendByte(CS8416ADDERSS); 
    Audio_in_ReceiveACK();
    Audio_in_I2C_SendByte(0x04); 
    Audio_in_ReceiveACK();
    if (ch)Audio_in_I2C_SendByte(0x88);
    else   Audio_in_I2C_SendByte(0x80);     
    Audio_in_ReceiveACK();
    Audio_in_I2C_Stop();     // Stop and return 
}

#if 0
#pragma location="CS8416_IIC_API"
void CS8416_IN_OFF(void)
{
    Audio_in_I2C_Start(); 
    Audio_in_I2C_SendByte(CS8416ADDERSS); 
    Audio_in_ReceiveACK();
    Audio_in_I2C_SendByte(0x04); 
    Audio_in_ReceiveACK();
    Audio_in_I2C_SendByte(0x88);
    Audio_in_ReceiveACK();
    Audio_in_I2C_Stop();     // Stop and return 
}
#endif
#endif

