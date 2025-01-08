/**
 * \file        bgf.c
 * \author      Melvyn MUNOZ
 * \date        2025-01-06
 * \brief       This is the code to encode the message that BCGV send do BGF.
 */

//on va faire une fonction general pour nimporte quelle message bgf car ils ont tous la meme forme
// seul l'indice du message change

#include "bit_utils.h"
#include "../lib/bcgv_api/include/bcgv_api.h"

//a enlever
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

//en gros pour les id des message faire id_message & 0xFF pour avoir sous forme de binaire le nombre
// puis on mets à la suite soit le (flag_? & 0xFF)

// et on a la fonction pour encoder si on arrive à avoir les 2 octets à la suite
// donc voir si on fait une variable qui contient les 2 octets ou si il faut juste faire un message qui contient 2 octets

//met en parametre l'id et la valeur du flag
//on fait une verife sur la valeur de l'ID (entre 1 et 5)

//return le message à envoyer

uint16_t bcgv_bgf_msg_encode(int id_message, bool flag_value){
	uint16_t message_encoded = 0;
	message_encoded = id_message & 0xFF;
	printf("1) message = %u ou = %x\n", message_encoded, message_encoded);
	message_encoded = (message_encoded << 8);
	printf("2) message = %u ou = %X\n", message_encoded, message_encoded);

	message_encoded += flag_value;

	return message_encoded;
}

int bgf_bcgv_get_id(uint16_t message_received){
	return (message_received >> 8);
}

void bgf_check_msg_received(uint16_t message_send, uint16_t message_received){
	if (message_send == message_received){
		//set le bit a 1 car le retour du bgf est correct et la commande est bien reçu
		bit_flag_t bit_flag = 1;
		//set le bit à 1 au bon emplacement en fonction de l'id du message
		bit_flag = bit_flag << (bgf_bcgv_get_id(message_received) - 1);
		bit_flag_t bit_flag_actual = get_bit_flag_bgf_ack();
		if((message_received & 0x01) == 1){
			printf("bit_flag = %u", bit_flag);
			set_bit_flag_bgf_ack(bit_flag_actual | bit_flag);
		}else{
			printf("bit_flag = %u", bit_flag);
			set_bit_flag_bgf_ack(CLEAR_BIT(bit_flag_actual, bit_flag));
		}
	}
}

int main(int argc, char const *argv[])
{
	//test de la fonction d'encodage

	int id_message = 2;
	bool flag_value = true;

	uint16_t message_send = bcgv_bgf_msg_encode(id_message, flag_value);
	printf("fin) message = %u ou = %X\n", message_send, message_send);

	//pour ensuite set le bit il faut que le message retourner par bgf soit egal au message envoyé
	// si c'est egal on set_bit_flag_bgf_ack => si impaire flag = 1 
	// et on le decale le 1 vers la gauche de l'id_message-1 sinon flag = 0 et on set à 0 le bit 
	uint16_t message_received = 0;
	
	bgf_check_msg_received(message_send, message_received);

	printf("bit set : %u", get_bit_flag_bgf_ack());

	return 0;
}
