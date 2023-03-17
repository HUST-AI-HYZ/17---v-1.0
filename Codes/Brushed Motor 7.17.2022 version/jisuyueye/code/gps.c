#include "gps.h"
#include "stdlib.h"
#include <math.h>
#include "isr.h"
#include <stdlib.h>
#include <string.h>
#include "route_generating.h"

#define HALF_DIRECT_LENTH 42.1
#define RADIUS 55

//static double x_pi = 3.14159265358979324 * 3000.0 / 180.0;
#define PI 3.14159265358979324

/*********GPS��ر���*********/

//��һ�ζ�ȡ���������
_SaveData Save_Data;
//�洢�ؼ��������  ��ʱ����40��
_KeyPoint Points_Stored[ALL_POINTS];
_KeyPoint Points_Use[ALL_POINTS + 3];
//_KeyPoint ERR_Points[ALL_POINTS];
//�ɵ�ǰ��͹ؼ���������������м�����
_UseData Use_Data;
//��ʻ����
//����ģʽ���Ǵ�0������������Ѱ����
//Ĭ�ϴ�-1��ʼ
int16 guiding_mode = 1;
//���Ƶ����(������������0��ʼ)
uint16 guess_start = 0;
//����ʱ���ַ��������ڴ���sdcard����
char StartTime[12] = "f_714";
//΢���жϣ��൱�ڻ����������
int16 area_tag = 0;
//GPS΢��
//ÿ�ο��Ƶ���С��������λ
//�ڱ��ء�
//γ�ȼ�1    ����仯0.111319
//���ȼ�1    ����仯0.095901
//����ZONE  5
int16 micro_op_lati[ROUTE][ZONE] = {{0} , {0} ,{0}};
int16 micro_op_longi[ROUTE][ZONE] = {{0} , {0} ,{0}};
//΢�ٶ�
int16 micro_area = 0;
//��һ��΢�ٶ�
int16 last_micro_area = 0;
//GPS��λ
//int16 gps_reset = 0;
//��ǰ�����������
float error_lenth_s_p = 0.0;
//���򣨵�ǰ��ָ���0���㣩
float error_direction = 0.0;
//�ܵ���
int16 run_sections = 10;
//�Ѿ�ͨ���Ķ�������Ҳ��ʾ��һ��ͨ���ĵ㣩
int16 passed_sections = 0;
//GPS��ģʽ
int16 gps_mode = 0;
//ѡ�е��Ѳɼ������ݵ�����ֵ
uint16 point_selected_gps = 0;
//��ɲɼ������ݵ�
uint16 finished_gpspoints = 0;
//ѡ�е��γ��
uint32 point_latitude_selected = 0.0;
//ѡ�е�ľ���
uint32 point_longitude_selected = 0.0;
//����һ�����������߹��ľ��롣(������һ����...)
float distance_from_lastpoint = 0.00001;
//ÿ����ļ��(ֱ��·)
float distance_each_point_direct = 6.0;
//ÿ����ļ�������·��
float distance_each_point_round = 6.0;
//�Ƿ񻻵�
int8 change_point = 0;
//�����͸�ֵ

//Ĭ��Ϊֱ��
int16 point_type = 0;
int16 point_type_pageread = 0;
//ˮƽ����
float horizontal_error = 0;
//����
//ֱ����Ϊ0����Ĭ����
int8 now_area = 0;
int8 last_area = 0;
//ֱ����ֵ
float direct_thres = 250;
//icm ����
float error_icm_last = 0.0;

//·��ѡ��
//��Ϊ���ڲ������Զ����ɣ�����Ϊ�˱��⼷��ԭ�е����ݣ�������2��
int route_read = 1;
int route_write = 0;
//�Ƿ��ӳٷ���
int whether_delay = 1;
//���ȶ���С��
float longitude_FQ[50] = {
    0.0,
};
//��������
int16 longitude_ZQ[50] = {
    0.0,
};

//γ�ȶ���
float latitude_FQ[50] = {
    0,
};

float latitude_ZQ[50] = {
    0.0,
};
//GPS��������

//close gga
uint8 close_gga_cmd[] = {
    0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x00, 0x00, 0xFA, 0x0F};
//F1 D9 06 01 03 00 F0 00 00 FA 0F

//CLOSE ANT
uint8 close_ant_cmd[] = {
    0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x20, 0x00, 0x1A, 0x4F};
//F1 D9 06 01 03 00 F0 20 00 1A 4F

//close gsv
uint8 close_gsv_cmd[] = {
    0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x04, 0x00, 0xFE, 0x17};
