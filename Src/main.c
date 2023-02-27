/*******************************************************************************
 *				 _ _                                             _ _
				|   |                                           (_ _)
				|   |        _ _     _ _   _ _ _ _ _ _ _ _ _ _   _ _
				|   |       |   |   |   | |    _ _     _ _    | |   |
				|   |       |   |   |   | |   |   |   |   |   | |   |
				|   |       |   |   |   | |   |   |   |   |   | |   |
				|   |_ _ _  |   |_ _|   | |   |   |   |   |   | |   |
				|_ _ _ _ _| |_ _ _ _ _ _| |_ _|   |_ _|   |_ _| |_ _|
								(C)2023 Lumi
 * Copyright (c) 2023
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File name: main.c
 *
 * Description:
 *
 * Author: CuuNV
 *
 * Last Changed By:  $Author: CuuNV $
 * Revision:         $Revision: $
 * Last Changed:     $Date: $Feb 21, 2023
 *
 * Code sample:
 ******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "GUI.h"
#include "picture.h"
#include "string.h"
#include "serial-uart.h"
#include "timer.h"
#include "qrcode-to-lcd.h"
#include "utilities.h"
#include "button-v1-1.h"
#include "menu.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
//define other
#define RX_MAX_INDEX_BYTE						256
#define LENGTH_OF_MAC							8
#define LENGTH_OF_VERSION						3
#define LENGTH_OF_DEVICE_TYPE					1
#define LENGTH_OF_PID							2
#define CMD_ID_ZIGBEE_AND_BLE				0xFF
#define CMD_ID_MCU_TOUCH					0xAB
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
// enum of system
typedef enum{
	STATE_APP_STARTUP,
	STATE_APP_IDLE,
	STATE_APP_RESET
}StateApp_e;

// enum of LUMI_SWITCH
typedef enum {
	PROTOCOL_TYPE_ZIGBEE     = 0x00,
	PROTOCOL_TYPE_BLUETOOTH	 = 0x01,
	PROTOCOL_TYPE_ZWAVE		 = 0x02
}ProtocolType_e;

typedef enum {
	SWITCH_DEVICE			= 0x01,
	SOCKETS_DEVICE			= 0x02,
	CURTAIN_DEVICE			= 0x03,
	DIMMER_DEVICE			= 0x04,
	FAN_DEVICE				= 0x05,
	IR_CONTROLLER_DEVICE	= 0x06,
	PIR_SENSOR				= 0x07,
	LIGHT_SENSOR			= 0x08,
	TEMP_SENSOR				= 0x09,
	HUMI_SENSOR				= 0x0A
}DeviceType_e;

typedef enum {
	BLUETOOTH				= 0x00,
	ZIGBEE					= 0x01,
	MCU						= 0x02
}InforType_e;

typedef enum {
	UN_PROVISION			= 0x00,
	PROVISIONING			= 0x01,
	PROVISIONED				= 0x02
}ProvisionState_e;

typedef struct {
	uint8_t 			byCmdId;
	uint8_t				protocolType;
	uint8_t				deviceType;
	uint8_t				byEndpointCnt;
	ProvisionState_e	provisonState;
	uint8_t				pbyMAC[8];
	uint8_t				pbyVersion[3];
	uint8_t				pbyInFor[20];
}CmdData_t;

typedef struct {
    uint8_t		cmd_id;
    uint8_t  	msg_from;
    uint8_t  	version[3];
    uint8_t  	type;
    uint8_t  	endpoint_cnt;
    uint8_t  	led_intensity;
    uint8_t  	full_port;
    uint8_t  	relay_type;
    uint8_t  	cz_type[4];
    uint8_t  	c_Xor;
}McuInfor_t;

static uint8_t g_byEnpointCntMCU = 0;
static uint8_t g_byEnpointCntBLE = 0;
static uint8_t g_byEnpointCntZigBee = 0;
static char g_pstrMACZigbee[LENGTH_OF_MAC * 2+1] = {0};
static char g_pstrMACBle[LENGTH_OF_MAC * 2+1] = {0};
static char g_pstrMACLast[LENGTH_OF_MAC * 2+1] = {0};
static char g_pstrVersionBluetooth[LENGTH_OF_VERSION*2+1] = {0};
static char g_pstrVersionZigBee[LENGTH_OF_VERSION*2+1] = {0};
static char g_pstrVersionMCU[LENGTH_OF_VERSION*2+1] = {0};
static uint8_t g_byTypeMCU = 0;

static TestSwMode_e modeTest = NONE;
ValueKey_e valueKey = NOKEY;
StateApp_e eCurrentState = STATE_APP_STARTUP;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void appInitCommon(void);

static void setStateApp(StateApp_e state);

static StateApp_e getStateApp(void);

static void appStateManager(void);

void printMACLcd(char *pTextMAC,u16 x,u16 y,uint8_t bySize);

void printEndPointCnt(u8 pTextEpc,u16 x,u16 y,uint8_t bySize, InforType_e type);

void printVersion(char *pTextMAC,u16 x,u16 y,uint8_t bySize,InforType_e type);

void printModelId(char *pText,u16 x,u16 y,uint8_t bySize);

void printProductID(char *pText,u16 x,u16 y,uint8_t bySize);

void printTypeMCU(u8 pText,u16 x,u16 y,uint8_t bySize);

void getModelID(char * pOutPut,u8 *pInPut);

static void hexToAscii(char *pByDataOutPut,uint8_t *pByDataInPut,uint8_t byDataLength);

static void processedUartReceivedNewsOfZigbeeAndBLE(CmdData_t *pCmd);

static void processedUartReceivedNewsOfTouch(McuInfor_t *pCmd);

static void procUartCmd(void *arg);


/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
int main(void)
{
	appInitCommon();

    /* Loop forever */
	while(1)
	{
		appStateManager();
		processSerialUartReceiver();
	}
}
/**
 * @func   appInitCommon
 * @brief  Initializes functions
 * @param  None
 * @retval None
 */

