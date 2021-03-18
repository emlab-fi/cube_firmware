#include "catch2/catch.hpp"
#include "common/coordinate.hpp"
#include "planner.hpp"
#include <iostream>

void check_approx_position(cube::point actual, cube::point target) {
    REQUIRE(actual[0] == Approx(target[0]));
    REQUIRE(actual[1] == Approx(target[1]));
    REQUIRE(actual[2] == Approx(target[2]));
}

void check_position(cube::point actual, cube::point target) {
    REQUIRE(actual[0] == target[0]);
    REQUIRE(actual[1] == target[1]);
    REQUIRE(actual[2] == target[2]);
}

TEST_CASE("Basic planner ops") {
    cube::planner_config conf = {
        .machine = cube::planner_machine::cartesian,
        .bounds = cube::point(5.5f, 5.5f, 3.5f),
        .step_resolution_a = 10U,
        .step_resolution_b = 10U,
        .step_resolution_c = 10U
    };

    cube::planner plan = cube::planner(conf);

    auto result = plan.do_move(cube::point(1.0, 1.0, 1.0));
    REQUIRE(result.err == cube::planner_error::none);
    REQUIRE(result.steps_a == 10);
    REQUIRE(result.steps_b == 10);
    REQUIRE(result.steps_c == 10);

    check_position(plan.get_absolute_pos(), cube::point(1.0, 1.0, 1.0));

    result = plan.do_move(cube::point(6.0, 6.0, 6.0));
    REQUIRE(result.err == cube::planner_error::out_of_bounds);

    result = plan.do_move(cube::point(0.0, 0.0, 1.0));
    REQUIRE(result.err == cube::planner_error::none);
    REQUIRE(result.steps_a == -10);
    REQUIRE(result.steps_b == -10);
    REQUIRE(result.steps_c == 0);
}

TEST_CASE("Planner for cartesian machine") {
    SECTION("scaling 10") {
        cube::planner_config conf = {
            .machine = cube::planner_machine::cartesian,
            .bounds = cube::point(10.0f, 10.0f, 10.0f),
            .step_resolution_a = 10U,
            .step_resolution_b = 10U,
            .step_resolution_c = 10U
        };

        cube::planner plan = cube::planner(conf);

        auto result = plan.do_move(cube::point(2.5, 1.37, 1.413));
        REQUIRE(result.err == cube::planner_error::none);
        REQUIRE(result.steps_a == 25);
        REQUIRE(result.steps_b == 13);
        REQUIRE(result.steps_c == 14);
    }
    SECTION("scaling 123") {
        cube::planner_config conf = {
            .machine = cube::planner_machine::cartesian,
            .bounds = cube::point(10.0f, 10.0f, 10.0f),
            .step_resolution_a = 123U,
            .step_resolution_b = 123U,
            .step_resolution_c = 123U
        };

        cube::planner plan = cube::planner(conf);

        auto result = plan.do_move(cube::point(2.5, 1.37, 1.413));
        REQUIRE(result.err == cube::planner_error::none);
        REQUIRE(result.steps_a == 307);
        REQUIRE(result.steps_b == 168);
        REQUIRE(result.steps_c == 173);
    }
}

TEST_CASE("Planner for corexy machine") {
    SECTION("diagonal moves") {
        cube::planner_config conf = {
            .machine = cube::planner_machine::corexy,
            .bounds = cube::point(10.0f, 10.0f, 10.0f),
            .step_resolution_a = 10U,
            .step_resolution_b = 10U,
            .step_resolution_c = 10U
        };

        cube::planner plan = cube::planner(conf);

        auto result = plan.do_move(cube::point(5, 5, 0));
        REQUIRE(result.err == cube::planner_error::none);
        REQUIRE(result.steps_a == 50);
        REQUIRE(result.steps_b == 0);

        result = plan.do_move(cube::point(10, 0, 0));
        REQUIRE(result.err == cube::planner_error::none);
        REQUIRE(result.steps_a == 0);
        REQUIRE(result.steps_b == 50);
    }
    SECTION("linear moves") {
        cube::planner_config conf = {
            .machine = cube::planner_machine::corexy,
            .bounds = cube::point(10.0f, 10.0f, 10.0f),
            .step_resolution_a = 10U,
            .step_resolution_b = 10U,
            .step_resolution_c = 10U
        };

        cube::planner plan = cube::planner(conf);

        auto result = plan.do_move(cube::point(5, 0, 0));
        REQUIRE(result.err == cube::planner_error::none);
        REQUIRE(result.steps_a == 25);
        REQUIRE(result.steps_b == 25);

        result = plan.do_move(cube::point(5, 5, 0));
        REQUIRE(result.err == cube::planner_error::none);
        REQUIRE(result.steps_a == 25);
        REQUIRE(result.steps_b == -25);
    }
}

