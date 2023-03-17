#include "parameter.h"
#include "gps.h"
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include "route_generating.h"
#include "button.h"
//���ڱ��������·��
typedef struct RPoints
{
    //γ��
    double rps_latitude;
    double rps_longitude;
    //������
    int rps_type;
} RPoints;
//��ʱ��double�ͱ��棬������Ϳ��Ժ���ת��
int out1 = 84;
int out2 = 172;

FourPoints kps_plygrnd[4];
RPoints rps_plygrnd[150];

int generate_round_road(float step, FourPoints strt, FourPoints end, int* delt_dots);
int generate_direct_road(float step, FourPoints strt, FourPoints end, int* delt_dots);
int write_in(int delt_dots, double*dis, double*direction);




void generating_route_init()
{
		//���Ź����ݼ����м䣩0
	out1 = 78, out2 = 172;
	kps_plygrnd[3].kps_latitude = 32.20905;
	kps_plygrnd[3].kps_longitude = 118.72094;
	kps_plygrnd[3].kps_type = 3;

	kps_plygrnd[2].kps_latitude = 32.20828;
	kps_plygrnd[2].kps_longitude = 118.72105;
	kps_plygrnd[2].kps_type = 2;

	kps_plygrnd[1].kps_latitude = 32.20819;
	kps_plygrnd[1].kps_longitude = 118.72019;
	kps_plygrnd[1].kps_type = 1;

	kps_plygrnd[0].kps_latitude = 32.20894;
	kps_plygrnd[0].kps_longitude = 118.72008;
	kps_plygrnd[0].kps_type = 0;

	//���Ź����ݼ������ڣ�1
	out1 = 78, out2 = 172;
	kps_plygrnd[3].kps_latitude = 32.20905;
	kps_plygrnd[3].kps_longitude = 118.720925;
	kps_plygrnd[3].kps_type = 3;

	kps_plygrnd[2].kps_latitude = 32.20828;
	kps_plygrnd[2].kps_longitude = 118.721035;
	kps_plygrnd[2].kps_type = 2;

	kps_plygrnd[1].kps_latitude = 32.20819;
	kps_plygrnd[1].kps_longitude = 118.720205;
	kps_plygrnd[1].kps_type = 1;

	kps_plygrnd[0].kps_latitude = 32.20894;
	kps_plygrnd[0].kps_longitude = 118.720095;
	kps_plygrnd[0].kps_type = 0;
	
	 //ÿ����ļ��(ֱ��·)
   distance_each_point_direct = 5.0;
   //ÿ����ļ�������·��
   distance_each_point_round = 3.0;
}







void generating_route()
{
    //��ʼ������
    //���0
    for (int i = 0; i < ALL_POINTS; i++)
    {
        Points_Stored[i].lat_keypoint = 0.0;
        Points_Stored[i].lon_keypoint = 0.0;

        Points_Stored[i].lat_kypnt_f = 0.0;
        Points_Stored[i].lon_kypnt_f = 0.0;
        Points_Stored[i].lat_kypnt_z = 0.0;
        Points_Stored[i].lon_kypnt_z = 0.0;

        Points_Stored[i].type_point = 0.0;
    }
    //��ʱ���ݣ��е㣩
    double temp_mid_point_lati = 0.0;
    double temp_mid_point_longi = 0.0;

    //����ĵ���
    int delt_dots = 0;
    //У��������
    double exam_distance[200] = { 0.0 };
    double exam_direction[200] = { 0.0 };

    //�����һ��ֱ��
    rps_plygrnd[0].rps_latitude = kps_plygrnd[0].kps_latitude;
    rps_plygrnd[0].rps_longitude = kps_plygrnd[0].kps_longitude;
    rps_plygrnd[0].rps_type = 0;
    generate_direct_road(distance_each_point_direct, kps_plygrnd[0], kps_plygrnd[1],&delt_dots);
    generate_round_road(distance_each_point_round, kps_plygrnd[1], kps_plygrnd[2], &delt_dots);
    generate_direct_road(distance_each_point_direct, kps_plygrnd[2], kps_plygrnd[3], &delt_dots);
    generate_round_road(distance_each_point_round, kps_plygrnd[3], kps_plygrnd[0], &delt_dots);

    //У����
    for (size_t i = 0; i < delt_dots + 1; i++)
    {
        exam_distance[i] = GPSDistance(rps_plygrnd[i].rps_latitude, rps_plygrnd[i].rps_longitude, rps_plygrnd[i + 1].rps_latitude, rps_plygrnd[i + 1].rps_longitude);
        exam_direction[i] = GPSBearingAngle(rps_plygrnd[i].rps_latitude, rps_plygrnd[i].rps_longitude, rps_plygrnd[i + 1].rps_latitude, rps_plygrnd[i + 1].rps_longitude);
    }

    write_in(delt_dots, exam_distance, exam_direction);
    //��������
    round_in_out();
}

