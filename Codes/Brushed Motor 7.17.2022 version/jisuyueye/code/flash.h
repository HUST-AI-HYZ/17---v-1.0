#ifndef _FLASH_H
#define _FLASH_H

#include "common.h"
#include "parameter.h"

#include "gps.h"
extern int8 FLASH_WRITE_FLAG;
extern int write_flash_points;
// 底层函数
#define FLASH_PAGE_READ(buf)	flash_read_page(FLASH_SECTION, FLASH_PAGE, buf, FLASH_LENGTH)
#define FLASH_PAGE_WRITE(buf)	flash_write_page(FLASH_SECTION, FLASH_PAGE, buf, FLASH_LENGTH)

extern float _PLACE_HOLDER_; // 占位符，用于
 //��Ҫ�ǵ㣬��ô�����أ��� 
// Flash变量列表
#define FLASH_VAR_NUM	23  // 参数�?�? // 最�? 256 �?变量 // 256 * 4 Byte == 1KB
#define FLASH_VAR_LIST	{\
        {(void *)&gps_kp_z, (uint8 *)NULL, TYPE_FLOAT},\
	{(void *)&gps_kd_z, (uint8 *)NULL, TYPE_FLOAT},\
        {(void *)&gps_kp_w, (uint8 *)NULL, TYPE_FLOAT},\
	{(void *)&gps_kd_w, (uint8 *)NULL, TYPE_FLOAT},\
        {(void *)&gps_kp_in, (uint8 *)NULL, TYPE_FLOAT},\
	{(void *)&gps_kd_in, (uint8 *)NULL, TYPE_FLOAT},\
        {(void *)&pwm_max_angle, (uint8 *)NULL, TYPE_UINT16},\
        {(void *)&pwm_min_angle, (uint8 *)NULL, TYPE_UINT16},\
        {(void *)&pwm_mid_angle, (uint8 *)NULL, TYPE_UINT16},\
        {(void *)&g_speed_set, (uint8 *)NULL, TYPE_FLOAT},\
        {(void *)&finished_gpspoints, (uint8 *)NULL, TYPE_UINT16},\
        {(void *)&distance_each_point_direct, (uint8 *)NULL, TYPE_FLOAT},\
        {(void *)&speed_max_region[0], (uint8 *)NULL, TYPE_FLOAT},\
        {(void *)&speed_max_region[1], (uint8 *)NULL, TYPE_FLOAT},\
        {(void *)&speed_max_region[2], (uint8 *)NULL, TYPE_FLOAT},\
        {(void *)&run_sections, (uint8 *)NULL, TYPE_INT16},\
        {(void *)&distance_each_point_round, (uint8 *)NULL, TYPE_FLOAT},\
        {(void *)&direct_thres, (uint8 *)NULL, TYPE_FLOAT},\
        {(void *)&whether_delay, (uint8 *)NULL, TYPE_INT16},\
         \
}


 // �ո�Ҫ���
//�᲻��������ݼ�ռ��

// Flash地址配置
// 0x0807F000 在IAR�?需要配�?，�?�置烧写时�?�区域保护不�?覆盖
#define FLASH_SECTION	FLASH_SECTION_126
#define FLASH_PAGE		FLASH_PAGE_0
#define FLASH_LENGTH	(FLASH_VAR_NUM)

void FlashReadParams();
void FlashWriteParams();

#endif