static void appInitCommon(void)
{
	SystemCoreClockUpdate();
	buttonInit();
	TimerInit();
	serialUartInit();
	LCD_Init();
	SerialHandleEventCallback(procUartCmd);
	eCurrentState = STATE_APP_STARTUP;
}
/**
 * @func   setStateApp
 * @brief  Set state of application
 * @param  state: State of application
 * @retval None
 */

static void setStateApp(StateApp_e state)
{
	eCurrentState = state;
}
/**
 * @func   setStateApp
 * @brief  Get state of application
 * @param  None
 * @retval State of application
 */
static StateApp_e getStateApp(void)
{
	return eCurrentState;
}
/**
 * @func   appStateManager
 * @brief  Manager state application
 * @param  event: Su kien
 * @retval None
 */
static void appStateManager(void)
{
	StateApp_e event = getStateApp();
	switch(event)
	{
	case STATE_APP_STARTUP: //Su kien khi he thong bat dau duoc cap nguon
		Gui_Drawbmp16(0,0,gImage_logo);
		delay_ms(2000);
		do{
			modeTest = getModeTest();
		}
		while(modeTest == NONE);

		setStateApp(STATE_APP_IDLE);
		USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
		break;
	case STATE_APP_IDLE:
		if(processEventButton() == RETURN)
				{
					USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);
					setStateApp(STATE_APP_RESET);
				}
				processSerialUartReceiver();
		break;
	case STATE_APP_RESET:
		memset(g_pstrMACLast,0,sizeof(g_pstrMACLast));
		setStateApp(STATE_APP_STARTUP);
		break;
	default:
		break;

	}
}



static void processedUartReceivedNewsOfTouch(McuInfor_t *pCmd)
{

	g_byEnpointCntMCU = pCmd->endpoint_cnt;

	g_byTypeMCU = pCmd->type;

	hexToAscii(g_pstrVersionMCU,pCmd->version,LENGTH_OF_VERSION);

}


