/*
 * Project5.h
 *
 *  Created on: Nov 8, 2019
 *      Author: john
 */

#ifndef PROJECT_5_H_
#define PROJECT_5_H_

#define FREEDOM
#include <stdio.h>
#include "mode.h"
#include "util.h"
#include <stdlib.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "LED/RGB.h"
#include "Logger/logger.h"
#include "Test/tests.h"

#define RED_BASE GPIOB
#define RED_PIN 18U

#define GREEN_BASE GPIOB
#define GREEN_PIN 19U

#define BLUE_BASE GPIOD
#define BLUE_PIN 1U
#define ESCAPE_CHARACTER 27
extern LoggerHandle logger;
extern RGBLEDHandle led;

#endif /* PROJECT_5_H_ */
