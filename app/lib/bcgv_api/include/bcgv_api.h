/**
 * \file bcgv_api.h
 * \brief Type definitions and context functions for project
 * \details Contains all custom types, enumerations, and context initialization/accessor functions used in the project
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cédric TAYO
 */

#ifndef BCGV_API_H
#define BCGV_API_H

#include <stdint.h>
#include <stdbool.h>

// [Domain values]
#define FRAME_NUMBER_MIN 1
#define FRAME_NUMBER_MAX 100

#define SPEED_MIN 0
#define SPEED_MAX 255

#define MOTOR_FUEL_MIN 0
#define MOTOR_FUEL_MAX 40

#define MOTOR_RPM_MIN 0
#define MOTOR_RPM_MAX 10000

#define DASHB_FUEL_MIN 0
#define DASHB_FUEL_MAX 100

#define DASHB_RPM_MIN 0
#define DASHB_RPM_MAX 1000


// [Command]
typedef bool cmd_t;

// [Trame Number] Looping 100->1
typedef uint8_t frame_number_t;

// [Chassis] Distance (km)
typedef uint32_t distance_t;

// [Chassis] Speed (km/h)
typedef uint8_t speed_t;

// [Chassis] Chassis issues
typedef enum {
    CHASSIS_ISSUE_OK = 0x0,
    CHASSIS_ISSUE_TYRES = 0x1,
    CHASSIS_ISSUE_BRAKES = 0x2,
} chassis_issues_t;

// [Motor] Motor issues
typedef enum {
    MOTOR_ISSUE_OK = 0x0,
    MOTOR_ISSUE_TYRES = 0x1,
    MOTOR_ISSUE_TEMPERATURE_LDR = 0x2,
    MOTOR_ISSUE_OIL_OVERHEATING = 0x4,
} motor_issues_t;

// [Motor] Fuel level (litres)
typedef uint8_t motor_fuel_t;

// [Motor] Revolutions per minute
typedef uint32_t motor_rpm_t;

// [Battery] Battery issues
typedef enum {
    BATTERY_ISSUES_OK = 0x0,
    BATTERY_ISSUES_DISCHARGED = 0x1,
    BATTERY_ISSUES_KO = 0x2,
} battery_issues_t;

// [MUX] CRC8 - Calculated on the rest of the frame
typedef uint8_t crc8_t;

// [BCGV -> BGF] Message ID
typedef enum {
    BCGV_BGF_MSG_ID_1 = 0x01,
    BCGV_BGF_MSG_ID_2 = 0x02,
    BCGV_BGF_MSG_ID_3 = 0x03,
    BCGV_BGF_MSG_ID_4 = 0x04,
    BCGV_BGF_MSG_ID_5 = 0x05,
} bcgv_bgf_msg_id_t;

// [Dashboard] Fuel level display (%)
typedef uint8_t dashb_fuel_t;

// [Dashboard] RPM display (RPM/10) -> 3000 RPM = 300
typedef uint16_t dashb_rpm_t;

// [BCGV -> MUX] Alert flag
typedef bool flag_t;

void bcgv_init();

/**
 * \brief Gets the cmd_warning value.
 * \details Returns the current state of the cmd_warning.
 * \return cmd_t : The cmd_warning value.
 */
cmd_t get_cmd_warning();

/**
 * \brief Sets the cmd_warning value.
 * \details Sets the cmd_warning to the given value.
 * \param value : The new value for the cmd_warning.
 */
void set_cmd_warning(cmd_t value);

/**
 * \brief Gets the cmd_position_light value.
 * \details Returns the current state of the cmd_position_light.
 * \return cmd_t : The cmd_position_light value.
 */
cmd_t get_cmd_position_light();

/**
 * \brief Sets the cmd_position_light value.
 * \details Sets the cmd_position_light to the given value.
 * \param value : The new value for the cmd_position_light.
 */
void set_cmd_position_light(cmd_t value);

/**
 * \brief Gets the cmd_cross_light value.
 * \details Returns the current state of the cmd_cross_light.
 * \return cmd_t : The cmd_cross_light value.
 */
cmd_t get_cmd_cross_light();

/**
 * \brief Sets the cmd_cross_light value.
 * \details Sets the cmd_cross_light to the given value.
 * \param value : The new value for the cmd_cross_light.
 */
void set_cmd_cross_light(cmd_t value);

/**
 * \brief Gets the cmd_high_light value.
 * \details Returns the current state of the cmd_high_light.
 * \return cmd_t : The cmd_high_light value.
 */
cmd_t get_cmd_high_light();

/**
 * \brief Sets the cmd_high_light value.
 * \details Sets the cmd_high_light to the given value.
 * \param value : The new value for the cmd_high_light.
 */
void set_cmd_high_light(cmd_t value);

/**
 * \brief Gets the cmd_right_blinker value.
 * \details Returns the current state of the cmd_right_blinker.
 * \return cmd_t : The cmd_right_blinker value.
 */
cmd_t get_cmd_right_blinker();

/**
 * \brief Sets the cmd_right_blinker value.
 * \details Sets the cmd_right_blinker to the given value.
 * \param value : The new value for the cmd_right_blinker.
 */
void set_cmd_right_blinker(cmd_t value);

/**
 * \brief Gets the cmd_left_blinker value.
 * \details Returns the current state of the cmd_left_blinker.
 * \return cmd_t : The cmd_left_blinker value.
 */
cmd_t get_cmd_left_blinker();