TEST_CASE("Proper bounds handling") {
    cube::planner_config conf = {
        .machine = cube::planner_machine::cartesian,
        .bounds = cube::point(10.0f, 10.0f, 10.0f),
        .step_resolution_a = 10U,
        .step_resolution_b = 10U,
        .step_resolution_c = 10U
    };
    cube::planner plan = cube::planner(conf);

    auto result = plan.do_move(cube::point(5.0, 5.0, 5.0));
    REQUIRE(result.err == cube::planner_error::none);
    check_position(plan.get_absolute_pos(), cube::point(5.0, 5.0, 5.0));

    SECTION("non-relative input (zero_pos == {0})") {
        result = plan.do_move(cube::point(11.0, 11.0, 11.0));
        REQUIRE(result.err == cube::planner_error::out_of_bounds);
        check_position(plan.get_absolute_pos(), cube::point(5.0, 5.0, 5.0));

        result = plan.do_move(cube::point(-1.0, -1.0, -1.0));
        REQUIRE(result.err == cube::planner_error::out_of_bounds);
        check_position(plan.get_absolute_pos(), cube::point(5.0, 5.0, 5.0));

        result = plan.do_move(cube::point(10.0, 10.0, 10.0));
        REQUIRE(result.err == cube::planner_error::none);
        check_position(plan.get_absolute_pos(), cube::point(10.0, 10.0, 10.0));

        result = plan.do_move(cube::point(0.0, 0.0, 0.0));
        REQUIRE(result.err == cube::planner_error::none);
        check_position(plan.get_absolute_pos(), cube::point(0.0, 0.0, 0.0));
    }

    SECTION("relative input") {
        plan.set_zero_pos();

        result = plan.do_move(cube::point(6.0, 6.0, 6.0));
        REQUIRE(result.err == cube::planner_error::out_of_bounds);
        check_position(plan.get_absolute_pos(), cube::point(5.0, 5.0, 5.0));

        result = plan.do_move(cube::point(-6.0, -6.0, -6.0));
        REQUIRE(result.err == cube::planner_error::out_of_bounds);
        check_position(plan.get_absolute_pos(), cube::point(5.0, 5.0, 5.0));

        result = plan.do_move(cube::point(5.0, 5.0, 5.0));
        REQUIRE(result.err == cube::planner_error::none);
        check_position(plan.get_absolute_pos(), cube::point(10.0, 10.0, 10.0));

        result = plan.do_move(cube::point(-5.0, -5.0, -5.0));
        REQUIRE(result.err == cube::planner_error::none);
        check_position(plan.get_absolute_pos(), cube::point(0.0, 0.0, 0.0));
    }
}

TEST_CASE("Cylindrical input handling") {
    cube::planner_config conf = {
        .machine = cube::planner_machine::cartesian,
        .bounds = cube::point(10.0f, 10.0f, 10.0f),
        .step_resolution_a = 10U,
        .step_resolution_b = 10U,
        .step_resolution_c = 10U
    };
    cube::planner plan = cube::planner(conf);
    plan.do_move(cube::point(5.0, 5.0, 5.0));
    plan.set_zero_pos();
    plan.set_mode(cube::planner_mode::cylindrical);
    check_position(plan.get_relative_pos(), cube::point(0.0, 0.0, 0.0));

    SECTION("Out of bounds") {
        auto result = plan.do_move(cube::point(10.0, -M_PI/4, 3.8));
        REQUIRE(result.err == cube::planner_error::out_of_bounds);
        check_position(plan.get_relative_pos(), cube::point(0.0, 0.0, 0.0));

        result = plan.do_move(cube::point(-10.0, M_PI/4, 1.2));
        REQUIRE(result.err == cube::planner_error::out_of_bounds);
        check_position(plan.get_relative_pos(), cube::point(0.0, 0.0, 0.0));
    }

    SECTION("Simple move r") {
        auto result = plan.do_move(cube::point(3.0, 0.0, 0.0));
        REQUIRE(result.err == cube::planner_error::none);
        plan.set_mode(cube::planner_mode::cartesian);
        check_approx_position(plan.get_absolute_pos(), cube::point(8.0, 5.0, 5.0));

        plan.set_mode(cube::planner_mode::cylindrical);
        result = plan.do_move(cube::point(-3.0, 0.0, 0.0));
        REQUIRE(result.err == cube::planner_error::none);
        check_approx_position(plan.get_absolute_pos(), cube::point(2.0, 5.0, 5.0));
    }

    SECTION("Simple move theta") {
        auto result = plan.do_move(cube::point(1.0, M_PI/2, 0.0));
        REQUIRE(result.err == cube::planner_error::none);
        plan.set_mode(cube::planner_mode::cartesian);
        check_approx_position(plan.get_absolute_pos(), cube::point(5.0, 6.0, 5.0));

        plan.set_mode(cube::planner_mode::cylindrical);
        result = plan.do_move(cube::point(1.0, -M_PI/2, 0.0));
        REQUIRE(result.err == cube::planner_error::none);
        check_approx_position(plan.get_absolute_pos(), cube::point(5.0, 4.0, 5.0));

    }

    SECTION("Simple move z") {
        auto result = plan.do_move(cube::point(0.0, 0.0, 3.0));
        REQUIRE(result.err == cube::planner_error::none);
        plan.set_mode(cube::planner_mode::cartesian);
        check_approx_position(plan.get_absolute_pos(), cube::point(5.0, 5.0, 8.0));

        plan.set_mode(cube::planner_mode::cylindrical);
        result = plan.do_move(cube::point(0.0, 0.0, -3.0));
        REQUIRE(result.err == cube::planner_error::none);
        check_approx_position(plan.get_absolute_pos(), cube::point(5.0, 5.0, 2.0));
    }

    SECTION("Combined moves") {
        auto result = plan.do_move(cube::point(1.5, M_PI/4, 1));
        REQUIRE(result.err == cube::planner_error::none);
        plan.set_mode(cube::planner_mode::cartesian);
        check_approx_position(plan.get_absolute_pos(), cube::point(6.06066, 6.06066, 6.0));

        plan.set_mode(cube::planner_mode::cylindrical);
        result = plan.do_move(cube::point(-1.25, 3*M_PI/4, -1.2));
        REQUIRE(result.err == cube::planner_error::none);
        check_approx_position(plan.get_absolute_pos(), cube::point(5.88388, 4.11612, 3.8));
    }
}

