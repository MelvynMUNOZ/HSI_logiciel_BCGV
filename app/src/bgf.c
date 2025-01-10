/**
 * \file        bgf.c
 * \author      Melvyn MUNOZ
 * \date        2025-01-06
 * \brief       This is the code to encode the message that BCGV send do BGF.
 */

// on va faire une fonction general pour nimporte quelle message bgf car ils ont tous la meme forme
//  seul l'indice du message change

// #include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "drv_api.h"
#include "bcgv_api.h"
#include "bit_utils.h"
#include "bgf.h"
#include "log.h"

// en gros pour les id des message faire id_message & 0xFF pour avoir sous forme de binaire le nombre
//  puis on mets à la suite soit le (flag_? & 0xFF)

// et on a la fonction pour encoder si on arrive à avoir les 2 octets à la suite
// donc voir si on fait une variable qui contient les 2 octets ou si il faut juste faire un message qui contient 2 octets

// met en parametre l'id et la valeur du flag
// on fait une verife sur la valeur de l'ID (entre 1 et 5)

// return le message à envoyer

// ##############Global variable####################
// Saved message send by bcgv to bgf during the last frame
static uint8_t bgf_msg_send[5][2];

static flag_t flag_position_light_saved = false;
static flag_t flag_crossing_light_saved = false;
static flag_t flag_highbeam_light_saved = false;
static flag_t flag_indic_right_saved = false;
static flag_t flag_indic_left_saved = false;

bool bgf_check_msg_received(uint8_t id, uint8_t flag)
{
	if (bgf_msg_send[id][0] == id && bgf_msg_send[id][1] == flag)
	{
		return true;
	}
	return false;
}

void bgf_set_bit_ack(uint8_t id, uint8_t flag)
{
	// set le bit a 1  par defaut
	bit_flag_t bit_flag = 1;
	bit_flag_t bit_flag_actual = get_bit_flag_bgf_ack();
	// set le bit à 1 au bon emplacement en fonction de l'id du message
	bit_flag = bit_flag << (id - 1);
	if ((flag & 0x01) == 1)
	{
		printf("bit_flag = %u", bit_flag);
		set_bit_flag_bgf_ack(bit_flag_actual | bit_flag);
	}
	else
	{
		printf("bit_flag = %u", bit_flag);
		set_bit_flag_bgf_ack(CLEAR_BIT(bit_flag_actual, bit_flag));
	}
}

// read le port serie jusqu'a ce qu'il y en ai plus à lire
// recupere l'id
// compare avec bgf_msg_send au niveau de l'ID
// si TRUE => on set le bit
// si False => rien
void bgf_read_serial_message(int32_t drvFd, serial_frame_t serialData[DRV_MAX_FRAMES], uint32_t serialDataLen)
{
	uint8_t i = 0;
	drv_read_ser(drvFd, serialData, &serialDataLen);
	while (serialDataLen != 0)
	{
		while (serialData[i].serNum != SERIAL_NUMBER_BGF && i < DRV_MAX_FRAMES)
		{
			i++;
		}
		if (serialData[i].frameSize == 2)
		{
			uint8_t id_msg = serialData[i].frame[0];
			uint8_t flag_msg = serialData[i].frame[1];
			bool check_ack = bgf_check_msg_received(id_msg, flag_msg);
			if (check_ack == true)
			{
				bgf_set_bit_ack(id_msg, flag_msg);
			}
		}
	}
}

void bgf_set_one_data(serial_frame_t *serialData_write, uint32_t i, uint8_t id, uint8_t flag_new)
{
	serialData_write[i].serNum = 11;
	serialData_write[i].frameSize = 2;
	serialData_write[i].frame[0] = id;
	serialData_write[i].frame[1] = flag_new;
}

