/*
 * main.h
 *
 * Created: 2016/9/1 9:54:10
 *  Author: mingwei.wu
 */ 


#ifndef MAIN_H_
#define MAIN_H_

enum USART_FSM_STATUS {Init,Analysis,Feedback,Reset,I2C};
enum FORM_FACTOR {p2p,p2u,u2u};

extern volatile enum USART_FSM_STATUS usart_status;
extern enum FORM_FACTOR module_form_factor; 

#endif /* MAIN_H_ */