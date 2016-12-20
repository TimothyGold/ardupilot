#include "Tracker.h"

/*
 * Manual control mode
 */

/*
* update_manual - runs the manual controller
*  called at 50hz while control_mode is 'MANUAL'
*/
void Tracker::update_manual(void)
{
    if ((enum ServoType)g.servo_pitch_type.get() == SERVO_TYPE_CR) {
    int16_t	radio_in = constrain_int16(channel_pitch.get_radio_in(), channel_pitch.get_radio_min(), channel_pitch.get_radio_max());

    //Check for Radio center to lock angle
    //Check to see if we should lock the pitch angle
    check_manual_pitch_limits(radio_in);

    if (pitch_lock) {
        float pitch = pitch_lock_angle + g.pitch_trim;
        update_pitch_servo(pitch);
    } else {
        channel_pitch.set_radio_out(radio_in);
        channel_pitch.enable_out();
        channel_pitch.output();
    }
    } else {
        channel_pitch.set_radio_out(constrain_int16(channel_pitch.get_radio_in(), channel_pitch.get_radio_min(), channel_pitch.get_radio_max()));
        channel_pitch.enable_out();
        channel_pitch.output();
    }

    // copy yaw input to output
    channel_yaw.set_radio_out(constrain_int16(channel_yaw.get_radio_in(), channel_yaw.get_radio_min(), channel_yaw.get_radio_max()));
    channel_yaw.output();
}

/*
*  Called to determine if Manual CR Pitch has hit range limits
*/
void Tracker::check_manual_pitch_limits(int16_t pitch_pwm)
{
    //Check measured pitch angle of ahrs
    float ahrs_pitch = degrees(ahrs.pitch);
    //float pitch_range = g.pitch_range/2;	

    if (ahrs_pitch >= g.pitch_max) { //MAX LIMIT
        //Check if servo is reversed and limit servo in one direction only
        if (!channel_pitch.get_reverse() && (pitch_pwm < channel_pitch.get_radio_trim())) { //Normal Servo Direction
            pitch_lock = false;
        } else if (channel_pitch.get_reverse() && (pitch_pwm > channel_pitch.get_radio_trim())) { //Reversed Servo Direction
            pitch_lock = false;
            } else {
            pitch_lock_angle = g.pitch_max;
            pitch_lock = true; //Stop if not in a direction to clear violation
        }
        return;
    }

    if (ahrs_pitch <= g.pitch_min) { //MIN LIMIT
        //Check if servo is reversed and limit servo in one direction only
        if (!channel_pitch.get_reverse() && (pitch_pwm > channel_pitch.get_radio_trim())) { //Normal Servo Direction
            pitch_lock = false;
        } else if (channel_pitch.get_reverse() && (pitch_pwm < channel_pitch.get_radio_trim())) { //Reversed Servo Direction
            pitch_lock = false;
        } else {
            pitch_lock_angle = g.pitch_min;
            pitch_lock = true; //Stop if not in a direction to clear violation
        }
        return;
    }

    if (pitch_pwm > (channel_pitch.get_radio_trim() - 10) && pitch_pwm < (channel_pitch.get_radio_trim() + 10)) {
        if (!pitch_lock) {
        pitch_lock_angle = ahrs_pitch;
        pitch_lock = true;
        }
        return;
    } 

    pitch_lock = false; //Not in pitch violation
}