/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             zf_driver_fsmc
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ3184284598)
* @version          查看doc内version文件 版本说明
* @Software         IAR 8.32.4 or MDK 5.28
* @Target core      MM32F327X_G9P
* @Taobao           https://seekfree.taobao.com/
* @date             2022-04-11
********************************************************************************************************************/

#include "zf_driver_fsmc.h"
#include "zf_driver_gpio.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief       IPS200 屏幕的 FSMC 外设初始化
// @param       void
// @return      void
// Sample usage:                fsmc_init_ips200();
//-------------------------------------------------------------------------------------------------------------------
void fsmc_init_ips200 (void)
{
    if(0 == (RCC->AHB3ENR & (0x00000001)))
    {
        RCC->AHB3ENR |= (0x00000001);                                               // 使能 FSMC 时钟
        RCC->AHB3RSTR |= (0x00000001);                                              // 复位 FSMC 外设
        RCC->AHB3RSTR &= ~(0x00000001);                                             // 复位 结束
    }
    FSMC->SMTMGRSET[0] = 0x000000C0;                                                // 120MHz [15:10]0 [9:8]0 [7:6]3 [5:0]1
    FSMC->SMCTLR = 0x00000001;
    FSMC->SMSKR0 = 0x0000002b;
}
