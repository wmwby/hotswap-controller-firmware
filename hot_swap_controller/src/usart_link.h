/*
 * usart_link.h
 *
 * Created: 2016/8/31 17:11:53
 *  Author: mingwei.wu
 */ 


#ifndef USART_LINK_H_
#define USART_LINK_H_

#define PINMUX_USART	3
extern struct usart_module usart_instance;
extern uint16_t usart_rx_byte;

void usart_link_init(void);
void usart_read_data(void);
void usart_send_data(void);
void usart_get_rx_frame(uint8_t *p_dst);
void usart_set_tx_frame(const uint8_t *p_src);

#endif /* USART_LINK_H_ */