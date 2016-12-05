/*
 * usart_app.c
 *
 * Created: 2016/8/31 17:12:20
 *  Author: mingwei.wu
 */ 

#include <asf.h>
#include "usart_app.h"
#include "usart_link.h"
#include "main.h"
#include "timer.h"
#include "fw_update.h"
#include "usart_i2c.h"

uint8_t hs_auto_mode_setting = 0;	//auto mode 0,1,...
uint8_t hs_auto_mode_delay = 5;		//delay time between signals, 5ms default
uint8_t usart_rx_data[USART_FRAME_LENGTH] = {0};	//receiving data from usart
uint8_t usart_cmd_error = USART_PROTOCOL_FB_OK;

/*
 * Brief: control pci express power up or down
 * 
 * param sw: open or close
 * sw = 1: open (connected);
 * sw = 0: close (unconnected)
 */
static void hs_power_sw(uint8_t sw)
{
	bool gpio_level = 0;
	gpio_level = (sw > 0)?true:false;
	
	port_pin_set_output_level(PE_12V_SW,gpio_level);
	port_pin_set_output_level(PE_3V3_SW,gpio_level);
	port_pin_set_output_level(PE_AUX3V3_SW,gpio_level);
}

/*
 * Brief: control pci express signal to open or close
 * 
 * param sw: open or close
 * sw = 1: open (connected);
 * sw = 0: close (unconnected)
 */
static void hs_signal_sw(uint8_t sw)
{
	bool gpio_level = 0;
	gpio_level = (sw > 0)?true:false;
	
	/*
	 * different form factors need to control different signals, except for the common ones.
	 */
	if(module_form_factor == p2p)//p2p module
	{
		port_pin_set_output_level(PE_LANE_TWO_SW,gpio_level);
	}
	else if(module_form_factor == u2u)//u2u module
	{
		port_pin_set_output_level(PE_CLK1_SW,gpio_level);
		port_pin_set_output_level(PE_RST1_SW,gpio_level);
		port_pin_set_output_level(PE_IF_DET_SW,gpio_level);
		port_pin_set_output_level(PE_12V_PRE_SW,gpio_level);
		port_pin_set_output_level(PE_DP_SW,gpio_level);
		port_pin_set_output_level(PE_LANE_TWO_SW,gpio_level);
	}
	//p2u module, also the common ones
	port_pin_set_output_level(PE_SMBS_SW,gpio_level);
	port_pin_set_output_level(PE_CLK0_SW,gpio_level);
	port_pin_set_output_level(PE_RST0_SW,gpio_level);
	port_pin_set_output_level(PE_LANE_ONE_SW,gpio_level);
	port_pin_set_output_level(PE_PRSNT_SW,gpio_level);
}

/*
 * Brief: control pci express all, including power and signals
 * 
 * param sw: open or close
 * sw = 1: open (connected);
 * sw = 0: close (unconnected)
 */
static void hs_all_sw(uint8_t sw)
{
	hs_power_sw(sw);
	hs_signal_sw(sw);
}
/*
 * Brief: control relay
 * 
 * param[0] rly: the relay index
 * 1 : MCU_RELAY1
 * 2 : MCU_RELAY2
 * 3 : MCU_RELAY3
 * 4 : relay 1 on module
 * 5 : relay 2 on module
 * param[1] sw: open or close. 1 = connect, and 0 = disconnect
 */
static void relay_switch(uint8_t rly,uint8_t sw)
{
	bool gpio_level = 0;
	gpio_level = (sw > 0)?false:true;
	
	switch(rly)
	{
		case 1:
			port_pin_set_output_level(MCU_RELAY_1,gpio_level);
			break;
		case 2:
			port_pin_set_output_level(MCU_RELAY_2,gpio_level);
			break;
		case 3:
			port_pin_set_output_level(MCU_RELAY_3,gpio_level);
			break;
		case 4:
			port_pin_set_output_level(RLY0_SW,gpio_level);
			break;
		case 5:
			port_pin_set_output_level(RLY1_SW,gpio_level);
			break;
		default:
			break;
	}
}
/*
 * Brief: auto mode. power up of mode 0: power on -> delay -> signal on
 */
