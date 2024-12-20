/**
 * \file mutex.c
 * \brief File to handle MUTEX
 * \details It contains code and decode functions and main to call FSMs
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#include <stdint.h>
#include <stdio.h>

#include "mux.h"

static uint8_t mux_compute_crc8()
{
    return 0;
}

static uint32_t mux_get_uint32_at(const uint8_t *frame, int index)
{
    return 0;
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
void mux_decode(const uint8_t *frame)
{
    // Frame number decoding (1 octet)
    uint8_t frame_number = frame[0];
    printf("Frame number: %d\n", frame_number);
    // Need to swap it
    set_frame_number(frame_number);

    // Kilometers decoding (4 octets)
    uint32_t distance = (frame[1] << 24) | (frame[2] << 16) | (frame[3] << 8) | frame[4];
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
    uint32_t engine_rpm = (frame[9] << 24) | (frame[10] << 16) | (frame[11] << 8) | frame[12];
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

    // CRC8 - Calcul (1 octet)
    uint8_t crc8 = frame[14];
    printf("CRC8: 0x%02X\n", crc8);
    set_crc8(crc8);
}
