#ifndef _USER_TAS5711_
#define _USER_TAS5711_

/* TAS5711接口 */

// A_SEL引脚电平
#define A_SEL               0

#define AUDIO_DATA_PORT                   0       //P06
#define AUDIO_DATA                        6
#define AUDIO_CLK_PORT                    1       //P14
#define AUDIO_CLK                         4

#define __AUDIO_DATA_CLK_OUT(DATA_PORT, CLK_PORT)   P ## DATA_PORT ## PU &= ~BIT(AUDIO_DATA);\
                                                    P ## CLK_PORT ## PU &= ~BIT(AUDIO_CLK);\
                                                    P ## DATA_PORT ## DIR &= ~BIT(AUDIO_DATA);\
                                                    P ## CLK_PORT ## DIR &= ~BIT(AUDIO_CLK);\
                                                    delay(10)                                       // 第二中间转换宏
#define _AUDIO_DATA_CLK_OUT(DATA_PORT, CLK_PORT)    __AUDIO_DATA_CLK_OUT(DATA_PORT, CLK_PORT)         // 第一中间转换宏
#define AUDIO_DATA_CLK_OUT()                        do{\
                                                        _AUDIO_DATA_CLK_OUT(AUDIO_DATA_PORT, AUDIO_CLK_PORT);\
                                                    }while(0)
#define __AUDIO_DATA_IN(PORT)                       P ## PORT ## DIR |= BIT(AUDIO_DATA);\
                                                    P ## PORT ## PU |= BIT(AUDIO_DATA)                // 第二中间转换宏
#define _AUDIO_DATA_IN(PORT)                        __AUDIO_DATA_IN(PORT)                             // 第一中间转换宏
#define AUDIO_DATA_IN()                             do{\
                                                        _AUDIO_DATA_IN(AUDIO_DATA_PORT);\
                                                    }while(0)
#define __AUDIO_DATA_OUT(PORT)                      P ## PORT ## PU |= BIT(AUDIO_DATA);\
                                                    P ## PORT ## DIR &= ~BIT(AUDIO_DATA)              // 第二中间转换宏
#define _AUDIO_DATA_OUT(PORT)                       __AUDIO_DATA_OUT(PORT)                            // 第一中间转换宏
#define AUDIO_DATA_OUT()                            do{\
                                                        _AUDIO_DATA_OUT(AUDIO_DATA_PORT);\
                                                    }while(0)
#define __AUDIO_DATA_CLK_HIGH(DATA_PORT, CLK_PORT)  do{\
                                                        P ## CLK_PORT |= BIT(AUDIO_CLK);\
                                                        P ## DATA_PORT |= BIT(AUDIO_DATA);\
                                                    }while(0)                                       // 第二中间转换宏
#define _AUDIO_DATA_CLK_HIGH(DATA_PORT, CLK_PORT)   __AUDIO_DATA_CLK_HIGH(DATA_PORT, CLK_PORT)        // 第一中间转换宏
#define AUDIO_DATA_CLK_HIGH()                       _AUDIO_DATA_CLK_HIGH(AUDIO_DATA_PORT, AUDIO_CLK_PORT)

#define __AUDIO_DATA_HIGH(DATA_PORT)                P ## DATA_PORT |= BIT(AUDIO_DATA)     // 第二中间转换宏
#define _AUDIO_DATA_HIGH(DATA_PORT)                 __AUDIO_DATA_HIGH(DATA_PORT)          // 第一中间转换宏
#define AUDIO_DATA_HIGH()                           _AUDIO_DATA_HIGH(AUDIO_DATA_PORT)

#define __AUDIO_DATA_LOW(DATA_PORT)                 P ## DATA_PORT &= ~BIT(AUDIO_DATA)    // 第二中间转换宏
#define _AUDIO_DATA_LOW(DATA_PORT)                  __AUDIO_DATA_LOW(DATA_PORT)           // 第一中间转换宏
#define AUDIO_DATA_LOW()                            _AUDIO_DATA_LOW(AUDIO_DATA_PORT)

