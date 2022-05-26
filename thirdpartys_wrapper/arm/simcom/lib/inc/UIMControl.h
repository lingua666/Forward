#ifndef _UIM_CONTROL_H_
#define _UIM_CONTROL_H_


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef unsigned char boolean;
typedef unsigned char uint8;

typedef enum
{
    UIM_CARD_STATE_ENUM_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
    UIM_CARD_STATE_ABSENT_V01 = 0x00, /**<  Absent \n  */
    UIM_CARD_STATE_PRESENT_V01 = 0x01, /**<  Present \n  */
    UIM_CARD_STATE_ERROR_V01 = 0x02, /**<  Error  */
    UIM_CARD_STATE_ENUM_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}uim_card_state_enum_v01;

typedef enum
{
    UIM_APP_TYPE_ENUM_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
    UIM_APP_TYPE_UNKNOWN_V01 = 0x00, /**<  Unknown \n  */
    UIM_APP_TYPE_SIM_V01 = 0x01, /**<  SIM card \n  */
    UIM_APP_TYPE_USIM_V01 = 0x02, /**<  USIM application \n  */
    UIM_APP_TYPE_RUIM_V01 = 0x03, /**<  RUIM card \n  */
    UIM_APP_TYPE_CSIM_V01 = 0x04, /**<  CSIM application \n  */
    UIM_APP_TYPE_ISIM_V01 = 0x05, /**<  ISIM application  */
    UIM_APP_TYPE_ENUM_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}uim_app_type_enum_v01;

typedef enum
{
    UIM_APP_STATE_ENUM_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
    UIM_APP_STATE_UNKNOWN_V01 = 0x00, /**<  Unknown \n  */
    UIM_APP_STATE_DETECTED_V01 = 0x01, /**<  Detected \n  */
    UIM_APP_STATE_PIN1_OR_UPIN_REQ_V01 = 0x02, /**<  PIN1 or UPIN is required \n  */
    UIM_APP_STATE_PUK1_OR_PUK_REQ_V01 = 0x03, /**<  PUK1 or PUK for UPIN is required \n  */
    UIM_APP_STATE_PERSON_CHECK_REQ_V01 = 0x04, /**<  Personalization state must be checked \n  */
    UIM_APP_STATE_READY_V01 = 0x05, /**<  Ready  */
    UIM_APP_STATE_ENUM_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}uim_app_state_enum_v01;

typedef enum
{
    UIM_PIN_STATE_ENUM_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
    UIM_PIN_STATE_UNKNOWN_V01 = 0x00, /**<  Unknown \n  */
    UIM_PIN_STATE_ENABLED_NOT_VERIFIED_V01 = 0x01, /**<  Enabled and not verified \n  */
    UIM_PIN_STATE_ENABLED_VERIFIED_V01 = 0x02, /**<  Enabled and verified \n  */
    UIM_PIN_STATE_DISABLED_V01 = 0x03, /**<  Disabled \n  */
    UIM_PIN_STATE_BLOCKED_V01 = 0x04, /**<  Blocked \n  */
    UIM_PIN_STATE_PERMANENTLY_BLOCKED_V01 = 0x05, /**<  Permanently blocked  */
    UIM_PIN_STATE_ENUM_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}uim_pin_state_enum_v01;

typedef struct
{
    uim_pin_state_enum_v01 pin_state;

    uint8_t pin_retries;
    /**<   Indicates the number of retries remaining to verify this PIN.*/
    uint8_t puk_retries;
    /**<   Indicates the number of retries remaining to unblock this PIN.*/
}uim_pin_info_type_v01;  /* Type */

typedef struct
{
    uim_card_state_enum_v01  card_status;
    uim_app_type_enum_v01    app_type;
    uim_app_state_enum_v01   app_state;
    uim_pin_info_type_v01    pin;
}SimCard_Status_type;


extern int uim_init();
extern void uim_deinit();

/*****************************************************************************
* Function Name : getSimCardStatus
* Description   : 获取SIM卡的状态
* Input         : 无
* Output        : simStatus
			  card_status:     (0: not exist 1: Exist: 2: UNKNOW)
			  app_type:        (1: SIM 2: USIM 3: RUIM 4: CSIM 5: ISIM)
			  app_state        (1: detected 2: pin block  3: puk block 4: person check  5:Permanently blocked 6: illgeal)
			  pin_state        (1: unknown  2: Enabled and not verified 3: Enabled and verified  4: Disabled 5: Blocked  6:Permanently blocked)
              pin_retries      剩余解pin次数
			  puk_retries      剩余解puk次数
* Return        : boolean
* Auther        : zsj
* Date          : 2020.08.28
*****************************************************************************/
extern int getSimCardStatus(SimCard_Status_type *simStatus);
/*****************************************************************************
* Function Name : getIccid
* Description   : 查询sim卡iccid
* Input         : 无
* Output        : pIccid
* Return        : 0： succes     -1： Fail
* Auther        : zsj
* Date          : 2020.08.28
*****************************************************************************/	
extern int get_iccid(char *piccid);

/*****************************************************************************
* Function Name : get_imsi
* Description   : 查询sim卡imsi
* Input         : 无
* Output        : pImsi
* Return        : 0： succes     -1： Fail
* Auther        : zsj
* Date          : 2020.08.28
*****************************************************************************/
extern int get_imsi(char *imsi);

/*****************************************************************************
* Function Name : enable_pin
* Description   : 打开PIN功能
* Input         : char *pin 4-8位
* Output        : int *retries_remaining 剩余次数
* Return        : 0： succes     -1： Fail
* Auther        : 
* Date          : 
*****************************************************************************/
int enable_pin(char *pin, int *retries_remaining);

/*****************************************************************************
* Function Name : disable_pin
* Description   : 关闭PIN功能
* Input         : char *pin 4-8位
* Output        : int *retries_remaining 剩余次数
* Return        : 0： succes     other： Fail
* Auther        : 
* Date          : 
*****************************************************************************/
int disable_pin(char *pin, int *retries_remaining);

/*****************************************************************************
* Function Name : enter_sim_pin
* Description   : 解锁PIN Lock
* Input         : char *pin 4-8位
* Output        : int *retries_remaining 剩余次数
* Return        : 0： succes     other： Fail
* Auther        : 
* Date          : 
*****************************************************************************/
int enter_sim_pin(char *pin, int *retries_remaining);

/*****************************************************************************
* Function Name : enter_sim_puk
* Description   : 解锁PUK Lock
* Input         : char *puk 8位,char *new_pin 4-8位
* Output        : int *retries_remaining 剩余次数
* Return        : 0： succes     other： Fail
* Auther        : 
* Date          : 
*****************************************************************************/
int enter_sim_puk(char *puk, char *new_pin, int *retries_remaining);

/*****************************************************************************
* Function Name : change_pin
* Description   : 修改PIN
* Input         : char *pin 4-8位,char *new_pin 4-8位
* Output        : int *retries_remaining 剩余次数
* Return        : 0： succes     other： Fail
* Auther        : 
* Date          : 
*****************************************************************************/
int change_pin(char *pin, char *new_pin, int *retries_remaining);


#endif
