/**
 * \file mux.c
 * \brief
 * \details
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#include <stdio.h>
#include "bcgv_api.h"
#include "mux.h"
#include "crc8.h"
#include "bit_utils.h"

/**
 * \brief Extract a 32-bit unsigned integer from a frame starting at the given index.
 * \param frame Pointer to the frame buffer.
 * \param index Starting index in the frame buffer.
 * \return 32-bit unsigned integer value extracted from the frame.
 */
static uint32_t mux_get_uint32_at(const uint8_t frame[DRV_UDP_100MS_FRAME_SIZE], int index_start)
{
    return (frame[index_start] << 24) | (frame[index_start + 1] << 16) | (frame[index_start + 2] << 8) | frame[index_start + 3];
}

/**
 * \details Frame format (15 bytes):
 *          - byte 0: Frame number (1-100)
 *          - bytes 1-4: Distance (km)
 *          - byte 5: Speed (km/h)
 *          - byte 6: Chassis issues
 *          - byte 7: Motor issues
 *          - byte 8: Fuel level (litres)
 *          - bytes 9-12: Engine RPM (tours/minute)
 *          - byte 13: Battery issues
 *          - byte 14: CRC8
 */
bool mux_decode(const uint8_t frame[DRV_UDP_100MS_FRAME_SIZE])
{
    // Frame number decoding (1 byte)
    uint8_t frame_number = frame[0];
    printf("Frame number: %u\n", frame_number);
    set_frame_number(frame_number);

    /* TODO: check for valid frame_number, incremental */

    // Kilometers decoding (4 bytes)
    uint32_t distance = mux_get_uint32_at(frame, 1);
    printf("Distance: %u km\n", distance);
    set_distance(distance);

    // Speed decoding (1 byte)
    uint8_t speed = frame[5];
    printf("Speed: %d km/h\n", speed);
    set_speed(speed);

    // Chassis issues decoding (1 byte)
    uint8_t chassis_issues = frame[6];
    printf("Chassis issues: ");
    if (chassis_issues == CHASSIS_ISSUE_NONE)
    {
        printf("None");
    }
    else
    {
        if (chassis_issues AND CHASSIS_ISSUE_TYRES)
            printf("Tyres ");
        if (chassis_issues AND CHASSIS_ISSUE_BRAKES)
            printf("Brakes ");
    }
    printf("\n");
    set_chassis_issues(chassis_issues);

    // Motor issues decoding (1 byte)
    uint8_t motor_issues = frame[7];
    printf("Motor issues: ");
    if (motor_issues == MOTOR_ISSUE_NONE)
    {
        printf("None");
    }
    else
    {
        if (motor_issues AND MOTOR_ISSUE_TYRES)
            printf("Tyres ");
        if (motor_issues AND MOTOR_ISSUE_TEMPERATURE_LDR)
            printf("Temperature LDR ");
        if (motor_issues AND MOTOR_ISSUE_OIL_OVERHEATING)
            printf("Oil Overheating ");
    }
    printf("\n");
    set_motor_issues(motor_issues);

    // Fuel level decoding (1 byte)
    uint8_t fuel_level = frame[8];
    printf("Fuel level: %d litres\n", fuel_level);
    set_fuel_level(fuel_level);

    // RPM decoding (4 bytes)
    uint32_t engine_rpm = mux_get_uint32_at(frame, 9);
    printf("Engine RPM: %u\n", engine_rpm);
    set_engine_rpm(engine_rpm);

    // Battery issues decoding (1 byte)
    uint8_t battery_issues = frame[13];
    printf("Battery issues: ");
    if (battery_issues == BATTERY_ISSUES_NONE)
    {
        printf("None");
    }
    else
    {
        if (battery_issues AND BATTERY_ISSUES_DISCHARGED)
            printf("Discharged ");
        if (battery_issues AND BATTERY_ISSUES_KO)
            printf("KO ");
    }
    printf("\n");
    set_battery_issues(battery_issues);

    // CRC8 - Calculation and verification (1 byte)
    uint8_t computed_crc8 = compute_crc8(frame, DRV_UDP_100MS_FRAME_SIZE - 1); // Calculate CRC8 on first 14 bytes
    uint8_t frame_crc8 = frame[DRV_UDP_100MS_FRAME_SIZE - 1];
    printf("Calculated CRC8: 0x%02X\n", computed_crc8);
    printf("Frame CRC8: 0x%02X\n", frame_crc8);

    if (computed_crc8 != frame_crc8)
    {
        printf("ERROR: CRC mismatch!\n");
        return false;
    }
    set_crc8(frame_crc8);
    return true;
}
