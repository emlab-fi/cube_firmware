#pragma once
#include "planner.hpp"
#include "TMC429.hpp"
#include "peripherals.h"
#include "fsl_dspi.h"

TMC429_Driver::TMC429* steppers_ptr = nullptr;


TMC429_Driver::Status SPITransfer(uint8_t* tx, uint8_t *rx, uint8_t length) {
    dspi_transfer_t transfer;
    transfer.rxData = rx;
    transfer.txData = tx;
    transfer.dataSize = length;
    transfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcsContinuous;

    status_t res = DSPI_MasterTransferBlocking(SPI0_PERIPHERAL, &transfer);
    if (res != kStatus_Success) {
        return TMC429_Driver::Status::fail;
    }
    return TMC429_Driver::Status::success;
}


cube::planner_config planner_conf = {
        .machine = cube::planner_machine::corexy,
        .bounds = cube::point(150.0f, 150.0f, 150.0f),
        .step_resolution_a = 400U,
        .step_resolution_b = 400U,
        .step_resolution_c = 1600U
};

TMC429_Driver::Config tmc429_conf = {
    .transfer = SPITransfer,
    .steppers = {
        {  // Stepper 1
            .v_min = 1U,
            .v_max = 1573U,
            .a_max = 1320U,
            .pmul = 163U,
            .pdiv = 6U,
            .ramp_div = 9U,
            .pulse_div = 4U,
        },
        {  // Stepper 2
            .v_min = 1U,
            .v_max = 1573U,
            .a_max = 1320U,
            .pmul = 163U,
            .pdiv = 6U,
            .ramp_div = 9U,
            .pulse_div = 4U,
        },
        {  // Stepper 3
            .v_min = 1U,
            .v_max = 1573U,
            .a_max = 1320U,
            .pmul = 163U,
            .pdiv = 7U,
            .ramp_div = 9U,
            .pulse_div = 4U,
        }
    }
};
