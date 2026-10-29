#include "main.h"
#include "liblvgl/core/lv_obj_pos.h"
#include "liblvgl/core/lv_obj_style.h"
#include "liblvgl/font/lv_font.h"
#include "liblvgl/misc/lv_style.h"
#include "liblvgl/widgets/lv_img.h"
#include "okapi/api/device/motor/abstractMotor.hpp"
#include "okapi/impl/device/controllerUtil.hpp"
#include "pros/rtos.hpp"
#include "bb.h"

static lv_style_t text_style;
static lv_color_t colors[] = { lv_color_make(120, 120, 255), lv_color_make(200, 200, 200), lv_color_make(255, 255, 255), lv_color_make(200, 200, 200) };

lv_obj_t* label;
int color_cycle = 0;
int color_idx = 0;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

	lv_obj_t* bonzi_img = lv_img_create(lv_scr_act());
	lv_img_set_src(bonzi_img, &bonzi);
	lv_obj_center(bonzi_img);

	label = lv_label_create(lv_scr_act());
	
	lv_style_set_text_font(&text_style, &lv_font_unscii_16);
	lv_style_set_text_color(&text_style, colors[0]);
	lv_style_set_text_align(&text_style, LV_TEXT_ALIGN_CENTER);

	lv_obj_add_style(label, &text_style, 0);
	lv_label_set_text(label, "  __________  _______ \n <  / __/ _ \\/_  / _ |\n / / _ \\\\_, / / / __ |\n/_/\\___/___/ /_/_/ |_|");

	lv_obj_center(label);

	lift.setBrakeMode(AbstractMotor::brakeMode::hold);
	claw.setBrakeMode(AbstractMotor::brakeMode::hold);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	drive->moveDistance(1_ft);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	Controller controller;

	bool bWasPressed = false;
	while (true) {
		drive->getModel()->arcade(controller.getAnalog(okapi::ControllerAnalog::leftY), controller.getAnalog(okapi::ControllerAnalog::rightX));
		lift.controllerSet(controller.getDigital(ControllerDigital::L2) ? 1 : controller.getDigital(ControllerDigital::L1) ? -1 : 0);
		claw.controllerSet(controller.getDigital(ControllerDigital::R2) ? 1 : controller.getDigital(ControllerDigital::R1) ? -1 : 0);
		toggler.controllerSet(controller.getDigital(ControllerDigital::A) ? -1 : 0);
		if (controller.getDigital(ControllerDigital::B) && !bWasPressed) {
			alsoClaw.toggle();
		}

		color_cycle++;
		if (color_cycle > 50) {
			color_cycle = 0;
			color_idx = (color_idx + 1) % 4;
			lv_style_set_text_color(&text_style, colors[color_idx]);
			lv_obj_invalidate(label);
		}

		bWasPressed = controller.getDigital(ControllerDigital::B);

		pros::delay(10);
	}
}