//�ǶȽ���Ҫע��360�������

int generate_round_road(float step, FourPoints strt, FourPoints end,int *delt_dots)
{
    double mid_lati = (strt.kps_latitude + end.kps_latitude) / 2;
    double mid_longi = (strt.kps_longitude + end.kps_longitude) / 2;
    double radius = GPSDistance(mid_lati, mid_longi, strt.kps_latitude, strt.kps_longitude);
    //����Ƕȸı���,sina��a,ע�⻡��ֵ�ͽǶ�ֵת��
    double delta_sita = (step / radius) * 180 / PI;
    //�������
    int times = (int)(180 / delta_sita);
    //������Ƕ�
    delta_sita = 180 * 1.0 / times;
    double vector_lati = 0.0;
    double vector_longi = 0.0;
    //�����е����Ҳ�������
    //���ҵ���

    vector_lati = strt.kps_latitude - mid_lati;
    vector_longi = strt.kps_longitude - mid_longi;

    //��û�й���
    int passbyhalf = 0;
    //��֮��ֱ������
    double verti_vector_lati = vector_longi;
    double verti_vector_longi = -vector_lati;
    //��������
    //�Ƕȱ��ʣ����ʹ��������
    double a_ratio = 0.001;
    double dis_ratio = 0.001;
    double error_ratio = 0;
    double direction_vector_lati = vector_lati + verti_vector_lati * a_ratio;
    double direction_vector_longi = vector_longi + verti_vector_longi * a_ratio;
    //Բֱ����׼��
    double basic_angle = 0.0;
    //���ҵ���
    basic_angle = GPSBearingAngle(mid_lati, mid_longi, strt.kps_latitude, strt.kps_longitude);

    //�˳���ʽ
    bool out_way = true;

    int i = 1;

    //���ֺ�ľ���(������)
    double angle_sum = 0;
    double gene_dis1[80] = {0.0};
    double gene_dis2[80] = { 0.0 };
    double gene_dis3[80] = { 0.0 };
    double di_angle[80] = { 0.0 };

    //ˮƽ��������

    while (i < times + 1)
    {
        //��i������Բ�ĵķ�λ��
        double dir_angle = GPSBearingAngle(mid_lati, mid_longi, mid_lati + direction_vector_lati, mid_longi + direction_vector_longi);
        //�ı���(��360�Ǹ���)
        double change_angle = delta_heading_angle(basic_angle, dir_angle);

        while (change_angle < angle_sum + delta_sita)
        {
            if (passbyhalf == 0)
            {
                direction_vector_lati = vector_lati + verti_vector_lati * a_ratio;
                direction_vector_longi = vector_longi + verti_vector_longi * a_ratio;
                a_ratio = a_ratio + 0.001;
            }
            else
            {
                direction_vector_lati = - vector_lati + verti_vector_lati * a_ratio;
                direction_vector_longi = - vector_longi + verti_vector_longi * a_ratio;
                a_ratio = a_ratio - 0.001;
            }
            //��i������Բ�ĵķ�λ��
            dir_angle = GPSBearingAngle(mid_lati, mid_longi, mid_lati + direction_vector_lati, mid_longi + direction_vector_longi);
            //�ı���
            change_angle = delta_heading_angle(basic_angle, dir_angle);
        }

        //�����ĵ�ľ���
        double dis_tomidpnt = GPSDistance(mid_lati, mid_longi, mid_lati + direction_vector_lati* dis_ratio, mid_longi + direction_vector_longi* dis_ratio);

        while (dis_tomidpnt < radius)
        {
            dis_ratio = dis_ratio + 0.001;
            dis_tomidpnt = GPSDistance(mid_lati, mid_longi, mid_lati + direction_vector_lati * dis_ratio, mid_longi + direction_vector_longi * dis_ratio);
        }
        //��һ�η���
        rps_plygrnd[*delt_dots + i].rps_latitude = mid_lati + direction_vector_lati * dis_ratio;
        rps_plygrnd[*delt_dots + i].rps_longitude = mid_longi + direction_vector_longi * dis_ratio;
        rps_plygrnd[*delt_dots + i].rps_type = 1;


        gene_dis1[i] = GPSDistance(rps_plygrnd[*delt_dots + i].rps_latitude, rps_plygrnd[*delt_dots + i].rps_longitude, rps_plygrnd[*delt_dots + i - 1].rps_latitude, rps_plygrnd[*delt_dots + i - 1].rps_longitude);

        //��������������
        if (gene_dis1[i] < step * 0.90 || gene_dis1[i] > step * 1.10)
        {
            error_ratio = step / gene_dis1[i];

            //����һ��
            rps_plygrnd[*delt_dots + i].rps_latitude = rps_plygrnd[*delt_dots + i - 1].rps_latitude + (rps_plygrnd[*delt_dots + i].rps_latitude - rps_plygrnd[*delt_dots + i - 1].rps_latitude) * error_ratio;
            rps_plygrnd[*delt_dots + i].rps_longitude = rps_plygrnd[*delt_dots + i - 1].rps_longitude + (rps_plygrnd[*delt_dots + i].rps_longitude - rps_plygrnd[*delt_dots + i - 1].rps_longitude) * error_ratio;
            rps_plygrnd[*delt_dots + i].rps_type = 1;
        }
        else
        {

        }
        di_angle[i] = GPSBearingAngle(mid_lati, mid_longi, rps_plygrnd[*delt_dots + i].rps_latitude, rps_plygrnd[*delt_dots + i].rps_longitude);
        angle_sum = delta_heading_angle(basic_angle, di_angle[i]);

        gene_dis2[i] = GPSDistance(rps_plygrnd[*delt_dots + i].rps_latitude, rps_plygrnd[*delt_dots + i].rps_longitude, rps_plygrnd[*delt_dots + i - 1].rps_latitude, rps_plygrnd[*delt_dots + i - 1].rps_longitude);
        gene_dis3[i] = GPSDistance(rps_plygrnd[*delt_dots + i].rps_latitude, rps_plygrnd[*delt_dots + i].rps_longitude, mid_lati, mid_longi);
        dis_ratio = 0.001;

        //ƽʱ75 170�����Ź� 84 172
        if (delta_heading_angle(basic_angle, di_angle[i]) > out1)
        {
            passbyhalf = 1;
        }

        if (delta_heading_angle(basic_angle, di_angle[i]) > out2)
        {
            out_way = false;
            break;
        }

        i++;

    }

    if (out_way == true)
    {
        i--;
    }

    *delt_dots = *delt_dots + i;
    double dis_l = GPSDistance(end.kps_latitude, end.kps_longitude, rps_plygrnd[*delt_dots].rps_latitude, rps_plygrnd[*delt_dots].rps_longitude);

    if (dis_l < distance_each_point_round)
    {
        rps_plygrnd[*delt_dots].rps_latitude = end.kps_latitude;
        rps_plygrnd[*delt_dots].rps_longitude = end.kps_longitude;
        rps_plygrnd[*delt_dots].rps_type = 1;
    }
    else
    {
        rps_plygrnd[*delt_dots].rps_latitude = (end.kps_latitude + rps_plygrnd[*delt_dots - 1].rps_latitude)*0.5;
        rps_plygrnd[*delt_dots].rps_longitude = (end.kps_longitude + rps_plygrnd[*delt_dots - 1].rps_longitude)*0.5;
        rps_plygrnd[*delt_dots].rps_type = 1;
        *delt_dots = *delt_dots + 1;

        rps_plygrnd[*delt_dots].rps_latitude = end.kps_latitude ;
        rps_plygrnd[*delt_dots].rps_longitude = end.kps_longitude;
        rps_plygrnd[*delt_dots].rps_type = 1;
    }

    return 1;
}









