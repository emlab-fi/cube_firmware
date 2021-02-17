#include <type_traits>
#include "catch2/catch.hpp"
#include "common/coordinate.hpp"

/*
Mostly numeric tests so that I know I did not fuck up any +/- anywhere
*/

TEST_CASE("coordinate operations") {

    cube::coordinate a{1.0, 2.0, 3.0};
    cube::coordinate b{{1.1, 2.1, 3.1}};
    cube::coordinate c;

    SECTION("indexing") {
        REQUIRE(a[0] == 1.0f);
        REQUIRE(a[1] == 2.0f);
        REQUIRE(a[2] == 3.0f);
        REQUIRE(b[2] == 3.1f);
        REQUIRE(c[1] == 0.0f);
    }
    SECTION("addition") {
        c += a;
        REQUIRE(c[0] == Approx(1.0f));
        REQUIRE(c[1] == Approx(2.0f));
        REQUIRE(c[2] == Approx(3.0f));
    }

    SECTION("substraction") {
        b -= a;
        REQUIRE(b[0] == Approx(0.1f));
        REQUIRE(b[1] == Approx(0.1f));
        REQUIRE(b[2] == Approx(0.1f));

    }
    SECTION("scalar multiplication") {
        a *= 2;
        REQUIRE(a[0] == Approx(2.0f));
        REQUIRE(a[1] == Approx(4.0f));
        REQUIRE(a[2] == Approx(6.0f));
    }
}

TEST_CASE("point operations") {

    cube::point a(1.0, 2.0, 3.0);
    cube::vec b({1.1, 2.1, 3.1});
    cube::point c;

    SECTION("indexing") {
        REQUIRE(a[0] == 1.0f);
        REQUIRE(a[1] == 2.0f);
        REQUIRE(a[2] == 3.0f);
        REQUIRE(b[2] == 3.1f);
        REQUIRE(c[1] == 0.0f);
    }
    SECTION("vector addition") {
        c += b;
        REQUIRE(b[0] == Approx(1.1f));
        REQUIRE(b[1] == Approx(2.1f));
        REQUIRE(b[2] == Approx(3.1f));

        auto d = a + b;
        REQUIRE(std::is_same_v<decltype(d), cube::point>);
        REQUIRE(d[0] == Approx(2.1f));
        REQUIRE(d[1] == Approx(4.1f));
        REQUIRE(d[2] == Approx(6.1f));
    }
    SECTION("vector substraction") {
        a -= b;
        REQUIRE(a[0] == Approx(-0.1f));
        REQUIRE(a[1] == Approx(-0.1f));
        REQUIRE(a[2] == Approx(-0.1f));

        auto d = a - b;
        REQUIRE(std::is_same_v<decltype(d), cube::point>);
        REQUIRE(a[0] == Approx(-0.1f));
        REQUIRE(a[1] == Approx(-0.1f));
        REQUIRE(a[2] == Approx(-0.1f));
    }
    SECTION("point substraction") {
        auto d = c - a;
        REQUIRE(std::is_same_v<decltype(d), cube::vec>);
        REQUIRE(d[0] == Approx(-1.0f));
        REQUIRE(d[1] == Approx(-2.0f));
        REQUIRE(d[2] == Approx(-3.0f));
    }
    SECTION("scalar multiplication") {
        a *= 2;
        REQUIRE(a[0] == Approx(2.0f));
        REQUIRE(a[1] == Approx(4.0f));
        REQUIRE(a[2] == Approx(6.0f));

        auto d = -1 * a;
        REQUIRE(std::is_same_v<decltype(d), cube::point>);
        REQUIRE(d[0] == Approx(-2.0f));
        REQUIRE(d[1] == Approx(-4.0f));
        REQUIRE(d[2] == Approx(-6.0f));
    }
}

TEST_CASE("vector operations") {

    cube::vec a(1.0, 2.0, 3.0);
    cube::vec b({1.1, 2.1, 3.1});
    cube::vec c;

    SECTION("indexing") {
        REQUIRE(a[0] == 1.0f);
        REQUIRE(a[1] == 2.0f);
        REQUIRE(a[2] == 3.0f);
        REQUIRE(b[2] == 3.1f);
        REQUIRE(c[1] == 0.0f);
    }
    SECTION("vector addition") {
        c += b;
        REQUIRE(b[0] == Approx(1.1f));
        REQUIRE(b[1] == Approx(2.1f));
        REQUIRE(b[2] == Approx(3.1f));

        auto d = a + b;
        REQUIRE(std::is_same_v<decltype(d), cube::vec>);
        REQUIRE(d[0] == Approx(2.1f));
        REQUIRE(d[1] == Approx(4.1f));
        REQUIRE(d[2] == Approx(6.1f));
    }
    SECTION("vector substraction") {
        a -= b;
        REQUIRE(a[0] == Approx(-0.1f));
        REQUIRE(a[1] == Approx(-0.1f));
        REQUIRE(a[2] == Approx(-0.1f));

        auto d = b - a;
        REQUIRE(std::is_same_v<decltype(d), cube::vec>);
        REQUIRE(d[0] == Approx(1.2f));
        REQUIRE(d[1] == Approx(2.2f));
        REQUIRE(d[2] == Approx(3.2f));
    }
    SECTION("point addition") {
        cube::point d(2.0, 2.0, 2.0);

        d += c;
        REQUIRE(d[0] == Approx(2.0f));
        REQUIRE(d[1] == Approx(2.0f));
        REQUIRE(d[2] == Approx(2.0f));

        auto e = d + a;
        REQUIRE(std::is_same_v<decltype(d), cube::point>);
        REQUIRE(e[0] == Approx(3.0f));
        REQUIRE(e[1] == Approx(4.0f));
        REQUIRE(e[2] == Approx(5.0f));
    }
    SECTION("point substraction") {
        cube::point d(2.0, 2.0, 2.0);

        d -= b;
        REQUIRE(d[0] == Approx(0.9f));
        REQUIRE(d[1] == Approx(-0.1f));
        REQUIRE(d[2] == Approx(-1.1f));

        auto e = d - a;
        REQUIRE(std::is_same_v<decltype(d), cube::point>);
        REQUIRE(e[0] == Approx(-0.1f));
        REQUIRE(e[1] == Approx(-2.1f));
        REQUIRE(e[2] == Approx(-4.1f));
    }
    SECTION("scalar multiplication") {
            a *= 2;
        REQUIRE(a[0] == Approx(2.0f));
        REQUIRE(a[1] == Approx(4.0f));
        REQUIRE(a[2] == Approx(6.0f));

        auto d = -1 * a;
        REQUIRE(std::is_same_v<decltype(d), cube::vec>);
        REQUIRE(d[0] == Approx(-2.0f));
        REQUIRE(d[1] == Approx(-4.0f));
        REQUIRE(d[2] == Approx(-6.0f));
    }

}
