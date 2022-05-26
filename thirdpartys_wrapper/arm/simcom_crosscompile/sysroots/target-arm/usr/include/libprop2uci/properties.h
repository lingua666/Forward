 /*------------------------------------------------------------
  * (C) Copyright [2006-2008] Marvell International Ltd.
  * All Rights Reserved
  * ------------------------------------------------------------*/

#ifndef __PROPERTIES_H
#define __PROPERTIES_H

#ifdef __cplusplus
extern "C" {
#endif

#define PROPERTY_KEY_MAX   32
#define PROPERTY_VALUE_MAX  92

/* property_get: returns the length of the value which will never be
** greater than PROPERTY_VALUE_MAX - 1 and will always be zero terminated.
** (the length does not include the terminating zero).
**
** If the property read fails or returns an empty value, the default
** value is used (if nonnull).
*/
int property_get(const char *key, char *value, const char *default_value);

/* property_set: returns 0 on success, < 0 on failure
*/
int property_set(const char *key, const char *value);

/* property_del: returns 0 on success, < 0 on failure
*/
int property_del(const char *name);
    
int property_list(void (*propfn)(const char *key, const char *value, void *cookie), void *cookie);


#ifdef __cplusplus
}
#endif

#endif /*#PROPERTIES_H*/
