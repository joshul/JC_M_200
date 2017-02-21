#ifndef  _LED_7P7S_H_
#define  _LED_7P7S_H_

#if IS_LEDSEG_7PIN_DISPLAY
#define PIN0  BIT(0)
#define PIN1  BIT(1)
#define PIN2  BIT(2)
#define PIN3  BIT(3)
#define PIN4  BIT(4)
#define PIN5  BIT(5)
#define PIN6  BIT(6)

//7PIN �����ɨ�跽ʽ
#define SCAN_MODE_COM           0               //��COMɨ��, ������7��IOȫ�����ڲ�330R����, ��ʾ���ȶ�
#define SCAN_MODE_SEG           1               //��SEGɨ�裬������û��330R������IO, ���ζ���ʾ���ᶶ��
extern u8 pin_disp_buf[7];
void led_7p7s_init(void);
void led_7p7s_value_set(void);
__near_func void led_7p7s_scan(void);
__near_func bool led_7p7s_reuse(void);
#endif

#endif
