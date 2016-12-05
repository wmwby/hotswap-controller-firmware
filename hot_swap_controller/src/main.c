
#include <asf.h>
#include "main.h"
#include "gpio.h"
#include "timer.h"
#include "usart_link.h"
#include "usart_app.h"
#include "fw_update.h"
#include "i2c_master_link.h"

volatile enum USART_FSM_STATUS usart_status = Init;
enum FORM_FACTOR module_form_factor = p2p;


int main (void)
{
	system_init();
	gpio_init();
	timer_init();
	usart_link_init();
	i2c_link_init();
	fw_update_init();
	
	while(1)
	{
		switch(usart_status)
		{
			case Init:
				usart_waiting();
				break;
			case Analysis:
				usart_status = usart_protocol_analysis();
				break;
			case Feedback:
				usart_status = usart_feedback();
				break;
			case Reset:
				usart_status = usart_sys_reset();
				break;
			case I2C:
				usart_status = usart_2_i2c_mode();
				break;
			default:
				break;
		}
	}
}