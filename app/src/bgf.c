/**
 * \file        bgf.c
 * \author      Melvyn MUNOZ
 * \date        2025-01-06
 * \brief       This is the code to encode the message that BCGV send do BGF
 * 				and check the message return by bgf to set the ack for finite state machine.
 */

#include <stdbool.h>
#include <stdio.h>

#include "drv_api.h"
#include "bcgv_api.h"
#include "bit_utils.h"
#include "bgf.h"
#include "log.h"

#define SERIAL_NUMBER_BGF (10) // numero de serie 11 - 1
#define SIZE_MSG_TO_WRITE (5)

// Saved message send by bcgv to bgf during the last frame
static uint8_t bgf_msg_saved[5][2];

static flag_t flag_position_light_saved = false;
static flag_t flag_crossing_light_saved = false;
static flag_t flag_highbeam_light_saved = false;
static flag_t flag_indic_right_saved = false;
static flag_t flag_indic_left_saved = false;

/**
 * \brief check if the message received is the same at the message sended
 *
 * \param id : message ID
 * \param flag : flag value of the equipement
 * \return boolean
 */
bool bgf_check_msg_received(uint8_t id, uint8_t flag)
{
	if (bgf_msg_saved[id][0] == id && bgf_msg_saved[id][1] == flag)
	{
		return true;
	}
	return false;
}

/**
 * \brief set modifications in the ack bit
 *
 * \param id : message ID
 * \param flag : flag value of the equipement
 */
void bgf_set_bit_ack(uint8_t id, uint8_t flag)
{
	bit_flag_t bit_flag = 1;
	bit_flag_t bit_flag_actual = get_bit_flag_bgf_ack();
	bit_flag = bit_flag << (id - 1);
	if ((flag & 0x01) == 1)
	{
		set_bit_flag_bgf_ack(bit_flag_actual | bit_flag);
	}
	else
	{
		set_bit_flag_bgf_ack(CLEAR_BIT(bit_flag_actual, bit_flag));
	}
}

/**
 * \brief read the serial data of the driver, set the bit ack if there are differents values
 *
 * \param drvFd : driver fd value
 * \return boolean
 */
bool bgf_read_serial_message(int32_t drvFd)
{
	serial_frame_t serialData[DRV_MAX_FRAMES];
	uint32_t serialDataLen = 0;
	int32_t ret = drv_read_ser(drvFd, serialData, &serialDataLen);

	if (ret == DRV_ERROR)
	{
		log_error("Error driver : serial reading problem", NULL);
		return false;
	}

	if (serialDataLen > 0 && serialData[SERIAL_NUMBER_BGF].frameSize == SER_MAX_FRAME_SIZE)
	{
		uint8_t id_msg = serialData[SERIAL_NUMBER_BGF].frame[0];
		uint8_t flag_msg = serialData[SERIAL_NUMBER_BGF].frame[1];
		log_info("Id message : %d", id_msg);
		log_info("Flag message : %d", flag_msg);
		bool check_ack = bgf_check_msg_received(id_msg, flag_msg);
		if (check_ack == true)
		{
			bgf_set_bit_ack(id_msg, flag_msg);
			log_info("Bit set", NULL);
		}
	}
	return true;
}

/**
 * \brief set all data of one message bcgv to bgf
 *
 * \param serialData_write : serial data to write
 * \param i
 * \param id
 * \param flag_new
 */
void bgf_set_one_data(serial_frame_t *serialData_write, uint8_t id, uint8_t flag_new)
{
	serialData_write[11].serNum = 11;
	serialData_write[11].frameSize = 2;
	serialData_write[11].frame[0] = id;
	serialData_write[11].frame[1] = flag_new;
}

/**
 * \brief
 *
 * \param drvFd
 * \param serialdatalen
 * \param serialData_write : data to write of serial data
 * \param id : message id
 * \param flag_new : new flag to set
 * \return boolean
 */
bool bgf_write_one_data(int32_t drvFd, uint32_t serialdatalen, serial_frame_t *serialData_write, uint8_t id, uint8_t flag_new)
{
	int32_t ret = 0;
	bgf_set_one_data(serialData_write, id, flag_new);
	ret = drv_write_ser(drvFd, serialData_write, serialdatalen);
	if (ret == DRV_ERROR)
	{
		log_error("Error during the writing of bcgv message to bgf", NULL);
		return false;
	}
	return true;
}

/**
 * \brief
 *
 * \param drvFd
 * \return boolean
 */
bool bgf_write_serial_message(int32_t drvFd)
{
	// initalize the serialData
	serial_frame_t serialData_write[DRV_MAX_FRAMES];
	uint32_t serialdatalen = 1;
	bool success = 0;

	flag_t flag_position_light_new = get_flag_position_light();
	flag_t flag_crossing_light_new = get_flag_crossing_light();
	flag_t flag_highbeam_light_new = get_flag_highbeam_light();
	flag_t flag_indic_right_new = get_flag_indic_right();
	flag_t flag_indic_left_new = get_flag_indic_left();

	// Check flags, if they are different, we write in serialData_write
	if (flag_position_light_new != flag_position_light_saved)
	{
		success = bgf_write_one_data(drvFd, serialdatalen, serialData_write, 1, flag_position_light_new);
		if (success == false)
		{
			return false;
		}
	}

	if (flag_crossing_light_new != flag_crossing_light_saved)
	{
		success = bgf_write_one_data(drvFd, serialdatalen, serialData_write, 2, flag_crossing_light_new);
		if (success == false)
		{
			return false;
		}
	}

	if (flag_highbeam_light_new != flag_highbeam_light_saved)
	{
		success = bgf_write_one_data(drvFd, serialdatalen, serialData_write, 3, flag_highbeam_light_new);
		if (success == false)
		{
			return false;
		}
	}

	if (flag_indic_right_new != flag_indic_right_saved)
	{
		success = bgf_write_one_data(drvFd, serialdatalen, serialData_write, 4, flag_indic_right_new);
		if (success == false)
		{
			return false;
		}
	}

	if (flag_indic_left_new != flag_indic_left_saved)
	{
		success = bgf_write_one_data(drvFd, serialdatalen, serialData_write, 5, flag_indic_left_new);
		if (success == false)
		{
			return false;
		}
	}
	return true;
}