#define __AUDIO_CLK_HIGH(CLK_PORT)                  P ## CLK_PORT |= BIT(AUDIO_CLK)       // 第二中间转换宏
#define _AUDIO_CLK_HIGH(CLK_PORT)                   __AUDIO_CLK_HIGH(CLK_PORT)            // 第一中间转换宏
#define AUDIO_CLK_HIGH(CLK_PORT)                    _AUDIO_CLK_HIGH(AUDIO_CLK_PORT)

#define __AUDIO_CLK_LOW(CLK_PORT)                   P ## CLK_PORT &= ~BIT(AUDIO_CLK)      // 第二中间转换宏
#define _AUDIO_CLK_LOW(CLK_PORT)                    __AUDIO_CLK_LOW(CLK_PORT)             // 第一中间转换宏
#define AUDIO_CLK_LOW()                             _AUDIO_CLK_LOW(AUDIO_CLK_PORT)

#define __AUDIO_DATA_IS_HIGH(DATA_PORT)             (P ## DATA_PORT & BIT(AUDIO_DATA))    // 第二中间转换宏
#define _AUDIO_DATA_IS_HIGH(DATA_PORT)              __AUDIO_DATA_IS_HIGH(DATA_PORT)       // 第一中间转换宏
#define AUDIO_DATA_IS_HIGH()                        _AUDIO_DATA_IS_HIGH(AUDIO_DATA_PORT)

// reset引脚
#define RST_PORT            4   // P40
#define RST_PIN             0
// PDN引脚
#define PDN_PORT            1   // P17
#define PDN_PIN             7

#define RST_PIN_DIR()               _RST_PIN_DIR(RST_PORT, RST_PIN)
#define RST_LOW()                   _RST_LOW(RST_PORT, RST_PIN)
#define RST_HIGH()                  _RST_HIGH(RST_PORT, RST_PIN)
#define PDN_PIN_DIR()               _PDN_PIN_DIR(PDN_PORT, PDN_PIN)
#define PDN_HIGH()                  _PDN_LOW(PDN_PORT, PDN_PIN)
#define PDN_LOW()                   _PDN_HIGH(PDN_PORT, PDN_PIN)

//#define PDN_HIGH()                  SET_WKO_LOW();
//#define PDN_LOW()                   SET_WKO_HIGH();

#define __RST_PIN_DIR(port, pin)    P ## port ## DIR &= ~BIT(pin);
#define _RST_PIN_DIR(port, pin)     __RST_PIN_DIR(port, pin)
#define __RST_LOW(port, pin)        P ## port &= ~BIT(pin)
#define _RST_LOW(port, pin)         __RST_LOW(port, pin)
#define __RST_HIGH(port, pin)       P ## port |= BIT(pin)
#define _RST_HIGH(port, pin)        __RST_HIGH(port, pin)

#define __PDN_PIN_DIR(port, pin)    P ## port ## DIR &= ~BIT(pin)
#define _PDN_PIN_DIR(port, pin)     __PDN_PIN_DIR(port, pin)
#define __PDN_HIGH(port, pin)       P ## port |= BIT(pin)
#define _PDN_HIGH(port, pin)        __PDN_HIGH(port, pin)
#define __PDN_LOW(port, pin)        P ## port &= ~BIT(pin)
#define _PDN_LOW(port, pin)         __PDN_LOW(port, pin)

void tas5711_write_byte(u8 adr, u8 value);
void tas5711_write_4byte(u8 adr, u8 value0, u8 value1, u8 value2, u8 value3);
void TAS5731M_EQ_Set(void);
void tas5711_init(void);
void TAS5711_AMP_Mute_Set(u8 con);
void TAS5711_AMP_Volume_Set(u8 volume);

void get_5711_err_sta(void);
u8 tas5711_read_byte(u8 adr);

#endif