TEST_CASE("Spherical input handling") {
    cube::planner_config conf = {
        .machine = cube::planner_machine::cartesian,
        .bounds = cube::point(10.0f, 10.0f, 10.0f),
        .step_resolution_a = 10U,
        .step_resolution_b = 10U,
        .step_resolution_c = 10U
    };
    cube::planner plan = cube::planner(conf);
    plan.do_move(cube::point(5.0, 5.0, 5.0));
    plan.set_zero_pos();
    plan.set_mode(cube::planner_mode::spherical);
    check_position(plan.get_relative_pos(), cube::point(0.0, 0.0, 0.0));

    SECTION("Out of bounds") {
        auto result = plan.do_move(cube::point(10.0, 1.2, 3.8));
        REQUIRE(result.err == cube::planner_error::out_of_bounds);
        check_position(plan.get_relative_pos(), cube::point(0.0, 0.0, 0.0));

        result = plan.do_move(cube::point(-10.0, M_PI/4, 1.2));
        REQUIRE(result.err == cube::planner_error::out_of_bounds);
        check_position(plan.get_relative_pos(), cube::point(0.0, 0.0, 0.0));
    }

    SECTION("Simple move r") {
        auto result = plan.do_move(cube::point(3.0, 0.0, 0.0));
        REQUIRE(result.err == cube::planner_error::none);
        plan.set_mode(cube::planner_mode::cartesian);
        check_approx_position(plan.get_absolute_pos(), cube::point(5.0, 5.0, 8.0));

        plan.set_mode(cube::planner_mode::spherical);
        result = plan.do_move(cube::point(-3.0, 0.0, 0.0));
        REQUIRE(result.err == cube::planner_error::none);
        check_approx_position(plan.get_absolute_pos(), cube::point(5.0, 5.0, 2.0));
    }

    SECTION("Simple move theta") {
        auto result = plan.do_move(cube::point(1.0, M_PI/2, 0.0));
        REQUIRE(result.err == cube::planner_error::none);
        plan.set_mode(cube::planner_mode::cartesian);
        check_approx_position(plan.get_absolute_pos(), cube::point(6.0, 5.0, 5.0));

        plan.set_mode(cube::planner_mode::spherical);
        result = plan.do_move(cube::point(1.0, -M_PI/2, 0.0));
        REQUIRE(result.err == cube::planner_error::none);
        check_approx_position(plan.get_absolute_pos(), cube::point(4.0, 5.0, 5.0));
    }

    SECTION("Simple move phi") {
        auto result = plan.do_move(cube::point(1.0, 0.0, M_PI/2));
        REQUIRE(result.err == cube::planner_error::none);
        plan.set_mode(cube::planner_mode::cartesian);
        check_approx_position(plan.get_absolute_pos(), cube::point(5.0, 5.0, 6.0));

        plan.set_mode(cube::planner_mode::spherical);
        result = plan.do_move(cube::point(1.0, 0.0, -M_PI/2));
        REQUIRE(result.err == cube::planner_error::none);
        check_approx_position(plan.get_absolute_pos(), cube::point(5.0, 5.0, 6.0));
    }

    SECTION("Combined moves") {
        auto result = plan.do_move(cube::point(1.5, -M_PI/4, M_PI/4));
        REQUIRE(result.err == cube::planner_error::none);
        plan.set_mode(cube::planner_mode::cartesian);
        check_approx_position(plan.get_absolute_pos(), cube::point(4.25, 4.25, 6.06066));

        plan.set_mode(cube::planner_mode::spherical);
        result = plan.do_move(cube::point(-1.25, M_PI/4, -M_PI/2));
        REQUIRE(result.err == cube::planner_error::none);
        check_approx_position(plan.get_absolute_pos(), cube::point(5.0, 5.88388, 4.11612));
    }
}