/*
 * �ļ����ƣ�Vofa.h
 * ����ʱ��: 2020��7��15��
 * ����޸ģ�2020��7��15��
 * �ļ�����: Wind_Just_Fall
 */
 
#include "zf_common_headfile.h"
#include "VOFA.h"
#include "parameter.h"
#include "gps.h"


 
void Float_to_Byte(float f,uint8 byte[])
{
	FloatLongType fl;
	fl.fdata=f;
        //unsigned char
	byte[0]=(uint8)fl.ldata;
	byte[1]=(uint8)(fl.ldata>>8);
	byte[2]=(uint8)(fl.ldata>>16);
	byte[3]=(uint8)(fl.ldata>>24);
}

void Vofa_1data(float data)
{
	uint8 i,buff[4];
	
	Float_to_Byte(data,buff);
	for(i=0;i<4;i++)
          
	uart_putchar(UART_3, buff[i]);

}

void Vofa_2data(float data1,float data2)
{
  uint8 i,buff[4];
	
	Float_to_Byte(data1,buff);
	for(i=0;i<4;i++)
		uart_putchar(UART_3, buff[i]);
	
	Float_to_Byte(data2,buff);
	for(i=0;i<4;i++)
		uart_putchar(UART_3, buff[i]);
  
}

void Vofa_3data(float data1,float data2,float data3)
{
  uint8 i,buff[4];
	
	Float_to_Byte(data1,buff);
	for(i=0;i<4;i++)
		uart_putchar(UART_3, buff[i]);
	
	Float_to_Byte(data2,buff);
	for(i=0;i<4;i++)
		uart_putchar(UART_3, buff[i]);
        Float_to_Byte(data3,buff);
	for(i=0;i<4;i++)
		uart_putchar(UART_3, buff[i]);
 

}

void Vofa_4data(float data1,float data2,float data3,float data4)
{
	uint8 i,buff[4];
	
	Float_to_Byte(data1,buff);
	for(i=0;i<4;i++)
		uart_putchar(UART_3, buff[i]);
	
	Float_to_Byte(data2,buff);
	for(i=0;i<4;i++)
		uart_putchar(UART_3, buff[i]);
	
	Float_to_Byte(data3,buff);
	for(i=0;i<4;i++)
		uart_putchar(UART_3, buff[i]);
	
	Float_to_Byte(data4,buff);
	for(i=0;i<4;i++)
		uart_putchar(UART_3, buff[i]);
}

void Vofa_5data(float data1,float data2,float data3,float data4,float data5)
{
  uint8 i,buff[4];

    Float_to_Byte(data1,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);

    Float_to_Byte(data2,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);

    Float_to_Byte(data3,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);

    Float_to_Byte(data4,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);

    Float_to_Byte(data5,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);
}

void Vofa_7data(float data1,float data2,float data3,float data4,float data5,float data6,float data7)
{
    uint8 i,buff[4];

    Float_to_Byte(data1,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);

    Float_to_Byte(data2,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);

    Float_to_Byte(data3,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);

    Float_to_Byte(data4,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);

    Float_to_Byte(data5,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);

    Float_to_Byte(data6,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);

    Float_to_Byte(data7,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);
}

void Vofa_10data(float data1,float data2,float data3,float data4,float data5,float data6,float data7 ,float data8,float data9, float data10)
{
    uint8 i,buff[4];

    Float_to_Byte(data1,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);

    Float_to_Byte(data2,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);

    Float_to_Byte(data3,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);

    Float_to_Byte(data4,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);

    Float_to_Byte(data5,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);

    Float_to_Byte(data6,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);

    Float_to_Byte(data7,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);
    
      Float_to_Byte(data8,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);
    
      Float_to_Byte(data9,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);
    
    Float_to_Byte(data10,buff);
    for(i=0;i<4;i++)
        uart_putchar(UART_3, buff[i]);
    
   
}
//��ʾ����������֡
void end_vofa()
{
     //����֡����
    uart_putchar(UART_3, 0x00);
    uart_putchar(UART_3, 0x00);
    uart_putchar(UART_3, 0x80);
    uart_putchar(UART_3, 0x7f);
}

