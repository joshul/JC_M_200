#ifndef __TASK_NPCP215X_H_
#define __TASK_NPCP215X_H_

#if NPCP215X

#define AUDIO_RESET_HIGH()      P1DIR &= ~BIT(5);P1 |= BIT(5)
#define AUDIO_RESET_LOW()       P1DIR &= ~BIT(5);P1 &= ~BIT(5)

#if 0
#define AUDIO_DATA                        1       //P21
#define AUDIO_CLK                         0       //P20
#define AUDIO_DATA_CLK_OUT()              P2PU &= ~(BIT(AUDIO_DATA) | BIT(AUDIO_CLK)); P2DIR &= ~(BIT(AUDIO_DATA) | BIT(AUDIO_CLK))
#define AUDIO_DATA_IN()                   P2DIR |= BIT(AUDIO_DATA); P2PU |= BIT(AUDIO_DATA)
#define AUDIO_DATA_OUT()                  P2PU |= BIT(AUDIO_DATA); P2DIR &= ~BIT(AUDIO_DATA)
#define AUDIO_DATA_CLK_HIGH()             P2 |= BIT(AUDIO_DATA) | BIT(AUDIO_CLK)
#define AUDIO_DATA_HIGH()                 P2 |= BIT(AUDIO_DATA)
#define AUDIO_DATA_LOW()                  P2 &= ~BIT(AUDIO_DATA)
#define AUDIO_CLK_HIGH()                  P2 |= BIT(AUDIO_CLK)
#define AUDIO_CLK_LOW()                   P2 &= ~BIT(AUDIO_CLK)
#define AUDIO_DATA_IS_HIGH()              (P2 & BIT(AUDIO_DATA))
#else
#define TA5731_DATA_OUTPUT     P0DIR  &= ~ BIT(6);P0     |=   BIT(6)
#define TA5731_DATA_INPUT      P0DIR  |=   BIT(6)
#define TA5731_DATA_HIGH       P0     |=   BIT(6)
#define TA5731_DATA_LOW        P0     &= ~ BIT(6)
#define TA5731_DATA            P0     &    BIT(6)

#define TA5731_CLK_OUTPUT      P1DIR  &= ~ BIT(4);P1     |=   BIT(4)
#define TA5731_CLK_HIGH        P1     |=   BIT(4)
#define TA5731_CLK_LOW         P1     &= ~ BIT(4)

#endif
#define Delay_myself	{asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); /*asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");asm("nop");asm("nop");asm("nop"); asm("nop"); asm("nop");asm("nop");asm("nop");asm("nop");*/}

#define Audio_MainVolume_Max    32
#define Audio_MainMaxVol_Value  0x190
#define Audio_MaxBassVol_Value  0x7f
#define Audio_MainVolume_Min    0

void AudioData_Init0(void);
void AudioData_Init1(void);
void AudioData_Init2(void);

//void AudioData_Init3(void);
void Audio_Slow_Volume(U8 Mute_Status);
void AudioCodec_MUTE(U8 Mute_Status);
void Set_AudioCodec_Volume(u8 volume);
void Set_AudioCodec_MaxxBass(u8 volume);
void AudioCodec_Volume_Set(u8 function,bool Up_Down);
void AudioCodec_InputMode(void);
void AudioCodec_3d_Set(bool Up_Down);	
void AudioCodec_MUTE(U8 Mute_Status);
void Main_Vol_Set(void);
void Bass_Vol_Set(void);
void Treble_Vol_Set(void);
void SW_Vol_Set(void);
void AudioCodec_Eq_Set(u8 Types);

//void Audio_Software_Reset(void);

void Peq_Music_Set(void);

void sta369_write_byte(u8 adr, u8 value);


#endif

#endif


