#ifndef _BUTTON_H
#define _BUTTON_H
//ע�ͱ�ʾ������KEY_�Ķ�Ӧ��Ҫ��ֻ�ܸ�����ֵ


#define KEY1				B8						// ����
#define KEY2				E5						// ����
#define KEY3				E3						// ����
#define KEY4				E2                                             // ����
#define KEY5				B9						// �м�								






#define SWITCH_1			A15					//������״̬									
#define SWITCH_2			A10	
#define SWITCH_3			A9				
#define SWITCH_4			A8	

#define TYPE_INT16  1
#define TYPE_UINT16 2
#define TYPE_INT32  3
#define TYPE_UINT32 4
#define TYPE_FLOAT  5

#define Key_left   1
#define Key_right   2
#define Key_up      3
#define Key_down    4
#include "zf_common_headfile.h"

#include "common.h"

extern uint8 past_page;
static int mode=1;
extern uint16 key_num;

//void button_scan()��
//void delay()��
#endif