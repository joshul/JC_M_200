// IIS的音量设置程序，需要按实际需求修改iis_set_vol
// 注意：因IIS器件调节音量的需要，IIC接口不能与SD接口复用，需要另外定义IIC引脚
// 启用IIS后，P16、P22、P25、P26将被占用。
//      MCLK---P16
//      BCLK---P25
//      LRCLK--P26
//      DO/DI--P22

#include "include.h"
#include "user_iis.h"
#include "task.h"
#include "task_aux_eq.h"
#include "task_fm.h"
#include "task_record.h"

//采用中断，目前效果最好(必须优先级最高，不能被打断)
#if (FM_RECORD_EN || AUX_RECORD_EN) && IIS_EN
#pragma vector = VECTOR_OFFSET(IIS_INT)
#pragma register_bank=3  //0优先级(最低优先级)必须用bank1 //2优先级必须用bank2 //3优先级必须用bank3
__interrupt __near_func void iis_interrupt(void)
{
    ISR_ENTER();
    if((IIS_CON2 & BIT(3)) != 0x00) {
        IIS_CON2 = 0x00;
#if FM_RECORD_EN && AUX_RECORD_EN
        if(task_ctl.work_sta == TASK_FM)
#endif
#if FM_RECORD_EN
#if (REC_TYPE == REC_WAV_ADPCM || REC_TYPE == REC_WAV_PCM)
        {
            // 数据量很大，不推荐
            IIS_ADR1 = (u8)(adpcm_input >> 8);
            IIS_ADR1 = (u8)adpcm_input;
            IIS_DMA_RD_CNT0 = (u8)(t_fm_rec.spl * adc_ch/2-1);
            IIS_DMA_RD_CNT1 = (u8)((t_fm_rec.spl * adc_ch/2-1) >> 8);
        }
#else
        {
            // 使用mp3录音，数据量小，可以使用48K采样率录音
            IIS_ADR1 = (u8)((u16)adc_buf+(t_fm_rec.pos*t_fm_rec.spl*adc_ch) >> 8);
            IIS_ADR1 = (u8)((u16)adc_buf+(t_fm_rec.pos*t_fm_rec.spl*adc_ch));
            IIS_DMA_RD_CNT0 = (u8)(t_fm_rec.spl*adc_ch/2-1);
            IIS_DMA_RD_CNT1 = (u8)((t_fm_rec.spl*adc_ch/2-1) >> 8);
        }
#endif
#endif
#if FM_RECORD_EN && AUX_RECORD_EN
        else
#endif
#if AUX_RECORD_EN
#if (REC_TYPE == REC_WAV_ADPCM || REC_TYPE == REC_WAV_PCM)
        {
            // 数据量很大，不推荐
            IIS_ADR1 = (u8)(adpcm_input >> 8);
            IIS_ADR1 = (u8)adpcm_input;
            IIS_DMA_RD_CNT0 = (u8)(t_rec.spl * adc_ch/2-1);
            IIS_DMA_RD_CNT1 = (u8)((t_rec.spl * adc_ch/2-1) >> 8);
        }
#else
        {
            // 使用mp3录音，数据量小，可以使用48K采样率录音
            IIS_ADR1 = (u8)((u16)adc_buf+(t_rec.pos*t_rec.spl*adc_ch) >> 8);
            IIS_ADR1 = (u8)((u16)adc_buf+(t_rec.pos*t_rec.spl*adc_ch));
            IIS_DMA_RD_CNT0 = (u8)(t_rec.spl*adc_ch/2-1);
            IIS_DMA_RD_CNT1 = (u8)((t_rec.spl*adc_ch/2-1) >> 8);
        }
#endif
#endif
    }
    ISR_EXIT();
}
#endif

#pragma location="TASK_MUSIC_SEG_3"
void iis_kick(void)
{
    // 手动kick IIS的DMA中断
    IIS_ADR1 = (u8)((u16)adc_buf >> 8);
    IIS_ADR1 = (u8)(u16)adc_buf;
    IIS_DMA_RD_CNT0 = 1;
    IIS_DMA_RD_CNT1 = 0;
}

