#include "Sub.h"

// manual_init - initialise manual controller
bool Sub::manual_init() {
    // set target altitude to zero for reporting
    pos_control.set_alt_target(0);

    // attitude hold inputs become thrust inputs in manual mode
    // set to neutral to prevent chaotic behavior (esp. roll/pitch)
    set_neutral_controls();

    return true;
}

// manual_run - runs the manual (passthrough) controller
// should be called at 100hz or more
void Sub::manual_run() {
    // if not armed set throttle to zero and exit immediately
    if (!motors.armed()) {
        motors.set_desired_spool_state(AP_Motors::DESIRED_GROUND_IDLE);
        attitude_control.set_throttle_out(0, true, g.throttle_filt);
        attitude_control.relax_attitude_controllers();
        return;
    }

    motors.set_desired_spool_state(AP_Motors::DESIRED_THROTTLE_UNLIMITED);

    motors.set_roll(channel_roll->norm_input());
    motors.set_pitch(channel_pitch->norm_input());
    motors.set_yaw(channel_yaw->norm_input() * g.acro_yaw_p / ACRO_YAW_P);
    motors.set_throttle(channel_throttle->norm_input());
    motors.set_forward(channel_forward->norm_input());
    motors.set_lateral(channel_lateral->norm_input());
}

// manual_init - initialise manual controller
bool Sub::raw_init() {
    // set target altitude to zero for reporting
    pos_control.set_alt_target(0);

    // attitude hold inputs become thrust inputs in manual mode
    // set to neutral to prevent chaotic behavior (esp. roll/pitch)
    set_neutral_controls();

    return true;
}

// raw_run - runs the manual (passthrough) controller
// should be called at 100hz or more
void Sub::raw_run() {
    // if not armed set throttle to zero and exit immediately
    if (!motors.armed()) {
        motors.set_desired_spool_state(AP_Motors::DESIRED_GROUND_IDLE);
        attitude_control.set_throttle_out(0, true, g.throttle_filt);
        attitude_control.relax_attitude_controllers();
        return;
    }

    uint32_t tnow = AP_HAL::millis();

    motors.set_desired_spool_state(AP_Motors::DESIRED_THROTTLE_UNLIMITED);

    // If there is a dedicated message and values for pwm are set - move!
    for (int i=0; i<6; i++) {
        if ((motors_raw_sp.pwm[i] != UINT16_MAX) && (tnow - motors_raw_sp.last_message_ms < 1*1000)) {
            motors.output_raw(i, motors_raw_sp.pwm[i]);
        } else {
            motors.output_raw(i, 1500U);
        }
    }

    // TODO Check if there is a possibility to use joystick as a override for the emergency.
//
//    motors.set_roll(channel_roll->norm_input());
//    motors.set_pitch(channel_pitch->norm_input());
//    motors.set_yaw(channel_yaw->norm_input() * g.acro_yaw_p / ACRO_YAW_P);
//    motors.set_throttle(channel_throttle->norm_input());
//    motors.set_forward(channel_forward->norm_input());
//    motors.set_lateral(channel_lateral->norm_input());

//  1	Pitch
//  2	Roll
//  3	Throttle
//  4	Yaw
//  5	Forward
//  6	Lateral

//    motors.output_raw(0, channel_pitch->get_radio_in());
//    motors.output_raw(1, channel_roll->get_radio_in());
//    motors.output_raw(2, channel_throttle->get_radio_in());
//    motors.output_raw(3, channel_yaw->get_radio_in());
//    motors.output_raw(4, channel_forward->get_radio_in());
//    motors.output_raw(5, channel_lateral->get_radio_in());

}
