/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/*!
*
* @file        uart_drv.c
*
* @author    trongkn
*
* @version   1.0
*
* @date
*
* @brief
*
*******************************************************************************
*
* Detailed Description of the file. If not used, remove the separator above.
*
******************************************************************************/

#include <uart.h>
#include "tools.h"
#include "gpio.h"

/******************************************************************************
* External objects
******************************************************************************/

///* Global variable for changing CMT0 interval */
//volatile uint16_t interval_level = 1;
///* String used to print message at PC terminal */
//static char print_str[250];
///* Flag used to detect whether data is received from PC terminal */
//extern volatile uint8_t g_rx_flag;
///* Global variable used for storing data received from PC terminal */
//extern volatile uint8_t g_rx_char;
///* Sends SCI2 data and waits for transmit end flag. */

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
typedef ERR_E (*PACKET_FUNC_PTR)(uint8_t *input, uint8_t *out);

/**
 * @brief Use brief, otherwise the index won't have a brief explanation.
 *
 * Detailed explanation.
 */
typedef struct parse_cmd_process_tag
{
    uint8_t msg_id;
    const uint8_t packet_str[COMMAND_MAX_LEN];
    PACKET_FUNC_PTR packet_func_ptr;
} PARSE_PACKET_PROCESS_T;

/******************************************************************************
* Local function prototypes
******************************************************************************/

LOCAL ERR_E UART_HandleCalibTds(uint8_t *value, uint8_t *out);

LOCAL ERR_E UART_HandleSoftVer(uint8_t *value, uint8_t *out);

LOCAL ERR_E UART_HandleDbg(uint8_t *value, uint8_t *out);

LOCAL ERR_E UART_HandleTdsParam(uint8_t *value, uint8_t *out);

LOCAL ERR_E UART_HandleGetAdcTable(uint8_t *value, uint8_t *out);

LOCAL ERR_E UART_HandleGetTdsTable(uint8 *value, uint8 *out);

LOCAL ERR_E UART_HandleReset(uint8 *value, uint8 *out);

LOCAL ERR_E UART_HandleIoTestEn(uint8 *value, uint8 *out);

LOCAL ERR_E UART_HandleOutSet(uint8 *value, uint8 *out);

LOCAL ERR_E UART_HandleInCheck(uint8 *value, uint8 *out);

LOCAL ERR_E UART_HandleDisableTdsIn(uint8 *value, uint8 *out);

LOCAL ERR_E UART_HandleSentCalibADC(uint8 *value, uint8 *out);

LOCAL ERR_E UART_HandleSentCalibTDS(uint8 *value, uint8 *out);

LOCAL ERR_E UART_HandleSetWaitTimeUpdateTds(uint8 *value, uint8 *out);

/**
* @brief One line documentation
*
* A more detailed documentation
*
* @param arg1 the first function argument
* @param arg2 the second function argument
*
* @return descrition for the function return value
*/

/******************************************************************************
* Local variables
******************************************************************************/

/* String used to print message at PC terminal */
static char print_str[UART_SEND_MAX_LEN];

/* Global variable used for storing data received from PC terminal */
extern volatile uint8_t g_rx_char;
extern bool g_disableTdsIn;

PUBLIC bool g_ioTestEn = FALSE;
LOCAL bool s_dbg_en = FALSE;

LOCAL const PARSE_PACKET_PROCESS_T packet_process_table[] =
    {

        {1, "CALIB_TDS", UART_HandleCalibTds},
        {2, "SOFTWAREVER", UART_HandleSoftVer},
        {3, "DEBUG_EN", UART_HandleDbg},
        {4, "TDS_PARAM", UART_HandleTdsParam},
        {5, "RESET", UART_HandleReset},
        {6, "ADC_TABLE", UART_HandleGetAdcTable},
        {7, "TDS_TABLE", UART_HandleGetTdsTable},
        {8, "IOTEST_EN", UART_HandleIoTestEn},
        {9, "OUT_SET", UART_HandleOutSet},
        {10, "IN_CHECK", UART_HandleInCheck},
        {11, "DISABLE_TDS_IN", UART_HandleDisableTdsIn},
        {12, "SENT_ADC", UART_HandleSentCalibADC},
        {13, "SENT_TDS", UART_HandleSentCalibTDS},
        {14, "SET_TIME_UTDS", UART_HandleSetWaitTimeUpdateTds}

};