int generate_direct_road(float step, FourPoints strt, FourPoints end, int* delt_dots)
{
    //������ʱ���н�����
    if (! ((strt.kps_type == 0 && end.kps_type == 1)  || (strt.kps_type == 2 && end.kps_type == 3)) )
    {
        //��ʾ���д���
        return 0;
    }
    else
    {

    }

    //�����������룬���л���
    double dis = 0.0;
    dis = GPSDistance(strt.kps_latitude, strt.kps_longitude, end.kps_latitude, end.kps_longitude);
    //�����������������㻮�����
    double minstep = dis / (step * 1.05);
    double maxstep = dis / (step * 0.95);
    int stepnumber = 0;
    //���ֵ
    //����Ƚ�С
    if (maxstep - minstep >= 1.0)
    {
        stepnumber = (int)((maxstep + minstep)/2);
    }
    else
    {
        stepnumber = (int)((maxstep)/2);
    }
    //�ȷֻ���
    int i = 1;
    //���ֺ�ľ���(������)
    double gene_dis1[50] = {0.0};
    double gene_dis2[50] = { 0.0 };

    //ÿһС��
    double lati_step = (end.kps_latitude - strt.kps_latitude) / stepnumber;
    double longi_step = (end.kps_longitude - strt.kps_longitude) / stepnumber;
    while (i < stepnumber)
    {
        rps_plygrnd[*delt_dots + i].rps_latitude = rps_plygrnd[*delt_dots + i - 1].rps_latitude + lati_step;
        rps_plygrnd[*delt_dots + i].rps_longitude = rps_plygrnd[*delt_dots + i - 1].rps_longitude + longi_step;
        rps_plygrnd[*delt_dots + i].rps_type = 0;

        gene_dis1[i] = GPSDistance(rps_plygrnd[*delt_dots + i].rps_latitude, rps_plygrnd[*delt_dots + i].rps_longitude, end.kps_latitude, end.kps_longitude);
        gene_dis2[i] = GPSDistance(rps_plygrnd[*delt_dots + i].rps_latitude, rps_plygrnd[*delt_dots + i].rps_longitude, rps_plygrnd[*delt_dots + i - 1].rps_latitude, rps_plygrnd[*delt_dots + i - 1].rps_longitude);
        i++;
    }
    //���һ��ֱ��������
    rps_plygrnd[*delt_dots + i].rps_latitude = end.kps_latitude;
    rps_plygrnd[*delt_dots + i].rps_longitude = end.kps_longitude;
    rps_plygrnd[*delt_dots + i].rps_type = 0;

    *delt_dots = *delt_dots + i;
    return 1;
}

