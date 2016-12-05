/*
 * usart_i2c.h
 *
 * Created: 2016/10/25 19:41:18
 *  Author: mingwei.wu
 */ 


#ifndef USART_I2C_H_
#define USART_I2C_H_

#define DATA_LENGTH			256
#define USART_I2C_TIMEOUT	200

#define I2C_READ		1
#define I2C_WRITE		0

struct usart_i2c_protocol
{
	uint8_t rd_or_wr;
	uint8_t address;
	uint8_t speed;
	uint8_t length;
};

uint8_t usart_2_iic_operation(uint8_t *p_rx_data);
void usart_2_iic_timeout(void);

#endif /* USART_I2C_H_ */