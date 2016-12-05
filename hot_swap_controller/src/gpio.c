/*
 * gpio.c
 *
 * Created: 2016/8/31 15:57:23
 *  Author: mingwei.wu
 */ 
#include <asf.h>
#include "gpio.h"
#include "main.h"

static void get_module_form_factor(void)
{
	bool id0 = false;
	bool id1 = false;
	struct port_config config_port_pin;
	
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_UP;
	
	port_pin_set_config(MDL_ID0,&config_port_pin);
	port_pin_set_config(MDL_ID1,&config_port_pin);
	
	id0 = port_pin_get_input_level(MDL_ID1);
	id1 = port_pin_get_input_level(MDL_ID1);
	
	if((!id1) && (!id0)) //b'00, p2p
	{
		module_form_factor = p2p;
	}
	else if((!id1) && (id0)) // b'01, p2u
	{
		module_form_factor = p2u;
	}
	else if((id1) && (!id0)) // b'10, u2u
	{
		module_form_factor = u2u;
	}
	else
	{
		module_form_factor = p2p;
	}
}


void gpio_init(void)
{
	struct port_config config_port_pin;
	
	get_module_form_factor();
	
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
	
	port_pin_set_config(MCU_LED1,&config_port_pin);
	port_pin_set_output_level(MCU_LED1,true);
	port_pin_set_config(MCU_RELAY_1,&config_port_pin);
	port_pin_set_output_level(MCU_RELAY_1,true);
	port_pin_set_config(MCU_RELAY_2,&config_port_pin);
	port_pin_set_output_level(MCU_RELAY_2,true);
	port_pin_set_config(MCU_RELAY_3,&config_port_pin);
	port_pin_set_output_level(MCU_RELAY_3,true);
	
	port_pin_set_config(PE_PWR_SLT,&config_port_pin);
	port_pin_set_output_level(PE_PWR_SLT,false);
	port_pin_set_config(PE_SMBS_SLT,&config_port_pin);
	port_pin_set_output_level(PE_SMBS_SLT,true);
	port_pin_set_config(PE_12V_SW,&config_port_pin);
	port_pin_set_output_level(PE_12V_SW,true);
	port_pin_set_config(PE_3V3_SW,&config_port_pin);
	port_pin_set_output_level(PE_3V3_SW,true);
	port_pin_set_config(PE_AUX3V3_SW,&config_port_pin);
	port_pin_set_output_level(PE_AUX3V3_SW,true);
	port_pin_set_config(PE_SMBS_SW,&config_port_pin);
	port_pin_set_output_level(PE_SMBS_SW,true);
	port_pin_set_config(PE_CLK0_SW,&config_port_pin);
	port_pin_set_output_level(PE_CLK0_SW,true);
	port_pin_set_config(PE_RST0_SW,&config_port_pin);
	port_pin_set_output_level(PE_RST0_SW,true);
	port_pin_set_config(PE_LANE_ONE_SW,&config_port_pin);
	port_pin_set_output_level(PE_LANE_ONE_SW,true);
	port_pin_set_config(PE_PRSNT_SW,&config_port_pin);
	port_pin_set_output_level(PE_PRSNT_SW,true);
	port_pin_set_config(RLY0_SW,&config_port_pin);
	port_pin_set_output_level(RLY0_SW,true);
	port_pin_set_config(RLY1_SW,&config_port_pin);
	port_pin_set_output_level(RLY1_SW,true);
	port_pin_set_config(FAN_SW,&config_port_pin);
	port_pin_set_output_level(FAN_SW,true);
	port_pin_set_config(VCC3_3_OUT_EN,&config_port_pin);
	port_pin_set_output_level(VCC3_3_OUT_EN,true);
	
	if(module_form_factor == u2u)	// then set the signals to output
	{
		port_pin_set_config(PE_CLK1_SW,&config_port_pin);
		port_pin_set_output_level(PE_CLK1_SW,true);
		port_pin_set_config(PE_RST1_SW,&config_port_pin);
		port_pin_set_output_level(PE_RST1_SW,true);
		port_pin_set_config(PE_LANE_TWO_SW,&config_port_pin);
		port_pin_set_output_level(PE_LANE_TWO_SW,true);
		port_pin_set_config(PE_IF_DET_SW,&config_port_pin);
		port_pin_set_output_level(PE_IF_DET_SW,true);
		port_pin_set_config(PE_12V_PRE_SW,&config_port_pin);
		port_pin_set_output_level(PE_12V_PRE_SW,true);
		port_pin_set_config(PE_DP_SW,&config_port_pin);
		port_pin_set_output_level(PE_DP_SW,true);
	}
	else 
	{
		
		if(module_form_factor == p2p)	// then set the signals to input except for lane_2_sw
		{
			port_pin_set_config(PE_LANE_TWO_SW,&config_port_pin);
			port_pin_set_output_level(PE_LANE_TWO_SW,true);
		}
		config_port_pin.direction = PORT_PIN_DIR_INPUT;
		config_port_pin.input_pull = PORT_PIN_PULL_DOWN;
		port_pin_set_config(PE_CLK1_SW,&config_port_pin);
		port_pin_set_config(PE_RST1_SW,&config_port_pin);
		port_pin_set_config(PE_IF_DET_SW,&config_port_pin);
		port_pin_set_config(PE_12V_PRE_SW,&config_port_pin);
		port_pin_set_config(PE_DP_SW,&config_port_pin);
		if(module_form_factor == p2u)	// then set the signals to input with pull down
		{
			port_pin_set_config(PE_LANE_TWO_SW,&config_port_pin);
		}
	}
	config_port_pin.direction = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(VCC3_3_OUT_FLT_N,&config_port_pin);
}
