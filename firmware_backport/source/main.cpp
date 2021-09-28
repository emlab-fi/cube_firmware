#include "fsl_device_registers.h"
#include "board.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "clock_config.h"
#include "TMC429.hpp"

#include "config.hpp"
#include "cube.hpp"


/*
Entry point
*/
int main(void)
{
    // Init board hardware.
    BOARD_InitBootPins();
    BOARD_InitBootPeripherals();
    BOARD_BootClockRUN();

    cube::cube_main cube_core{planner_conf};
    TMC429_Driver::TMC429 steppers{tmc429_conf};
    steppers.setPosition(0, 0, 0);
    steppers_ptr = &steppers;

    cube_hw::init_hardware();
    cube_hw::log_info("System initialized!\n");
    cube_hw::set_motor_power(false);

    while (1)
    {
        cube_core.main_loop();
    }
}
