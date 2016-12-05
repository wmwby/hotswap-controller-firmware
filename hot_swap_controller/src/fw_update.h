/*
 * fw_update.h
 *
 * Created: 2016/9/23 11:04:35
 *  Author: mingwei.wu
 */ 


#ifndef FW_UPDATE_H_
#define FW_UPDATE_H_


#define APP_START_ADDRESS			0x00004000	
#define APP_IMAGE_ADDRESS			0x0001D000	//100KB Firmware area
#define CFG_START_ADDRESS			0x00036000	//100KB Firmware ares
#define FW_SIZE						0x00019000	
#define FW_RX_BUFF_HEAD				1
#define FW_RX_BUFF_TAIL				(USART_FRAME_LENGTH-2)
#define FW_TX_BUFF_HEAD				0
#define FW_TX_BUFF_TAIL				(NVMCTRL_PAGE_SIZE-1)

extern uint8_t fw_update_en;
extern uint32_t fw_update_length;
extern uint8_t cfg_buff[NVMCTRL_PAGE_SIZE];

void fw_update_init(void);
void fw_update(uint8_t *rx);
void fw_update_done(uint8_t status);


#endif /* FW_UPDATE_H_ */