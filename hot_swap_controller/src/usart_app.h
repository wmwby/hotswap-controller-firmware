/*
 * usart_app.h
 *
 * Created: 2016/8/31 17:12:35
 *  Author: mingwei.wu
 */ 


#ifndef USART_APP_H_
#define USART_APP_H_

/*
 *	usart protocol definition
 */

#define USART_FRAME_LENGTH				8		// the frame length
// Byte 0: Header
#define USART_PROTOCOL_HEADER			0xFE	// the usart communication frame header
#define USART_PROTOCOL_HEADER_FW_DWLD	0xFA
#define USART_PROTOCOL_HEADER_USART_I2C	0xFC	// the header of usart 2 i2c

// Byte 1
#define USART_PROTOCOL_TEST				0x0		// test the usart link or read some status
#define USART_PROTOCOL_SHOT				0x1		// control single signal or a set of signals
#define USART_PROTOCOL_QUEUE			0x2		// set the power up or down sequence 
#define USART_PROTOCOL_SWAP				0x3		// simulate hot plug or hot pull 
#define USART_PROTOCOL_CFG				0x4		// configuration mode



#define USART_PROTOCOL_PWR_LOAD_TEST	0x50	// used to control power transient response 

#define USART_PROTOCOL_FW_UPDATE		0x60	// used to update firmware
#define USART_PROTOCOL_I2C_EN			0x61	// usart 2 i2c communication
// Byte 2
#define USART_PROTOCOL_TEST_LINK		0x0		
#define USART_PROTOCOL_TEST_QUEUE_MODE	0x1
#define USART_PROTOCOL_TEST_FW_REV		0x2		// firmware version

#define USART_PROTOCOL_TEST_RSTN		0x5
#define USART_PROTOCOL_TEST_BOOTEN		0x6

#define USART_PROTOCOL_SHOT_RELAY_1		0x1
#define	USART_PROTOCOL_SHOT_RELAY_2		0x2
#define	USART_PROTOCOL_SHOT_RELAY_3		0x3
#define USART_PROTOCOL_SHOT_ALL			0x40
#define	USART_PROTOCOL_SHOT_POWER		0x41
#define	USART_PROTOCOL_SHOT_SIGNAL		0x42
#define USART_PROTOCOL_SHOT_PE12V		0x43
#define USART_PROTOCOL_SHOT_EX_RELAY_1	0x64	// relay 1 on p2p p2u u2u module
#define USART_PROTOCOL_SHOT_EX_RELAY_2	0x65	// relay 2 on p2p p2u u2u module


#define USART_PROTOCOl_QUEUE_MODE_00	0x0
#define USART_PROTOCOl_QUEUE_MODE_01	0x1
#define USART_PROTOCOl_QUEUE_MODE_02	0x2
#define USART_PROTOCOl_QUEUE_MODE_03	0x3
#define USART_PROTOCOl_QUEUE_MODE_04	0x4

#define USART_PROTOCOl_QUEUE_MODE_10	0x20
#define USART_PROTOCOl_QUEUE_MODE_11	0x21
#define USART_PROTOCOl_QUEUE_MODE_12	0x22
#define USART_PROTOCOl_QUEUE_MODE_13	0x23
#define USART_PROTOCOl_QUEUE_MODE_14	0x24

#define USART_PROTOCOL_SWAP_POWER_UP	0x1
#define USART_PROTOCOL_SWAP_POWER_DW	0x0

#define USART_PROTOCOL_FW_UPDATE_STRT	0x1
#define USART_PROTOCOL_FW_UPDATE_FNSH	0x2
#define USART_PROTOCOL_FW_UPDATE_RSTN	0x3

#define USART_PROTOCOL_I2C_READ			0x1
#define USART_PROTOCOL_I2C_WRITE		0x0

//#define USART_PROTOCOL_PLT_SET			0x0		// set the combination of loads
//#define USART_PROTOCOL_PLT_CTRL			0x1		// switch on or off the load

