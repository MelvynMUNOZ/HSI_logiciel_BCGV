/**
 * \file mux.c
 * \brief
 * \details
 * \author Raphael CAUSSE - Melvyn MUNOZ - Rol& Cedric TAYO
 */

#include "bcgv_api.h"
#include "mux.h"
#include "crc8.h"
#include "log.h"
#include "bit_utils.h"

#define FUEL_LEVEL_5_PERCENT (FUEL_LEVEL_MAX * 5 / 100)

#define MUX_100MS_GET_UINT32_AT(index) ((mux_frame_100ms[index] << 24) |     \
                                        (mux_frame_100ms[index + 1] << 16) | \
                                        (mux_frame_100ms[index + 2] << 8) |  \
                                        mux_frame_100ms[index + 3])

#define MUX_200MS_SET_UINT32_AT(index, value)              \
    do                                                     \
    {                                                      \
        mux_frame_200ms[index] = (value >> 24) & 0xFF;     \
        mux_frame_200ms[index + 1] = (value >> 16) & 0xFF; \
        mux_frame_200ms[index + 2] = (value >> 8) & 0xFF;  \
        mux_frame_200ms[index + 3] = value & 0xFF;         \
    } while (0)

#define MUX_200MS_SET_UINT16_AT(index, value)         \
    do                                                \
    {                                                 \
        mux_frame_200ms[index] = (value >> 8) & 0xFF; \
        mux_frame_200ms[index + 1] = value & 0xFF;    \
    } while (0)

/* Internal variables */
static uint8_t mux_frame_100ms[DRV_UDP_100MS_FRAME_SIZE] = {0};
static uint8_t mux_frame_200ms[DRV_UDP_200MS_FRAME_SIZE] = {0};

bool mux_read_frame_100ms(int32_t drv_fd)
{
    int32_t ret = drv_read_udp_100ms(drv_fd, mux_frame_100ms);
    if (ret == DRV_ERROR)
    {
        log_error("error while reading from MUX 100ms frame", NULL);
    }

    return (ret == DRV_SUCCESS);
}

bool mux_write_frame_200ms(int32_t drv_fd)
{
    int32_t ret = drv_write_udp_200ms(drv_fd, mux_frame_200ms);
    if (ret == DRV_ERROR)
    {
        log_error("error while writing to MUX 200ms frame", NULL);
    }

    return (ret == DRV_SUCCESS);
}

void mux_check_frame_number()
{
    frame_number_t frame_number = mux_frame_100ms[0];
    frame_number_t expected_frame_number = get_frame_number();

    if (frame_number != expected_frame_number)
    {
        log_warn("frame number mismatch: %u (expected %u)", frame_number, expected_frame_number);
    }

    /* Increment for next frame check */
    expected_frame_number = (expected_frame_number % 100) + 1;
    set_frame_number(expected_frame_number);
}

bool mux_decode_frame_100ms()
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
    frame_crc8 = mux_frame_100ms[DRV_UDP_100MS_FRAME_SIZE - 1];
    computed_crc8 = crc8_compute(mux_frame_100ms, DRV_UDP_100MS_FRAME_SIZE - 1);
    if (frame_crc8 != computed_crc8)
    {
        log_error("invalid CRC8: 0x%02X (computed 0x%02X)", frame_crc8, computed_crc8);
        ret = false;
    }
    else
    {
        /* Extract from frame */
        distance = MUX_100MS_GET_UINT32_AT(1);
        speed = mux_frame_100ms[5];
        chassis_issues = mux_frame_100ms[6];
        motor_issues = mux_frame_100ms[7];
        fuel_level = mux_frame_100ms[8];
        engine_rpm = MUX_100MS_GET_UINT32_AT(9);
        battery_issues = mux_frame_100ms[13];

        /* Store data in app context */
        set_distance(distance);
        set_speed(speed);
        set_chassis_issues(chassis_issues);
        set_motor_issues(motor_issues);
        set_fuel_level(fuel_level);
        set_engine_rpm(engine_rpm);
        set_battery_issues(battery_issues);
        set_crc8(frame_crc8);

        /* DEBUG */
        printf("====================");
        mux_print_raw(mux_frame_100ms, DRV_UDP_100MS_FRAME_SIZE);
        mux_print_decoded();
        printf("====================\n");
        /* END DEBUG*/

        ret = true;
    }

    return ret;
}