static void processedUartReceivedNewsOfZigbeeAndBLE(CmdData_t *pCmd)
{
	//0.Get mode

	//1. Khai bao chuoi chua thong tin in ra QR
	char byDataPrint[5+(LENGTH_OF_MAC+LENGTH_OF_VERSION*2 +LENGTH_OF_PID \
			+ LENGTH_OF_DEVICE_TYPE)*2] = {0};

	//2. Chuoi chua thong tin hien tai
	char pstrMAC[LENGTH_OF_MAC * 2+1] = {0};
	char pstrDeviceType[LENGTH_OF_DEVICE_TYPE * 2 +1] = {0};
	static char pStrPID[LENGTH_OF_PID * 2 +1] = {0};
	static char pStrModelID[20] = {0};
	//3. Xoa du lieu cu
	memset(byDataPrint,0,sizeof(byDataPrint));
	//4. Chuyen doi du lieu tu dang Hex sang ma ASCII


	hexToAscii( pstrMAC,pCmd->pbyMAC, LENGTH_OF_MAC);

	hexToAscii(pstrDeviceType, &(pCmd->deviceType), LENGTH_OF_DEVICE_TYPE);

	//5. Quet 2 lan de lay version cua zigbee va bluetooth
	static uint8_t byStatusTemp = 0;

	if(pCmd->protocolType == PROTOCOL_TYPE_ZIGBEE)
	{
		byStatusTemp ++;
		hexToAscii( g_pstrVersionZigBee,pCmd->pbyVersion, LENGTH_OF_VERSION);

		memset(g_pstrMACZigbee,0,sizeof(g_pstrMACZigbee));
		strcpy(g_pstrMACZigbee,pstrMAC);
		g_byEnpointCntZigBee = pCmd->byEndpointCnt;

		getModelID(pStrModelID, pCmd->pbyInFor);

	}else if(pCmd->protocolType == PROTOCOL_TYPE_BLUETOOTH)
	{
		byStatusTemp ++;
		hexToAscii( g_pstrVersionBluetooth,pCmd->pbyVersion, LENGTH_OF_VERSION);

		memset(g_pstrMACBle,0,sizeof(g_pstrMACBle));
		strcpy(g_pstrMACBle,pstrMAC);
		g_byEnpointCntBLE = pCmd->byEndpointCnt;

		hexToAscii( pStrPID,pCmd->pbyInFor, LENGTH_OF_PID);
	}

		//6.1 Reset buffer khi mac thay doi
	static uint8_t byFlagOfBufReset = 0;

	if((strcmp(&g_pstrMACZigbee[4],&g_pstrMACLast[4])!=0)||(strcmp(&g_pstrMACBle[4],&g_pstrMACLast[4])!=0))
	{
		USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);
		if(byFlagOfBufReset == 0)
		{
			//Reset data of queue
			resetBuffer();
			g_byEnpointCntMCU = 0;
			memset(g_pstrVersionBluetooth,0,sizeof(g_pstrVersionBluetooth));
			memset(g_pstrVersionZigBee,0,sizeof(g_pstrVersionZigBee));
			byFlagOfBufReset = 1;

		}
		USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
	}
	//7. So sanh MAC , Ghep thong tin vao 1 chuoi, va in ma Qr_Code ra man hinh
		//Gia tri dem so lan quet lai ban tin khi thay doi thiet bi co endpoint khac
	static uint8_t byCountTemp = 0;

	//7.1 Dual mode

	switch(modeTest)
			{
			case DUAL_MODE:
				if(strcmp(&g_pstrMACZigbee[4],&g_pstrMACLast[4])!=0 &&(byStatusTemp>=2))
				{
					// In ra gia tri khi quet du thong tin
					strcpy(g_pstrMACLast,pstrMAC);


					if((g_byEnpointCntMCU == g_byEnpointCntBLE)&&(g_byEnpointCntMCU == g_byEnpointCntZigBee))
					{
					//Ghep thong tin can luu tru trong QR-code
						strcat(byDataPrint,g_pstrMACZigbee);

						strcat(byDataPrint,",");

						strcat(byDataPrint,pstrDeviceType);

						strcat(byDataPrint,",");

						strcat(byDataPrint,pStrPID);

						strcat(byDataPrint,",");

						strcat(byDataPrint,g_pstrVersionZigBee);

						strcat(byDataPrint,",");

						strcat(byDataPrint,g_pstrVersionBluetooth);

					//prinf Qr-code
						generateQRCode(0,25,byDataPrint,strlen(byDataPrint));

					//prinf Information

						LCD_ClearCursor(0, 155, 240, 320, WHITE);

						printMACLcd(g_pstrMACZigbee,10,155,16);
						//Information General
						printEndPointCnt(g_byEnpointCntMCU, 10, 175, 16,MCU);

						LCD_DrawLine(10,190,230,190);

						//Information of Zigbee chip
						printVersion(g_pstrVersionZigBee, 10, 195, 16,ZIGBEE);

						printModelId(pStrModelID, 10 ,215 ,16);

						LCD_DrawLine(10,235,230,235);

						//Information of BLE chip
						printVersion(g_pstrVersionBluetooth, 10, 235, 16,BLUETOOTH);

						printProductID(pStrPID, 10 ,255 ,16);

						LCD_DrawLine(10,275,230,275);

						//Information of MCU
						printVersion(g_pstrVersionMCU, 10, 275, 16, MCU);

						printTypeMCU(g_byTypeMCU, 10, 295, 16);

						//Reset varialble
						byFlagOfBufReset = 0;

						byCountTemp = 0;

						g_byEnpointCntMCU = 0;

						g_byEnpointCntBLE = 0;

						g_byEnpointCntZigBee = 0;

					}else
					{
						//7. Neu gia tri endPointCnt khac thi cho quet lai lan nua
							if(byCountTemp == 0)
							{
								memset(g_pstrMACLast,0,sizeof(g_pstrMACLast));
								byStatusTemp =0;
								byCountTemp ++;
							}else if(byCountTemp>=1)
							{
								//7.1 Neu firmware loi
								if((g_byEnpointCntMCU != g_byEnpointCntBLE))
								{
									LCD_ClearCursor(0, 25, 240, 320, WHITE);
									//In ra MAC loi.
									Gui_StrCenter(0,100,RED, WHITE, (u8 *)"Firmware BLE ERROR!!!", 16, 0);

									printMACLcd(g_pstrMACZigbee,10,120,16);

									printEndPointCnt(g_byEnpointCntZigBee, 10, 140, 16,ZIGBEE);

									printEndPointCnt(g_byEnpointCntBLE, 10, 160, 16,BLUETOOTH);


									memset(g_pstrVersionBluetooth,0,sizeof(g_pstrVersionBluetooth));
									memset(g_pstrVersionZigBee,0,sizeof(g_pstrVersionZigBee));
									memset(pStrPID,0,sizeof(pStrPID));
									memset(pstrDeviceType,0,sizeof(pstrDeviceType));
									byCountTemp = 0;
								}
								if(g_byEnpointCntMCU != g_byEnpointCntZigBee)
								{
									LCD_ClearCursor(0, 25, 240, 320, WHITE);

									Gui_StrCenter(0,100,RED, WHITE, (u8 *)"Firmware ZigBee ERROR!!!", 16, 0);

									//In ra MAC loi.

									printMACLcd(g_pstrMACZigbee,10,120,16);
									//In ra endpointCnt
									printEndPointCnt(g_byEnpointCntZigBee, 10, 140, 16,ZIGBEE);

									printEndPointCnt(g_byEnpointCntBLE, 10, 160, 16,BLUETOOTH);

									//Reset mang chua ca thong tin: Version, Product ID, Device Type
									memset(g_pstrVersionBluetooth,0,sizeof(g_pstrVersionBluetooth));
									memset(g_pstrVersionZigBee,0,sizeof(g_pstrVersionZigBee));
									memset(pStrPID,0,sizeof(pStrPID));
									memset(pstrDeviceType,0,sizeof(pstrDeviceType));
									byCountTemp = 0;
								}
							}
					}
				}
				break;
			case ZIGBEE_MODE:
				if(strcmp(&g_pstrMACZigbee[4],&g_pstrMACLast[4])!=0 &&(byStatusTemp>=2))
				{
					// In ra gia tri khi quet du thong tin
					strcpy(g_pstrMACLast,pstrMAC);


					if(g_byEnpointCntMCU == g_byEnpointCntZigBee)
					{

						strcat(byDataPrint,g_pstrMACZigbee);

						strcat(byDataPrint,",");

						strcat(byDataPrint,pstrDeviceType);

						strcat(byDataPrint,",");

						strcat(byDataPrint,g_pstrVersionZigBee);

						generateQRCode(0,25,byDataPrint,strlen(byDataPrint));

						//prinf Information

						LCD_ClearCursor(0, 155, 240, 320, WHITE);

						printMACLcd(g_pstrMACZigbee,10,155,16);
						//Information General
						printEndPointCnt(g_byEnpointCntMCU, 10, 175, 16,MCU);

						LCD_DrawLine(10,195,230,195);

						//Information of Zigbee chip
						printVersion(g_pstrVersionZigBee, 10, 195, 16,ZIGBEE);

						printModelId(pStrModelID, 10 ,215 ,16);

						LCD_DrawLine(10,235,230,235);

						//Information of MCU
						printVersion(g_pstrVersionMCU, 10, 235, 16, MCU);

						printTypeMCU(g_byTypeMCU, 10, 255, 16);

						//Reset varialble
						byFlagOfBufReset = 0;

						byCountTemp = 0;

						g_byEnpointCntMCU = 0;

						g_byEnpointCntBLE = 0;

						g_byEnpointCntZigBee = 0;

					}else
					{
						//7. Neu gia tri endPointCnt khac thi cho quet lai lan nua

							if(byCountTemp == 0)
							{
								memset(g_pstrMACLast,0,sizeof(g_pstrMACLast));
								byStatusTemp =0;
								byCountTemp ++;
							}else if(byCountTemp>=1)
							{
								LCD_ClearCursor(0, 25, 240, 320, WHITE);
								Gui_StrCenter(0,100,RED, WHITE, (u8 *)"Firmware ERROR!!!", 16, 0);
								printMACLcd(g_pstrMACZigbee,10,120,16);
								memset(g_pstrVersionBluetooth,0,sizeof(g_pstrVersionBluetooth));
								memset(g_pstrVersionZigBee,0,sizeof(g_pstrVersionZigBee));
								memset(pStrPID,0,sizeof(pStrPID));
								memset(pstrDeviceType,0,sizeof(pstrDeviceType));
								byCountTemp = 0;
							}
					}
				}
				break;
			case BLE_MODE:
				if(strcmp(&g_pstrMACBle[4],&g_pstrMACLast[4])!=0 &&(byStatusTemp>=2))
				{
					// In ra gia tri khi quet du thong tin
					strcpy(g_pstrMACLast,pstrMAC);


					if(g_byEnpointCntMCU == g_byEnpointCntBLE)
					{


						//Ghep thong tin can luu tru trong QR-code
						strcat(byDataPrint,g_pstrMACBle);

						strcat(byDataPrint,",");

						strcat(byDataPrint,pstrDeviceType);

						strcat(byDataPrint,",");

						strcat(byDataPrint,pStrPID);

						strcat(byDataPrint,",");

						strcat(byDataPrint,g_pstrVersionBluetooth);

					//prinf Qr-code
						generateQRCode(0,25,byDataPrint,strlen(byDataPrint));

					//prinf Information

						LCD_ClearCursor(0, 155, 240, 320, WHITE);

						printMACLcd(g_pstrMACBle,10,155,16);
						//Information General
						printEndPointCnt(g_byEnpointCntMCU, 10, 175, 16,MCU);

						LCD_DrawLine(10,195,230,195);

						//Information of BLE chip
						printVersion(g_pstrVersionBluetooth, 10, 195, 16,BLUETOOTH);

						printProductID(pStrPID, 10 ,215 ,16);

						LCD_DrawLine(10,235,230,235);

						//Information of MCU
						printVersion(g_pstrVersionMCU, 10, 235, 16, MCU);

						printTypeMCU(g_byTypeMCU, 10, 255, 16);

						//Reset varialble
						byFlagOfBufReset = 0;

						byCountTemp = 0;

						g_byEnpointCntMCU = 0;

						g_byEnpointCntBLE = 0;

						g_byEnpointCntZigBee = 0;

					}else
					{
						//7. Neu gia tri endPointCnt khac thi cho quet lai lan nua

							if(byCountTemp == 0)
							{
								memset(g_pstrMACLast,0,sizeof(g_pstrMACLast));
								byStatusTemp =0;
								byCountTemp ++;
							}else if(byCountTemp>=1)
							{
								LCD_ClearCursor(0, 25, 240, 320, WHITE);
								Gui_StrCenter(0,100,RED, WHITE, (u8 *)"Firmware ZigBee ERROR!!!", 16, 0);
								printMACLcd(g_pstrMACZigbee,10,120,16);
								printEndPointCnt(g_byEnpointCntBLE, 10, 160, 16,BLUETOOTH);
								memset(g_pstrVersionBluetooth,0,sizeof(g_pstrVersionBluetooth));
								memset(g_pstrVersionZigBee,0,sizeof(g_pstrVersionZigBee));
								memset(pStrPID,0,sizeof(pStrPID));
								memset(pstrDeviceType,0,sizeof(pstrDeviceType));
								byCountTemp = 0;
							}
					}
				}
				break;
			default:
				break;
			}

	if(byStatusTemp >=2)
	{
		byStatusTemp =0;
	}
}
/**
 * @func   procUartCmd
 * @brief  Xu ly truong CMD_ID cua thiet bi
 * @param  void *arg
 * @retval None
 */
