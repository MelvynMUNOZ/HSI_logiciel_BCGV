/**
 * \file context.h
 * \brief Context initialization and accessors for project
 * \details Contains initialization and accessor functions for all custom types and enumerations used in the project
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cédric TAYO
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include "typedefs.h"

void context_init();

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

#endif // CONTEXT_H