static void hs_auto_mode_0_power_up(void)
{
	hs_power_sw(1);
	timer_delay_ms(hs_auto_mode_delay);
	while(timer_delay_ms(0))
	{
		;
	}
	hs_signal_sw(1);
}

/*
 * Brief: auto mode. power down of mode 0: signal off -> delay -> power off
 */
static void hs_auto_mode_0_power_dw(void)
{
	hs_signal_sw(0);
	timer_delay_ms(hs_auto_mode_delay);
	while(timer_delay_ms(0))
	{
		;
	}
	hs_power_sw(0);
}

static void hs_pwr_sig_sel(uint8_t pwr_or_sig, uint8_t sel)
{
	bool gpio_level = (sel > 0)?true:false;
	if(pwr_or_sig == 1) // power
	{
		port_pin_set_output_level(PE_PWR_SLT,(!gpio_level));
	}
	else if(pwr_or_sig == 2) //smbus
	{
		port_pin_set_output_level(PE_SMBS_SLT,!gpio_level);
	}
}

/*
 * Brief: control relay perform mode 1
 *
 * param rly:
 * 1 : MCU_RELAY1
 * 2 : MCU_RELAY2
 * 3 : MCU_RELAY3
 * 4 : relay 1 on module
 * 5 : relay 2 on module
 */
static void rly_auto_mode_0(uint8_t rly)
{
	if(rly > 5 || rly == 0)
	{
		return;
	}
	
	relay_switch(rly,1); // connect the relay rly
	timer_delay_ms(500); // delay 0.5 sec
	while(timer_delay_ms(0))
	{
		;
	}
	relay_switch(rly,0); // disconnect the relay rly
}

/*
 * Brief: control relay perform mode 2
 *
 * param rly:
 * 1 : MCU_RELAY1
 * 2 : MCU_RELAY2
 * 3 : MCU_RELAY3
 * 4 : relay 1 on module
 * 5 : relay 2 on module
 */
static void rly_auto_mode_1(uint8_t rly)
{
	if(rly > 5 || rly == 0)
	{
		return;
	}
	
	relay_switch(rly,1); // connect the relay rly
	timer_delay_ms(10000); // delay 10 sec
	while(timer_delay_ms(0))
	{
		;
	}
	relay_switch(rly,0); // disconnect the relay rly
	timer_delay_ms(30000); // delay 30 sec
	while(timer_delay_ms(0))
	{
		;
	}
	relay_switch(rly,1); // connect the relay rly
	timer_delay_ms(500); // delay 0.5 sec
	while(timer_delay_ms(0))
	{
		;
	}
	relay_switch(rly,0); // disconnect the relay rly
}

static void power_load_test(uint8_t load,uint8_t sw)
{
	bool gpio_level = false;
	gpio_level = (sw > 0)?true:false;
	
	switch(load)
	{
		case USART_PROTOCOL_PLT_SET_00:
			port_pin_set_output_level(PWR_LD0,gpio_level);
			break;
		case USART_PROTOCOL_PLT_SET_01:
			port_pin_set_output_level(PWR_LD1,gpio_level);
			break;
		case USART_PROTOCOL_PLT_SET_04:
			port_pin_set_output_level(PWR_LD0,gpio_level);
			port_pin_set_output_level(PWR_LD1,gpio_level);
			break;
		default:
			break;
	}
}


/*
 * Brief: usart protocol analysis function
 */
enum USART_FSM_STATUS usart_protocol_analysis(void)
{
	// get the rx frame data
	usart_get_rx_frame((uint8_t *)usart_rx_data);
	
	// firmware downloading
	if(usart_rx_data[0] == USART_PROTOCOL_HEADER_FW_DWLD)
	{
		if(!fw_update_en)
		{
			usart_cmd_error = USART_PROTOCOL_FB_ERR_FW_DWLD;
			return Feedback;
		}
		fw_update((uint8_t *)usart_rx_data);
		usart_rx_data[1] = 0;
		usart_rx_data[2] = 0;
		usart_rx_data[3] = 0;
		usart_rx_data[4] = 0;
		usart_rx_data[5] = 0;
		return Feedback;
	}
	// usart 2 i2c communication
	if(usart_rx_data[0] == USART_PROTOCOL_HEADER_USART_I2C)
	{
		return I2C;
	}
	