void send_spin_vofa(void)
{
  if(g_send == 1)
  {
      //��ǰ��ľ�γ������
      Vofa_2data(Save_Data.f_longi , Save_Data.f_lati);
      /*//��ǰ����
      Vofa_3data(now_area ,area_tag, passed_sections);
      //������
      Vofa_2data(Data_Gyro.angle_sum_once,Data_Gyro.angle_sum_area);
      //��ǰ����
      Vofa_2data(Save_Data.f_ground_direction,Save_Data.f_ground_speed);
      //�����
      Vofa_2data(Use_Data.TwoDotsDis , Use_Data.TwoDotsDirec);
      Vofa_4data(Use_Data.angle_next_heading_now , Use_Data.distan_next_point_now , Use_Data.gps_angle_error_now, Use_Data.change_angle_delta );
      //������ʱ��  
      Vofa_1data(distance_from_lastpoint);
      Vofa_2data(duoji,g_car_speed);
      Vofa_1data(duty_mid);
       //ˮƽ�������
       //�������õ�flag
      Vofa_1data(debug_flag);
      Vofa_1data(pulse_read);
      */
      //���Ե�flag�ر�
      debug_flag = 0;
      //һ�η��ͽ���
      end_vofa();
      
  }
  else if (g_send == 2)
  {
      static int i = 0;
      while ( i < finished_gpspoints)
      {
        Vofa_5data(Points_Stored[i].lon_keypoint , Points_Stored[i].lat_keypoint , Points_Stored[i].lat_kypnt_f , Points_Stored[i].lat_kypnt_z , i );
        Vofa_1data(Points_Stored[i].type_point);
        end_vofa();
        i = i + 1;
      }
      
      if ( i == finished_gpspoints)
      {
          i = 0;
      }
  }
}


//16th��λ��
void send_vars(uint8 number, Varible *vars)
{
    uint8 bag[SEND_MAX_BUFF] = {0};
    uint32 len = 0; // ��Ϣ�ܳ���

    //��Ϣͷ
    bag[0] = SEND_HEAD0;
    bag[1] = SEND_HEAD1;
    bag[2] = SEND_HEAD2;
    bag[3] = SEND_HEAD3;
    len = len + 4;

    //��Ϣ���ݳ���
    len = len + 1; // �ճ�

    //��Ϣ����
    uint8 *p = &bag[5];
    Varible *v = vars;
    uint8 index = 0;
    while (index < number)
    {
        //������־
        *p++ = v->name[0];
        len++;
        //��������
        float temp;
        switch (v->type)
        {
        case TYPE_FLOAT:
            temp = (float)*(float *)v->buff;
            break;
        case TYPE_INT16:
            temp = (float)*(int16 *)v->buff;
            break;
        case TYPE_INT32:
            temp = (float)*(int32 *)v->buff;
            break;
        case TYPE_UINT16:
            temp = (float)*(uint16 *)v->buff;
            break;
        case TYPE_UINT32:
            temp = (float)*(uint32 *)v->buff;
            break;
        default:
            break;
        }
        p[0] = *((char *)&temp + 0);
        p[1] = *((char *)&temp + 1);
        p[2] = *((char *)&temp + 2);
        p[3] = *((char *)&temp + 3);
        p = p + 4;
        len = len + 4;

        index++;
        v++;
    }

    //��Ϣβ?
    *p = 0;
    len = len + 1;

    //������Ϣ���ݳ���
    p = bag;
    *(p + 4) = index;
    
    //seekfree_wireless_send_buff(bag, len);
    //��ϸ��麯����û��д��ȷ
    SEND(bag, len);
}

void send_spin(void)
{
    if (!g_send)
    {
        return;
    }

    // ���1 ���͵�����
    if (g_send == 1)
    {
        static Varible vars[1] = {
          //���
          {(void *)&duoji, (uint8 *)"\1", TYPE_FLOAT},
            
        }; 
        send_vars(1, vars);
    }
    /*
     else if (g_send == 2)
     {
       
       
     }
    // 发送图�?
    else if (g_send == 2)
    {
        seekfree_sendimg_03x(UART_3, g_image[0], MT9V03X_W, MT9V03X_H);
    }
    else if (g_send == 3)
    {
        static Varible vars[9] = {
            
        };
        send_vars(9, vars);
    }
    else if (g_send == 4)
    {
        static Varible vars[4] = {
            {(void *)&g_time, (uint8 *)"\1", TYPE_UINT32},
            {(void *)&g_road_width_bias, (uint8 *)"\61", TYPE_FLOAT},
            {(void *)&g_road_width_ratio, (uint8 *)"\62", TYPE_FLOAT},
            {(void *)&g_euler.pitch, (uint8 *)"\5", TYPE_FLOAT},
        };
        send_vars(4, vars);
    }*/
}