int write_in(int delt_dots, double* dis, double* direction)
{
    finished_gpspoints = delt_dots;

    for (int i = 0; i < delt_dots; i++)
    {
        int16 hund = 0, ten = 0;
        //double����
        Points_Stored[i].lon_keypoint = rps_plygrnd[i].rps_longitude;
        Points_Stored[i].lat_keypoint = rps_plygrnd[i].rps_latitude;

        //����С������
        ten = (int)(rps_plygrnd[i].rps_latitude /10);
        Points_Stored[i].lat_kypnt_z =  ten * 10 + (int)(rps_plygrnd[i].rps_latitude /1 - (ten * 10 ) ) ;
        Points_Stored[i].lat_kypnt_f = rps_plygrnd[i].rps_latitude - Points_Stored[i].lat_kypnt_z;
        //����С������
        hund = (int)(rps_plygrnd[i].rps_longitude /100);
        ten = (int)(rps_plygrnd[i].rps_longitude /10) - hund * 10;
        Points_Stored[i].lon_kypnt_z = hund*100 + ten*10 + (int)(rps_plygrnd[i].rps_longitude - (hund*100 + ten*10));
        Points_Stored[i].lon_kypnt_f = rps_plygrnd[i].rps_longitude - Points_Stored[i].lon_kypnt_z;
        //������
        Points_Stored[i].type_point = rps_plygrnd[i].rps_type;
    }


}

