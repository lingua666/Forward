#ifndef _FTP_CONTROL_H_
#define _FTP_CONTROL_H_

#define MAX_RESP_CODE_NUM 10
#define MAX_RESP_STR_LEN 256

typedef struct
{
  int resp_code[MAX_RESP_CODE_NUM];
  char resp_str[MAX_RESP_STR_LEN];
} ftp_ctrl_resp_type;

typedef struct
{
  int init;
  char host[128];
  int port;
  char user[64];
  char pwd[64];
  int ctrl_sock;
  int data_sock;
  int isConnected;
  int bTransOK;
  ftp_ctrl_resp_type ctrl_resp;
} simcom_ftp_info;

typedef void (*ftp_dl_recv_cb_fcn)(void *data, int data_len, int index, long long total_size);

/*****************************************************************************
* Function Name : ftp_login_init
* Description   : 登录FTP服务器
* Input         : host: 服务器IP
                : port: 端口, 一般是21
                : user: 用户名
                : pwd:  密码
* Output        : host: 输入的服务器IP
                : port: 21
                : user: 输入的用户名
                : pwd:  输入的密码
                : ftp_list:当前文件列表
* Return        : 0:  成功       -1：失败
* Auther        : lhz
* Date          : 2022.02.12
*****************************************************************************/
int ftp_login(char *host, int port, char *user, char *pwd);

/*****************************************************************************
* Function Name : ftp_logout
* Description   : 登出FTP
* Input         : None
* Output        : None
* Return        : 0： 成功      -1： 失败    
* Auther        : qjh
* Date          : 2018.07.11
*****************************************************************************/
int ftp_logout();

/*****************************************************************************
* Function Name : ftp_pwd
* Description   : 查询当前工作目录并列出目录下的文件
* Input         : None
* Output        : path  当前工作目录以及目录下的文件
* Return        : 0： 成功      -1： 失败
* Auther        : linhuanzhong
* Date          : 2022.02.12
*****************************************************************************/
int ftp_pwd(char *path);

/*****************************************************************************
* Function Name : ftp_size
* Description   : 查看文件大小
* Input         : None
* Output        : 
* Return        : 0： 成功      -1： 失败
* Auther        : qjh
* Date          : 2018.07.11
*****************************************************************************/
int ftp_size(char *path, long long *size);

/*****************************************************************************
* Function Name : ftp_cwd
* Description   : 改变工作目录 （相当于控制台命令 cd）
* Input         : 路径:
                :    .  当前目录
                ：   ..  上级目录
                ：  test 进入当前目录的下一级test目录
                ：/test 进入绝对路径/test
* Output        : None
* Return        : 0： 成功      -1： 失败
* Auther        : qjh
* Date          : 2018.07.11
*****************************************************************************/
int ftp_cwd(char *path);

/*****************************************************************************
* Function Name : ftp_mkdir
* Description   : 创建工作目录 
* Input         : 目录名称
* Output        : path  当前工作目录
* Return        : 0： 成功      -1： 失败
* Auther        : qjh
* Date          : 2018.07.11
*****************************************************************************/
int ftp_mkdir(char *path);

/*****************************************************************************
* Function Name : ftp_mv
* Description   : 移动或重命名文件，文件夹
* Input         : 文件、目录名称
* Output        : None
* Return        : 0： 成功      -1： 失败
* Auther        : qjh
* Date          : 2018.07.11
*****************************************************************************/
int ftp_mv(char *s_path, char *d_path);

/*****************************************************************************
* Function Name : ftp_deltefile
* Description   : 删除文件
* Input         : 文件名称
* Output        : None
* Return        : 0： 成功      -1： 失败
* Auther        : qjh
* Date          : 2018.07.11
*****************************************************************************/
int ftp_deltefile(char *path);

/*****************************************************************************
* Function Name : ftp_deletefolder
* Description   : 删除文件夹
* Input         : 文件夹名称
* Output        : None
* Return        : 0： 成功      -1： 失败
* Auther        : qjh
* Date          : 2018.07.11
*****************************************************************************/
int ftp_deletefolder(char *path);


/*****************************************************************************
* Function Name : ftp_download
* Description   : 下载文件
* Input         : None
* Output        : path  当前工作目录
* Return        : 0： 成功      -1： 失败
* Auther        : qjh
* Date          : 2018.07.11
*****************************************************************************/
int ftp_download(char *path, ftp_dl_recv_cb_fcn handle);

/*****************************************************************************
* Function Name : ftp_upload
* Description   : 上传文件
* Input         : local_path：  本地待上传文件名
                ：ftp_path:     存储在ftp的路径
* Output        : path  当前工作目录
* Return        : 0： 成功      -1： 失败
* Auther        : qjh
* Date          : 2018.07.11
*****************************************************************************/
int ftp_upload(char *local_file, char *ftp_path);

int ftp_list(char *path, void *data, int max_data_len, int *data_len);
int get_init_status();
int ftp_connect();
int get_connect_status();
int ftp_init();

#endif
