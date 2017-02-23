#include "Tracker.h"

/*
 * Stop control mode
 */

/*
 * update_auto - runs the auto controller
 *  called at 50hz while control_mode is 'STOP'
 */
void Tracker::update_stop(void)
{
    if ((enum ServoType)g.servo_pitch_type.get() == SERVO_TYPE_CR) {
	    //Pitch control is stopped
	    if (!pitch_lock) {
            //Check measured pitch angle of ahrs
	        float ahrs_pitch = degrees(ahrs.pitch);

		    pitch_lock_angle = ahrs_pitch;
		    pitch_lock = true;
	    }

		if (pitch_lock) {
            float pitch = pitch_lock_angle + g.pitch_trim;
			update_pitch_servo(pitch);
		}
    }

	if ((enum ServoType)g.servo_yaw_type.get() == SERVO_TYPE_CR) {
        SRV_Channels::set_output_pwm(SRV_Channel::k_tracker_yaw, RC_Channels::rc_channel(CH_YAW)->get_radio_trim());
        SRV_Channels::constrain_pwm(SRV_Channel::k_tracker_yaw);
	}
}