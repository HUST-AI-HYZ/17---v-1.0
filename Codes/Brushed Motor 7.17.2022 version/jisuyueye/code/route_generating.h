#ifndef _ROUTE_GENERATING_H
#define _ROUTE_GENERATING_H
#include "gps.h"
#include "stdlib.h"
#include <math.h>
#include "isr.h"
#include <stdlib.h>
#include <string.h>
#include "common.h"

void round_in_out();

//�����ĸ�ֱ���л������·��
typedef struct FourPoints
{
	//�ؼ���γ��
	double kps_latitude;
	double kps_longitude;
	//������
	int kps_type;
	//0 ��ʾ��㣬Ҳ���ǵ�һ��ֱ���л���
}FourPoints;

extern FourPoints kps_hust_est_plygrnd[4];

#endif