static void procUartCmd(void *arg)
{

	CmdData_t *CmdData = (CmdData_t*)arg;
	McuInfor_t * McuInfor = (McuInfor_t*)arg;
	switch(CmdData->byCmdId)
	{
	case CMD_ID_ZIGBEE_AND_BLE:
		processedUartReceivedNewsOfZigbeeAndBLE(CmdData);
		break;
	case CMD_ID_MCU_TOUCH:
		processedUartReceivedNewsOfTouch(McuInfor);
		break;
	default:
		break;
	}

}
static void hexToAscii(char *pByDataOutPut,uint8_t *pByDataInPut,uint8_t byDataLength)
{
	uint8_t byMsb,byLsb;
	memset(pByDataOutPut,0,byDataLength);
	static char str[2]={0};
	for(uint8_t i = 0; i<byDataLength; i++)
	{
		byMsb = (pByDataInPut[i]>>4);
		byLsb = pByDataInPut[i] & 0x0F;
		if((byMsb>=0)&&(byMsb<=9))
		{
			byMsb += 48;
		}else
		{
			byMsb += 55;
		}
		if((byLsb>=0)&&(byLsb<=9))
		{
			byLsb += 48;
		}else
		{
			byLsb += 55;
		}
		str[0] = (char)byMsb;
		str[1] = (char)byLsb;
		strcat(pByDataOutPut,str);
	}
}
/**
 * @func   printMACLcd
 * @brief  In ra man hinh MAC cua thiet bi theo form XX:XX:XX:XX
 * @param
 * @retval None
 */
