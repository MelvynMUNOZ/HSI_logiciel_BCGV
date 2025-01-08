/**
 * \file mux.c
 * \brief
 * \details
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cedric TAYO
 */

#include "bcgv_api.h"
#include "mux.h"
#include "crc8.h"
#include "log.h"
#include "bit_utils.h"

/**
 * \brief Extract a 32-bit unsigned integer from a frame starting at the given index.
 * \param frame : Pointer to the frame buffer.
 * \param index_start : Starting index in the frame buffer.
 * \return uint32_t : Value extracted from the frame.
 */
static uint32_t mux_get_uint32_at(const uint8_t frame[DRV_UDP_100MS_FRAME_SIZE], int index_start)
{
    return (frame[index_start] << 24) | (frame[index_start + 1] << 16) | (frame[index_start + 2] << 8) | frame[index_start + 3];
}

bool mux_read_frame_100ms(int32_t drv_fd, uint8_t frame[DRV_UDP_100MS_FRAME_SIZE])
{
    int32_t ret = drv_read_udp_100ms(drv_fd, frame);
    if (ret == DRV_ERROR)
    {
        log_error("failed to read MUX 100ms UDP frame (%u)", get_frame_number());
    }

    return (ret == DRV_SUCCESS);
}

void mux_check_frame_number(const uint8_t frame[DRV_UDP_100MS_FRAME_SIZE])
{
    frame_number_t frame_number = frame[0];
    frame_number_t expected_frame_number = get_frame_number();

    if (frame_number != expected_frame_number)
    {
        log_warn("frame number mismatch: %u (expected %u)", frame_number, expected_frame_number);
    }

    /* Increment for next frame check */
    expected_frame_number = (expected_frame_number % 100) + 1;
    set_frame_number(expected_frame_number);
}

bool mux_decode_frame(const uint8_t frame[DRV_UDP_100MS_FRAME_SIZE])
{
    bool ret = false;
    distance_t distance = 0;
    speed_t speed = 0;
    issues_t chassis_issues = 0;
    issues_t motor_issues = 0;
    fuel_level_t fuel_level = 0;
    engine_rpm_t engine_rpm = 0;
    issues_t battery_issues = 0;
    crc8_t frame_crc8 = 0;
    crc8_t computed_crc8 = 0;

    /* Decode frame only if CRC8 is valid */
    frame_crc8 = frame[DRV_UDP_100MS_FRAME_SIZE - 1];
    computed_crc8 = crc8_compute(frame, DRV_UDP_100MS_FRAME_SIZE - 1);
    if (frame_crc8 != computed_crc8)
    {
        log_error("invalid CRC8: 0x%02X (computed 0x%02X)", frame_crc8, computed_crc8);
        ret = false;
    }
    else
    {
        /* Extract from frame */
        distance = mux_get_uint32_at(frame, 1);
        speed = frame[5];
        chassis_issues = frame[6];
        motor_issues = frame[7];
        fuel_level = frame[8];
        engine_rpm = mux_get_uint32_at(frame, 9);
        battery_issues = frame[13];

        /* Store data in app context */
        set_distance(distance);
        set_speed(speed);
        set_chassis_issues(chassis_issues);
        set_motor_issues(motor_issues);
        set_fuel_level(fuel_level);
        set_engine_rpm(engine_rpm);
        set_battery_issues(battery_issues);
        set_crc8(frame_crc8);

        ret = true;
    }

    return ret;
}

void mux_print_raw(const uint8_t frame[DRV_UDP_100MS_FRAME_SIZE])
{
    printf("\nMUX [ ");
    for (size_t i = 0; i < DRV_UDP_100MS_FRAME_SIZE; i++)
    {
        printf("%02X ", frame[i]);
    }
    printf("]\n");
}

void mux_print_decoded(void)
{
    frame_number_t frame_number = get_frame_number();
    distance_t distance = get_distance();
    speed_t speed = get_speed();
    issues_t chassis_issues = get_chassis_issues();
    issues_t motor_issues = get_motor_issues();
    fuel_level_t fuel_level = get_fuel_level();
    engine_rpm_t engine_rpm = get_engine_rpm();
    issues_t battery_issues = get_battery_issues();
    crc8_t crc8 = get_crc8();

    printf("Frame number: %u\n", frame_number);
    printf("Distance: %u km\n", distance);
    printf("Speed: %u km/h\n", speed);
    printf("Chassis issues: ");
    if (chassis_issues == CHASSIS_ISSUE_NONE)
    {
        printf("None");
    }
    else
    {
        if (chassis_issues AND CHASSIS_ISSUE_TYRES)
        {
            printf("Tyres ");
        }
        if (chassis_issues AND CHASSIS_ISSUE_BRAKES)
        {
            printf("Brakes ");
        }
    }
    printf("\n");
    printf("Motor issues: ");
    if (motor_issues == MOTOR_ISSUE_NONE)
    {
        printf("None");
    }
    else
    {
        if (motor_issues AND MOTOR_ISSUE_TYRES)
        {
            printf("Tyres ");
        }
        if (motor_issues AND MOTOR_ISSUE_TEMPERATURE_LDR)
        {
            printf("Temperature LDR ");
        }
        if (motor_issues AND MOTOR_ISSUE_OIL_OVERHEATING)
        {
            printf("Oil Overheating ");
        }
    }
    printf("\n");
    printf("Fuel level: %u\n", fuel_level);
    printf("Engine RPM: %u\n", engine_rpm);
    printf("Battery issues: ");
    if (battery_issues == BATTERY_ISSUES_NONE)
    {
        printf("None");
    }
    else
    {
        if (battery_issues AND BATTERY_ISSUES_DISCHARGED)
        {
            printf("Discharged ");
        }
        if (battery_issues AND BATTERY_ISSUES_KO)
        {
            printf("KO ");
        }
    }
    printf("\n");
    printf("CRC8: %u\n", crc8);
}