	// analysis
	// test mode
	if(usart_rx_data[1] == USART_PROTOCOL_TEST)
	{
		switch(usart_rx_data[2])	
		{
			case USART_PROTOCOL_TEST_LINK:			// link test
				break;
			case USART_PROTOCOL_TEST_QUEUE_MODE:	// get auto mode setting
				usart_rx_data[3] = hs_auto_mode_setting;
				usart_rx_data[4] = hs_auto_mode_delay;
				break;
			case USART_PROTOCOL_TEST_FW_REV:		// get firmware version
				usart_rx_data[3] = FW_VERSION_HW;
				usart_rx_data[4] = FW_VERSION_INT;
				usart_rx_data[5] = FW_VERSION_DEC;
				break;
			case USART_PROTOCOL_TEST_RSTN:
				usart_feedback();
				timer_delay_ms(10);
				while(timer_delay_ms(0));
				usart_sys_reset();
				break;
			case USART_PROTOCOL_TEST_BOOTEN:
				fw_update_done(0xff);
				break;
			default:
				usart_cmd_error = USART_PROTOCOL_FB_ERR_BYT2;
				break;
		}
		return Feedback;
	}
	// shot mode
	else if(usart_rx_data[1] == USART_PROTOCOL_SHOT)
	{
		switch(usart_rx_data[2])
		{
			case USART_PROTOCOL_SHOT_ALL:		// control all signals
				hs_all_sw(usart_rx_data[3]);
				break;
			case USART_PROTOCOL_SHOT_POWER:		// control pcie power
				hs_power_sw(usart_rx_data[3]);
				break;
			case USART_PROTOCOL_SHOT_SIGNAL:	// control pcie signal
				hs_signal_sw(usart_rx_data[3]);
				break;
			case USART_PROTOCOL_SHOT_RELAY_1:	// control relay 1
				relay_switch(1,usart_rx_data[3]);
				break;
			case USART_PROTOCOL_SHOT_RELAY_2:	// control relay 2
				relay_switch(2,usart_rx_data[3]);
				break;
			case USART_PROTOCOL_SHOT_RELAY_3:	// control relay 3
				relay_switch(3,usart_rx_data[3]);
				break;
			case USART_PROTOCOL_SHOT_EX_RELAY_1:// control relay 1 on p2p p2u u2u module
				relay_switch(4,usart_rx_data[3]);
				break;
			case USART_PROTOCOL_SHOT_EX_RELAY_2:// control relay 2 on p2p p2u u2u module
				relay_switch(5,usart_rx_data[3]);
				break;
			default:
				usart_cmd_error = USART_PROTOCOL_FB_ERR_BYT2;
				break;
		}
		return Feedback;
	}
	// auto mode setting
	else if(usart_rx_data[1] == USART_PROTOCOL_QUEUE)
	{
		switch(usart_rx_data[2])
		{
			case USART_PROTOCOl_QUEUE_MODE_00:
				hs_auto_mode_setting = 0;
				hs_auto_mode_delay = usart_rx_data[3];
				if(hs_auto_mode_delay > USART_PROTOCOL_QUEUE_DELAY_MAX)
				{
					hs_auto_mode_delay = USART_PROTOCOL_QUEUE_DELAY_MAX;
				}
				else if(hs_auto_mode_delay < USART_PROTOCOL_QUEUE_DELAY_MIN)
				{
					hs_auto_mode_delay = USART_PROTOCOL_QUEUE_DELAY_MIN;
				}
				break;
			default:
				usart_cmd_error = USART_PROTOCOL_FB_ERR_BYT2;
				break;
		}
		return Feedback;
	}
	// auto mode power up and down
	else if(usart_rx_data[1] == USART_PROTOCOL_SWAP)
	{
		switch(usart_rx_data[2])
		{
			case USART_PROTOCOL_SWAP_POWER_UP:
				if(hs_auto_mode_setting == 0)
				{
					hs_auto_mode_0_power_up();
				}
				else
				{
					usart_cmd_error = USART_PROTOCOL_FB_ERR_BYT2;
				}
				break;
			case USART_PROTOCOL_SWAP_POWER_DW:
				if(hs_auto_mode_setting == 0)
				{
					hs_auto_mode_0_power_dw();
				}
				else
				{
					usart_cmd_error = USART_PROTOCOL_FB_ERR_BYT2;
				}
				break;
			case USART_PROTOCOL_AT_RLY_MD_01:	// relay auto mode 1
				rly_auto_mode_0((usart_rx_data[3]+1));
				break;
			case USART_PROTOCOL_AT_RLY_MD_02:	// relay auto mode 2
				rly_auto_mode_1((usart_rx_data[3]+1));
				break;
			default:
				break;
		}
		return Feedback;
	}
	// configuration mode
	else if(usart_rx_data[1] == USART_PROTOCOL_CFG)
	{
		hs_pwr_sig_sel(usart_rx_data[2],usart_rx_data[3]);
		return Feedback;
	}
	// firmware update
	else if(usart_rx_data[1] == USART_PROTOCOL_FW_UPDATE)	
	{
		if(usart_rx_data[2] == USART_PROTOCOL_FW_UPDATE_STRT)	// start
		{
			fw_update_en = 1;
			fw_update_length = (usart_rx_data[6] << 24) | (usart_rx_data[5] << 16) | (usart_rx_data[4] << 8) | (usart_rx_data[3]);
			usart_rx_data[3] = 0;
			usart_rx_data[4] = 0;
			usart_rx_data[5] = 0;
			return Feedback;
		}
		else if(usart_rx_data[2] == USART_PROTOCOL_FW_UPDATE_FNSH)	// end
		{
			if(fw_update_en)
			{
				usart_cmd_error = USART_PROTOCOL_FB_ERR_FW_DWLD;
				return Feedback;
			}
			fw_update_done(2);
			return Feedback;
		}
		else if(usart_rx_data[2] == USART_PROTOCOL_FW_UPDATE_RSTN)	// reset
		{
			usart_feedback();
			timer_delay_ms(10);
			while(timer_delay_ms(0));
			usart_sys_reset();
		}
	}
	else if(usart_rx_data[1] == USART_PROTOCOL_PWR_LOAD_TEST) // load test
	{
		power_load_test(usart_rx_data[2],usart_rx_data[3]);
		return Feedback;
	}
	else
	{
		usart_cmd_error = USART_PROTOCOL_FB_ERR_BYT1;
		return Feedback;
	}
	return Init;
}