void printMACLcd(char *pTextMAC,u16 x,u16 y,uint8_t bySize)
{

	char strTemp[28];

	char strTemp2[] = ":";

	char strTemp3[] = "MAC ";

	//Show_Str(x+20,y,BLACK,WHITE,(u8*)strTemp3,bySize,1);

	memset(strTemp,0,sizeof(strTemp));

	strcpy(strTemp,strTemp3);

	uint8_t j = 4;
	uint8_t i = 0;
	for(i = 0; i<strlen(pTextMAC); i++)
	{
			if((i%2 == 0)&&(i !=0))
			{
				j++;
				strcat(strTemp,strTemp2);
			}
			strTemp[j] = pTextMAC[i];
			j++;
	}
	Show_Str(x,y,BLACK,WHITE,(u8*)strTemp,bySize,1);
}
/**
 * @func   printEndPointCnt
 * @brief  In ra man hinh EndPointCnt cua thiet bi theo form
 * @param
 * @retval None
 */
void printEndPointCnt(u8 pTextEpc,u16 x,u16 y,uint8_t bySize, InforType_e type)
{

	char strTemp1[20];
	char strTemp2[3] = {0};
	char strTemp3[] = "Button     :";
	char strTemp4[] = "Button Zgb :";
	char strTemp5[] = "Button BLE :";
	hexToAscii(strTemp2, &pTextEpc, 1);

	memset(strTemp1,0,sizeof(strTemp1));
	switch(type)
	{
	case BLUETOOTH:
		strcpy(strTemp1,strTemp5);
		break;
	case ZIGBEE:
		strcpy(strTemp1,strTemp4);
		break;
	case MCU:
		strcpy(strTemp1,strTemp3);
		break;
	default:
		break;
	}


	uint8_t j = 12;
	uint8_t i = 0;
	for(i = 0; i<strlen(strTemp2); i++)
	{
			strTemp1[j] = strTemp2[i];
			j++;
	}
	Show_Str(x,y,BLACK,WHITE,(u8*)strTemp1,bySize,1);
}
/**
 * @func   printVersion
 * @brief  In ra man hinh Version cua thiet bi theo form
 * @param
 * @retval None
 */