#pragma location="TASK_MUSIC_SEG_3"
void pll2_record_set(u8 channel)
{
    iis_kick();
    // 配置PLL2做IIS时钟
    //IIS_CON1 = 0x05;  // DOUT和DIN在一起
    IIS_CON1 = 0x15;  // DOUT和DIN分开
    // IIS 32数据模式
    IIS_ALLBIT = 15;
    IIS_BAUD = 31;
    IIS_BCLK_CFG = 15;
/*
    // IIS 64数据模式
    IIS_ALLBIT = 31;
    IIS_BAUD = 15;
    IIS_BCLK_CFG = 31;
*/
    IIS_VALBIT = 0x0F;
    IIS_REFCLK_CFG = 15|BIT(5);
    CLKCON2 = 0xa0;
    // 设置IIS DMA中断最高级
    IPH1 |= BIT(6);
    IP1 |= BIT(6);

    // 判断是不是单声道
    if(!(channel & TWO_CHANNEL)) {
        IIS_CON0 = 0x11 | BIT(2);   // 只传左声道数据
#if IIS_TYPE == IIS_TAS5711
        tas5711_write_4byte(0x25, 0x01, 0x02, 0x02, 0x45);  // IIS器件2个声道都输出左声道的声音
#endif
    }else {
        IIS_CON0 = 0x01 | BIT(2);   // 双声道
    }
}

#pragma location="TASK_MUSIC_SEG_3"
void pll2_record_exit(void)
{
    IIS_CON3 &= ~BIT(3);        // 先关断IIS DMA的中断
    IE1 &= BIT(6);              // 再关断IE1中相应的中断
    IIS_CON0 = 0x01 | BIT(2);   // IIS恢复双声道
    //IIS_CON1 = 0x09;  // DOUT和DIN在一起
    IIS_CON1 = 0x19;  // DOUT和DIN分开
    pll1_set();
#if IIS_TYPE == IIS_TAS5711
    tas5711_write_4byte(0x25, 0x01, 0x02, 0x13, 0x45);  // IIS器件输出双声道的声音
#endif
}

#pragma location="TASK_MUSIC_SEG_3"
void pll1_set(void)
{
    //set_sys_clk(SYS_48M);
#if 0
    // IIS 32数据模式
    IIS_ALLBIT = 15;
    IIS_BAUD = 33;
    IIS_BCLK_CFG = 15;
#endif
    //工作在IIS 64数据模式
    IIS_ALLBIT = 31;
    IIS_BAUD = 16;
    IIS_BCLK_CFG = 31;

    IIS_VALBIT = 0x0F;
    IIS_REFCLK_CFG = 16|BIT(5);
    CLKCON2 = 0x00;
}

#pragma location="TASK_MUSIC_SEG_3"
void iis_conf(void)
{
    P1DIR &= ~BIT(6);
    P2DIR &= ~(BIT(2) | BIT(5) | BIT(6));
    IIS_CON0 = 0x01 | BIT(2);   // BCLK传数据时应该用上升沿
    //IIS_CON1 = 0x09;  // DOUT和DIN在一起
    IIS_CON1 = 0x19;  // DOUT和DIN分开
    IIS_CON2 = 0x00;
    IIS_CON3 = 0x80;
}

#pragma location="TASK_MUSIC_SEG_3"
void iis_init(void)
{
    iis_conf();
    pll1_set();
}

#pragma location="TASK_MUSIC_SEG_3"
void iis_set_vol(u8 volume)
{
#if IIS_TYPE == IIS_TAS5711
    ////    0x08    0dB
    ////    数值变化1  对应0.5dB
    
    if(volume > 0)
        //tas5711_write_byte(0x07, 0x20+(VOLUME_MAX-volume)*(3*(VOLUME_MAX-volume)/5));
        //tas5711_write_byte(0x07, 0x20+(VOLUME_MAX-volume)*3);
        TAS5711_AMP_Volume_Set(volume);
    else
        tas5711_write_byte(0x07, 0xff);
#endif
}

#pragma location="TASK_MUSIC_SEG_3"
void iis_powerdown(void)
{
#if IIS_TYPE == IIS_TAS5711
    tas5711_write_byte(0x40, 0x05);
    delay_ms(3);
    PDN_PIN_DIR();
    PDN_LOW();
    delay_ms(2);
    RST_PIN_DIR();
    RST_LOW();
#endif
}