//�Զ��滮����
//��ȷ��������ʱ�뷽�򡢸��ݲ²�������������ݵ���Ϊ��0����������Points_Store����Ŀ�������Զ�����������һȦ�����ӵ���㲢��Ӽ���β��������ǰͣ��
//�ǵü��
void gps_auto_plannation(void)
{
#define PNTNUM 4
    double dis_test_1[PNTNUM]= {0,};
    double dis_test_2[PNTNUM]= {0,};
    int8 incre_1[PNTNUM - 1]= {0};
    int8 incre_2[PNTNUM - 1]= {0};
    int8 incre1_sum = 0;
    int8 incre2_sum = 0;

    //����о���С��20�ģ���ֱ�ӱ�������
    double guess_dis = 10000.0;
    //��Сֵ
    double min_dis = 10000.0;
    //��Ӧ�����
    int16 min_num = -1;
    //���ѵ�
    uint16 search_start = 0;
    //��ֹ��
    uint16 search_end = 0;

    //��ֹԽ��
    if (guess_start < 0)
    {
        guess_start = 0;
    }
    else if (guess_start >= finished_gpspoints)
    {
        guess_start = finished_gpspoints - 1;
    }
    //��ʼ����
    min_dis = GPSDistance(Save_Data.f_lati, Save_Data.f_longi,Points_Stored[guess_start].lat_keypoint, Points_Stored[guess_start].lon_keypoint);
    min_num = guess_start;
    //��������
    for (int i = 1 ; i < finished_gpspoints ; )
    {
        //��������
        if (guess_start + i > finished_gpspoints - 1)
        {
            search_start = - (finished_gpspoints - guess_start - 0) + i;
        }
        else
        {
            search_start = guess_start + i;
        }
        double temp = GPSDistance(Save_Data.f_lati, Save_Data.f_longi,Points_Stored[search_start].lat_keypoint, Points_Stored[search_start].lon_keypoint);

        if( temp < min_dis )
        {
            min_dis = GPSDistance(Save_Data.f_lati, Save_Data.f_longi,Points_Stored[search_start].lat_keypoint, Points_Stored[search_start].lon_keypoint);
            min_num = search_start;
        }
        //������ʽ
        if(i < ALL_POINTS - 8)
        {
            if (temp < 20.0)
            {
                i = i + 1;
            }
            else if(temp >= 20.0 && temp < 50.0)
            {
                i = i + 3;
            }
            else
            {
                i = i + 5;
            }
        }
        else
        {
            i = i + 1;
        }

    }
    //�����ж�
    //��ʱ��
    double angle1 = GPSBearingAngle(Save_Data.f_lati, Save_Data.f_longi,Points_Stored[min_num].lat_keypoint, Points_Stored[min_num].lon_keypoint);
    double angle2 = 0;
    if( (min_num + 1) < finished_gpspoints)
    {
        angle2 = GPSBearingAngle(Points_Stored[min_num].lat_keypoint, Points_Stored[min_num].lon_keypoint,Points_Stored[min_num + 1].lat_keypoint, Points_Stored[min_num + 1].lon_keypoint);
    }
    else
    {
        angle2 = GPSBearingAngle(Points_Stored[min_num].lat_keypoint, Points_Stored[min_num].lon_keypoint,Points_Stored[0].lat_keypoint, Points_Stored[0].lon_keypoint);
    }

    //ͬһ������ô���������Ϊ0��
    int last_one = 0;
    if( fabs(delta_heading_angle(angle1,angle2) < 100) )
    {
        Points_Use[0].lat_keypoint = Points_Stored[min_num].lat_keypoint;
        Points_Use[0].lon_keypoint =  Points_Stored[min_num].lon_keypoint;
        Points_Use[0].speed_type = Points_Stored[min_num].speed_type;
        //�Զ����
        //��ʱ��
        //finished_gpspointsָ������
        for (int i = 1; i < finished_gpspoints ; i ++)
        {
            int temp_pt = 0;
            if (min_num + i > finished_gpspoints - 1)
            {
                temp_pt = - (finished_gpspoints - min_num - 0) + i;
            }
            else
            {
                temp_pt = min_num + i;
            }
            Points_Use[i].lat_keypoint = Points_Stored[temp_pt].lat_keypoint;
            Points_Use[i].lon_keypoint =  Points_Stored[temp_pt].lon_keypoint;
            Points_Use[i].type_point = Points_Stored[temp_pt].type_point;
						Points_Use[i].speed_type = Points_Stored[temp_pt].speed_type;
            last_one =  temp_pt;

        }
    }
    //�µ�һ�ξ���̫���ˣ�������С�����һ������Ϊ0��
    else
    {
        //��һ����ı��
        int first_num = 0;
        if( (min_num + 1) < finished_gpspoints)
        {
            Points_Use[0].lat_keypoint = Points_Stored[min_num + 1].lat_keypoint;
            Points_Use[0].lon_keypoint =  Points_Stored[min_num + 1].lon_keypoint;
            Points_Use[0].type_point =  Points_Stored[min_num + 1].type_point;
					  Points_Use[0].speed_type = Points_Stored[min_num + 1].speed_type;
            first_num = min_num + 1;
        }
        else
        {
            Points_Use[0].lat_keypoint = Points_Stored[0].lat_keypoint;
            Points_Use[0].lon_keypoint =  Points_Stored[0].lon_keypoint;
            Points_Use[0].type_point = Points_Stored[0].type_point;
					  Points_Use[0].speed_type = Points_Stored[0].speed_type;
            first_num = 0;
        }


        //�Զ����
        //��ʱ��
        //finished_gpspointsָ������
        for (int i = 1; i < finished_gpspoints ; i ++)
        {
            int temp_pt = 0;
            if (first_num + i > finished_gpspoints - 1)
            {
                temp_pt = - (finished_gpspoints - first_num - 0) + i;
            }
            else
            {
                temp_pt = first_num + i;
            }
            Points_Use[i].lat_keypoint = Points_Stored[temp_pt].lat_keypoint;
            Points_Use[i].lon_keypoint =  Points_Stored[temp_pt].lon_keypoint;
            Points_Use[i].type_point = Points_Stored[temp_pt].type_point;
						Points_Use[i].speed_type = Points_Stored[temp_pt].speed_type;
            last_one = temp_pt;
        }
    }
    
    //���䰲ȫ����(4����)
    for (int i = 1; i < 4 ; i ++)
    {
        int temp_pt = 0;
        if (last_one + i >= finished_gpspoints)
        {
            temp_pt = - (finished_gpspoints - last_one - 0) + i;
        }
        else
        {
            temp_pt = last_one + i;
        }
        Points_Use[finished_gpspoints + i - 1].lat_keypoint = Points_Stored[temp_pt].lat_keypoint;
        Points_Use[finished_gpspoints + i - 1].lon_keypoint =  Points_Stored[temp_pt].lon_keypoint;
        Points_Use[finished_gpspoints + i - 1].type_point = Points_Stored[temp_pt].type_point;
				Points_Use[finished_gpspoints + i - 1].speed_type = Points_Stored[temp_pt].speed_type;
    }

    if (finished_gpspoints + 3 > ALL_POINTS + 3)
    {
        tft180_show_string(0, 7 * 16, "overflow");
    }
    //�ɼ����������60���Ļ�
    //run��ʱ��������63��

}

