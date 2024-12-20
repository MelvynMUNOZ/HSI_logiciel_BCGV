/**
 * \file mutex.c
 * \brief File to handle MUTEX
 * \details It contains code and decode functions and main to call FSMs
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#include <stdint.h>
#include <stdio.h>

#include "bcgv_api.h"
#include "mux.h"
#include "crc8.h"

/**
 * \brief Extract a 32-bit unsigned integer from a frame starting at the given index.
 * \param frame Pointer to the frame buffer.
 * \param index Starting index in the frame buffer.
 * \return 32-bit unsigned integer value extracted from the frame.
 */
static uint32_t mux_get_uint32_at(const uint8_t frame[DRV_UDP_100MS_FRAME_SIZE], int index)
{
    return (frame[index] << 24) | (frame[index + 1] << 16) | (frame[index + 2] << 8) | frame[index + 3];
}

/**
 * \details Frame format (15 octets):
 *          - Octet 0: Frame number (1-100)
 *          - Octets 1-4: Distance (km)
 *          - Octet 5: Speed (km/h)
 *          - Octet 6: Chassis issues
 *          - Octet 7: Motor issues
 *          - Octet 8: Fuel level (litres)
 *          - Octets 9-12: Engine RPM (tours/minute)
 *          - Octet 13: Battery issues
 *          - Octet 14: CRC8
 */
bool mux_decode(const uint8_t frame[DRV_UDP_100MS_FRAME_SIZE])
{
    // Frame number decoding (1 octet)
    uint8_t frame_number = frame[0];
    printf("Frame number: %d\n", frame_number);
    set_frame_number(frame_number);

    // Kilometers decoding (4 octets)
    uint32_t distance = mux_get_uint32_at(frame, 1);
    printf("Distance: %u km\n", distance);
    set_distance(distance);

    // Speed decoding (1 octet)
    uint8_t speed = frame[5];
    printf("Speed: %d km/h\n", speed);
    set_speed(speed);

    // Chassis issues decoding (1 octet)
    uint8_t chassis_issues = frame[6];
    printf("Chassis issues: ");
    if (chassis_issues == CHASSIS_ISSUE_NONE)
    {
        printf("None");
    }
    else
    {
        if (chassis_issues & CHASSIS_ISSUE_TYRES)
            printf("Tyres ");
        if (chassis_issues & CHASSIS_ISSUE_BRAKES)
            printf("Brakes ");
    }
    printf("\n");
    set_chassis_issues(chassis_issues);

    // Motor issues decoding (1 octet)
    uint8_t motor_issues = frame[7];
    printf("Motor issues: ");
    if (motor_issues == MOTOR_ISSUE_NONE)
    {
        printf("None");
    }
    else
    {
        if (motor_issues & MOTOR_ISSUE_TYRES)
            printf("Tyres ");
        if (motor_issues & MOTOR_ISSUE_TEMPERATURE_LDR)
            printf("Temperature LDR ");
        if (motor_issues & MOTOR_ISSUE_OIL_OVERHEATING)
            printf("Oil Overheating ");
    }
    printf("\n");
    set_motor_issues(motor_issues);

    // Fuel level decoding (1 octet)
    uint8_t fuel_level = frame[8];
    printf("Fuel level: %d litres\n", fuel_level);
    set_fuel_level(fuel_level);

    // Revolution/Minuts decoding (4 octets)
    uint32_t engine_rpm = mux_get_uint32_at(frame, 9);
    printf("Engine RPM: %u\n", engine_rpm);
    set_engine_rpm(engine_rpm);

    // Battery issues decoding (1 octet)
    uint8_t battery_issues = frame[13];
    printf("Battery issues: ");
    if (battery_issues == BATTERY_ISSUES_NONE)
    {
        printf("None");
    }
    else
    {
        if (battery_issues & BATTERY_ISSUES_DISCHARGED)
            printf("Discharged ");
        if (battery_issues & BATTERY_ISSUES_KO)
            printf("KO ");
    }
    printf("\n");
    set_battery_issues(battery_issues);

    // CRC8 - Calculation and verification (1 octet)
    uint8_t calculated_crc8 = crc_8(frame, DRV_UDP_100MS_FRAME_SIZE - 1); // Calculate CRC8 on first 14 octets
    uint8_t frame_crc8 = frame[14];
    printf("Calculated CRC8: 0x%02X\n", calculated_crc8);
    printf("Frame CRC8: 0x%02X\n", frame_crc8);

    if (calculated_crc8 != frame_crc8)
    {
        printf("ERROR: CRC mismatch!\n");
        return false; // CRC mismatch, return false
    }
    set_crc8(frame_crc8);
    return true; // Successful decode, return true;
}
