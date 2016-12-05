/*
 * timer.c
 *
 * Created: 2016/8/31 15:24:45
 *  Author: mingwei.wu
 */ 

#include <asf.h>
#include "timer.h"
#include "gpio.h"
#include "usart_link.h"

uint8_t led_blink_en = 0;
uint8_t timer_usart_i2c_timeout = 0;

struct tc_module tc_instance;
volatile uint16_t delay_time = 0;


uint16_t timer_delay_ms(uint16_t ms)
{
	if(!ms)
	{
		return delay_time>>1;
	}
	delay_time = ms<<1;
	return ms;
}

static void configure_tc(void)
{
	struct tc_config config_tc;
	tc_get_config_defaults(&config_tc);
	
	config_tc.counter_size		= TC_COUNTER_SIZE_8BIT;
	config_tc.clock_source		= GCLK_GENERATOR_2;
	config_tc.clock_prescaler	= TC_CLOCK_PRESCALER_DIV16;
	config_tc.reload_action		= TC_RELOAD_ACTION_GCLK;
	config_tc.oneshot			= false;
	config_tc.counter_8_bit.period = 250;
	
	tc_init(&tc_instance,TIMER_MODULE,&config_tc);
	tc_enable(&tc_instance);
}

/*
 *	Brief: callback function of 0.5ms
 *	
 *	fun1: led is blinking every 1 second
 */
static void tc_callback_0_5ms(struct tc_module *const module_inst)
{
	static uint16_t timer_counter = 0;
	static uint8_t timer_counter_usart = 0;
	static uint8_t  led_blink_times = 0;
	static uint16_t first = 0;
	
	if(timer_counter_usart == 3)
	{
		timer_counter_usart = 0;
		
	}
	else
	{
		timer_counter_usart++;
	}
	
	usart_read_data();
	
	
	if(delay_time)
	{
		delay_time--;
	}
	
	if(timer_counter == 999)
	{
		timer_counter = 0;
	}
	else
	{
		timer_counter++;
	}
	
	if(first < 3000)
	{
		first++;
		port_pin_set_output_level(MCU_LED1,false);
	}
	else
	{
		if(!led_blink_en)
		{
			if(timer_counter == 0)
			{
				port_pin_toggle_output_level(MCU_LED1);
			}
		}
		else
		{
			if((timer_counter+1) % 250 == 0)
			{
				port_pin_toggle_output_level(MCU_LED1);
				led_blink_times++;
			}
			if(led_blink_times >= 8)
			{
				if(led_blink_en == 2)
				{
					led_blink_en = 0;
					led_blink_times = 0;
				}
			}
		}
	}
	
	if(timer_usart_i2c_timeout)
	{
		timer_usart_i2c_timeout--;
	}
}

static void configure_tc_callback(void)
{
	tc_register_callback(&tc_instance,tc_callback_0_5ms,(TC_CALLBACK_OVERFLOW));
	tc_register_callback(&tc_instance,tc_callback_0_5ms,(TC_CALLBACK_CC_CHANNEL0));
	tc_register_callback(&tc_instance,tc_callback_0_5ms,(TC_CALLBACK_CC_CHANNEL1));
	
	tc_enable_callback(&tc_instance,TC_CALLBACK_OVERFLOW);
}


void timer_init(void)
{
	configure_tc();
	configure_tc_callback();
}