//F1 D9 06 01 03 00 F0 04 00 FE 17

//close gsa
uint8 close_gsa_cmd[] = {
    0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x02, 0x00, 0xFC, 0x13};
//F1 D9 06 01 03 00 F0 02 00 FC 13

//close zda
uint8 close_zda_cmd[] = {
    0xF1, 0xD9, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x07, 0x00, 0x01, 0x1D};
//F1 D9 06 01 03 00 F0 07 00 01 1D

//set10 hz
uint8 set_rate_10hz[] = {
    0xF1, 0xD9, 0x06, 0x42, 0x14, 0x00, 0x00, 0x0A, 0x05, 0x00, 0x64, 0x00, 0x00, 0x00,
    0x60, 0xEA, 0x00, 0x00, 0xD0, 0x07, 0x00, 0x00, 0xC8, 0x00, 0x00, 0x00, 0xB8, 0xED};
//F1 D9 06 42 14 00 00 0A 05 00 64 00 00 00 60 EA 00 00 D0 07 00 00 C8 00 00 00 B8 ED

void my_gps_init(void)
{
  // ��ʼ����������  GPSģ��
  gpio_init(A1, GPI, GPIO_HIGH, GPI_PULL_UP);											
  uart_init(UART_2, 115200 , UART2_TX_A2 , UART2_RX_A3);

  uart_putbuff(UART_2, (uint8 *)close_gga_cmd, sizeof(close_gga_cmd));
  uart_putbuff(UART_2, (uint8 *)close_gsa_cmd, sizeof(close_gsa_cmd));
  uart_putbuff(UART_2, (uint8 *)close_gsv_cmd, sizeof(close_gsv_cmd));

  uart_putbuff(UART_2, (uint8 *)close_zda_cmd, sizeof(close_zda_cmd));
  uart_putbuff(UART_2, (uint8 *)set_rate_10hz, sizeof(set_rate_10hz));
  uart_putbuff(UART_2, (uint8 *)close_ant_cmd, sizeof(close_ant_cmd));

  uart_rx_irq(UART_2, ENABLE);
}

