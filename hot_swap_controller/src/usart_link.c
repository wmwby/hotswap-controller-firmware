/*
 * usart_link.c
 *
 * Created: 2016/8/31 17:12:06
 *  Author: mingwei.wu
 */ 
#include <asf.h>
#include "usart_link.h"
#include "main.h"
#include "usart_app.h"
#include "timer.h"

struct usart_module usart_instance;
uint16_t usart_rx_byte = 0;
uint8_t usart_rx_frame[USART_FRAME_LENGTH] = {USART_PROTOCOL_HEADER,0,0,0,0,0,0,0};
uint8_t usart_tx_frame[USART_FRAME_LENGTH] = {USART_PROTOCOL_HEADER,0,0,0,0,0,0,0};

static void configure_usart(void)
{
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);
	
	config_usart.baudrate		=	9600;
	config_usart.mux_setting	=	USART_RX_3_TX_2_XCK_3;
	config_usart.pinmux_pad0	=	PINMUX_UNUSED;
	config_usart.pinmux_pad1	=	PINMUX_UNUSED;
	config_usart.pinmux_pad2	=	MCU_USB_UART_TX;
	config_usart.pinmux_pad3	=	MCU_USB_UART_RX;
	config_usart.data_order		=	USART_DATAORDER_LSB;
	config_usart.character_size =	USART_CHARACTER_SIZE_8BIT;
	config_usart.transfer_mode	=	USART_TRANSFER_ASYNCHRONOUSLY;
	config_usart.generator_source = GCLK_GENERATOR_0;
	
	while(usart_init(&usart_instance,USART_MODULE,&config_usart) != STATUS_OK)
	{
		;
	}
	
	usart_enable(&usart_instance);
}

static void usart_read_callback(struct usart_module *const usart_module)
{
	static uint8_t rx_data_seq = 0;
	static uint8_t check_sum = 0;
	uint8_t rx_data = 0;
	
	if(usart_status != Init)
	{
		return;
	}
	
	rx_data = (uint8_t)usart_rx_byte;
	switch(rx_data_seq)
	{
		case 0:
			if(rx_data == USART_PROTOCOL_HEADER)
			{
				check_sum = rx_data;
				usart_rx_frame[0] = USART_PROTOCOL_HEADER;
				rx_data_seq = 1;
			}
			else if(rx_data == USART_PROTOCOL_HEADER_FW_DWLD || rx_data == USART_PROTOCOL_HEADER_USART_I2C)
			{
				check_sum = rx_data;
				usart_rx_frame[0] = rx_data;
				rx_data_seq = 11;
			}
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			if(rx_data == USART_PROTOCOL_HEADER)
			{
				check_sum = rx_data;
				usart_rx_frame[0] = USART_PROTOCOL_HEADER;
				rx_data_seq = 1;
			}
			else
			{
				check_sum += rx_data;
				usart_rx_frame[rx_data_seq] = rx_data;
				rx_data_seq++;
			}
			break;
		case 7:
			if(check_sum == rx_data)
			{
				usart_status = Analysis;
				led_blink_en = 1;
			}
			rx_data = 0;
			check_sum = 0;
			rx_data_seq = 0;
			break;
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
			check_sum += rx_data;
			usart_rx_frame[rx_data_seq-10] = rx_data;
			rx_data_seq++;
			break;
		case 17:
			if(check_sum == rx_data)
			{
				usart_status = Analysis;
				led_blink_en = 1;
			}
			rx_data = 0;
			check_sum = 0;
			rx_data_seq = 0;
			break;
		default:
			rx_data = 0;
			check_sum = 0;
			rx_data_seq = 0;
			break;
	}
}

static void configure_usart_callback(void)
{
	//usart_register_callback(&USART_INSTANCE,usart_write_callback,USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_register_callback(&usart_instance,usart_read_callback,USART_CALLBACK_BUFFER_RECEIVED);
	
	//usart_enable_callback(&USART_INSTANCE,USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&usart_instance,USART_CALLBACK_BUFFER_RECEIVED);
}

static void configure_pinmux_usart(void)
{
	struct system_pinmux_config configure_pinmux;
	system_pinmux_get_config_defaults(&configure_pinmux);
	
	configure_pinmux.mux_position = PINMUX_USART;
	
	system_pinmux_pin_set_config(MCU_USB_UART_TX,&configure_pinmux);
	system_pinmux_pin_set_config(MCU_USB_UART_RX,&configure_pinmux);
}

void usart_link_init(void)
{
	configure_usart();
	configure_usart_callback();
	configure_pinmux_usart();
}

void usart_read_data(void)
{
	usart_read_job(&usart_instance,(uint16_t *)(&usart_rx_byte));
}

void usart_get_rx_frame(uint8_t *p_dst)
{
	uint8_t i = 0;
	for(i = 0;i < USART_FRAME_LENGTH;i++)
	{
		*(p_dst+i) = usart_rx_frame[i];
	}
}
void usart_set_tx_frame(const uint8_t *p_src)
{
	uint8_t i = 0;
	uint8_t check_sum = 0;
	
	for(i = 0;i < (USART_FRAME_LENGTH-1);i++)
	{
		usart_tx_frame[i] = *(p_src+i);
		check_sum += usart_tx_frame[i];
	}
	usart_tx_frame[USART_FRAME_LENGTH-1] = check_sum;
}

void usart_send_data(void)
{
	usart_write_buffer_job(&usart_instance,(uint8_t *)usart_tx_frame,USART_FRAME_LENGTH);
}
