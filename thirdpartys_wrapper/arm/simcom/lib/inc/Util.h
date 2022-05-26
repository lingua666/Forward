#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initGpio(int n);
void deInitGpio(int n);
void setGpioDirection(int n, char *direction);
void setGpioEdge(int n, char *edge);
int setGpioValue(int n, int val);
int getGpioValue(int n);
int file_w_string(const char *path, char *str);
int led_setblink(const char *led, int mode, int light_t);


int endswith(const char *str, const char *p);

void activeAMP();
void disableAMP();

void activeAUX();
void disableAUX();

void activeGNNS();

void disableGNNS();

void activeWIFI();

void disableWIFI();
#endif