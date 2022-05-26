#ifndef __SYSTEM_CONTROL_H__
#define __SYSTEM_CONTROL_H__

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef enum
{
    GMT0 =0,
    POSITIVE_GMT_1,            //1.GMT+1
    POSITIVE_GMT_2,            //2.GMT+2
    POSITIVE_GMT_3,            //3.GMT+3
    POSITIVE_GMT_4,            //4.GMT+4
    POSITIVE_GMT_5,            //5.GMT+5
    POSITIVE_GMT_6,            //6.GMT+6
    POSITIVE_GMT_7,            //7.GMT+7
    POSITIVE_GMT_8,            //8.GMT+8
    POSITIVE_GMT_9,            //9.GMT+9
    POSITIVE_GMT_10,           //10.GMT+10
    POSITIVE_GMT_11,           //11.GMT+11
    POSITIVE_GMT_12,           //12.GMT+12
    NEGATIVE_GMT_1,            //13.GMT-1
    NEGATIVE_GMT_2,            //14.GMT-2
    NEGATIVE_GMT_3,            //15.GMT-3
    NEGATIVE_GMT_4,            //16.GMT-4
    NEGATIVE_GMT_5,            //17.GMT-5
    NEGATIVE_GMT_6,            //18.GMT-6
    NEGATIVE_GMT_7,            //19.GMT-7
    NEGATIVE_GMT_8,            //20.GMT-8    BeiJing
    NEGATIVE_GMT_9,            //21.GMT-9
    NEGATIVE_GMT_10,           //22.GMT-10
    NEGATIVE_GMT_11,           //23.GMT-11
    NEGATIVE_GMT_12,           //24.GMT-12
}timezone_type;

/*****************************************************************************
* Function Name : reset_factory
* Description   : 恢复出厂设置
* Input         : None
* Output        : 1:成功   0:失败
* Return        : 1 or 0
* Auther        : rui,zhu
* Date          : 2021.12.30
*****************************************************************************/ 
int reset_factory();
/*****************************************************************************
* Function Name : reboot
* Description   : 重启
* Input         : None
* Output        : 1:成功   0:失败
* Return        : 1 or 0
* Auther        : rui,zhu
* Date          : 2021.12.30
*****************************************************************************/ 
int reboot();

/*****************************************************************************
* Function Name : autosleep_enable
* Description   : 自动休眠开关
* Input         : opt=1 : 开启自动休眠
                  opt=0 : 关闭自动休眠
* Output        : 无
* Return        : true : 执行成功
                  false: 执行失败
* Auther        : caihaichuan
* Date          : 2022.1.4
*****************************************************************************/ 
bool autosleep_enable(int opt);

/*****************************************************************************
* Function Name : creat_soure_lock
* Description   : 创建唤醒锁
* Input         : lock : 无空格制表回车的字符串作为唤醒锁名
                : timeout : 唤醒锁持有时间 单位ns 设置为0则为一直持有
* Output        : 无
* Return        :  true : 执行成功
                  false: 执行失败
* Auther        : caihaichuan
* Date          : 2022.1.4
*****************************************************************************/ 
bool creat_soure_lock(const char *lock,unsigned long timeout);

/*****************************************************************************
* Function Name : remove_soure_lock
* Description   : 移除唤醒锁
* Input         : unlock : 要移除的唤醒锁名(无空格回车制表)
* Output        : 无
* Return        : true : 执行成功
                  false: 执行失败
* Auther        : caihaichuan
* Date          : 2022.1.4
*****************************************************************************/ 
bool remove_soure_lock(const char *unlock);


/*****************************************************************************
* Function Name : view_soure_lock
* Description   : 查看当前唤醒锁
* Input         : 无
* Output        : 打印唤醒锁名
* Return        : true : 执行成功
                  false: 执行失败
* Auther        : caihaichuan
* Date          : 2022.1.4
*****************************************************************************/ 
bool view_soure_lock(void);



typedef struct              //CPU Occupy data from "/proc/stat"
{
    char name[20];
    unsigned long int user;
    unsigned long int nice;
    unsigned long int system;
    unsigned long int idle;
    unsigned long int iowait;
    unsigned long int irq;
    unsigned long int softirq;
}cpu_data_t;

typedef struct          //Memory data from "/proc/meminfo"
{
    unsigned int total;
    unsigned int free;
    unsigned int available;
    unsigned int buffers;
    unsigned int cached;
}mem_info;

/*****************************************************************************
* Function Name : get_cpu_usage
* Description   : Get the CPU usage
* Input         : NULL
* Output        : cpuusage
* Return        : true : 执行成功
                  false: 执行失败
* Auther        : linhuanzhong
* Date          : 2022.3.5
*****************************************************************************/
int get_cpu_usage(int *cpuusage);

/*****************************************************************************
* Function Name : get_memory_usage
* Description   : Get the Memory usage
* Input         : NULL
* Output        : memoryusage 
* Return        : true : 执行成功
                  false: 执行失败
* Auther        : linhuanzhong
* Date          : 2022.3.7
*****************************************************************************/
int get_momory_usage(int *memoryusage);

/*****************************************************************************
* Function Name : set_system_timezone
* Description   : 设置系统时区
                  在本系统中，时区以西为正，故北京时间(东八区)为GMT-8/UTC-8,
                  除零时区外所有东西时区相反

* Input         : timezone:时区,对应timezone_type
* Output        :
* Return        : TRUE : 1
                  FALSE: 0
* Auther        : zhurui
* Date          : 2022.3.22
*****************************************************************************/
int set_system_timezone(int new_timezone);

/*****************************************************************************
* Function Name : power_off
* Description   : 关机
* Input         : None
* Output        : None
* Return        : TRUE : 1
                  FALSE: 0
* Auther        : rui,zhu
* Date          : 2021.4.13
*****************************************************************************/ 
int power_off();
#endif
