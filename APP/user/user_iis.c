// IIS���������ó�����Ҫ��ʵ�������޸�iis_set_vol
// ע�⣺��IIS����������������Ҫ��IIC�ӿڲ�����SD�ӿڸ��ã���Ҫ���ⶨ��IIC����
// ����IIS��P16��P22��P25��P26����ռ�á�
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

//�����жϣ�ĿǰЧ�����(�������ȼ���ߣ����ܱ����)
#if (FM_RECORD_EN || AUX_RECORD_EN) && IIS_EN
#pragma vector = VECTOR_OFFSET(IIS_INT)
#pragma register_bank=3  //0���ȼ�(������ȼ�)������bank1 //2���ȼ�������bank2 //3���ȼ�������bank3
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
            // �������ܴ󣬲��Ƽ�
            IIS_ADR1 = (u8)(adpcm_input >> 8);
            IIS_ADR1 = (u8)adpcm_input;
            IIS_DMA_RD_CNT0 = (u8)(t_fm_rec.spl * adc_ch/2-1);
            IIS_DMA_RD_CNT1 = (u8)((t_fm_rec.spl * adc_ch/2-1) >> 8);
        }
#else
        {
            // ʹ��mp3¼����������С������ʹ��48K������¼��
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
            // �������ܴ󣬲��Ƽ�
            IIS_ADR1 = (u8)(adpcm_input >> 8);
            IIS_ADR1 = (u8)adpcm_input;
            IIS_DMA_RD_CNT0 = (u8)(t_rec.spl * adc_ch/2-1);
            IIS_DMA_RD_CNT1 = (u8)((t_rec.spl * adc_ch/2-1) >> 8);
        }
#else
        {
            // ʹ��mp3¼����������С������ʹ��48K������¼��
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
    // �ֶ�kick IIS��DMA�ж�
    IIS_ADR1 = (u8)((u16)adc_buf >> 8);
    IIS_ADR1 = (u8)(u16)adc_buf;
    IIS_DMA_RD_CNT0 = 1;
    IIS_DMA_RD_CNT1 = 0;
}

#pragma location="TASK_MUSIC_SEG_3"
void pll2_record_set(u8 channel)
{
    iis_kick();
    // ����PLL2��IISʱ��
    //IIS_CON1 = 0x05;  // DOUT��DIN��һ��
    IIS_CON1 = 0x15;  // DOUT��DIN�ֿ�
    // IIS 32����ģʽ
    IIS_ALLBIT = 15;
    IIS_BAUD = 31;
    IIS_BCLK_CFG = 15;
/*
    // IIS 64����ģʽ
    IIS_ALLBIT = 31;
    IIS_BAUD = 15;
    IIS_BCLK_CFG = 31;
*/
    IIS_VALBIT = 0x0F;
    IIS_REFCLK_CFG = 15|BIT(5);
    CLKCON2 = 0xa0;
    // ����IIS DMA�ж���߼�
    IPH1 |= BIT(6);
    IP1 |= BIT(6);

    // �ж��ǲ��ǵ�����
    if(!(channel & TWO_CHANNEL)) {
        IIS_CON0 = 0x11 | BIT(2);   // ֻ������������
#if IIS_TYPE == IIS_TAS5711
        tas5711_write_4byte(0x25, 0x01, 0x02, 0x02, 0x45);  // IIS����2�����������������������
#endif
    }else {
        IIS_CON0 = 0x01 | BIT(2);   // ˫����
    }
}

#pragma location="TASK_MUSIC_SEG_3"
void pll2_record_exit(void)
{
    IIS_CON3 &= ~BIT(3);        // �ȹض�IIS DMA���ж�
    IE1 &= BIT(6);              // �ٹض�IE1����Ӧ���ж�
    IIS_CON0 = 0x01 | BIT(2);   // IIS�ָ�˫����
    //IIS_CON1 = 0x09;  // DOUT��DIN��һ��
    IIS_CON1 = 0x19;  // DOUT��DIN�ֿ�
    pll1_set();
#if IIS_TYPE == IIS_TAS5711
    tas5711_write_4byte(0x25, 0x01, 0x02, 0x13, 0x45);  // IIS�������˫����������
#endif
}

#pragma location="TASK_MUSIC_SEG_3"
void pll1_set(void)
{
    //set_sys_clk(SYS_48M);
#if 0
    // IIS 32����ģʽ
    IIS_ALLBIT = 15;
    IIS_BAUD = 33;
    IIS_BCLK_CFG = 15;
#endif
    //������IIS 64����ģʽ
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
    IIS_CON0 = 0x01 | BIT(2);   // BCLK������ʱӦ����������
    //IIS_CON1 = 0x09;  // DOUT��DIN��һ��
    IIS_CON1 = 0x19;  // DOUT��DIN�ֿ�
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
    ////    ��ֵ�仯1  ��Ӧ0.5dB
    
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
