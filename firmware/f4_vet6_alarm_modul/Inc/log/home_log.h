#ifndef PROCESS_ERRORS_H_
#define PROCESS_ERRORS_H_

#include "stm32f4xx_hal.h"
//#include "usart.h"

#define NO_ERRORS 0
#define IR_QUEUE_FULL 1
#define IR_QUEUE_CANNOT_BE_CREATED 2
#define IR_NOT_RECOGNISED_KEY_PRESSED 3
#define IR_READ_ERROR 4
#define CAN_INIT_FAILED 5
#define BME280_READ_ERROR 6
#define BME280_INIT_ERROR 7
#define DS18B20_READ_ERROR 8
#define DS18B20_WRITE_ERROR 9
#define DS18B20_RESET_ERROR 10
#define DS18B20_READ_TEMP_ERROR 11
#define CAN_QUEUE_CANNOT_BE_CREATED 13
#define SPI_QUEUE_CANNOT_BE_CREATED 14
#define SWITCH_QUEUE_CANNOT_BE_CREATED 15
#define CAN_QUEUE_FULL 16
#define SWITCH_QUEUE_FULL 17
#define SWITCH_INIT_FAILED 18
#define RELAY_SEND_FAILED 19
#define CONFIG_QUEUE_CANNOT_BE_CREATED 20
#define CAN_FAILED_RX 21


#define MODULE_IR_RECIEVER 500
#define MODULE_BME_280 501
#define MODULE_DS18B20 502
#define MODULE_CAN 503
#define MODULE_SPI 504
#define MODULE_SWITCH 505
#define MODULE_RELAY 506
#define MODULE_TIME 507
#define MODULE_CONFIG 508

extern uint8_t home_debug_mode;

void home_warning(uint16_t id, uint16_t errorCode);
void home_error(uint16_t id, uint16_t errorCode);
void home_log(uint16_t id, char message[], uint8_t addEndLine);
void home_log_int(uint16_t id, uint32_t message, uint8_t addEndLine);
void home_log_byte_hex(uint16_t id, uint8_t message, uint8_t addEndLine);

void writeOut(char * str);
void writeOutInt(uint32_t message);
void writeOutByteInHex(uint8_t hex);

void sendChar(char ch);

void prntnum(uint32_t num, int base, char *outbuf, uint8_t size);

#endif