enum USART_FSM_STATUS usart_feedback(void)
{
	usart_rx_data[6] = usart_cmd_error;
	usart_set_tx_frame((uint8_t *)usart_rx_data);
	usart_send_data();
	led_blink_en = 2;
	usart_cmd_error = USART_PROTOCOL_FB_OK;
	return Init;
}

enum USART_FSM_STATUS usart_sys_reset(void)
{
	//NVIC_SystemReset();
	struct wdt_conf wdt_config;
	
	/* Get WDT default configuration */
	wdt_get_config_defaults(&wdt_config);

	/* Set the required clock source and timeout period */
	wdt_config.clock_source   = GCLK_GENERATOR_4;

	/* Initialize and enable the Watchdog with the user settings */
	wdt_set_config(&wdt_config);

	while (1) 
	{
		/* Wait for watchdog reset */
	}
	return Init;
}


enum USART_FSM_STATUS usart_2_i2c_mode(void)
{
	uint8_t rstn = 0;
	uint8_t i = 0;
	
	rstn = usart_2_iic_operation((uint8_t *)usart_rx_data);
	if(rstn == 0)	// ACK
	{
		for(i = 1;i < 6;i++)
		{
			usart_rx_data[i] = 0;
		}
		usart_cmd_error = 0;
		return Feedback;
	}
	if(rstn == 1)	// error, no feedback
	{	
		return Init;
	}
	if(rstn == 2)	// sending, no clear data
	{
		usart_cmd_error = usart_rx_data[6];
		return Feedback;
	}
	if(rstn == 3)	// has send all data, no feedback
	{
		return Init;
	}
	return Feedback;
}

enum USART_FSM_STATUS usart_waiting(void)
{
	usart_2_iic_timeout();
	return Init;
}