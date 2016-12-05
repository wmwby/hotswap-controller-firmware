/*
 * fw_update.c
 *
 * Created: 2016/9/23 11:04:23
 *  Author: mingwei.wu
 */ 

#include <asf.h>
#include "fw_update.h"
#include "usart_app.h"
#include "main.h"

uint8_t fw_update_en = 0;
uint32_t fw_update_length = 0;
uint8_t fw_rx_buff[USART_FRAME_LENGTH];
uint8_t cfg_buff[NVMCTRL_PAGE_SIZE] = {0};
	
	
uint8_t buff_fifo[NVMCTRL_PAGE_SIZE+USART_FRAME_LENGTH] = {0};
uint8_t p_fifo_head = 0;
uint8_t p_fifo_tail = 0;

//uint8_t temp_fw[100] = {0};

/*
 * return the valid data length of fifo
 */
static uint8_t fifo_get_valid_length(void)
{
	if(p_fifo_tail >= p_fifo_head)
	{
		return (p_fifo_tail - p_fifo_head);
	}
	return(p_fifo_tail + NVMCTRL_PAGE_SIZE+USART_FRAME_LENGTH - p_fifo_head);
}	

/*
 * return the empty length of fifo
 */
static uint8_t fifo_get_empty_length(void)
{
	return (NVMCTRL_PAGE_SIZE+USART_FRAME_LENGTH-fifo_get_valid_length());
}
/* 
 * write data to fifo
 * return 0 = ok, 1 = err
 */
static uint8_t write_fifo(uint8_t *buff, uint8_t offset, uint8_t len)
{
	uint8_t i = 0;
	if(fifo_get_empty_length() < len)
	{
		return 1;
	}
	for(i = 0; i < len; i++)	// write data to fifo
	{
		buff_fifo[p_fifo_tail] = *(buff+offset+i);
		if(p_fifo_tail == (NVMCTRL_PAGE_SIZE+USART_FRAME_LENGTH-1)) // the last data is at then end
		{
			p_fifo_tail = 0;
		}
		else
		{
			p_fifo_tail++;
		}		
	}
	return 0;
}

/* 
 * read data from fifo
 * return 0 = ok, 1 = err
 */
static uint8_t read_fifo(uint8_t *buff, uint8_t offset, uint8_t len)
{
	uint8_t i = 0;
	if(fifo_get_valid_length() < len)
	{
		return 1;
	}
	for(i = 0; i < len; i++)
	{
		*(buff+offset+i) = buff_fifo[p_fifo_head];
		if(p_fifo_head == (NVMCTRL_PAGE_SIZE+USART_FRAME_LENGTH-1)) // the first data is at the end
		{
			p_fifo_head = 0;
		}
		else
		{
			p_fifo_head++;
		}
	}
	return 0;
}


static void program_memory(uint32_t address, uint8_t *buffer, uint16_t len)
{
	/* Check if length is greater than Flash page size */
	if (len > NVMCTRL_PAGE_SIZE) 
	{
		uint32_t offset = 0;

		while (len > NVMCTRL_PAGE_SIZE) 
		{
			/* Check if it is first page of a row */
			if ((address & 0xFF) == 0) 
			{
				/* Erase row */
				nvm_erase_row(address);
			}
			/* Write one page data to flash */
			nvm_write_buffer(address, buffer + offset, NVMCTRL_PAGE_SIZE);
			/* Increment the address to be programmed */
			address += NVMCTRL_PAGE_SIZE;
			/* Increment the offset of the buffer containing data */
			offset += NVMCTRL_PAGE_SIZE;
			/* Decrement the length */
			len -= NVMCTRL_PAGE_SIZE;
		}

		/* Check if there is data remaining to be programmed */
		if (len > 0) 
		{
			/* Write the data to flash */
			nvm_write_buffer(address, buffer + offset, len);
		}
	} 
	else 
	{
		/* Check if it is first page of a row) */
		if ((address & 0xFF) == 0) 
		{
			/* Erase row */
			nvm_erase_row(address);
		}
		/* Write the data to flash */
		nvm_write_buffer(address, buffer, len);
	}
}

void fw_update_init(void)
{
	struct nvm_config config_nvm;
	nvm_get_config_defaults(&config_nvm);
	config_nvm.manual_page_write = false;
	nvm_set_config(&config_nvm);
}

/*
 * fw update function
 */
void fw_update(uint8_t *rx)
{
	uint8_t i = 0;
	uint32_t tmp_len = 0;
	uint8_t tx_buff[NVMCTRL_PAGE_SIZE] = {0};
	static uint8_t status = 0;
	static uint32_t len = 0;
	static uint32_t curr_prog_addr = 0;
	
	/* get the rx data */
	for(i = 0;i < USART_FRAME_LENGTH;i++)
	{
		fw_rx_buff[i] = *(rx+i);
	}
	
	switch (status)
	{
		/* the start, init params then program flash */
		case 0:	
			len = fw_update_length;
			curr_prog_addr = APP_IMAGE_ADDRESS;
			p_fifo_head = 0;
			p_fifo_tail = 0;
			status = 1;
			cfg_buff[0] = 0;
			program_memory(CFG_START_ADDRESS,cfg_buff,NVMCTRL_PAGE_SIZE);
		/* program flash */
		case 1:	
			if(len)
			{
				tmp_len = min(NVMCTRL_PAGE_SIZE,len);
				write_fifo((uint8_t *)fw_rx_buff,1,6);
				if(fifo_get_valid_length() >= tmp_len)
				{
					read_fifo((uint8_t *)tx_buff,0,tmp_len);
					program_memory(curr_prog_addr,tx_buff,tmp_len);
					curr_prog_addr += tmp_len;
					len -= tmp_len;
					if(!len)
					{
						status = 0;		
						fw_update_en = 0;	
					}
				}
			}
			else
			{
				status = 0;		
				fw_update_en = 0;	
			}
			break;
		default:
			status = 0;
			fw_update_en = 0;
			break;
	}	
}


void fw_update_done(uint8_t status)
{
	cfg_buff[0] = status;
	program_memory(CFG_START_ADDRESS,cfg_buff,NVMCTRL_PAGE_SIZE);
}
