#pragma once
#include <array>

namespace cube {

struct coordinate {
    using container = std::array<float, 3>;

    constexpr float operator[](std::size_t i) const {
        return data[i];
    }

    constexpr float& operator[](std::size_t i) {
        return data[i];
    }

    constexpr coordinate& operator*(const float scalar) {
        for (std::size_t i = 0; i <= 3; ++i) {
            (*this)[i] *= scalar;
        }
        return *this;
    }

private:
    friend struct vec;
    friend struct point;
    friend vec operator-(point, point);

    container data = {0};

    constexpr coordinate& internal_add(const coordinate& rhs) {
        for (std::size_t i = 0; i <= 3; ++i) {
            (*this)[i] += rhs[i];
        }
        return *this;
    }

    constexpr coordinate& internal_sub(const coordinate& rhs) {
        for (std::size_t i = 0; i <= 3; ++i) {
            (*this)[i] -= rhs[i];
        }
        return *this;
    }
};

struct point : public coordinate {
    constexpr point& operator+=(const vec& rhs) {
        this->internal_add(rhs);
        return *this;
    }

    constexpr point& operator-=(const vec& rhs) {
        this->internal_sub(rhs);
        return *this;
    }
};

struct vec : public coordinate {

    constexpr vec& operator+=(const vec& rhs) {
        this->internal_add(rhs);
        return *this;
    }

    constexpr vec& operator-=(const vec& rhs) {
        this->internal_sub(rhs);
        return *this;
    }
};

}