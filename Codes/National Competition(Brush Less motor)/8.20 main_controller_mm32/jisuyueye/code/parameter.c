#include "parameter.h"
#include "gps.h"




uint16 x_cursor = 96;
uint16 y_cursor = 32;
uint16 key_num = 0;
int8 modify_flag = 0;
int8 op = 0;
int8 FLASH_WRITE_FLAG = 0;
uint32 total_time = 0;
int write_flash_points = 0;

//debugר�ã��ж��Ƿ�����˶�Ӧ�Ĵ����
int16 debug_flag = 0;

//duty
int32 duty_mid = 0;
//button control
int16 button_mod = 2;

//ң������
char CarControlOrder[20];

//����ģʽ
uint16 car_mode = 0;

//����PID����ֱ��
float gps_kp_z = 1.0;
float gps_kd_z = 0.0;
//PID �������
float gps_kp_w = 1.0;
float gps_kd_w = 0.0;
//gps�ɼ�
float gps_kp_in = 1.0;
float gps_kd_in = 0.0;


float gps_error_pd_pwm = 0.0;

float speed_step = 10.0;

//PID相关
//PID结构�?
/*ID_struct  motor_angle;*/
uint16 pwm_max_angle = 4200;
uint16 pwm_min_angle = 2800;
uint16 pwm_mid_angle = 3600;  //�е�����
float start_run = 0.0;



/*************************
 * �ٶȿ���
**************************/
int32 encoder_Q[8]={0,};
int32 pulse_onemeter = 41648;
//���һȦ  1024
// 1m��Ӧ���ת41648/1024 = 40.672Ȧ 
//���������⣿����
int32 pulse_read = 0;



// 速度PI参数
float g_speed_i_max = 10000.0;
float g_speed_i_err = 0.0;

// 期望速度
float g_speed_set = 200.0;

//���ڲ������ٶ�ֵ   //(����ÿ��)(������Ϊ��׼)
float speed_max_region[4]=
{
   //0��
    200.0,
   //1
    200.0,
   //2
    0.0,
    0.0
  
};

uint8 speed_level = 0;


//�ڲ��ٶ�*��1+����ֵ��/2  Ϊʵ���ٶ�
float g_car_speed = 0.0;

/*******************
 * ���ٶȿ���
*********************/
Gyro_Spi Data_Gyro;



// 运�?�距�?
float g_cur_distance = 0.0;

// 系统时间
uint32 g_time = 0;
// 计时器时�?
uint32 g_clock = 0;

// 按键�?
uint8 g_button_value = 0;

// 页面使能
uint16 g_page = ENABLE;
// 页面�?
uint16 g_page_value = 0;

// 调试发送功�?
uint16 g_send = 1;

//��ת��
uint16 start_thres = 100;

float duoji = 3600;

