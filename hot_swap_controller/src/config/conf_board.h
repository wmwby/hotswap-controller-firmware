/**
 * \file
 *
 * \brief User board configuration template
 *
 * Copyright (C) 2012-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#define FT_CONTROLLER	0
#define HS_CONTROLLER	1

#define MY_BOARD	HS_CONTROLLER

#if MY_BOARD == FT_CONTROLLER
	#define MCU_LED1					PIN_PA09
	#define MCU_LED2					PIN_PA08
	#define MCU_USB_UART_TX				PIN_PB22	//PIN49
	#define MCU_USB_UART_RX				PIN_PB23	//PIN50
	
	
	#define TIMER_MODULE				TC0
	#define USART_MODULE				SERCOM5
	
#elif MY_BOARD == HS_CONTROLLER
	// PIN01~16
	#define VCC3_3_OUT_FLT_N			PIN_PA00
	#define VCC3_3_OUT_EN				PIN_PA01
	#define MCU_LED1					PIN_PA02
	#define MCU_EXT_INT0				PIN_PA03
	#define HSC_GPIO_22_AD_11			PIN_PB04
	#define HSC_GPIO_21_AD_10			PIN_PB05
									//	PIN07	:GND
									//  PIN08	:Analog 3.3V
	#define HSC_GPIO_20_AD_09			PIN_PB06
	#define HSC_GPIO_19_AD_08			PIN_PB07
	#define HSC_GPIO_18_AD_07			PIN_PB08
	#define HSC_GPIO_17_AD_06			PIN_PB09
	#define HSC_GPIO_16_AD_05			PIN_PA04
	#define HSC_GPIO_13_AD_04			PIN_PA05
	#define HSC_GPIO_12_AD_03			PIN_PA06
	#define HSC_GPIO_11_AD_02			PIN_PA07

	// PIN17~32
	#define HSC_GPIO_15_I2C_SDA			PIN_PA08
	#define HSC_GPIO_14_I2C_SCL			PIN_PA09
	#define HSC_GPIO_10_AD_01			PIN_PA10
	#define HSC_GPIO_09_AD_00			PIN_PA11
									//	PIN21	:Digital 3.3V
									//	PIN22	:GND
	#define HSC_GPIO_08					PIN_PB10
	#define HSC_GPIO_07					PIN_PB11
	#define HSC_GPIO_06					PIN_PB12
	#define HSC_GPIO_05					PIN_PB13
	#define HSC_GPIO_04					PIN_PB14
	#define HSC_GPIO_03					PIN_PB15
	#define HSC_GPIO_02					PIN_PA12
	#define HSC_GPIO_01					PIN_PA13
	#define HSC_GPIO_00					PIN_PA14
	#define MCU_RELAY_3					PIN_PA15

	//PIN33~48
									//	PIN33	:GND
									//	PIN34	:Digital 3.3V
	#define EEPROM_SPI_MOSI				PIN_PA16
	#define EEPROM_SPI_SCK				PIN_PA17	//PIN36
	#define EEPROM_SPI_SS				PIN_PA18	//PIN37
	#define EEPROM_SPI_MISO				PIN_PA19	//PIN38
	#define EXTEND_S_SDA				PIN_PB16	//PIN39
	#define EXTEND_S_SCL				PIN_PB17	//PIN40
	#define MCU_RELAY_1					PIN_PA20	//PIN41
	#define MCU_RELAY_2					PIN_PA21	//PIN42
	#define EXTEND_M_SDA				PIN_PA22	//PIN43
	#define EXTEND_M_SCL				PIN_PA23	//PIN44
	#define EXTEND_UART_TX				PIN_PA24	//PIN45
	#define EXTEND_UART_RX				PIN_PA25	//PIN46
									//	PIN47	:GND
									//	PIN48	:Digital 3.3V

	//PIN49~64
	#define MCU_USB_UART_TX				PIN_PB22	//PIN49
	#define MCU_USB_UART_RX				PIN_PB23	//PIN50
	#define ETHERNET_IRQ				PIN_PA27	//PIN51
	#define MCU_RST_N								//PIN52
	#define ETHERNET_PME				PIN_PA28	//PIN53
									//	PIN54	:GND
									//	PIN55	:VDDCORE
									//	PIN56	:VDDIN
	#define MCU_SWDCLK					PIN_PA30	//PIN57
	#define MCU_SWDIO					PIN_PA31	//PIN58
	#define ETHERNET_SPI_MOSI			PIN_PB30	//PIN59
	#define ETHERNET_SPI_SCK			PIN_PB31	//PIN60
	#define ETHERNET_SPI_MISO			PIN_PB00	//PIN61
	#define ETHERNET_SPI_SS_A			PIN_PB01	//PIN62
	#define EXTEND_GPIO					PIN_PB02	//PIN63
	#define ETHERNET_RST_N				PIN_PB03	//PIN64

	#define TIMER_MODULE				TC0
	#define USART_MODULE				SERCOM5
	
	
	#define PE_CLK1_SW					HSC_GPIO_04
	#define PE_RST1_SW					HSC_GPIO_03
	#define PE_12V_SW					HSC_GPIO_05
	#define PE_3V3_SW					HSC_GPIO_02
	#define PE_AUX3V3_SW				HSC_GPIO_06
	#define PE_PWR_SLT					HSC_GPIO_01
	#define PE_SMBS_SW					HSC_GPIO_07
	#define PE_SMBS_SLT					HSC_GPIO_00
	#define PE_CLK0_SW					HSC_GPIO_08
	#define PE_RST0_SW					HSC_GPIO_20_AD_09
	#define PE_LANE_ONE_SW				HSC_GPIO_09_AD_00
	#define PE_LANE_TWO_SW				HSC_GPIO_19_AD_08
	#define PE_PRSNT_SW					HSC_GPIO_10_AD_01
	#define PE_IF_DET_SW				HSC_GPIO_18_AD_07
	#define PE_I2C_SCL					HSC_GPIO_14_I2C_SCL
	#define PE_I2C_SDA					HSC_GPIO_15_I2C_SDA
	#define PE_12V_PRE_SW				HSC_GPIO_17_AD_06
	#define PE_DP_SW					HSC_GPIO_22_AD_11
	#define MDL_ID0						HSC_GPIO_16_AD_05
	#define MDL_ID1						HSC_GPIO_21_AD_10
	#define RLY0_SW						HSC_GPIO_13_AD_04
	#define RLY1_SW						HSC_GPIO_11_AD_02
	#define FAN_SW						HSC_GPIO_12_AD_03

	#define PWR_LD0						HSC_GPIO_05
	#define PWR_LD1						HSC_GPIO_06

#endif

#endif // CONF_BOARD_H
