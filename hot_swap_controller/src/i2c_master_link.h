/*
 * i2c_master_link.h
 *
 * Created: 2016/10/30 21:27:43
 *  Author: mingwei.wu
 */ 


#ifndef I2C_MASTER_LINK_H_
#define I2C_MASTER_LINK_H_

#define I2C_TIMEOUT		100
#define I2C_DELAY		5

#define I2C_PINMUX		2


#define SMBUS_SCL		PIN_PA23
#define SMBUS_SDA		PIN_PA22
#define I2C_MODULE		SERCOM3

#define I2C_MASTER_OK	0
#define I2C_MASTER_ERR	1

void i2c_link_init(void);
uint8_t i2c_master_write(struct i2c_master_packet packet);
uint8_t i2c_master_read(struct i2c_master_packet packet);

#endif /* I2C_MASTER_LINK_H_ */