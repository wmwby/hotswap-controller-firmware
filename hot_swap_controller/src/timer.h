/*
 * timer.h
 *
 * Created: 2016/8/31 15:24:32
 *  Author: mingwei.wu
 */ 


#ifndef TIMER_H_
#define TIMER_H_


extern uint8_t led_blink_en;
extern uint8_t timer_usart_i2c_timeout;

void timer_init(void);
/*
 *	delay function, max delay time  = 32768ms
 *	
 *	param:	if ms==0 return the left time;
 *			else set the delay time;
 *			the max value of ms is 32768
 */
uint16_t timer_delay_ms(uint16_t ms);

#endif /* TIMER_H_ */