void printVersion(char *pTextMAC,u16 x,u16 y,uint8_t bySize,InforType_e type)
{

	char strTemp1[26];

	char strTemp2[] = ".";

	char strTemp3[] = "Ver ZigBee :";

	char strTemp4[] = "Ver BLE    :";

	char strTemp5[] = "Ver MCU    :";

	//Show_Str(x+20,y,BLACK,WHITE,(u8*)strTemp3,bySize,1);

	memset(strTemp1,0,sizeof(strTemp1));

	switch(type)
	{
	case BLUETOOTH:
		strcpy(strTemp1,strTemp4);
		break;
	case ZIGBEE:
		strcpy(strTemp1,strTemp3);
		break;
	case MCU:
		strcpy(strTemp1,strTemp5);
		break;
	default:
		break;
	}


	uint8_t j = 12;
	uint8_t i = 0;
	for(i = 0; i<strlen(pTextMAC); i++)
	{
		if((i%2 == 0)&&(i !=0))
		{
			j++;
			strcat(strTemp1,strTemp2);
		}
			strTemp1[j] = pTextMAC[i];
			j++;
	}
	Show_Str(x,y,BLACK,WHITE,(u8*)strTemp1,bySize,1);
}
/**
 * @func   printModelId
 * @brief  In ra man hinh ModelId cua thiet bi theo form
 * @param
 * @retval None
 */
