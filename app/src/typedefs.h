/**
 * \file typedefs.h
 * \brief Type definitions for project
 * \details Contains all custom types and enumerations used in the project
 * \author Raphael CAUSSE - Melvyn MUNOZ - Roland Cédric TAYO
 */

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

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

// Command
typedef bool cmd_t;

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

#endif // TYPEDEFS_H