//��������΢��
void micro_operating()
{
    int time_tag = 0;
    int j =0;
    for (int i = 0; i < finished_gpspoints; i++)
    {
        //���������nan��ֱ���˳�
        if (isnan(Points_Stored[i].lat_keypoint))
        {
            break;
        }



        //�ж�Ҫ��Ҫ�л�
        if (time_tag == 0 && Points_Stored[i].type_point ==DIRECT_AREA )
        {
            time_tag = 1;
            j ++;
        }
        else if (time_tag == 1 && Points_Stored[i].type_point ==ROUND_IN_AREA )
        {
            time_tag = 2;
            j++;
        }
        else if (time_tag == 2 && Points_Stored[i].type_point ==ROUND_AREA )
        {
            time_tag = 3;
            j++;
        }
        else if (time_tag == 3 && Points_Stored[i].type_point ==ROUND_OUT_AREA )
        {
            time_tag = 4;
            j++;
        }
        else if (time_tag == 4 && Points_Stored[i].type_point ==DIRECT_AREA )
        {
            time_tag = 5;
            j++;
        }
        else if (time_tag == 5 && Points_Stored[i].type_point ==ROUND_IN_AREA )
        {
            time_tag = 6;
            j++;
        }
        else if (time_tag == 6 && Points_Stored[i].type_point ==ROUND_AREA )
        {
            time_tag = 7;
            j++;
        }
        else if (time_tag == 7 && Points_Stored[i].type_point ==ROUND_OUT_AREA )
        {
            Points_Stored[i].lat_keypoint += micro_op_lati[route_read][0]/ 1000000.0;
            Points_Stored[i].lon_keypoint += micro_op_longi[route_read][0]/ 1000000.0;
            Points_Stored[i].lat_kypnt_f +=  micro_op_lati[route_read][0]/ 1000000.0;
            Points_Stored[i].lon_kypnt_f +=  micro_op_longi[route_read][0]/ 1000000.0;
            Points_Stored[i].speed_type = 0;
            break;
        }



        if (time_tag == j )
        {
            Points_Stored[i].lat_keypoint += micro_op_lati[route_read][j]/ 1000000.0;
            Points_Stored[i].lon_keypoint += micro_op_longi[route_read][j]/ 1000000.0;
            Points_Stored[i].lat_kypnt_f +=  micro_op_lati[route_read][j]/ 1000000.0;
            Points_Stored[i].lon_kypnt_f +=  micro_op_longi[route_read][j]/ 1000000.0;
            Points_Stored[i].speed_type = j;
        }



    }

}