void printModelId(char *pText,u16 x,u16 y,uint8_t bySize)
{

	char strTemp1[30];

	char strTemp2[] = "Model ID   :";

	memset(strTemp1,0,sizeof(strTemp1));

	strcpy(strTemp1,strTemp2);

	uint8_t j = 12;
	uint8_t i = 0;
	for(i = 0; i<strlen(pText); i++)
	{
			strTemp1[j] = pText[i];
			j++;
	}
	Show_Str(x,y,BLACK,WHITE,(u8*)strTemp1,bySize,1);
}
/**
 * @func   printProductID
 * @brief  In ra man hinh ProductID cua thiet bi theo form
 * @param
 * @retval None
 */
void printProductID(char *pText,u16 x,u16 y,uint8_t bySize)
{

	char strTemp1[30];

	char strTemp2[] = "Product ID :";

	memset(strTemp1,0,sizeof(strTemp1));

	strcpy(strTemp1,strTemp2);

	uint8_t j = 12;
	uint8_t i = 0;
	for(i = 0; i<strlen(pText); i++)
	{
			strTemp1[j] = pText[i];
			j++;
	}
	Show_Str(x,y,BLACK,WHITE,(u8*)strTemp1,bySize,1);
}
/**
 * @func   printTypeMCU
 * @brief  In ra man hinh TypeMCU cua thiet bi theo form
 * @param
 * @retval None
 */
void printTypeMCU(u8 pText,u16 x,u16 y,uint8_t bySize)
{

	char strTemp1[30];
	char strTemp2[] = "Type MCU   :";
	char strTemp3[3];
	hexToAscii(strTemp3, &pText, 1);

	memset(strTemp1,0,sizeof(strTemp1));

	strcpy(strTemp1,strTemp2);

	uint8_t j = 12;
	uint8_t i = 0;
	for(i = 0; i<strlen(strTemp3); i++)
	{
			strTemp1[j] = strTemp3[i];
			j++;
	}

	Show_Str(x,y,BLACK,WHITE,(u8*)strTemp1,bySize,1);
}
void getModelID(char * pOutPut,u8 *pInPut)
{
	memset(pOutPut,0,20);
	u8 j = 1;
	for(u8 i = 0;i < pInPut[0];i++,j++)
	{
		pOutPut[i] = pInPut[j];
	}
}
