/*
 * usart_i2c.c
 *
 * Created: 2016/10/25 19:41:05
 *  Author: mingwei.wu
 */ 
#include <asf.h>
#include "usart_i2c.h"
#include "usart_app.h"
#include "i2c_master_link.h"
#include "usart_link.h"
#include "timer.h"


struct i2c_master_packet packet;
uint8_t i2c_buff[DATA_LENGTH] = {0};
uint16_t buff_head = 0;
uint16_t buff_tail = 0;
uint8_t fsm_status = 0;
uint8_t usart_2_iic_ing = 0;


struct usart_i2c_protocol op_info;

/*
 * reset buff
 */
static void buff_reset(void)
{
	buff_head = 0;
	buff_tail = 0;
}
/*
 * return the valid data length of buff
 */
static uint8_t buff_get_valid_length(void)
{
	return (uint8_t)(buff_tail - buff_head);
}	
/*
 * return the empty length of buff
 */
static uint8_t buff_get_empty_length(void)
{
	return (uint8_t)((uint16_t)DATA_LENGTH - buff_tail + buff_head);
}
/*
 * check the buff is full or not
 */
//static uint8_t buff_is_full(void)
//{
	//if(buff_get_empty_length() == 0)
	//{
		//return 1;
	//}
	//return 0;
//}
/*
 * check the buff is empty or not
 */
//static uint8_t buff_is_empty(void)
//{
	//if(buff_get_valid_length() == 0)
	//{
		//return 1;
	//}
	//return 0;
//}

static uint8_t write_buff(uint8_t *buff, uint8_t offset, uint8_t len)
{
	uint8_t i = 0;
	if(buff_get_empty_length() < len)
	{
		return 1;
	}
	for(i = 0;i < len;i++)
	{
		i2c_buff[buff_tail+i] = *(buff+offset+i);
	}
	return 0;
}

static uint8_t read_buff(uint8_t *buff, uint8_t offset, uint8_t len)
{
	uint8_t i = 0;
	//if(buff_get_valid_length() < len)
	//{
		//return 1;
	//}
	for(i = 0;i < len;i++)
	{
		//*(buff+offset+i) = i2c_buff[buff_head+i];
		*(buff+offset+i) = i+10;
	}
	return 0;
}

static inline void config_packet(void)
{
	packet.address = op_info.address;
	packet.data_length = op_info.length;
	packet.data = i2c_buff;
	packet.high_speed = false;
	packet.hs_master_code = 0x0;
	packet.ten_bit_address = false;
}

/*
 *	usart 2 iic operation function, fsm
 *	return:
 *	0: ACK;
 *  1: ERROR, Not feedback;
 *  2: Sending, Not clear data.
 *  3: Send End, Not feedback.
 */
uint8_t usart_2_iic_operation(uint8_t *p_rx_data)
{
	//static uint8_t fsm_status = 0;
	static uint8_t total_len = 0;
	uint8_t tem_len = 0;
	uint8_t i = 0;
	
	timer_usart_i2c_timeout = USART_I2C_TIMEOUT;
	
	switch(fsm_status)
	{
		/* initial state, check it's read or write operation */
		case 0:
			/* re-arrange the rx info */
			op_info.rd_or_wr = *(p_rx_data+1) & 0x1;
			op_info.address = (uint8_t)((*(p_rx_data+1))>>1);
			op_info.length = *(p_rx_data+3);
			
			/* config the i2c packet info */
			config_packet();
			
			/* write operation 
			 * 1. receive the first 3 bytes data to buff;
			 * 2. if total length is more than 3, then jump next status to receive the left data
			 * 3. else, send feedback directly
			 */
			if(op_info.rd_or_wr == I2C_WRITE)
			{
				/* begin to receive the first 3 bytes */
				total_len = op_info.length;
				buff_reset();
				tem_len = min(total_len,3);
				if(write_buff((uint8_t *)p_rx_data,4,3))
				{
					return 1;
				}
				total_len -= tem_len;
				/* check if all data has been received */
				if(!total_len)						
				{
					if(i2c_master_write(packet))	
					{
						return 1;
					}
					return 0;
				}
				else
				{
					fsm_status = 1;
					usart_2_iic_ing = 1;
					return 0;
				}
			}
			/* read operation */
			else if(op_info.rd_or_wr == I2C_READ)
			{
				buff_reset();
				total_len = op_info.length;
				/* read and check */
				//if(i2c_master_read(packet))
				//{
					//return 1;
				//}
				/* send to host that read is successful */
				/* clear the usart rx buff [1]~[5] to 0 */
				for(i = 1;i < 7;i++)
				{
					*(p_rx_data+i) = 0;
				}
				usart_set_tx_frame((uint8_t *)p_rx_data);
				usart_send_data();
				
				/* wait 10ms for the tx end */
				timer_delay_ms(10);
				while(timer_delay_ms(0));
				
				/* then, begin to send valid data */
				buff_reset();
				
				tem_len = min(total_len,5);
				*(p_rx_data+1) = total_len;
				if(read_buff((uint8_t*)p_rx_data,2,tem_len))
				{
					return 1;
				}
				total_len -= tem_len;
				fsm_status = 2;
				usart_2_iic_ing = 1;
				return 2; // sending, no clear
			}
			else
			{
				return 1; // return error
			}
			break;
		/* receive all write data from host */
		case 1:
			/* fill the left bytes data to buff */
			tem_len = min(total_len,6);
			if(write_buff((uint8_t *)p_rx_data,1,tem_len))
			{
				fsm_status = 0;
				usart_2_iic_ing = 0;
				return 1;
			}
			total_len -= tem_len;
			/* check whether all data has been filled into fifo */
			if(!total_len)
			{
				fsm_status = 0;
				usart_2_iic_ing = 0;
				if(i2c_master_write(packet))
				{
					return 1;
				}
				return 0;
			}
			return 0;
		case 2:
			/* if all data have been send, jump to the initial state */
			if(!total_len)
			{
				fsm_status = 0;
				usart_2_iic_ing = 0;
				return 3;// has send all data, no feedback
			}
			tem_len = min(total_len,6);
			/* if read buff error happens, cancel the procedure */
			if(read_buff((uint8_t*)p_rx_data,1,tem_len))
			{
				fsm_status = 0;
				usart_2_iic_ing = 0;
				return 1;
			}
			total_len -= tem_len;
			return 2;
			break;
		default:
			break;
	}
	return 0;
}

void usart_2_iic_timeout(void)
{
	if(!usart_2_iic_ing)
	{
		return;
	}
	if(!timer_usart_i2c_timeout)
	{
		fsm_status = 0;
		usart_2_iic_ing = 0;
	}
}