//΢�ٻָ�
void micro_operating_recover()
{
    int time_tag = 0;
    int j = 0;
    for (int i = 0; i < finished_gpspoints; i++)
    {
        //���������nan��ֱ���˳�
        if (isnan(Points_Stored[i].lat_keypoint))
        {
            break;
        }

        //�ж�Ҫ��Ҫ�л�
        if (time_tag == 0 && Points_Stored[i].type_point ==DIRECT_AREA )
        {
            time_tag = 1;
            j ++;
        }
        else if (time_tag == 1 && Points_Stored[i].type_point ==ROUND_IN_AREA )
        {
            time_tag = 2;
            j++;
        }
        else if (time_tag == 2 && Points_Stored[i].type_point ==ROUND_AREA )
        {
            time_tag = 3;
            j++;
        }
        else if (time_tag == 3 && Points_Stored[i].type_point ==ROUND_OUT_AREA )
        {
            time_tag = 4;
            j++;
        }
        else if (time_tag == 4 && Points_Stored[i].type_point ==DIRECT_AREA )
        {
            time_tag = 5;
            j++;
        }
        else if (time_tag == 5 && Points_Stored[i].type_point ==ROUND_IN_AREA )
        {
            time_tag = 6;
            j++;
        }
        else if (time_tag == 6 && Points_Stored[i].type_point ==ROUND_AREA )
        {
            time_tag = 7;
            j++;
        }
        else if (time_tag == 7 && Points_Stored[i].type_point ==ROUND_OUT_AREA )
        {
            Points_Stored[i].lat_keypoint -= micro_op_lati[route_read][0]/ 1000000.0;
            Points_Stored[i].lon_keypoint -= micro_op_longi[route_read][0]/ 1000000.0;
            Points_Stored[i].lat_kypnt_f -=  micro_op_lati[route_read][0]/ 1000000.0;
            Points_Stored[i].lon_kypnt_f -=  micro_op_longi[route_read][0]/ 1000000.0;
            break;
        }


        if (time_tag == j )
        {
            Points_Stored[i].lat_keypoint -= micro_op_lati[route_read][j]/ 1000000.0;
            Points_Stored[i].lon_keypoint -= micro_op_longi[route_read][j]/ 1000000.0;
            Points_Stored[i].lat_kypnt_f -=  micro_op_lati[route_read][j]/ 1000000.0;
            Points_Stored[i].lon_kypnt_f -=  micro_op_longi[route_read][j]/ 1000000.0;
        }



    }

}


