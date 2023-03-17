#include "zf_common_headfile.h"
#include "math.h"

#include "button.h"
#include "common.h"

#include "parameter.h"
#include "isr.h"
#include "motor.h"
#include "zf_driver_gpio.h"
#include "zf_driver_pwm.h" // ���ļ��е�PWM_DUTY_MAX��Ҫע��
#include "gps.h"
//�ݶ�dutyֵ��Χ��
// mid 2.8
// left 3.6
//right 2.0
int incre_pwm = 0;
//ע�ⵥλ������ÿ��
//0.9������
//#define MAX_CAR_SPEED 1921.4
//#define MAX_CAR_SPEED 1868.0
//3360  3900
#define MAX_CAR_SPEED 1024.0
int16 direct_servopwm_max = 600;

void change_motor(int32 duty, uint8 compensate);

void motor_init()
{
   //�������pwm
   pwm_init( MotorA, 1000, 0); // PWM ͨ��0 ��ʼ��Ƶ��17KHz ռ�ձȳ�ʼΪ0
   pwm_init(MotorB, 1000, 1000); // PWM ͨ��1 ��ʼ��Ƶ��17KHz ռ�ձȳ�ʼΪ0
   //�ű3�ת��
    change_motor(1000,0);
   //gpio_init(G15, GPO, 0, GPO_PUSH_PULL);
   //GPIO��ʼ��
	 //gpio_init(A0, GPI, 0, GPO_PUSH_PULL);
	gpio_init(A0, GPI, 0, GPI_PULL_DOWN);
}

void servo_motor_init()
{
   pwm_init(SERVO_PIN, 50, 0); // ռ�ձȳ�ʼΪ0
   pwm_set_duty(SERVO_PIN, duoji);

}


void change_motor(int32 duty, uint8 compensate)
{
   uint16 duty_1 = 0;
   uint16 duty_2 = 0;

  
   //�����޷�
   if (duty <= MAX_N_DUTY_1) //duty�����������Ҫ��
   {
      duty = MAX_N_DUTY_1;
   }
   else if (duty >= MAX_P_DUTY_1) // duty��������Ҫ��
   {
      duty = MAX_P_DUTY_1;
   }

   //��С�޷����Ȳ��ӣ�
 

   //������
   if (duty >=  0)
   {
      duty_1 = 0;
      duty_2 = duty;
   }
   else
   {
      duty_1 = 50000;
      duty_2 = - duty;
   }
   
   
   duty_mid = duty;
   pwm_set_duty(MotorA, duty_1);
   pwm_set_duty(MotorB, duty_2);
}
//���趨�ٶ�ȷ��PWMֵ
void setspeed_to_pwm()
{
   int32 set_duty = 0;
   set_duty =  (int32)(g_speed_set / MAX_CAR_SPEED * PWM_DUTY_MAX);

   change_motor(set_duty,0); 
}

//��������
//����ʽPI  1000  8
void gps_servo_control()
{
   //����gps���ʵ�ʺ�����Ŀ�꺽���ƫ����������ǽ��о�ȷ����
   //ÿ�μ������Ҫ�ı�Ĵ��ֵ
  
   //���ܲ�ʹ��0���
   
   static float last_duoji = 0;
   //ʵ�ʴ����Ŀ���ǵ����
   float error_icm =    Data_Gyro.angle_sum_once - Use_Data.gps_angle_error_now;

   //���������ǵ�������󣬽���pd����
   //ֱ��
   if( (area_tag == DIRECT_AREA )&& gps_mode == 1)
   {
      gps_error_pd_pwm = gps_kp_z * error_icm + gps_kd_z * (error_icm - error_icm_last);
   }
   else if ( (area_tag == ROUND_AREA ) && gps_mode == 1)
   {
      gps_error_pd_pwm = gps_kp_w * error_icm + gps_kd_w * (error_icm - error_icm_last);
   }
   else if ((area_tag == ROUND_IN_AREA || area_tag == ROUND_OUT_AREA )&& gps_mode == 1)
   {
      gps_error_pd_pwm = gps_kp_in * error_icm + gps_kd_in * (error_icm - error_icm_last);
   }
   
   //������
   error_icm_last = error_icm;
   
   
   //gps_mode1 �ոջ������
    
    
   //���޷���������
	  if (run_circle == 0 && passed_sections <= 5)
		{
			 if (gps_error_pd_pwm > start_thres)
          {
              duoji = pwm_mid_angle + start_thres; 
           }
           else if (gps_error_pd_pwm < -start_thres)
          {     
              duoji = pwm_mid_angle - start_thres;
          }
          else
          {
             duoji = pwm_mid_angle + gps_error_pd_pwm;
          }
			
		}
    else if (area_tag == DIRECT_AREA || area_tag == ROUND_OUT_AREA)
    {
      
       //����ֱ��                
           if (gps_error_pd_pwm > direct_thres)
          {
              duoji = pwm_mid_angle + direct_thres; 
           }
           else if (gps_error_pd_pwm < -direct_thres)
          {     
              duoji = pwm_mid_angle - direct_thres;
          }
          else
          {
             duoji = pwm_mid_angle + gps_error_pd_pwm;
          }
    }
    else
    {
        duoji = pwm_mid_angle + gps_error_pd_pwm;
    }
    
    //����޷�
    if (duoji > pwm_max_angle)
    {
       duoji = pwm_max_angle;
     }
     else if (duoji < pwm_min_angle)
    {
       duoji = pwm_min_angle;
    }
   
   
   //Ҫ��ƽ������
   last_duoji = duoji;
   
   pwm_set_duty(SERVO_PIN, duoji);

}
//ͨ�������ۼӣ�����֪��һ�׵ľ��룬���ת�˶���Ȧ
//

