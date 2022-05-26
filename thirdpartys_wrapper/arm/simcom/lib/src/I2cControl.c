#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>


#define I2CCONTROL_I2C_BUS "/dev/i2c-0"
#define I2CCONTROL_I2C_MAXLEN 1026

static inline int sim_i2c_open(void)
{
    return open(I2CCONTROL_I2C_BUS, O_RDWR);
}

static inline void sim_i2c_close(int f_desc)
{
    close(f_desc);
}

static int i2c_write(int fd, uint16_t slave, uint16_t reg, uint8_t *data, int len)
{
    struct i2c_msg msg;
    struct i2c_rdwr_ioctl_data packets;

    if((fd <= 0) || !data || !len)
    {
        return -1;
    }

    packets.nmsgs = 1;
    packets.msgs = &msg;
    ioctl(fd, I2C_RETRIES, 2);

    msg.buf = (uint8_t *)malloc(len+2);
    if(NULL == msg.buf)
    {
        return -2;
    }
    memset(msg.buf, 0, len+2);

    msg.flags = I2C_M_RD & 0x00;
    msg.addr = slave;
    if(reg >> 0x08)
    {
        msg.len = len+2;
        msg.buf[0] = reg >> 0x08;
        msg.buf[1] = reg & 0xff;
        memcpy((msg.buf + 2), data, len);
    }
    else
    {
        msg.len = len+1;
        msg.buf[0] = reg & 0xff;
        memcpy((msg.buf + 1), data, len);
    }
    ioctl(fd, I2C_RDWR,(unsigned long)&packets);

    if(NULL != msg.buf)
    {
        free(msg.buf);
        msg.buf = NULL;
    }
    return 0;
}

int sim_i2c_write(uint16_t slave, uint16_t reg, uint8_t *data, int len)
{
    int fd, ret;

    if(!data || !len || (len > (I2CCONTROL_I2C_MAXLEN -2)))
    {
        return -1;
    }

    fd = sim_i2c_open();
    if(fd < 0)
    {
        return -2;
    }

    ret = i2c_write(fd, slave, reg, data, len);
    sim_i2c_close(fd);
    return ret;
}

static int i2c_read(int fd, uint16_t slave, uint16_t reg, uint8_t *buf, int len)
{
    struct i2c_msg msg[2];
    struct i2c_rdwr_ioctl_data packets;

    if((fd <= 0) || !buf || !len)
    {
        return -1;
    }

    packets.nmsgs = 2;
    packets.msgs = msg;
    ioctl(fd, I2C_RETRIES, 2);

    msg[0].buf = (uint8_t *)malloc(2);
    if(NULL == msg[0].buf)
    {
        return -2;
    }

    if(reg >> 0x08)
    {
        msg[0].len = 2;
        msg[0].buf[0] = reg >> 0x08;
        msg[0].buf[1] = reg & 0xff;
    }
    else
    {
        msg[0].len = 1;
        msg[0].buf[0] = reg & 0xff;
    }
    msg[0].flags = I2C_M_RD & 0x00;
    msg[0].addr = slave;

    msg[1].len = len;
    msg[1].flags = I2C_M_RD;
    msg[1].addr = slave;
    msg[1].buf = buf;

    ioctl(fd, I2C_RDWR,(unsigned long)&packets);
    if(NULL != msg[0].buf)
    {
        free(msg[0].buf);
        msg[0].buf = NULL;
    }
    return 0;
}

int sim_i2c_read(uint16_t slave, uint16_t reg, uint8_t *buf, int len)
{
    int fd, ret;

    if(!buf || !len)
    {
        return -1;
    }

    fd = sim_i2c_open();
    if(fd < 0)
    {
        return -2;
    }

    ret = i2c_read(fd, slave, reg, buf, len);
    sim_i2c_close(fd);
    return ret;
}
