#ifndef WIFI_C_TEST_H
#define WIFI_C_TEST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "WiFiControl.h"

/*=============================================
	Convert MAC Hex To String
=============================================*/
int convert_MAC_asc_to_str(unsigned char *in_str, char *out_str);

/*=============================================
	Convert IP Hex To String
=============================================*/
int convert_ip_asc_to_str(unsigned char *in_str, char *out_str);

/*=============================================
   单向链表的释放，连头结点一起释放
=============================================*/
void client_info_list_destroy(client_info_node_ptr client_info_head_node);

/*=============================================
   单向链表的释放，保留头结点
=============================================*/
void client_info_list_empty(client_info_node_ptr client_info_head_node);

/*=============================================
   遍历链表，打印name，ip，mac，statues等信息
=============================================*/
void client_info_list_display(client_info_node_ptr client_info_head_node);


#endif

