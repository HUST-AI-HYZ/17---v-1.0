#ifndef _GPS_H
#define _GPS_H

#include "common.h"
#include "general.h"
#include "parameter.h"
#define ALL_POINTS 190

#define DIRECT_AREA 0
#define ROUND_AREA 1
#define ROUND_IN_AREA 2
#define ROUND_OUT_AREA 3
#define COLLECTION_BY_HAND 5
double GPSBearingAngle(double lati1,double longi1,double lati2,double longi2);
double GPSDistance(double lati1,double longi1,double lati2,double longi2);
double delta_heading_angle(double base_angle , double aiming_angle);

typedef struct SaveData 
{
  char GPS_Buffer[128];         //��������
  char isGetData;		//�Ƿ��ȡ��GPS����
  char isParseData;	        //�Ƿ�������
  char UTCTime[11];		//UTCʱ��
  char latitude[11];		//γ��
  char N_S[2];		        //N/S
  char longitude[12];		//����
  char E_W[2];		        //E/W
  char ground_speed[6];      //�Ե����ٶ�
  char ground_direction[6];   //�Եغ���

  char isUsefull;		        //��λ��Ϣ�Ƿ���Ч

  double lati_X ;           //γ��С������
  double lati_Z ;           //γ����������

  double longi_X;
  double longi_Z;

  double f_longi;          //�����ȷ���ת��֮��ľ���ֵ
  double f_lati;

  double f_ground_speed;
  double f_ground_direction;

} _SaveData;

typedef struct UseData  //ʵ�ʵĲ�ֵ
{
  //int lat_err;          //γ�Ȳ�
  //int lon_err;          //���Ȳ�
  //int lat_centre;       //�����������ĵ�γ��
  //int lon_centre;       //�����������ĵ㾭��
  //float radius;         //Բ���İ뾶
  
  //float distan_centre;  //��Բ�ĵľ���
  //float lat_cent_err;   //�����ĵ�γ�Ȳ�
  //float lon_cent_err;   //�����ĵľ��Ȳ�
  double lat_last;
  double lon_last;


  double lat_next;   //��һ�����γ��
  double lon_next;   //��һ����ľ���


  double TwoDotsDis;   //ǰ������ľ���(�̶�ֵ)
  double TwoDotsDirec;  //�����ĺ���

  double angle_next_heading_now;  //��ǰ�㵽��һ����ĺ����
  double angle_nexttwo_heading_now;
  double distan_next_point_now;    //����һ��Ŀ���ľ���
  double gps_angle_error_now;   //ʵ�ʽǶ���Ŀ��Ƕȵ�ƫ��
  double change_angle_delta;
 
} _UseData;

//��ִ洢
typedef struct  KeyPoint
{
  double lat_keypoint;
  double lon_keypoint;
  
  float lat_kypnt_f;   //γ��С��
  float lon_kypnt_f;   //����
  int16 lat_kypnt_z;   //γ������
  int16 lon_kypnt_z;   //����
  
  
  int16 speed_type;      //�ٶ�����
  int16 type_point;       //�������
  //bool pass_by;         //�Ƿ�ͨ�� 
  
}_KeyPoint;



void  Gps_Mokato(double B , double L , double * X , double * Y);
//ͣ������
extern float stop_distance;
//Ŀ��Ȧ��
extern uint16 circle_time;

extern _SaveData Save_Data;
extern _UseData Use_Data;

extern _KeyPoint Points_Stored[ALL_POINTS];
extern _KeyPoint Points_Use[ALL_POINTS + 3];
//΢���жϣ��൱�ڻ����������
extern int16 area_tag;

//����ʱ���ַ��������ڴ���sdcard����
extern char StartTime[12];


extern int16 passed_sections;
//gps_����
//extern int16 gps_reset;
//GPS��ģʽ
extern int16 gps_mode;
//ѡ�е��Ѳɼ������ݵ�����ֵ
extern uint16 point_selected_gps;
//��ɲɼ������ݵ�
extern uint16 finished_gpspoints;
//ѡ�е��γ��
extern uint32 point_latitude_selected;
//ѡ�е�ľ���
extern uint32 point_longitude_selected;
//����һ�����������߹��ľ��롣(������һ����...)
extern float distance_from_lastpoint;
//ÿ����ļ��(ֱ��·)
extern float distance_each_point_direct;
//ÿ����ļ�������·��
extern float distance_each_point_round;

//��ʻ����
//���Ϊ������������ʱ��
extern int16 guiding_mode;
//���Ƶ����(������������0��ʼ)
extern uint16 guess_start;
//���ȶ���
extern double longitude_Q[50];
//γ�ȶ���
extern double latitude_Q[50];
extern double angle_error_next[5];
//�Ƿ񻻵�
extern int8 change_point;
//������
extern int16 point_type;
extern int16 point_type_pageread;
//��������
extern int8 now_area;
extern int8 last_area;
//icm����
extern float error_icm_last;
//��ǰ������ʵ�������
extern float error_lenth_s_p;
//���򣨵�ǰ��ָ���0���㣩
extern float error_direction;
//·��ѡ��
extern int route_read;
extern int route_write;
//ˮƽ���
extern float horizontal_error;
//GPS΢��
//��һ���л���΢��
#define ZONE 8
//��������
#define ROUTE 4
extern int16 run_sections;
//�Ƿ��ӳٷ���
extern int whether_delay;
extern int16 micro_op_lati[ROUTE][ZONE];
extern int16 micro_op_longi[ROUTE][ZONE];
//΢�ٶ�
extern int16 micro_area;
//��һ��΢�ٶ�
extern int16  last_micro_area;
//ֱ����ֵ
extern uint16 direct_thres;
//ֱ������Ƕ�
extern float direc_change_dis;
//�������Ƕ�
extern float round_change_dis;
#endif 
