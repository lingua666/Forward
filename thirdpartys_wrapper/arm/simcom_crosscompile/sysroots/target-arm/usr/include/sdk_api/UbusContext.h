#ifndef __UBUS_CONTEXT_H__
#define __UBUS_CONTEXT_H__

unsigned char SimcomUbusInit(void);
void SimcomUbusDeinit(void);
struct ubus_context *SimcomGetUbusCtx(void);
struct ubus_context *SimcomGetUbusIndCtx(void);
struct ubus_context *SimcomGetUbusIndCtx(void);
void SimcombusIndCtxExit();

#endif
