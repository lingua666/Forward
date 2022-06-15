1.客户端的设备发现代码。

2.能发现支持onvif的设备，并获取设备信息返回。

3.云台接口测试完毕，可以用使用并有使用说明。

4.能接收指定设备的裸流h264。但是媒体地址的uri前面要加上用户名和密码。

5.测试完天地伟业和利凌的设备：

天地伟业：协议的地址中要在ip地址后面加上：8080这个端口号，才能正常工作。云台也要加。如果不加就会出现soap error 501， SOAP-ENV:server HTTP error 错误码；

利凌：协议的地址中要在ip地址后面加上：8899这个端口号，才能正常工作。云台也要加。如果不加就会出现soap error 501， SOAP-ENV:server HTTP error 错误码；

海康威视：使用的是 onvif默认的端口信息，所以不用加。

6.在使用RTSP 的uri地址进行获取视频流的时候，要在uri地址里面加上用户名和密码，
ps："rtsp://admin:abcd-1234@192.168.8.23:554/Streaming/Channels/101?transportmode=unicast&profile=Profile_1";

7.ONVIF_Interface.cpp 在工程中的onvif接口是使用在c的基础上封装成c++的类形式。