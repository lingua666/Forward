#ifndef _GPIO_CONTROL_H_
#define _GPIO_CONTROL_H_

typedef enum
{
    GPIO_IRQ_NONE = 0,
    GPIO_IRQ_RISING,
    GPIO_IRQ_FALLING,
    GPIO_IRQ_BOTH,
} gpio_edge_type;

/*****************************************************************************
* Function Name : gipo_wakeup_init
* Description   : gpio唤醒脚的配置
* Input         : None
* Output        : None
* Return        : int 
* Auther        : simcom
* Date          : 2018.04.16
*****************************************************************************/
void* gipo_wakeup_init(void* cmdpipefd);


/*****************************************************************************
* Function Name : modem_ri_notify_mcu
* Description   : 用于4g唤醒mcu
* Input         : None
* Output        : None
* Return        : int 
* Auther        : simcom
* Date          : 2018.04.16
*****************************************************************************/
int modem_ri_notify_mcu(void);

/*****************************************************************************
* Function Name : lowPowerMode
* Description   : go to low power mode,
                  isSleep ->0,  go to sleep
                  isSleep ->1,  wake up system
* Input         : None
* Output        : None
* Return        : 0: success 
* Auther        : simcom
* Date          : 2018.04.16
*****************************************************************************/
int lowPowerMode(int isSleep);

/*****************************************************************************
* Function Name : gpio_dtr_deinit
* Description   : Deinit DTR port, include remove it from gpio file.
* Input         : None
* Output        : None
* Return        : None 
* Auther        : simcom
* Date          : 2020.08.21
*****************************************************************************/
void gpio_dtr_deinit(void);

/*****************************************************************************
* Function Name : gpio_get_dir
* Description   : get gpio direction.
* Input         : None
* Output        : None
* Return        : -1 open or read file faild
                   0 in
                   1 out
* Auther        : simcom
* Date          : 2022.04.14
*****************************************************************************/
int gpio_get_dir(unsigned int gpio);

/*****************************************************************************
* Function Name : gpio_get_value
* Description   : get gpio value.
* Input         : None
* Output        : None
* Return        : -1 open or read file faild
                   0 low level
                   1 high level
* Auther        : simcom
* Date          : 2022.04.14
*****************************************************************************/
int gpio_get_value(unsigned int gpio);

/*****************************************************************************
* Function Name : gpio_set_isr
* Description   : set gpio isr.
* Input         : gpio number,edge mode
* Output        : None
* Return        : -1:faild
                :  0:success
* Auther        : simcom
* Date          : 2022.04.14
*****************************************************************************/
int gpio_set_isr(unsigned int gpio,gpio_edge_type edge);

#endif

