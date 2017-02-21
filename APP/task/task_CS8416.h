#ifndef __TASK_CS8416_H_
#define __TASK_CS8416_H_
#if CS8416

#define  CS8416ADDERSS	    0x28

#define CS8416_DATA_OUTPUT     P0DIR  &= ~ BIT(6);P0 |=   BIT(6)
#define CS8416_DATA_INPUT      P0DIR  |=   BIT(6)
#define CS8416_DATA_HIGH       P0     |=   BIT(6)
#define CS8416_DATA_LOW        P0     &= ~ BIT(6)
#define CS8416_DATA            P0     &    BIT(6)
#define CS8416_DATA_IS_HIGH()  (P0&BIT(6))

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
u8 CS8416_read_byte(u8 adr);
//void Get_CS8416_err_sta(void);



#endif
#endif

