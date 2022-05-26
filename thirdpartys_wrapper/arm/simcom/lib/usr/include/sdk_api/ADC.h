#ifndef _ADC_H_
#define _ADC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include "dirent.h"


typedef enum _ADC_CHAN_E
{
    ADC_CHAN_0 = 0,
    ADC_CHAN_1,
} ADC_CHAN_E;

int read_adc(ADC_CHAN_E ch);
int get_adc_voltage(int *voltage);
/*****************************************************************************
* Function Name : get_adc_voltage
* Description   : Gets the voltage of the ADC
* Input			: нч
* Output        : adc value
* Return        : 0:READ_SUCCESS ,-1:READ_FAILD ,-2:OPENFILE_FAILD 
* Auther        : linhuanzhong
* Date          : 2021.11.19
*****************************************************************************/

int get_vbat_voltage(int *voltage);
/*****************************************************************************
* Function Name : get_vbat_voltage
* Description   : Gets the value of the battery voltage
* Input			:нч
* Output        : vbat voltage
* Return        : 0:READ_SUCCESS ,-1:READ_FAILD ,-2:OPENFILE_FAILD
* Auther        : linhuanzhong
* Date          : 2021.11.19
*****************************************************************************/

#endif
