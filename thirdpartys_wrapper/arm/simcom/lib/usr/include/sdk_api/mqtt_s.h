#ifndef MQTT_S_H
#define MQTT_S_H
#include <stdio.h>
//MQTT_SUB
int mqtt_sub(char *host, int port);
//MQTT_PUB
int mqtt_pub(char *host, int port);
//MQTTS_SUB
int mqtts_sub(char *host, int port, char *cafile, char *path, char *certfile, char *keyfile);
//MQTTS_PUB
int mqtts_pub(char *host, int port, char *cafile, char *path, char *certfile, char *keyfile);

#endif
