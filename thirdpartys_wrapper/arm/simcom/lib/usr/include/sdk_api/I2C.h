#ifndef _I2C_H_
#define _I2C_H_

#include <stdint.h>

int sim_i2c_write(uint16_t slave, uint16_t reg, uint8_t *data, int len);
int sim_i2c_read(uint16_t slave, uint16_t reg, uint8_t *buf, int len);

#endif
