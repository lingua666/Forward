#ifndef __ACCOUNT_INFO_H__
#define __ACCOUNT_INFO_H__

#include <string>
using std::string;

class AccountInfo
{
private:
    AccountInfo();
    ~AccountInfo();
    static AccountInfo *instance_;
public:
    //��ȡʵ��
    static AccountInfo *GetInstance();
	static void ReleaseInstance();

    bool LoadFromFile(const string &account_info_file);
    bool LoadFromCode();

    string app_key();
    string developer_key();
    string cloud_url();
    string auth_path();
    string logfile_path();
    string cap_key();
    string data_path();
    string test_data_path();
protected:
    bool IsValid();
private:
    string app_key_;        //����Ӧ�����
    string developer_key_;  //���ƿ�������Կ
    string cloud_url_;      //�����Ʒ���Ľӿڵ�ַ
    string auth_path_;      //��Ȩ�ļ�����·�����ڹ��캯���и�ֵ�����޸�
    string logfile_path_;   //��־��·�����ڹ��캯���и�ֵ�����޸�
    string cap_key_;        //����capkey
    string data_path_;      //������Դ·��
    string test_data_path_; //������Դ·��
};

#endif //__ACCOUNT_INFO_H__
