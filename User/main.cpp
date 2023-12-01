#include "bsp.h"
#include "bsp_timer.h"
#include "colon.h"
#include "control_key.h"
#include "display.h"
#include "led.h"
#include "nixie_tube.h"
#include "tick.h"
#include "storage.h"

int main(void)
{
    /* Initialize hardwares and configurations*/
    bsp_init();
    stroage::restore_setting();
    _tick_controller.resume();
    _tick_controller.sync();
    control_key_init();


    /* Run the application */
    while (1)
    {
        _tick_controller.run();
        _display_controller.run();
        _nixie_tube_controller.run(get_100us_time());
        _colon_controler.run(get_100us_time());
        _led_controller.run(get_100us_time());
        _control_key_controller.run();
        bsp_idle();
    }
}
