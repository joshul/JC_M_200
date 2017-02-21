/*****************************************************************************
* Module    : IO
* File      : io_key.c
* Author    : Hanny
* Function  : 用户自定义按键定义
*****************************************************************************/
#ifndef CONFIG_C
#define IR_KEY_NUM     25

#define IR_USER_CODE8989		0x8989 //21KEY


IAR_CONST u8 tbl_ir_dat[IR_KEY_NUM] = {

    0x40,               0x4d,               0x41,               0x4c,               0x11,
	0x16,               0x51,               0x13,               0x50,               0x1a,
	0x10,               0x19,               0x18,               0x4e,               0x0d,   
	0x0c,               0x4a,               0x09,               0x08,               0x46,         
	0x05,               0x04,	            0x42,               0x01,               0x00,               
};          
                                   
IAR_CONST u8 tbl_ir_val[IR_KEY_NUM] = {
    
    T_KEY_POWER,        T_KEY_MODE,         T_KEY_MUTE,         T_KEY_MENU,         T_KEY_DMI,
	T_KEY_VOL_UP,	    T_KEY_PREV,	        T_KEY_PLAY,	        T_KEY_NEXT,	        T_KEY_VOL_DOWN,
	T_KEY_TV,		    T_KEY_MUSIC,        T_KEY_MOVIE,        T_KEY_NUM_1,        T_KEY_NUM_2, 	                
	T_KEY_NUM_3,        T_KEY_NUM_4,        T_KEY_NUM_5,        T_KEY_NUM_6,        T_KEY_NUM_7,
	T_KEY_NUM_8,        T_KEY_NUM_9,        T_KEY_PLAY_MODE,    T_KEY_NUM_0,        T_KEY_3D,
};

#endif