/*
 	SETTING		   LOAD
	-------------------
	  0				1
	  1				2	
	  2				3
	  3				4
	  4				1,2
	  5				1,3
	  6				1,4
	  7				2,3
	  8				2,4
	  9				3,4
	  10			1,2,3
	  11			1,2,4
	  12			1,3,4
	  13			2,3,4
	  14			1,2,3,4
*/
#define USART_PROTOCOL_PLT_SET_00		0x0		
#define USART_PROTOCOL_PLT_SET_01		0x1	
#define USART_PROTOCOL_PLT_SET_02		0x2		
#define USART_PROTOCOL_PLT_SET_03		0x3		
#define USART_PROTOCOL_PLT_SET_04		0x4		
#define USART_PROTOCOL_PLT_SET_05		0x5		
#define USART_PROTOCOL_PLT_SET_06		0x6		
#define USART_PROTOCOL_PLT_SET_07		0x7		
#define USART_PROTOCOL_PLT_SET_08		0x8
#define USART_PROTOCOL_PLT_SET_09		0x9
#define USART_PROTOCOL_PLT_SET_10		0xA
#define USART_PROTOCOL_PLT_SET_11		0xB
#define USART_PROTOCOL_PLT_SET_12		0xC
#define USART_PROTOCOL_PLT_SET_13		0xD
#define USART_PROTOCOL_PLT_SET_14		0xE

#define USART_PROTOCOL_AT_RLY_MD_01		0x10
#define USART_PROTOCOL_AT_RLY_MD_02		0x11


// Byte 3
#define	USART_PROTOCOL_SHOT_OFF			0x0
#define	USART_PROTOCOL_SHOT_ON			0x1

#define USART_PROTOCOL_QUEUE_DELAY_MIN	0x0
#define USART_PROTOCOL_QUEUE_DELAY_MAX	0x1E

#define USART_PROTOCOL_PLT_CTRL_ON		0x1
#define USART_PROTOCOL_PLT_CTRL_OFF		0x0

#define USART_PROTOCOL_AT_RLY_1			0x0
#define USART_PROTOCOL_AT_RLY_2			0x1
#define USART_PROTOCOL_AT_RLY_3			0x2
#define USART_PROTOCOL_AT_RLY_4			0x3
#define USART_PROTOCOL_AT_RLY_5			0x4




// Byte 4
#define USART_PROTOCOL_HHHL				0x0
#define USART_PROTOCOL_SFF				0x1

// Byte 6 Feedback ErrorCode
#define USART_PROTOCOL_FB_OK			0x0
#define USART_PROTOCOL_FB_ERR_BYT1		0x1
#define USART_PROTOCOL_FB_ERR_BYT2		0x2
#define USART_PROTOCOL_FB_ERR_BYT3		0x3
#define USART_PROTOCOL_FB_ERR_BYT4		0x4
#define USART_PROTOCOL_FB_ERR_BUSY		0x5
#define USART_PROTOCOL_FB_ERR_OTHERS	0x6
#define USART_PROTOCOL_FB_ERR_NONE		0x7

#define USART_PROTOCOL_FB_ERR_FW_DWLD	0x10

#define USART_PROTOCOL_I2C_ING			0x0
#define USART_PROTOCOL_I2C_END			0x1

/* firmware version 2.1 */
#define FW_VERSION_HW	0x0
#define FW_VERSION_INT	0x2
#define FW_VERSION_DEC	0x1

#define SET_END							0xff

/* usart 2 i2c error code */
#define USART_PROTOCOL_I2C_ERR_OK		0x0
#define USART_PROTOCOL_I2C_ERR_RX		0x2
#define USART_PROTOCOL_I2C_ERR_WR		0x3
#define USART_PROTOCOL_I2C_ERR_RD		0x4
#define USART_PROTOCOL_I2C_ERR_RD_END	0x10
#define USART_PROTOCOL_I2C_ERR_TIMEOUT	300	// 300ms


enum USART_FSM_STATUS usart_protocol_analysis(void);
enum USART_FSM_STATUS usart_feedback(void);
enum USART_FSM_STATUS usart_sys_reset(void);
enum USART_FSM_STATUS usart_2_i2c_mode(void);
enum USART_FSM_STATUS usart_waiting(void);

#endif /* USART_APP_H_ */