bool bgf_write_serial_message(int32_t drvFd)
{
	// initalize the serialData
	serial_frame_t serialData_write[SIZE_MSG_TO_WRITE];
	uint32_t i = 0;

	flag_t flag_position_light_new = get_flag_position_light();
	flag_t flag_crossing_light_new = get_flag_crossing_light();
	flag_t flag_highbeam_light_new = get_flag_highbeam_light();
	flag_t flag_indic_right_new = get_flag_indic_right();
	flag_t flag_indic_left_new = get_flag_indic_left();

	// Check flags, if they are different, we write in serialData_write
	if (flag_position_light_new != flag_position_light_saved)
	{
		bgf_set_one_data(serialData_write, i, 1, flag_position_light_new);
		i++;
	}
	if (flag_crossing_light_new != flag_crossing_light_saved)
	{
		bgf_set_one_data(serialData_write, i, 2, flag_crossing_light_new);
		i++;
	}
	if (flag_highbeam_light_new != flag_highbeam_light_saved)
	{
		bgf_set_one_data(serialData_write, i, 3, flag_highbeam_light_new);
		i++;
	}
	if (flag_indic_right_new != flag_indic_right_saved)
	{
		bgf_set_one_data(serialData_write, i, 4, flag_indic_right_new);
		i++;
	}
	if (flag_indic_left_new != flag_indic_left_saved)
	{
		bgf_set_one_data(serialData_write, i, 5, flag_indic_left_new);
		i++;
	}

	if (i > 0)
	{
		int32_t ret = drv_write_ser(drvFd, serialData_write, i);
		if (ret == DRV_ERROR)
		{
			log_error("Error during the writing for bcgv message to bgf", NULL);
			return false;
		}
	}
	return true;
}

// uint16_t bcgv_bgf_msg_encode(int id_message, bool flag_value)
// {
// 	uint16_t message_encoded = 0;
// 	message_encoded = id_message & 0xFF;
// 	printf("1) message = %u ou = %x\n", message_encoded, message_encoded);
// 	message_encoded = (message_encoded << 8);
// 	printf("2) message = %u ou = %X\n", message_encoded, message_encoded);

// 	message_encoded += flag_value;

// 	return message_encoded;
// }

// int bgf_bcgv_get_id(uint16_t message_received)
// {
// 	return (message_received >> 8);
// }

// void bgf_check_msg_received(uint16_t message_send, uint16_t message_received)
// {
// 	if (message_send == message_received)
// 	{
// 		// set le bit a 1 car le retour du bgf est correct et la commande est bien reçu
// 		bit_flag_t bit_flag = 1;
// 		// set le bit à 1 au bon emplacement en fonction de l'id du message
// 		bit_flag = bit_flag << (bgf_bcgv_get_id(message_received) - 1);
// 		bit_flag_t bit_flag_actual = get_bit_flag_bgf_ack();
// 		if ((message_received & 0x01) == 1)
// 		{
// 			printf("bit_flag = %u", bit_flag);
// 			set_bit_flag_bgf_ack(bit_flag_actual | bit_flag);
// 		}
// 		else
// 		{
// 			printf("bit_flag = %u", bit_flag);
// 			set_bit_flag_bgf_ack(CLEAR_BIT(bit_flag_actual, bit_flag));
// 		}
// 	}
// }

// int main(int argc, char const *argv[])
// {
// 	// test de la fonction d'encodage

// 	int id_message = 2;
// 	bool flag_value = true;

// 	uint16_t message_send = bcgv_bgf_msg_encode(id_message, flag_value);
// 	printf("fin) message = %u ou = %X\n", message_send, message_send);

// 	// pour ensuite set le bit il faut que le message retourner par bgf soit egal au message envoyé
// 	//  si c'est egal on set_bit_flag_bgf_ack => si impaire flag = 1
// 	//  et on le decale le 1 vers la gauche de l'id_message-1 sinon flag = 0 et on set à 0 le bit
// 	uint16_t message_received = 0;

// 	bgf_check_msg_received(message_send, message_received);

// 	printf("bit set : %u", get_bit_flag_bgf_ack());

// 	return 0;
// }
