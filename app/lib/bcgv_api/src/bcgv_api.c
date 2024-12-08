/**
 * \file bcgv_api.c
 * \brief Context initialization and definitions for project
 * \details Contains initialization and definition functions for all custom types and enumerations used in the project
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cédric TAYO
 */

#include "bcgv_api.h"

// Context structure
typedef struct {
    cmd_t cmd_warning; // Warning
    cmd_t cmd_position_light; // Position lights
    cmd_t cmd_cross_light; // ow beam headlights
    cmd_t cmd_high_light; // High beam lights
    cmd_t cmd_right_blinker; // Right Turn Signal
    cmd_t cmd_left_blinker; // Left turn signal
    cmd_t cmd_wiper; // Windshield wipers
    cmd_t cmd_windshield_washer; // Windshield washer
    frame_number_t frame_number; // Number of plots
    distance_t distance; // Speed (km/h)
    speed_t speed; // Speed (km/h)
    chassis_issues_t chassis_issues; // Chassis issues
    motor_issues_t motor_issues; // Motor Issues
    motor_fuel_t motor_fuel; // Tank level
    motor_rpm_t motor_rpm; // Revolution/minute
    battery_issues_t battery_issues; // Battery issues
    crc8_t crc8; // CRC8
    dashb_fuel_t dashb_fuel; // Tank level display
    dashb_rpm_t dashb_rpm; // Revolution/minute display
} context_t;

// Global context structure instance
static context_t context;

void context_init() {
    context.cmd_warning = 0;
    context.cmd_position_light = 0;
    context.cmd_cross_light = 0;
    context.cmd_high_light = 0;
    context.cmd_right_blinker = 0;
    context.cmd_left_blinker = 0;
    context.cmd_wiper = 0;
    context.cmd_windshield_washer = 0;
    context.frame_number = 1;
    context.distance = 0;
    context.speed = 0;
    context.chassis_issues = 0;
    context.motor_issues = 0;
    context.motor_fuel = 0;
    context.motor_rpm = 0;
    context.battery_issues = 0;
    context.crc8 = 0;
    context.dashb_fuel = 0;
    context.dashb_rpm = 0;
}

// Getters and Setters
cmd_t get_cmd_warning() {
    return context.cmd_warning;
}

void set_cmd_warning(cmd_t value) {
    context.cmd_warning = value;
}

cmd_t get_cmd_position_light() {
    return context.cmd_position_light;
}

void set_cmd_position_light(cmd_t value) {
    context.cmd_position_light = value;
}

cmd_t get_cmd_cross_light() {
    return context.cmd_cross_light;
}

void set_cmd_cross_light(cmd_t value) {
    context.cmd_cross_light = value;
}

cmd_t get_cmd_high_light() {
    return context.cmd_high_light;
}

void set_cmd_high_light(cmd_t value) {
    context.cmd_high_light = value;
}

cmd_t get_cmd_right_blinker() {
    return context.cmd_right_blinker;
}

void set_cmd_right_blinker(cmd_t value) {
    context.cmd_right_blinker = value;
}

cmd_t get_cmd_left_blinker() {
    return context.cmd_left_blinker;
}

void set_cmd_left_blinker(cmd_t value) {
    context.cmd_left_blinker = value;
}

cmd_t get_cmd_wiper() {
    return context.cmd_wiper;
}

void set_cmd_wiper(cmd_t value) {
    context.cmd_wiper = value;
}

cmd_t get_cmd_windshield_washer() {
    return context.cmd_windshield_washer;
}

void set_cmd_windshield_washer(cmd_t value) {
    context.cmd_windshield_washer = value;
}

frame_number_t get_frame_number() {
    return context.frame_number;
}

void set_frame_number(frame_number_t value) {
    if (value >= FRAME_NUMBER_MIN && value <= FRAME_NUMBER_MAX) {
        context.frame_number = value;
    }
}

distance_t get_distance() {
    return context.distance;
}

void set_distance(distance_t value) {
    context.distance = value;
}

speed_t get_speed() {
    return context.speed;
}

void set_speed(speed_t value) {
    if (value >= SPEED_MIN && value <= SPEED_MAX) {
        context.speed = value;
    }
}

chassis_issues_t get_chassis_issues() {
    return context.chassis_issues;
}

void set_chassis_issues(chassis_issues_t value) {
    context.chassis_issues = value;
}

motor_issues_t get_motor_issues() {
    return context.motor_issues;
}

void set_motor_issues(motor_issues_t value) {
    context.motor_issues = value;
}

motor_fuel_t get_motor_fuel() {
    return context.motor_fuel;
}

void set_motor_fuel(motor_fuel_t value) {
    if (value >= MOTOR_FUEL_MIN && value <= MOTOR_FUEL_MAX) {
        context.motor_fuel = value;
    }
}

motor_rpm_t get_motor_rpm() {
    return context.motor_rpm;
}

void set_motor_rpm(motor_rpm_t value) {
    if (value >= MOTOR_RPM_MIN && value <= MOTOR_RPM_MAX) {
        context.motor_rpm = value;
    }
}

battery_issues_t get_battery_issues() {
    return context.battery_issues;
}

void set_battery_issues(battery_issues_t value) {
    context.battery_issues = value;
}

crc8_t get_crc8() {
    return context.crc8;
}

void set_crc8(crc8_t value) {
    context.crc8 = value;
}

dashb_fuel_t get_dashb_fuel() {
    return context.dashb_fuel;
}

void set_dashb_fuel(dashb_fuel_t value) {
    if (value >= DASHB_FUEL_MIN && value <= DASHB_FUEL_MAX) {
        context.dashb_fuel = value;
    }
}

dashb_rpm_t get_dashb_rpm() {
    return context.dashb_rpm;
}

void set_dashb_rpm(dashb_rpm_t value) {
    if (value >= DASHB_RPM_MIN && value <= DASHB_RPM_MAX) {
        context.dashb_rpm = value;
    }
}