/******************************************************************************
* Local functions
******************************************************************************/
void LPUART_1_SERIAL_RX_TX_IRQHANDLER(void)
{
    /* If new data arrived. */
    if ((kLPUART_RxDataRegFullFlag)&LPUART_GetStatusFlags(LPUART0))
    {

    }

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

LOCAL void UART_Reply(uint8_t *data, uint8_t *msg_id, ERR_E err_code)
{
    uint8_t buffer_size;
    buffer_size = sprintf(print_str, "[%s,%d,%s]", msg_id, err_code, data);
    for (uint8_t index; index < buffer_size; index++)
    {
        LPUART_WriteByte(LPUART0, print_str[index]);
        while (!(kLPUART_TxDataRegEmptyFlag & LPUART_GetStatusFlags(LPUART0)))
            ;
    }
}

LOCAL ERR_E UART_HandleSoftVer(uint8 *value, uint8 *out)
{

    strcpy((char *)out, (char *)VERSION_SOFTWARE);
    return OK;
}

LOCAL ERR_E UART_HandleDbg(uint8 *value, uint8 *out)
{

    uint8_t param[9] = {0};
    int param_int = 0;
    if (strlen((char *)value) > 8)
    {

        return ERR_PARAM;
    }

    strcpy((char *)param, (char *)value);
    param_int = atoi((char *)param);
    s_dbg_en = (param_int == 1) ? 1 : 0;
    return OK;
}

LOCAL ERR_E UART_HandleIoTestEn(uint8 *value, uint8 *out)
{
    uint8_t param[9] = {0};
    int param_int = 0;
    if (strlen((char *)value) > 8)
    {

        return ERR_PARAM;
    }

    strcpy((char *)param, (char *)value);
    param_int = atoi((char *)param);
    g_ioTestEn = (param_int == 1) ? 1 : 0;
    return OK;
}

LOCAL ERR_E UART_HandleTdsParam(uint8 *value, uint8 *out)
{

    uint8_t param[9] = {0};
    int param_int = 0;
    TDS_E channel_tds = TDS_IN_VALUE;
    if (strlen((char *)value) > 8)
    {

        return ERR_PARAM;
    }
    //in 0: out 1

    strcpy((char *)param, (char *)value);
    param_int = atoi((char *)param);
    channel_tds = (param_int == 0) ? TDS_IN_VALUE : TDS_OUT_VALUE;
    ADC_GetCalibTdsParam(channel_tds, out);
    return OK;
}

LOCAL ERR_E UART_HandleGetAdcTable(uint8 *value, uint8 *out)
{
    uint8_t param[9] = {0};
    int param_int = 0;
    TDS_E channel_tds = TDS_IN_VALUE;
    if (strlen((char *)value) > 8)
    {
        return ERR_PARAM;
    }
    //in 0: out 1

    strcpy((char *)param, (char *)value);
    param_int = atoi((char *)param);
    channel_tds = (param_int == 0) ? TDS_IN_VALUE : TDS_OUT_VALUE;
    ADC_GetAdcTable(channel_tds, out);
    return OK;
}
LOCAL ERR_E UART_HandleGetTdsTable(uint8 *value, uint8 *out)
{
    uint8_t param[9] = {0};
    int param_int = 0;
    TDS_E channel_tds = TDS_IN_VALUE;
    if (strlen((char *)value) > 8)
    {
        return ERR_PARAM;
    }
    //in 0: out 1

    strcpy((char *)param, (char *)value);
    param_int = atoi((char *)param);
    channel_tds = (param_int == 0) ? TDS_IN_VALUE : TDS_OUT_VALUE;
    ADC_GetTdsTable(channel_tds, out);
    return OK;
}

/**
* @brief One line documentation
*
* A more detailed documentation
*
* @param arg1 the first function argument
* @param arg2 the second function argument
*
* @return descrition for the function return value
*/
LOCAL ERR_E UART_HandleCalibTds(uint8 *value, uint8 *out)
{
    uint8 data[20] = {0};
    //parser data in uart then handle
    ERR_E ret = OK;
    uint8_t *msg_field[MSG_FIELDS_MAX];

    uint8_t index = 0;
    uint8_t *msg_token;
    TDS_E channel_tds = TDS_IN_VALUE;

    // generic indexers
    int i;
    int tds = 0;
    //	len  = MIN(19,strlen_s((char*)value));
    strcpy(data, value);
    memset(msg_field, 0x00, sizeof(msg_field));
    int channel;
    i = 0;
    msg_token = data;
    msg_field[i] = msg_token;

    while (*msg_token != '\0')
    {

        if (*msg_token == ':')
        {

            // terminate string after field separator or end-of-message characters
            *msg_token = '\0';
            // save position of the next token
            msg_field[++i] = msg_token + 1;
        }
        msg_token++;
    }
    if (i < 2)
        return ERR;
    //0:1:2 mean chanel_in:index1:tds2
    channel = atoi((char *)msg_field[0]);
    channel_tds = (channel == 0) ? TDS_IN_VALUE : TDS_OUT_VALUE;
    index = atoi((char *)msg_field[1]);
    tds = atoi((char *)msg_field[2]);
    return ADC_CalibTdsValueFromUart(tds, channel_tds, index);
    //	return OK;
}

LOCAL ERR_E UART_HandleReset(uint8 *value, uint8 *out)
{
    mySoftwareReset();
    return OK;
}

LOCAL ERR_E UART_HandleOutSet(uint8 *value, uint8 *out)
{
    uint8 data[20] = {0};
    //parser data in uart then handle
    ERR_E ret = OK;
    uint8_t *msg_field[MSG_FIELDS_MAX];

    uint8_t index = 0;
    uint8_t *msg_token;
    TDS_E channel_tds = TDS_IN_VALUE;

    // generic indexers
    int i;
    int tds = 0;
    //	len  = MIN(19,strlen_s((char*)value));
    strcpy(data, value);
    memset(msg_field, 0x00, sizeof(msg_field));
    int channel;
    i = 0;
    msg_token = data;
    msg_field[i] = msg_token;

    while (*msg_token != '\0')
    {

        if (*msg_token == ':')
        {

            // terminate string after field separator or end-of-message characters
            *msg_token = '\0';
            // save position of the next token
            msg_field[++i] = msg_token + 1;
        }
        msg_token++;
    }
    if (i < 1)
        return ERR;
    int output = atoi((char *)msg_field[0]);
    int lever = atoi((char *)msg_field[1]);
    switch (output)
    {
    case 2:
        if (lever)
            TURN_ON_PUMP;
        else
            TURN_OFF_PUMP;
        break;
    case 1:
        if (lever)
            TURN_ON_VAN_XA;
        else
            TURN_OFF_VAN_XA;
        break;
    default:
        break;
    }
    return OK;
}

LOCAL ERR_E UART_HandleInCheck(uint8 *value, uint8 *out)
{
    uint8 data[20] = {0};
    //parser data in uart then handle

    uint8_t *msg_field[MSG_FIELDS_MAX];

    uint8_t index = 0;
    uint8_t *msg_token;

    // generic indexers
    int i;
    int tds = 0;
    //	len  = MIN(19,strlen_s((char*)value));
    strcpy(data, value);
    memset(msg_field, 0x00, sizeof(msg_field));
    int channel;
    i = 0;
    msg_token = data;
    msg_field[i] = msg_token;

    while (*msg_token != '\0')
    {
        if (*msg_token == ':')
        {
            // terminate string after field separator or end-of-message characters
            *msg_token = '\0';
            // save position of the next token
            msg_field[++i] = msg_token + 1;
        }
        msg_token++;
    }
    if (i < 1)
        return ERR;
    int input = atoi((char *)msg_field[0]);
    int lever = atoi((char *)msg_field[1]);

    ERR_E ret = OK;
    switch (input)
    {
    case 1:                               // ap thap
        if (lever && (!CHECK_CO_AP_THAP)) // co ap thap
        {
            ret = INPUT_FALSE;
        }
        else if (!lever && CHECK_CO_AP_THAP) // ko co ap thap
        {
            ret = INPUT_FALSE;
        }
        break;
    case 2:                              // ap cap
        if (lever && (!CHECK_CO_AP_CAO)) // co ap
        {
            ret = INPUT_FALSE;
        }
        else if (!lever && CHECK_CO_AP_CAO) // ko co ap
        {
            ret = INPUT_FALSE;
        }
        break;
    case 3:                                 // do 1
        if (lever && !ADC_GetH2oDet(H2O_1)) // co do
            ret = INPUT_FALSE;
        else if (!lever && ADC_GetH2oDet(H2O_1)) // ko do
        {
            ret = INPUT_FALSE;
        }
        break;
    case 4:                                 // do 1
        if (lever && !ADC_GetH2oDet(H2O_2)) // co do
            ret = INPUT_FALSE;
        else if (!lever && ADC_GetH2oDet(H2O_2)) // ko do
        {
            ret = INPUT_FALSE;
        }
        break;
    default:
        ret = NOT_SUPPORT;
        break;
    }
    return ret;
}
LOCAL void UART_HandleProcess(uint8 *data, uint16_t len)
{

    uint8_t *msg_field[MSG_FIELDS_MAX];
    ERR_E ret = NOT_SUPPORT;
    // pointer used in NMEA message tokenizer
    uint8_t *msg_token;
    uint8_t data_reply[UART_SEND_MAX_LEN] = {0};
    // generic indexers
    int i, j;
    if (*data != MSG_FIELD_SOF)
        goto end_handle;
    memset(msg_field, 0x00, sizeof(msg_field));

    i = 0;
    msg_token = data + 1;
    msg_field[0] = msg_token;

    while (*msg_token != '\0')
    {

        if ((*msg_token == MSG_FIELD_SEPARATOR) && (msg_field[1] == 0x00))
        {

            // terminate string after field separator or end-of-message characters
            *msg_token = '\0';

            // save position of the next token
            msg_field[1] = msg_token + 1;
        }
        if (*msg_token == MSG_FIELD_EOF)
        {
            *msg_token = '\0';
        }

        msg_token++;
    }

    for (j = 0; j < ARR_SIZE(packet_process_table); j++)
    {

        if (strcmp((char *)packet_process_table[j].packet_str, (char *)msg_field[MSG_FIELD_TYPE]) == 0)
        {
            ret = packet_process_table[j].packet_func_ptr((uint8_t *)msg_field[MSG_FIELD_DATA], (uint8_t *)data_reply);
            break;
        }
    }

end_handle:
    UART_Reply(data_reply, (uint8_t *)packet_process_table[j].packet_str, ret);
    return;
}

LOCAL ERR_E UART_HandleDisableTdsIn(uint8 *value, uint8 *out)
{
    uint8_t param[9] = {0};
    int param_int = 0;
    if (strlen((char *)value) > 8)
    {

        return ERR_PARAM;
    }

    strcpy((char *)param, (char *)value);
    param_int = atoi((char *)param);
    g_disableTdsIn = (param_int == 1) ? 1 : 0;
    return OK;
}

LOCAL ERR_E UART_HandleSentCalibADC(uint8 *value, uint8 *out)
{
    char data[150];
    //		//parser data in uart then handle
    ERR_E ret = OK;
    uint8_t *msg_field[(CALIB_POINT_MAX)];

    uint8_t index = 0;
    uint8_t *msg_token;
    TDS_E channel_tds = TDS_IN_VALUE;
    //
    //		// generic indexers
    int i;
    int tds = 0;
    //	len  = MIN(19,strlen_s((char*)value));
    int len = strlen((char *)value);
    strcpy(data, value);
    //		strcpy(data,(char*)value);
    memset(msg_field, 0x00, sizeof(msg_field));
    int channel;
    i = 0;
    msg_token = data;
    msg_field[i] = msg_token;

    while (*msg_token != '\0')
    {

        if (*msg_token == ',')
        {

            // terminate string after field separator or end-of-message characters
            *msg_token = '\0';
            // save position of the next token
            msg_field[++i] = msg_token + 1;
        }
        msg_token++;
    }
    if (i < 2)
        return ERR;
    //0:1:2 mean chanel_in:index1:tds2
    channel = atoi((char *)msg_field[0]);
    channel_tds = (channel == 0) ? TDS_IN_VALUE : TDS_OUT_VALUE;

    int j = 0;

    static int adcTable[CALIB_POINT_MAX];
    for (j = 1; j <= i; j++)
    {
        adcTable[j - 1] = atoi((char *)msg_field[j]);
    }

    return ADC_WriteAdcFromUart(adcTable, channel_tds);
}

LOCAL ERR_E UART_HandleSentCalib(uint8 *value, uint8 *out)
{
    static char data[300];
    //		//parser data in uart then handle
    ERR_E ret = OK;
    uint8_t *msg_field[(CALIB_POINT_MAX)];

    uint8_t index = 0;
    uint8_t *msg_token;
    TDS_E channel_tds = TDS_IN_VALUE;
    //
    //		// generic indexers
    int i;
    int tds = 0;
    //	len  = MIN(19,strlen_s((char*)value));
    int len = strlen((char *)value);
    memcpy(data, value, len + 1);
    //		strcpy(data,(char*)value);
    memset(msg_field, 0x00, sizeof(msg_field));
    int channel;
    i = 0;
    msg_token = data;
    msg_field[i] = msg_token;

    while (*msg_token != '\0')
    {

        if (*msg_token == ',')
        {

            // terminate string after field separator or end-of-message characters
            *msg_token = '\0';
            // save position of the next token
            msg_field[++i] = msg_token + 1;
        }
        msg_token++;
    }
    if (i < 2)
        return ERR;
    //0:1:2 mean chanel_in:index1:tds2
    channel = atoi((char *)msg_field[0]);
    channel_tds = (channel == 0) ? TDS_IN_VALUE : TDS_OUT_VALUE;

    //	uint8 data[150] = {0};
    //parser data in uart then handle
    //		ERR_E ret = OK;
    //		uint8_t	*msg_fieldAdc[25];
    //
    //		uint8_t index = 0;
    //		uint8_t	*msg_token;
    //		TDS_E channel_tds = TDS_IN_VALUE;

    //	len  = MIN(19,strlen_s((char*)value));
    //		strcpy(data,value);
    //		memset( msg_fieldAdc, 0x00, sizeof(msg_fieldAdc));
    //
    //		int channel;
    int j = 0;
    //		msg_token		= data;

    //		while (*msg_token != '\0')
    //		{
    //			if ( *msg_token == ',' )
    //			{
    //				// terminate string after field separator or end-of-message characters
    //				*msg_token = '\0';
    //				// save position of the next token
    //				msg_fieldAdc[i++] = msg_token + 1;
    //			}
    //			msg_token++;
    //		}
    //		if (i<2) return  ERR;
    static int adcTable[CALIB_POINT_MAX];
    for (j = 1; j <= i; j++)
    {
        adcTable[j - 1] = atoi((char *)msg_field[j]);
    }

    //0:1:2 mean chanel_in:index1:tds2
    //		channel = atoi((char*)data);
    //		channel_tds =  (channel == 0)?TDS_IN_VALUE:TDS_OUT_VALUE;

    return ADC_WriteAdcFromUart(adcTable, TDS_OUT_VALUE);
}

LOCAL ERR_E UART_HandleSentCalibTDS(uint8 *value, uint8 *out)
{
    static char data[150];
    //		//parser data in uart then handle
    ERR_E ret = OK;
    uint8_t *msg_field[(CALIB_POINT_MAX)];

    uint8_t index = 0;
    uint8_t *msg_token;
    TDS_E channel_tds = TDS_IN_VALUE;
    //
    //		// generic indexers
    int i;
    int tds = 0;
    //	len  = MIN(19,strlen_s((char*)value));
    int len = strlen((char *)value);
    memcpy(data, value, len + 1);
    //		strcpy(data,(char*)value);
    memset(msg_field, 0x00, sizeof(msg_field));
    int channel;
    i = 0;
    msg_token = data;
    msg_field[i] = msg_token;

    while (*msg_token != '\0')
    {

        if (*msg_token == ',')
        {

            // terminate string after field separator or end-of-message characters
            *msg_token = '\0';
            // save position of the next token
            msg_field[++i] = msg_token + 1;
        }
        msg_token++;
    }
    if (i < 2)
        return ERR;
    //0:1:2 mean chanel_in:index1:tds2
    channel = atoi((char *)msg_field[0]);
    channel_tds = (channel == 0) ? TDS_IN_VALUE : TDS_OUT_VALUE;

    int j = 0;

    static int tdsTable[CALIB_POINT_MAX];
    for (j = 1; j <= i; j++)
    {
        tdsTable[j - 1] = atoi((char *)msg_field[j]);
    }

    return ADC_WriteTdsFromUart(tdsTable, channel_tds);
}

LOCAL ERR_E UART_HandleSetWaitTimeUpdateTds(uint8 *value, uint8 *out)
{
    uint8 data[20] = {0};
    //parser data in uart then handle
    ERR_E ret = OK;
    uint8_t *msg_field[MSG_FIELDS_MAX];

    uint8_t index = 0;
    uint8_t *msg_token;
    TDS_E channel_tds = TDS_IN_VALUE;

    strcpy(data, value);
    memset(msg_field, 0x00, sizeof(msg_field));
    int channel;
    msg_token = data;
    msg_field[0] = msg_token;
    //0:1:2 mean chanel_in:index1:tds2
    int second = atoi((char *)msg_field[0]);
    if ((second < 0) || (second > 3600))
        return ERR;
    UserConfig_setWaitTimeUpdateTds(second * 1000);

    return OK;
}

/******************************************************************************
* Global functions
******************************************************************************/

/**
 * @brief One line documentation
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */
PUBLIC void UART_Init(void)
{
    //init UART here
    const lpuart_config_t uart_config = {
        .baudRate_Bps = 9600,
        .parityMode = kLPUART_ParityDisabled,
        .dataBitsCount = kLPUART_EightDataBits,
        .isMsb = false,
        .stopBitCount = kLPUART_OneStopBit,
        .txFifoWatermark = 0,
        .rxFifoWatermark = 1,
        .enableRxRTS = false,
        .enableTxCTS = false,
        .txCtsSource = kLPUART_CtsSourcePin,
        .txCtsConfig = kLPUART_CtsSampleAtStart,
        .rxIdleType = kLPUART_IdleTypeStartBit,
        .rxIdleConfig = kLPUART_IdleCharacter1,
        .enableTx = true,
        .enableRx = true
    };
    LPUART_Init(LPUART0, &uart_config, 24000000UL);
    sprintf(print_str, "UART init done\r\n");
    LPUART_WriteBlocking(LPUART0, (uint8_t *)print_str, (uint16_t)strlen(print_str) - 1);
    LPUART_EnableInterrupts(LPUART0, kLPUART_RxDataRegFullInterruptEnable);
    EnableIRQ(LPUART0_IRQn);
}

/**
 * @brief One line documentation
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */
PUBLIC void UART_UartPuts(uint8_t *s)
{
    uint8_t buffer_size;
    buffer_size = sprintf(print_str, "%s", s);

    for (uint8_t index; index < buffer_size; index++)
    {
        LPUART_WriteByte(LPUART0, print_str[index]);
        while (!(kLPUART_TxDataRegEmptyFlag & LPUART_GetStatusFlags(LPUART0)))
            ;
    }
}
/**
 * @brief One line documentation
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */
PUBLIC void UART_Debug(uint8_t *data)
{
    //	#ifdef DEBUG_ENABLE
    if (s_dbg_en)
    {
        UART_UartPuts(data);
    }
    //	#endif
}

PUBLIC void UART_Process()
{
    uint8_t data[300] = {0};
    uint16_t len = 0;
    if (UART_IsDoneFrame())
    {
        len = UART_ReadData(data, 300);
        UART_HandleProcess(data, len);
    }
}
