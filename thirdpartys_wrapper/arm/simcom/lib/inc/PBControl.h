#ifndef _PB_CONTROL_H_
#define _PB_CONTROL_H_

#include "ril.h"
#include <libubox/blobmsg_json.h>
#include <libubox/ustream.h>
#include <libubus.h>
#include "simcom_sdk_common.h"

typedef enum
{
    SUCCESS,
    PB_NOT_INIT,
    RIL_NOT_INIT,
    NOT_FIND_CONTACT,
    MALLOC_FAIL,
    ERROR_INPUT,
    STORAGE_FULL,
    STORAGE_EMPTY,
    SET_SM_FAILD,
    SET_CSCS_FAILD,
    UNKNOWN_ERROR,
}RETURN_INFO;

typedef enum
{
    PB_DEINIT,
    PB_INIT,
}PB_STATUS;

typedef enum {
    ADD_CONTACT,        /* AT+CPBW = ,"NUMBER",TYPE,"TEXT" */
    DELETE_CONTACT,     /* AT+CPBW = INDEX */
    EDIT_CONTACT,       /* AT+CPBW = INDEX,"NUMBER",TYPE,"TEXT" */
    GET_CONTACT,        /* AT+CPBR = INDEX */
    GET_PB_STATUS,      /* AT+MPBK */
    GET_STORAGE_STATUS, /* AT+CPBS*/
    SET_STORAGE,        /* AT+CPBS = */
    SET_FORMAT,         /* AT+CSCS = */
} PB_CMD;

typedef struct contact_info
{
    int index;          /* 联系人序列号 */
    char *number;       /* 号码 */
    int type;           /* 号码类型, 默认129 */
    char *name;         /* 姓名 */
}contact_info;

typedef struct contact_node
{
    struct contact_node *next;
    struct contact_node *prev;
    contact_info *info;
}contact_node;

typedef struct
{
    contact_node *head;
    contact_node *tail;
    int count;
}contact_list;

typedef struct storage_info
{
    int storage;
    int total;
    int used;
    int first_index;
}storage_info;

/*****************************************************************************
* Function Name : pb_init
* Description   : 初始化phonebook
* Input         : None
* Output        : None
* Return        : 0: success
                  >0: faild
* Note          : 选择首选存储器,选择编码格式,初始化SIM卡联系人链表
* Date          : 2021.12.29
*****************************************************************************/
int pb_init();

/*****************************************************************************
* Function Name : pb_deinit
* Description   : 关闭phonebook
* Input         : None
* Output        : None
* Return        : 0: success
                  >0: faild
* Note          : 释放在初始化时申请的链表空间
* Date          : 2021.12.29
*****************************************************************************/
int pb_deinit();

/*****************************************************************************
* Function Name : add_new_contact
* Description   : 添加联系人信息
* Input         : info: "contact_info"类型
* Output        : index: 联系人序列号
* Return        : 0: success
                  >0: faild
* Note          : None
* Date          : 2021.12.29
*****************************************************************************/
int add_new_contact(contact_info info, int *index);

/*****************************************************************************
* Function Name : delete_contact
* Description   : 删除联系人信息
* Input         : index: 联系人索引号
* Output        : None
* Return        : 0: success
                  >0: faild
* Note          : None
* Date          : 2021.12.29
*****************************************************************************/
int delete_contact(int index);

/*****************************************************************************
* Function Name : set_contact_info
* Description   : 修改联系人信息
* Input         : info: "contact_info"类型
* Output        : None
* Return        : 0: success
                  >0: faild
* Note          : 通过info.index确定修改的联系人
* Date          : 2021.12.29
*****************************************************************************/
int set_contact_info(contact_info info);

/*****************************************************************************
* Function Name : get_contact_info
* Description   : 获取联系人信息
* Input         : index: 联系人索引号
* Output        : info： "contact_info"类型
* Return        : 0: success
                  >0: faild
* Note          : info需要手动释放空间，可通过free_contact_info释放
* Date          : 2021.12.29
*****************************************************************************/
int get_contact_info(int index, contact_info *info);

/*****************************************************************************
* Function Name : get_contact_list
* Description   : 获取联系人列表
* Input         : None
* Output        : list: "contact_node"类型的链表
* Return        : 0: success
                  >0: faild
* Note          : list为包含所有联系人信息的链表，需要通过free_contact_list释放
* Date          : 2021.12.29
*****************************************************************************/
int get_contact_list(contact_node **list);

/*****************************************************************************
* Function Name : get_storage_status
* Description   : 获取存储情况
* Input         : None
* Output        : used:  已使用空间
                  total: 总空间
* Return        : 0: success
                  >0: faild
* Note          : None
* Date          : 2021.12.29
*****************************************************************************/
int get_storage_status(int *used, int *total);

/*****************************************************************************
* Function Name : free_contact_info
* Description   : 释放contact_info空间
* Input         : info: "contact_info"类型
* Output        : None
* Return        : 0: success
                  >0: faild
* Note          : None
* Date          : 2021.12.29
*****************************************************************************/
int free_contact_info(contact_info *info);

/*****************************************************************************
* Function Name : free_contact_node
* Description   : 释放contact_node空间
* Input         : node: "contact_node"类型
* Output        : None
* Return        : None
* Note          : None
* Date          : 2021.12.29
*****************************************************************************/
void free_contact_node(contact_node *node);

/*****************************************************************************
* Function Name : free_contact_list
* Description   : 释放contac_node链表空间
* Input         : list: "contact_node"类型的链表
* Output        : None
* Return        : 0: success
                  >0: faild
* Note          : None
* Date          : 2021.12.29
*****************************************************************************/
int free_contact_list(contact_node **list);

#endif