//���򻮷�
void round_in_out()
{
    int last_type_tag = Points_Stored[0].type_point;
    //ȷ��
    for (int i = 0; i < finished_gpspoints; i++)
    {
        //���������nan��ֱ���˳�
        if (isnan(Points_Stored[i].lat_keypoint))
        {
            break;
        }
        //�ж�Ҫ��Ҫ�л�
        if (last_type_tag != Points_Stored[i].type_point)
        {
            if (Points_Stored[i].type_point == 1 && last_type_tag == 0)
            {
							  Points_Stored[i - 1].type_point = ROUND_IN_AREA;
							  for (int j = 0; j <= 8; j++)
                {							
                   Points_Stored[i + j].type_point = ROUND_IN_AREA;
                }
							  i = i + 8;
                last_type_tag = 1;
            }
            else
            {
							if(i != 0)
							{
                Points_Stored[i].type_point = ROUND_OUT_AREA;
								for (int j = 0; j <= 6; j++)
								{
									Points_Stored[i - j].type_point = ROUND_OUT_AREA;
								}
                last_type_tag = 0;
							}
            }
        }
        else
        {
            last_type_tag = Points_Stored[i].type_point;
        }
    }

    Points_Stored[0].type_point = ROUND_OUT_AREA;
		for (int j = 0; j <= 6; j++)
		{
			Points_Stored[finished_gpspoints - j].type_point = ROUND_OUT_AREA;
		}
}

//�˹��ȵ㷨
void collection_by_hand()
{
    static bool collect_flag = false;

    if (button_mod % 2 == 1 && collect_flag == false)
    {
        if (gpio_get(SWITCH_4) == 0)
        {
            if(Save_Data.lati_Z == 32 && Save_Data.longi_Z == 118)
            {
                Points_Stored[finished_gpspoints].lat_keypoint =  Save_Data.f_lati;
                Points_Stored[finished_gpspoints].lon_keypoint = Save_Data.f_longi;

                Points_Stored[finished_gpspoints].lat_kypnt_f = Save_Data.lati_X;
                Points_Stored[finished_gpspoints].lon_kypnt_f = Save_Data.longi_X;
                Points_Stored[finished_gpspoints].lat_kypnt_z = Save_Data.lati_Z;
                Points_Stored[finished_gpspoints].lon_kypnt_z = Save_Data.longi_Z;

                Points_Stored[finished_gpspoints].type_point = area_tag;
                finished_gpspoints++;
                collect_flag = true;
                tft180_show_string(0, 16, "SAVEPNTS");
                FlashWhitePoints();
            }
        }
    }
    //gps_mode == 5��������
    else if (button_mod % 2 == 0 && collect_flag == true)
    {
        if (gpio_get(SWITCH_4) == 0)
        {
            if(Save_Data.lati_Z == 32 && Save_Data.longi_Z == 118)
            {
                Points_Stored[finished_gpspoints].lat_keypoint =  Save_Data.f_lati;
                Points_Stored[finished_gpspoints].lon_keypoint = Save_Data.f_longi;

                Points_Stored[finished_gpspoints].lat_kypnt_f = Save_Data.lati_X;
                Points_Stored[finished_gpspoints].lon_kypnt_f = Save_Data.longi_X;
                Points_Stored[finished_gpspoints].lat_kypnt_z = Save_Data.lati_Z;
                Points_Stored[finished_gpspoints].lon_kypnt_z = Save_Data.longi_Z;

                Points_Stored[finished_gpspoints].type_point = area_tag;

                finished_gpspoints++;
                collect_flag = false;
                tft180_show_string(0, 16, "SAVEPNTS");
                FlashWhitePoints();
            }
        }

    }
}
//�µ���������ʱ��ɾ���ɵ�΢������
void micro_delete()
{
    //���е�΢��ɾ��
    for (int i = 0 ; i < ZONE ; i++)
    {
        micro_op_lati[route_read][i] = 0;
        micro_op_longi[route_read][i] = 0;
    }
    FlashWriteMicro();
}