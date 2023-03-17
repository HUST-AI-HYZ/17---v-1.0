#include "common.h"
#include "parameter.h"
#include "encoder_head.h"
#include "zf_common_headfile.h"
#include "gps.h"

bool first_PI_input = true;
//���ź�����Ҫ�޸ĵ�
#define ENCODER1_TIM		TIM3_ENCOEDER
#define ENCODER1_A			TIM3_ENCOEDER_CH1_B4
#define ENCODER1_DIR			TIM3_ENCOEDER_CH2_B5



void encoder_init()
{
    //һ��QTIMER���� ����������������
    //������Ҫע��һ�£�����Ǵ���������ı���������������LSB����Ӧ����A������ DIR����Ӧ����B������ ���ɽ���

    //��ʼ�� QTIMER_1 A��ʹ��QTIMER1_TIMER0_C0 B��ʹ��QTIMER1_TIMER1_C1

    encoder_init_dir(ENCODER1_TIM, ENCODER1_A, ENCODER1_DIR);                
    	

    //qtimer_quad_init(QTIMER_1, QTIMER1_TIMER0_C0, QTIMER1_TIMER1_C1);
    //qtimer_quad_init(QTIMER_2, QTIMER2_TIMER0_C3, QTIMER2_TIMER3_C25);
}

//����
//һ��������������   ��������һ�θߵ�ƽ

void get_cur_speed() //��ȫ�ֱ���encoder_XX���ܱ�������ֵ���������ٶȺ��ٶ���
{
    uint8 i = 0;
    uint8 j = 0;
    int32 sum = 0;
    int32 temp = 0;
    if (first_PI_input) //��ʼ�����У�
    {
        for (i = 0; i < 8; i++)
        {
            //���������
          if(gpio_get(ENCODER1_DIR))
            encoder_Q[i] = encoder_get_count(ENCODER1_TIM);
          else
            encoder_Q[i] = - encoder_get_count(ENCODER1_TIM);
            
            encoder_clear_count(ENCODER1_TIM);

      
        }
        first_PI_input = false;

        //�����ۼ�
        pulse_read = pulse_read + encoder_Q[7];

        g_car_speed = (encoder_Q[7] * 1.0 / 3) * 2000 / 40.672; 
       
    }
    else
    {
        for (i = 0; i < 7; i++)
        {
            encoder_Q[i] = encoder_Q[i + 1]; //����Ųλ��
            sum += encoder_Q[i];

        }
        //���������
        if(gpio_get(ENCODER1_DIR))
            encoder_Q[7] = encoder_get_count(ENCODER1_TIM);
         else
            encoder_Q[7] = - encoder_get_count(ENCODER1_TIM);
        
        encoder_clear_count(ENCODER1_TIM);
       
        sum += encoder_Q[7];
 
        //ȡƽ��ֵ
        int32 encoder_aver = sum >> 3;
    
        g_car_speed = (encoder_aver * 1.0 / 3) * 2000 / 40.672; //�����ٶ�
        
        //�����ۼ�
        if ( gps_mode == 2 || gps_mode == 1)
        {
            //��һ�׵��ת40.672Ȧ,ÿ��һ����һ�����壬һת��������
            //distance_from_lastpoint = distance_from_lastpoint + (encoder_Q[7] * 1.0 / 3.0 )/ 40.672 ;
        }
        
        //�����encoder���ۼ�����
        pulse_read = pulse_read + encoder_Q[7];
    }
}



