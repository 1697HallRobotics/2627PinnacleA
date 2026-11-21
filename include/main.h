/**
 * \file main.h
 *
 * Contains common definitions and header files used throughout your PROS
 * project.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

/**
 * If defined, some commonly used enums will have preprocessor macros which give
 * a shorter, more convenient naming pattern. If this isn't desired, simply
 * comment the following line out.
 *
 * For instance, E_CONTROLLER_MASTER has a shorter name: CONTROLLER_MASTER.
 * E_CONTROLLER_MASTER is pedantically correct within the PROS styleguide, but
 * not convenient for most student programmers.
 */
#define PROS_USE_SIMPLE_NAMES

/**
 * If defined, C++ literals will be available for use. All literals are in the
 * pros::literals namespace.
 *
 * For instance, you can do `4_mtr = 50` to set motor 4's target velocity to 50
 */
#define PROS_USE_LITERALS

#include "api.h"

/**
 * You should add more #includes here
 */
#include "okapi/api.hpp"
#include "liblvgl/lvgl.h"

/**
 * If you find doing pros::Motor() to be tedious and you'd prefer just to do
 * Motor, you can use the namespace with the following commented out line.
 *
 * IMPORTANT: Only the okapi or pros namespace may be used, not both
 * concurrently! The okapi namespace will export all symbols inside the pros
 * namespace.
 */
//using namespace pros;
// using namespace pros::literals;
using namespace okapi;
using namespace okapi::literals;

std::shared_ptr<OdomChassisController> drive =
    ChassisControllerBuilder()
        .withMotors({-9, -8}, {19, 20})
        .withDimensions({AbstractMotor::gearset::blue, 60.0 / 36.0}, {{3.25_in, 13.0_in}, imev5BlueTPR})
        /*
        .withGains(
            {1.8 / 120000, 1.2 / 120000, 0.9 / 120000}, // Distance controller gains
            {1.8 / 120000, 1.2 / 120000, 0.9 / 120000}, // Turn controller gains
            {1.8 / 120000, 1.2 / 120000, 0.9 / 120000}  // Angle controller gains (helps drive straight)
        )
        */
        
        .withGains(
            {0.003, 0.0001, 0.0001},
            {0.000465, 0.0, 0.0001}
        )
        
        .withMaxVoltage(7500)
        /*
        .withGains(
            {0.001, 0, 0.0001}, // distance controller gains
            {0.001, 0, 0.0001}, // turn controller gains
            {0.001, 0, 0.0001}  // angle controller gains (helps drive straight)
        )
            */
        .withOdometry()
        .buildOdometry();

MotorGroup lift({-10, 18});
Motor toggler(13);
pros::adi::Pneumatics alsoClaw('b', false);

/**
 * Prototypes for the competition control tasks are redefined here to ensure
 * that they can be called from user code (i.e. calling autonomous from a
 * button press in opcontrol() for testing purposes).
 */
 /** Austin who the hell leaves comments on code only you access
 - nyaa :3
- hi its me other nico like jarona type shi
  */
#ifdef __cplusplus
extern "C" {
#endif
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/**
 * You can add C++-only headers here
 */
//#include <iostream>
#endif

#endif  // _PROS_MAIN_H_