//�ж��Ƿ�����������
int8 analysis_error = 0;
//���ķ���
//���ݱ���
void parseGpsBuffer(void)
{
  char *subString;
  char *subStringNext;
  char i = 0;
  if (Save_Data.isGetData) //����ַ�����Ϊ��
  {
    for (i = 0; i <= 8; i++) //ѭ��7��
    {
      if (i == 0)
      {
        if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL) //û�м�⵽����
        {
          analysis_error = 1;
        } //��������
        else
        {
          analysis_error = 0;
        }
      }
      else if (analysis_error == 0) //��⵽���ţ����ض��ŵ�λ��
      {
        subString++;                                          //λ�ü�1����λ�����ŵĺ�һλ��
        if ((subStringNext = strstr(subString, ",")) != NULL) //��λ��һ�����ŵ�λ��
        {
          char usefullBuffer[2];
          switch (i)
          {
          case 1:
            memcpy(Save_Data.UTCTime, subString, subStringNext - subString); //��������֮��Ϊ ʱ����Ϣ ��ת���ɱ���ʱ��
            Save_Data.UTCTime[1] = Save_Data.UTCTime[1] + 8;
            if (Save_Data.UTCTime[1] > '9')
            {
              Save_Data.UTCTime[0]++;
              if (Save_Data.UTCTime[0] == '3')
                Save_Data.UTCTime[0] = '0';
              Save_Data.UTCTime[1] = (Save_Data.UTCTime[1] % '9') + '0' - 1;
            }
            break; //����switch
          case 2:
            memcpy(usefullBuffer, subString, subStringNext - subString);
            break; //�����Ƿ���Ч��־
          case 3:
            memcpy(Save_Data.latitude, subString, subStringNext - subString);
            break; //��ȡγ����Ϣ
          case 4:
            memcpy(Save_Data.N_S, subString, subStringNext - subString);
            break; //��ȡN/S
          case 5:
            memcpy(Save_Data.longitude, subString, subStringNext - subString);
            break; //��ȡ������Ϣ
          case 6:
            memcpy(Save_Data.E_W, subString, subStringNext - subString);
            break; //��ȡE/W
          case 7:
            memcpy(Save_Data.ground_speed, subString, subStringNext - subString);
            break; //��ȡ�ٶ�
          case 8:
            memcpy(Save_Data.ground_direction, subString, subStringNext - subString);
            break; //��ȡ�����
          default:
            break;
          }
          subString = subStringNext;    //��һ������λ�ø���һ��ָ�룬
          Save_Data.isParseData = true; //�ֶ�����ֵ���������Ƿ������ɣ�

          /*�����Ϣ�Ƿ���Ч*/
          if (usefullBuffer[0] == 'A')
            Save_Data.isUsefull = true;
          else if (usefullBuffer[0] == 'V')
            Save_Data.isUsefull = false;
        }
        else
        {
          analysis_error = 1; //��������
        }
      }
    }
  }
}
//��еķ����ȡ
void zhufei_nmmsl()
{

  if (Save_Data.ground_direction[2] == '.')
  {
    Save_Data.f_ground_direction = (Save_Data.ground_direction[0] - '0') * 10 + (Save_Data.ground_direction[1] - '0') * 1 + (Save_Data.ground_direction[3] - '0') * 0.1 + (Save_Data.ground_direction[4] - '0') * 0.01 + (Save_Data.ground_direction[5] - '0') * 0.001;
  }
  else if (Save_Data.ground_direction[1] == '.')
  {
    Save_Data.f_ground_direction = (Save_Data.ground_direction[0] - '0') * 1 + (Save_Data.ground_direction[2] - '0') * 0.1 + (Save_Data.ground_direction[3] - '0') * 0.01 + (Save_Data.ground_direction[4] - '0') * 0.001;
  }
  else if (Save_Data.ground_direction[3] == '.')
  {
    Save_Data.f_ground_direction = (Save_Data.ground_direction[0] - '0') * 100 + (Save_Data.ground_direction[1] - '0') * 10 + (Save_Data.ground_direction[2] - '0') * 1 + (Save_Data.ground_direction[4] - '0') * 0.1 + (Save_Data.ground_direction[5] - '0') * 0.01;
  }
  else
  {
    Save_Data.f_ground_direction = 0;
  }

  //�ٶȻ�ȡ
  //��пƼ�

  if (Save_Data.ground_speed[2] == '.')
  {
    Save_Data.f_ground_speed = (Save_Data.ground_speed[0] - '0') * 10 + (Save_Data.ground_speed[1] - '0') * 1 + (Save_Data.ground_speed[3] - '0') * 0.1 + (Save_Data.ground_speed[4] - '0') * 0.01 + (Save_Data.ground_speed[5] - '0') * 0.001;
  }
  else if (Save_Data.ground_speed[1] == '.')
  {
    Save_Data.f_ground_speed = (Save_Data.ground_speed[0] - '0') * 1 + (Save_Data.ground_speed[2] - '0') * 0.1 + (Save_Data.ground_speed[3] - '0') * 0.01 + (Save_Data.ground_speed[4] - '0') * 0.001;
  }
  else if (Save_Data.ground_speed[3] == '.')
  {
    Save_Data.f_ground_speed = (Save_Data.ground_speed[0] - '0') * 100 + (Save_Data.ground_speed[1] - '0') * 10 + (Save_Data.ground_speed[2] - '0') * 1 + (Save_Data.ground_speed[4] - '0') * 0.1 + (Save_Data.ground_speed[5] - '0') * 0.01;
  }
  else
  {
    Save_Data.f_ground_speed = 0;
  }

  Save_Data.f_ground_speed = Save_Data.f_ground_speed * 1.85;
}

