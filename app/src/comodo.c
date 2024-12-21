/**
 * \file comodo.c
 * \brief File to handle COMODO
 * \details It contains functions to decode COMODO serial frames.
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#include "comodo.h"
#include "bcgv_api.h"
#include <stdio.h>

/**
 * \brief Get the value of a specific bit from a byte.
 * \param byte The byte from which to extract the bit.
 * \param bit The bit position (0-7) to extract.
 * \return bool true if the bit is set, false otherwise.
 */
bool get_bit(uint8_t byte, uint8_t bit)
{
    return (byte & (1 << bit)) != 0;
}

/**
 * \brief Decode the COMODO serial frame and update application data.
 * \param frame Pointer to the frame buffer.
 * \return bool true if the frame was successfully decoded, false otherwise.
 */
bool comodo_decode(const uint8_t frame[COMODO_SERIAL_FRAME_SIZE])
{
    // Print the raw frame
    printf("COMODO Serial Frame: 0x%02X\n", frame[0]);

    // Decode each command from the frame using get_bit function
    bool cmd_warning = get_bit(frame[0], 0);
    bool cmd_feux_de_position = get_bit(frame[0], 1);
    bool cmd_feux_de_croisement = get_bit(frame[0], 2);
    bool cmd_feux_de_route = get_bit(frame[0], 3);
    bool cmd_clignotant_droit = get_bit(frame[0], 4);
    bool cmd_clignotant_gauche = get_bit(frame[0], 5);
    bool cmd_essuie_glaces = get_bit(frame[0], 6);
    bool cmd_lave_glace = get_bit(frame[0], 7);

    // Print decoded values
    printf("Warning: %s\n", cmd_warning ? "ON" : "OFF");
    set_flag_indic_hazard(cmd_warning);

    printf("Position Light: %s\n", cmd_feux_de_position ? "ON" : "OFF");
    set_flag_position_light(cmd_feux_de_position);

    printf("Crossing Light: %s\n", cmd_feux_de_croisement ? "ON" : "OFF");
    set_flag_crossing_light(cmd_feux_de_croisement);

    printf("High Beam Light: %s\n", cmd_feux_de_route ? "ON" : "OFF");
    set_flag_highbeam_light(cmd_feux_de_route);

    printf("Right Indicator: %s\n", cmd_clignotant_droit ? "ON" : "OFF");
    set_cmd_indic_right(cmd_clignotant_droit ? 1 : 0);

    printf("Left Indicator: %s\n", cmd_clignotant_gauche ? "ON" : "OFF");
    set_cmd_indic_left(cmd_clignotant_gauche ? 1 : 0);

    printf("Windshield Wipers: %s\n", cmd_essuie_glaces ? "ON" : "OFF");
    set_cmd_wiper(cmd_essuie_glaces ? 1 : 0);

    printf("Windshield Washer: %s\n", cmd_lave_glace ? "ON" : "OFF");
    set_cmd_washer(cmd_lave_glace ? 1 : 0);

    return true; // Successful decode, return true
}
