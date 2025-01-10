/**
 * \file        bgf.h
 * \author      Melvyn MUNOZ
 * \date        2025-01-06
 * \brief       This is the code to encode the message that BCGV send do BGF.
 */

#ifndef BGF_H
#define BGF_H

#include <stdint.h>
#include "bcgv_api.h"

#define SERIAL_NUMBER_BGF (10) // numero de serie 11 - 1
#define SIZE_MSG_TO_WRITE (5)

void bgf_read_serial_message(int32_t drvFd, serial_frame_t serialData[DRV_MAX_FRAMES], uint32_t serialDataLen);

bool bgf_check_msg_received(uint8_t id, uint8_t flag);

void bgf_set_bit_ack(uint8_t id, uint8_t flag);

#endif