char Get_InitData(double *Lon_Z, double *Lon_X, double *Lat_Z, double *Lat_X)
{
  int32 num = 0;
  //��Ч����ֱ���Թ�
  if (Save_Data.isUsefull == false)
  {
    return 0;
  }
  //�ж�γ��ֵ���Ƿ��С�.'��:˵�������� ���磺3946.99715  �仯��Χ�ں���λ�б仯�����һλ���ȶ�����ȥ��
  if ((strstr(Save_Data.latitude, ".")) != NULL)
  {
    *Lat_Z = (Save_Data.latitude[0] - '0') * 10 + (Save_Data.latitude[1] - '0') * 1;
    num = (Save_Data.latitude[2] - '0') * 1000000 + (Save_Data.latitude[3] - '0') * 100000 + (Save_Data.latitude[5] - '0') * 10000 + (Save_Data.latitude[6] - '0') * 1000 + (Save_Data.latitude[7] - '0') * 100 + (Save_Data.latitude[8] - '0') * 10 + (Save_Data.latitude[9] - '0');

    *Lat_X = num * 1.0 / 100000; //ȡֵΪ39 �� 46.99715
  }
  else
  {
    Save_Data.isUsefull = false;
  }

  //����ԭ��ͬ�� ���磺11628.32198
  if ((strstr(Save_Data.longitude, ".")) != NULL)
  {
    *Lon_Z = (Save_Data.longitude[0] - '0') * 100 + (Save_Data.longitude[1] - '0') * 10 + (Save_Data.longitude[2] - '0') * 1;

    num = (Save_Data.longitude[3] - '0') * 1000000 + (Save_Data.longitude[4] - '0') * 100000 + (Save_Data.longitude[6] - '0') * 10000 + (Save_Data.longitude[7] - '0') * 1000 + (Save_Data.longitude[8] - '0') * 100 + (Save_Data.longitude[9] - '0') * 10 + (Save_Data.longitude[9] - '0');

    *Lon_X = num * 1.0 / 100000; //ȡֵΪ116 �� 28.32198
  }
  else
  {
    Save_Data.isUsefull = false;
  }
  //����ground_speed��ground_direction����
  //���⣬�����ַ�����ʽ����������
  //Save_Data.f_ground_direction = (Save_Data.ground_direction[0] - '0') * 100 + (Save_Data.ground_direction[1] - '0') * 10 + (Save_Data.ground_direction[2] - '0') * 1 + (Save_Data.ground_direction[4] - '0') * 0.1 + (Save_Data.ground_direction[5] - '0') * 0.01;

  //����ԭ��
  //Save_Data.f_ground_speed = (Save_Data.ground_speed[0] - '0') * 100 + (Save_Data.ground_speed[1] - '0') * 10 + (Save_Data.ground_speed[2] - '0') * 1 + (Save_Data.ground_speed[4] - '0') * 0.1 + (Save_Data.ground_speed[5] - '0') * 0.01;

  zhufei_nmmsl();

  return 1;
}

//��������С�����ֻ���λ,֮ǰ��ʱ����
void Trans_Data_Z_plus_X()
{
  //����������С����60�ٿ���
  // Save_Data.lati_Z = Lat_Z;
  Save_Data.lati_X = Save_Data.lati_X / 60;

  Save_Data.longi_X = Save_Data.longi_X / 60;
  //Save_Data.longi_Z = Lon_Z ;

  Save_Data.f_lati = Save_Data.lati_X + Save_Data.lati_Z;
  Save_Data.f_longi = Save_Data.longi_X + Save_Data.longi_Z;
}

//������������

double radians(double angle)
{
  return (angle * (PI / 180));
}

double sq(double a)
{
  return (a * a);
}
//���ϴ� ���ŶȲ���
double GPSDistance(double lati1, double longi1, double lati2, double longi2)
{
  double lat1 = lati1;

  double long1 = longi1;

  double lat2 = lati2;

  double long2 = longi2;

  double delta = radians(long1 - long2);
  double sdlong = sin(delta);
  double cdlong = cos(delta);

  double rlat1 = radians(lat1);
  double rlat2 = radians(lat2);

  double slat1 = sin(rlat1);
  double clat1 = cos(rlat1);

  double slat2 = sin(rlat2);
  double clat2 = cos(rlat2);

  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
  delta = sq(delta);
  delta += sq(clat2 * sdlong);
  delta = sqrt(delta);

  double denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
  delta = atan2(delta, denom);
  delta = delta * 6378137;

  return delta;
}

//�����㷽λ��
double GPSBearingAngle(double lati1, double longi1, double lati2, double longi2)
{ // The function that calculates the angle to destination
  // given our updated current GPS coordinates

  double lat1 = lati1;

  double long1 = longi1;

  double lat2 = lati2;

  double long2 = longi2;

  double y_angle = cos(lat2) * sin(long2 - long1);
  double x_angle = (cos(lat1) * sin(lat2)) - (sin(lat1) * cos(lat2) * cos(long2 - long1));

  double gpsAngleRads = atan2(y_angle, x_angle);                       // Angle is in radians
  double gpsAngleDegreesFloat = (360.00 + ((180 / PI) * gpsAngleRads)); // convert radians to degrees

  double angle = fmod(gpsAngleDegreesFloat, 360.00);

  return angle;
}

//��ʵ�ʷ�λ����Ŀ�귽λ��֮������
void GpsAngleError()
{
  double delta = 0.0;
  //gps�����������Ǻ���
  //�ڲٳ���������gps��
  //��һ��������������
  if (passed_sections <= 0)
  {
      //����GPS�Ƕ��������ǽǶ�
      delta = Use_Data.angle_next_heading_now - Data_Gyro.gyro_direction;
  }
  else
  {
     delta = Use_Data.angle_next_heading_now - Save_Data.f_ground_direction;
  }
 
  
  //���mod360�Ļ���
  if (fabs(delta) > 180)
  {
    if (delta < -180)
    {
      delta = 360 + (delta);
    }
    else
    {
      delta = -360 + (delta);
    }
  }
  else
  {
    delta = delta;
  }
 

      Use_Data.gps_angle_error_now = delta;
    
    
   
}



