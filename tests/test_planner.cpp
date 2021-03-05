#include "catch2/catch.hpp"
#include "common/coordinate.hpp"
#include "planner.hpp"

TEST_CASE("Basic planner test") {
    cube::planner_config conf = {
        .machine = cube::planner_machine::cartesian,
        .bounds = cube::point(5.5f, 5.5f, 3.5f),
        .step_resolution_x = 10U,
        .step_resolution_y = 10U,
        .step_resolution_z = 10U
    };

    cube::planner plan = cube::planner(conf);

    auto result = plan.do_move(cube::point(1.0, 1.0, 1.0));
    REQUIRE(result.err == cube::planner_error::none);
    REQUIRE(result.steps_a == 10);
    REQUIRE(result.steps_b == 10);
    REQUIRE(result.steps_c == 10);
}