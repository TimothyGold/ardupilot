#include "Tracker.h"

// Functions to read the RC radio input

void Tracker::read_radio()
{
    if (hal.rcin->new_input()) {
<<<<<<< HEAD
        channel_yaw.set_pwm(hal.rcin->read(CH_YAW));
        channel_pitch.set_pwm(hal.rcin->read(CH_PITCH));
        channel_mode.set_pwm(hal.rcin->read(CH_MODE));
=======
        RC_Channels::set_pwm_all();
>>>>>>> refs/remotes/ArduPilot/master
    }
}
