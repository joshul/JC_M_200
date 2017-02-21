#ifndef __TASK_ST350_H_
#define __TASK_ST350_H_
#if ST350BW 
#define SUB_RESET_HIGH()        P4DIR &= ~BIT(0);P4 |= BIT(0)
#define SUB_RESET_LOW()         P4DIR &= ~BIT(0);P4 &= ~BIT(0)

#define ST350_DATA_OUTPUT       P0DIR  &= ~ BIT(6);P0 |=   BIT(6)
#define ST350_DATA_INPUT        P0DIR  |=   BIT(6)
#define ST350_DATA_HIGH         P0     |=   BIT(6)
#define ST350_DATA_LOW          P0     &= ~ BIT(6)
#define ST350_DATA              P0     &    BIT(6)

#define ST350_CLK_OUTPUT        P1DIR  &= ~ BIT(4);P1 |=   BIT(4)
#define ST350_CLK_HIGH          P1     |=   BIT(4)
#define ST350_CLK_LOW           P1     &= ~ BIT(4)

void sta350_Init(void);
void Set_Sub_Volume(u8 volume);



#if 0
#define  CS8416ADDERSS	    0x28

#define CS8416_DATA_OUTPUT     P0DIR  &= ~ BIT(6);P0 |=   BIT(6)
#define CS8416_DATA_INPUT      P0DIR  |=   BIT(6)
#define CS8416_DATA_HIGH       P0     |=   BIT(6)
#define CS8416_DATA_LOW        P0     &= ~ BIT(6)
#define CS8416_DATA            P0     &    BIT(6)

#define CS8416_CLK_OUTPUT      P1DIR  &= ~ BIT(4);P1 |=   BIT(4)
#define CS8416_CLK_HIGH        P1     |=   BIT(4)
#define CS8416_CLK_LOW         P1     &= ~ BIT(4)

#define CS8416_RESET_HIGH()    P2DIR &= ~BIT(7);P2 |= BIT(7)
#define CS8416_RESET_LOW()     P2DIR &= ~BIT(7);P2 &= ~BIT(7)
enum
{
    Input_coax,
    Input_opti,
};

void cs8416_Init(void);
void CS8416_in_choose(bool ch);
void CS8416_write_byte(u8 adr, u8 value);

#endif
#endif
#endif

