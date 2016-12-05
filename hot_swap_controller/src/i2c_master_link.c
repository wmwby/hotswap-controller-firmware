/*
 * i2c_master_ling.c
 *
 * Created: 2016/10/30 21:27:23
 *  Author: mingwei.wu
 */ 
 #include <asf.h>
 #include "i2c_master_link.h"


 struct i2c_master_module i2c_master_instance; 


 /*
 * i2c master init function
 */
void i2c_link_init(void)
{
	struct i2c_master_config config_i2c_master;
	i2c_master_get_config_defaults(&config_i2c_master);
	config_i2c_master.baud_rate = I2C_MASTER_BAUD_RATE_400KHZ;
	config_i2c_master.buffer_timeout = 10000;
	config_i2c_master.pinmux_pad0 = SMBUS_SDA;
	config_i2c_master.pinmux_pad1 = SMBUS_SCL;
	i2c_master_init(&i2c_master_instance,I2C_MODULE,&config_i2c_master);
	i2c_master_enable(&i2c_master_instance);
}

uint8_t i2c_master_write(struct i2c_master_packet packet)
{
	enum status_code err = STATUS_OK;
	volatile uint16_t timeout = 0;
	
	err = i2c_master_write_packet_wait(&i2c_master_instance,&packet);
	while(err != STATUS_OK)
	{
		err = i2c_master_write_packet_wait(&i2c_master_instance,&packet);
		timeout++;
		if(timeout == I2C_TIMEOUT)
		{
			return I2C_MASTER_ERR;
		}
	}
	return I2C_MASTER_OK;
}

uint8_t i2c_master_read(struct i2c_master_packet packet)
{
	enum status_code err = STATUS_OK;
	volatile uint16_t timeout = 0;
	
	err = i2c_master_read_packet_wait(&i2c_master_instance,&packet);
	while(err != STATUS_OK)
	{
		err = i2c_master_read_packet_wait(&i2c_master_instance,&packet);
		timeout++;
		if(timeout == I2C_TIMEOUT)
		{
			return I2C_MASTER_ERR;
		}
	}
	return I2C_MASTER_OK;
}