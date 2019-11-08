/*
 * Project5.h
 *
 *  Created on: Nov 8, 2019
 *      Author: john
 */

#ifndef PROJECT5_H_
#define PROJECT5_H_

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

#define RED_BASE GPIOB
#define RED_PIN 18U

#define GREEN_BASE GPIOB
#define GREEN_PIN 19U

#define BLUE_BASE GPIOD
#define BLUE_PIN 1U
extern LoggerHandle logger;
extern RGBLEDHandle led;

#endif /* PROJECT5_H_ */