/********************************
���㴦��
**********************************/
void GpsDotSelection()
{
      static int change_time = 0;
      static  int last_change = 0;
      
      //��¼�ϴλ���
      if (passed_sections == 0)
      {
          last_change = 0;
      }
      
      
      
      //��Ŀ�꺽������㺽��Ĳ�ֵ����ֵ(������������)
      Use_Data.change_angle_delta = delta_heading_angle(Use_Data.angle_next_heading_now, Use_Data.TwoDotsDirec);
      
      //��������˽Ƕȷ�Χ
      if(area_tag == DIRECT_AREA || area_tag == ROUND_OUT_AREA)
      {
        if ( Use_Data.change_angle_delta > 30.0   || Use_Data.distan_next_point_now < (distance_each_point_direct * 0.3))
        {
              change_point = 1;
        }
      }
      else
      {
        if ( Use_Data.change_angle_delta > 50.0   || Use_Data.distan_next_point_now < 1.50 )
        {
              change_point = 1;
        }
      }
      
    

    if (change_point > 0) 
    {
      if (0 < passed_sections < finished_gpspoints)
      {
        passed_sections++;
      }
      
   
      last_change = change_point;
      //�������
      change_point = 0;
      //Ҫ�����¶�ȡ
      dot_read = false;
      //����
      change_time = 0;
      //���������¼���
      distance_from_lastpoint = 0;
    }
}
//�����������֮������Ƕȵľ���ֵ,���ڻ������
double delta_heading_angle(double base_angle, double aiming_angle)
{
  double delta = base_angle - aiming_angle;
  double solution = 0.0;

  if (fabs(delta) > 180)
  {
    if (delta < -180)
    {
      solution = 360 + (delta);
    }
    else
    {
      solution = 360 - (delta);
    }
  }
  else
  {
    solution = fabs(delta);
  }
  return solution;
}






  //��ʼʱ��ߴ��ںܴ���߼�©��
  //��һ�¹ص��޷�
  /********************ֱ�����޷�****************/
  /*if ( (area_tag == DIRECT_AREA || area_tag == ROUND_OUT_AREA) &&  ( passed_sections <= 0))
  {
    debug_flag = 1;
    //�ܹ��޷�
    //������ұ�̫��
    if (Data_Gyro.angle_sum_area > 4.0)
    {
        if (delta > 0)
        {
          delta = 0.0;
        }
    }
    else if (Data_Gyro.angle_sum_area < - 4.0)
    {
        if (delta < 0)
        {
          delta = 0.0;
        }
    }
    


    //�����޷�+�Ծ�ƫ
    if ( (delta + Data_Gyro.angle_sum_area)  > 4.0  )
    {
      Use_Data.gps_angle_error_now = 4.0 - Data_Gyro.angle_sum_area;
    }
    else if ( (delta + Data_Gyro.angle_sum_area) < -4.0 )
    {
      Use_Data.gps_angle_error_now = -4.0 - Data_Gyro.angle_sum_area;
    }
    else
    {
      Use_Data.gps_angle_error_now = delta;
    }
    
  }

    /****************ֱ���޷���������****************/
  //�����ֱ�����򣬵��δ�Ǹı�ѹ��20����
 /* else if ( (area_tag == DIRECT_AREA || area_tag == ROUND_OUT_AREA) && (passed_sections >= 1 ) )
  {
    //�ܹ��޷�
    //������ұ�̫��
    debug_flag = 2;
    if (Data_Gyro.angle_sum_area > direct_thres)
    {
        if (delta > 0)
        {
          delta = 0.0;
        }
    }
    else if (Data_Gyro.angle_sum_area < -direct_thres)
    {
        if (delta < 0)
        {
          delta = 0.0;
        }
    }
    


    //�����޷�+�Ծ�ƫ
    if ((delta + Data_Gyro.angle_sum_area) > direct_thres)
    {
      Use_Data.gps_angle_error_now = direct_thres - Data_Gyro.angle_sum_area;
    }
    else if ((delta + Data_Gyro.angle_sum_area) < -direct_thres)
    {
      Use_Data.gps_angle_error_now = -direct_thres - Data_Gyro.angle_sum_area;
    }
    else
    {
      Use_Data.gps_angle_error_now = delta;
    }
    
  }
  else
  {
    Use_Data.gps_angle_error_now = delta;
  }*/





  









