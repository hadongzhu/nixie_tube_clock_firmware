#include "bsp.h"
#include "bsp_timer.h"
#include "colon.h"
#include "control_key.h"
#include "cron.h"
#include "display.h"
#include "key.h"
#include "led.h"
#include "nixie_tube.h"
#include "storage.h"
#include "tick.h"

int main(void)
{
    /* Initialize hardwares and configurations*/
    bsp_init();
    tick_controller_entity.sync(tick_controller_entity.resume());
    stroage_controller_entity.restore();
    control_key_init();

    /* Run the application */
    while (1)
    {
        cron_controller_entity.run();
        tick_controller_entity.run();
        display_controller_entity.run();
        nixie_tube_controller_entity.run(get_100us_time());
        colon_controller_entity.run(get_100us_time());
        led_controller_entity.run(get_100us_time());
        control_key_controller_entity.run();
        bsp_idle();
    }
}
