#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include "stepper_generator.hpp"

TIM_HandleTypeDef mock_htim;
GPIO_TypeDef mock_gpio;
cube_hw::MotorPins mock_pins(mock_htim, TIM_CHANNEL_1, &mock_gpio, 0);

constexpr cube_hw::motor_config basicConfig = {
    ._const_speed_t = 30,
    ._section_t = 5,

    // example
    .start_v = 30,
    .end_v = 40,
    .speed_points = {70, 140, 170},
    .accelerations = {200, 600, 200},
    .decelerations = {250, 700, 300},

    // self.set_profile(10, 20, [40, 80, 100], [80, 150, 100], [80, 150, 100]) 
    // .start_v = 10,
    // .end_v = 20,
    // .speed_points = {40, 80, 100},
    // .accelerations = {80, 150, 100},
    // .decelerations = {80, 150, 100},

    // self.set_profile(30, 40, [60, 120, 150], [150, 450, 200], [200, 500, 250])
    // .start_v = 30,
    // .end_v = 40,
    // .speed_points = {60, 120, 150},
    // .accelerations = {150, 450, 200},
    // .decelerations = {200, 500, 250},

    // self.set_profile(50, 60, [80, 170, 200], [300, 700, 400], [350, 750, 450])  
    // .start_v = 50,
    // .end_v = 60,
    // .speed_points = {80, 170, 200},
    // .accelerations = {300, 700, 400},
    // .decelerations = {350, 750, 450},

    .max_speed = 1000,
    .max_acceleration = 2000,
    ._homing_velocity = 40,
    .min_velocity = 1.0f,
    .max_velocity = 60.0f,
};

enum class Modes {
    UNKNOWN = 0,
    SINGLE = 1,
    CORE_XY = 2,
    FAULTY = 0xff
};

struct GeneratorArgs {
    Modes mode = Modes::SINGLE;
    std::vector<int> steps;
};

void load_steps(int argc, char* argv[], std::vector<int>& data, int idx, int steps) {
    assert (steps >= 0);
    while (steps > 0) {
        ++idx;
        assert(idx < argc);
        std::string num_str = argv[idx];
        data.emplace_back(std::stoi(num_str));
        --steps;
    }
}

GeneratorArgs parse_args(int argc, char* argv[]) {
    GeneratorArgs result;
    for (int i = 0; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-m") {
            ++i;
            assert (i < argc); 
            std::string mode = argv[i];
            if (mode == "single") 
                result.mode = Modes::SINGLE;
            else if (mode == "corexy")
                result.mode = Modes::CORE_XY;
            else 
                result.mode = Modes::FAULTY;

        } else if (arg == "-s") {
            switch(result.mode) {
                case Modes::CORE_XY:
                    load_steps(argc, argv, result.steps, i, 2);
                    i += 2;
                    break;
                case Modes::SINGLE:
                    load_steps(argc, argv, result.steps, i, 1);
                    i += 1;
                    break;
                default:
                    result.mode = Modes::FAULTY;
            }
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
    GeneratorArgs args = parse_args(argc, argv);
    if (args.mode == Modes::UNKNOWN || args.mode == Modes::UNKNOWN || args.steps.size() == 0) {
        std::cout << "Please provide -m[mode] and according -s[steps] parameters\n";
        return 0;
    }

    cube_hw::StepperGenerator generator(mock_pins, 1600, basicConfig);
    generator.start_tim_base();

    std::cout << "{\n";
    if (args.mode == Modes::SINGLE) { 
        std::cout << "\"data\": ";
        generator.prepare_dma(args.steps[0]);
        generator.start();
    }
    else if (args.mode == Modes::CORE_XY) {
        const auto x_abs = std::abs(args.steps[0]);
        const auto y_abs = std::abs(args.steps[1]);
        float ratio = x_abs > y_abs ? static_cast<float>(y_abs) / x_abs : static_cast<float>(x_abs) / y_abs;

        std::cout << "\"data_x\": ";
        generator.prepare_dma(args.steps[0], x_abs > y_abs ? 1.0 : ratio);
        generator.start();
        generator.finished_callback();

        std::cout << ",\n\"data_y\": ";
        generator.prepare_dma(args.steps[1], y_abs > x_abs ? 1.0 : ratio);
        generator.start();
    }
    std::cout << "\n}";

    return 0;
}
