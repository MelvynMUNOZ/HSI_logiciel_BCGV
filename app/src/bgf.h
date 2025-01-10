/**
 * \file        bgf.h
 * \author      Melvyn MUNOZ
 * \date        2025-01-06
 * \brief       This is the code to encode the message that BCGV send do BGF.
 */

#ifndef BGF_H
#define BGF_H

#include <stdint.h>
#include "bcgv_api.h"

bool bgf_read_serial_message(int32_t drvFd);

bool bgf_write_serial_message(int32_t drvFd);

#endif