/**
 * \brief Sets the cmd_left_blinker value.
 * \details Sets the cmd_left_blinker to the given value.
 * \param value : The new value for the cmd_left_blinker.
 */
void set_cmd_left_blinker(cmd_t value);

/**
 * \brief Gets the cmd_wiper value.
 * \details Returns the current state of the cmd_wiper.
 * \return cmd_t : The cmd_wiper value.
 */
cmd_t get_cmd_wiper();

/**
 * \brief Sets the cmd_wiper value.
 * \details Sets the cmd_wiper to the given value.
 * \param value : The new value for the cmd_wiper.
 */
void set_cmd_wiper(cmd_t value);

/**
 * \brief Gets the cmd_windshield_washer value.
 * \details Returns the current state of the cmd_windshield_washer.
 * \return cmd_t : The cmd_windshield_washer value.
 */
cmd_t get_cmd_windshield_washer();

/**
 * \brief Sets the cmd_windshield_washer value.
 * \details Sets the cmd_windshield_washer to the given value.
 * \param value : The new value for the cmd_windshield_washer.
 */
void set_cmd_windshield_washer(cmd_t value);

/**
 * \brief Gets the frame_number value.
 * \details Returns the current state of the frame_number.
 * \return frame_number_t : The frame_number value.
 */
frame_number_t get_frame_number();

/**
 * \brief Sets the frame_number value.
 * \details Sets the frame_number to the given value.
 * \param value : The new value for the frame_number.
 */
void set_frame_number(frame_number_t value);

/**
 * \brief Gets the distance value.
 * \details Returns the current state of the distance.
 * \return distance_t : The distance value.
 */
distance_t get_distance();

/**
 * \brief Sets the distance value.
 * \details Sets the distance to the given value.
 * \param value : The new value for the distance.
 */
void set_distance(distance_t value);

/**
 * \brief Gets the speed value.
 * \details Returns the current state of the speed.
 * \return speed_t : The speed value.
 */
speed_t get_speed();

/**
 * \brief Sets the speed value.
 * \details Sets the speed to the given value.
 * \param value : The new value for the speed.
 */
void set_speed(speed_t value);

/**
 * \brief Gets the chassis_issues value.
 * \details Returns the current state of the chassis_issues.
 * \return chassis_issues_t : The chassis_issues value.
 */
chassis_issues_t get_chassis_issues();

/**
 * \brief Sets the chassis_issues value.
 * \details Sets the chassis_issues to the given value.
 * \param value : The new value for the chassis_issues.
 */
void set_chassis_issues(chassis_issues_t value);

/**
 * \brief Gets the motor_issues value.
 * \details Returns the current state of the motor_issues.
 * \return motor_issues_t : The motor_issues value.
 */
motor_issues_t get_motor_issues();

/**
 * \brief Sets the motor_issues value.
 * \details Sets the motor_issues to the given value.
 * \param value : The new value for the motor_issues.
 */
void set_motor_issues(motor_issues_t value);

/**
 * \brief Gets the motor_fuel value.
 * \details Returns the current state of the motor_fuel.
 * \return motor_fuel_t : The motor_fuel value.
 */
motor_fuel_t get_motor_fuel();

/**
 * \brief Sets the motor_fuel value.
 * \details Sets the motor_fuel to the given value.
 * \param value : The new value for the motor_fuel.
 */
void set_motor_fuel(motor_fuel_t value);

/**
 * \brief Gets the motor_rpm value.
 * \details Returns the current state of the motor_rpm.
 * \return motor_rpm_t : The motor_rpm value.
 */
motor_rpm_t get_motor_rpm();

/**
 * \brief Sets the motor_rpm value.
 * \details Sets the motor_rpm to the given value.
 * \param value : The new value for the motor_rpm.
 */
void set_motor_rpm(motor_rpm_t value);

/**
 * \brief Gets the battery_issues value.
 * \details Returns the current state of the battery_issues.
 * \return battery_issues_t : The battery_issues value.
 */
battery_issues_t get_battery_issues();

/**
 * \brief Sets the battery_issues value.
 * \details Sets the battery_issues to the given value.
 * \param value : The new value for the battery_issues.
 */
void set_battery_issues(battery_issues_t value);

/**
 * \brief Gets the crc8 value.
 * \details Returns the current state of the crc8.
 * \return crc8_t : The crc8 value.
 */
crc8_t get_crc8();

/**
 * \brief Sets the crc8 value.
 * \details Sets the crc8 to the given value.
 * \param value : The new value for the crc8.
 */
void set_crc8(crc8_t value);

/**
 * \brief Gets the dashb_fuel value.
 * \details Returns the current state of the dashb_fuel.
 * \return dashb_fuel_t : The dashb_fuel value.
 */
dashb_fuel_t get_dashb_fuel();

/**
 * \brief Sets the dashb_fuel value.
 * \details Sets the dashb_fuel to the given value.
 * \param value : The new value for the dashb_fuel.
 */
void set_dashb_fuel(dashb_fuel_t value);

/**
 * \brief Gets the dashb_rpm value.
 * \details Returns the current state of the dashb_rpm.
 * \return dashb_rpm_t : The dashb_rpm value.
 */
dashb_rpm_t get_dashb_rpm();

/**
 * \brief Sets the dashb_rpm value.
 * \details Sets the dashb_rpm to the given value.
 * \param value : The new value for the dashb_rpm.
 */
void set_dashb_rpm(dashb_rpm_t value);

#endif // BCGV_API_H