void mux_encode_frame_200ms()
{
    uint8_t byte = 0;
    distance_t distance = get_distance();
    speed_t speed = get_speed();
    issues_t chassis_issues = get_chassis_issues();
    issues_t motor_issues = get_motor_issues();
    fuel_level_t fuel_level = get_fuel_level();
    engine_rpm_t engine_rpm = get_engine_rpm();
    issues_t battery_issues = get_battery_issues();
    flag_t flag_position_light = get_flag_position_light();
    flag_t flag_crossing_light = get_flag_crossing_light();
    flag_t flag_highbeam_light = get_flag_highbeam_light();
    flag_t flag_indic_hazard = get_flag_indic_hazard();
    flag_t flag_wiper = get_flag_wiper();
    flag_t flag_washer = get_flag_washer();

    /* Set first 8 bits */
    byte |= (flag_position_light & 1U) << 7;
    byte |= (flag_crossing_light & 1U) << 6;
    byte |= (flag_highbeam_light & 1U) << 5;
    byte |= ((fuel_level < FUEL_LEVEL_5_PERCENT) ? 1U : 0U) << 4;
    byte |= ((motor_issues != MOTOR_ISSUE_NONE) ? 1U : 0U) << 3;
    byte |= ((chassis_issues & CHASSIS_ISSUE_TYRES_PRESSION) ? 1U : 0U) << 2;
    byte |= 0U << 1; /* Unused bit, set to 0 */
    byte |= ((battery_issues & BATTERY_ISSUES_DISCHARGED) ? 1U : 0U);
    mux_frame_200ms[0] = byte;

    /* Set next 8 bits */
    byte = 0; /* Reset */
    byte |= (flag_indic_hazard & 1U) << 7;
    byte |= ((battery_issues & BATTERY_ISSUES_KO) ? 1U : 0U) << 6;
    byte |= ((motor_issues & MOTOR_ISSUE_TEMPERATURE_LDR) ? 1U : 0U) << 5;
    byte |= ((motor_issues & MOTOR_ISSUE_PRESSION) ? 1U : 0U) << 4;
    byte |= ((motor_issues & MOTOR_ISSUE_OIL_OVERHEAT) ? 1U : 0U) << 3;
    byte |= ((chassis_issues & CHASSIS_ISSUE_BRAKES) ? 1U : 0U) << 2;
    byte |= (flag_wiper & 1U) << 1;
    byte |= (flag_washer & 1U);
    mux_frame_200ms[1] = byte;

    /* Set remaining bytes in Big Endian */
    MUX_200MS_SET_UINT32_AT(2, distance);
    mux_frame_200ms[6] = speed;
    mux_frame_200ms[7] = fuel_level * 100 / FUEL_LEVEL_MAX;
    engine_rpm /= 10;
    MUX_200MS_SET_UINT16_AT(8, engine_rpm);

    /* DEBUG */
    mux_print_raw(mux_frame_200ms, DRV_UDP_200MS_FRAME_SIZE);
    /* END DEBUG*/
}

void mux_print_raw(const uint8_t *frame, size_t length)
{
    printf("\nMUX [ ");
    for (size_t i = 0; i < length; i++)
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
        if (chassis_issues & CHASSIS_ISSUE_TYRES_PRESSION)
        {
            printf("Tyres ");
        }
        if (chassis_issues & CHASSIS_ISSUE_BRAKES)
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
        if (motor_issues & MOTOR_ISSUE_PRESSION)
        {
            printf("Pression ");
        }
        if (motor_issues & MOTOR_ISSUE_TEMPERATURE_LDR)
        {
            printf("Temperature LDR ");
        }
        if (motor_issues & MOTOR_ISSUE_OIL_OVERHEAT)
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
        if (battery_issues & BATTERY_ISSUES_DISCHARGED)
        {
            printf("Discharged ");
        }
        if (battery_issues & BATTERY_ISSUES_KO)
        {
            printf("KO ");
        }
    }
    printf("\n");
    printf("CRC8: %u\n", crc8);
}