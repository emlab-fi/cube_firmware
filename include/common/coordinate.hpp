#pragma once
#include <array>
#include <initializer_list>

namespace cube {

struct vec;
struct point;

struct coordinate {
    using container = std::array<float, 3>;

    constexpr float operator[](std::size_t i) const {
        return data[i];
    }

    constexpr float& operator[](std::size_t i) {
        return data[i];
    }

    constexpr container& operator*() {
        return data;
    }

    ~coordinate() = default;

private:
    friend struct vec;
    friend struct point;
    friend vec operator-(const point& p1, const point& p2);

    container data = {0};

    constexpr void internal_add(const coordinate& rhs) {
        for (std::size_t i = 0; i < 3; ++i) {
            (*this)[i] += rhs[i];
        }
    }

    constexpr void internal_sub(const coordinate& rhs) {
        for (std::size_t i = 0; i < 3; ++i) {
            (*this)[i] -= rhs[i];
        }
    }

    constexpr void scale(const float scalar) {
        for (std::size_t i = 0; i < 3; ++i) {
            (*this)[i] *= scalar;
        }
    }
};

struct point : public coordinate {

    point() = default;

    point(const point& other) {
        this->data = other.data;
    }

    ~point() = default;

    constexpr point& operator+=(const vec& rhs) {
        this->internal_add(rhs);
        return *this;
    }

    constexpr point& operator-=(const vec& rhs) {
        this->internal_sub(rhs);
        return *this;
    }

    constexpr point& operator*=(const float rhs) {
        this->scale(rhs);
        return *this;
    }
};

struct vec : public coordinate {

    vec() = default;

    vec(const vec& other) {
        this->data = other.data;
    }

    ~vec() = default;

    constexpr vec& operator+=(const vec& rhs) {
        this->internal_add(rhs);
        return *this;
    }

    constexpr vec& operator-=(const vec& rhs) {
        this->internal_sub(rhs);
        return *this;
    }

    constexpr vec& operator*=(const float rhs) {
        this->scale(rhs);
        return *this;
    }
};

vec operator-(const point& p1, const point& p2) {
    vec out;
    out.data = p1.data;
    out.internal_sub(p2);
    return out;
}

vec operator+(vec lhs, const vec& rhs) {
    lhs += rhs;
    return lhs;
}

vec operator-(vec lhs, const vec& rhs) {
    lhs -= rhs;
    return lhs;
}

vec operator*(const float lhs, vec rhs) {
    rhs *= lhs;
    return rhs;
}

point operator+(point lhs, const vec& rhs) {
    lhs += rhs;
    return lhs;
}

point operator-(point lhs, const vec& rhs) {
    lhs += rhs;
    return lhs;
}

point operator*(const float lhs, point rhs) {
    rhs *= lhs;
    return rhs;
}


} //namespace cube