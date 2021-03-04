#pragma once
#include <array>
#include <initializer_list>

namespace cube {

struct vec;
struct point;

struct coordinate {
    static constexpr std::size_t dim = 3;
    using container = std::array<float, dim>;

    coordinate() : data({0, 0, 0}) {}

    coordinate(const coordinate& other) : data(other.data) {}

    coordinate(const float x, const float y, const float z) : data({x, y, z}) {}

    coordinate(std::initializer_list<float> input) {
        auto iter = input.begin();
        for (auto i = data.begin(); i != data.end(); ++i) {
            *i = *iter;
            ++iter;
        }
    }

    ~coordinate() = default;

    constexpr float operator[](std::size_t i) const {
        return data[i];
    }

    constexpr float& operator[](std::size_t i) {
        return data[i];
    }

    constexpr coordinate& operator+=(const coordinate& rhs) {
        for (std::size_t i = 0; i < dim; ++i) {
            (*this)[i] += rhs[i];
        }
        return *this;
    }

    constexpr coordinate& operator-=(const coordinate& rhs) {
        for (std::size_t i = 0; i < dim; ++i) {
            (*this)[i] -= rhs[i];
        }
        return *this;
    }

    constexpr coordinate& operator*=(const float scalar) {
        for (std::size_t i = 0; i < dim; ++i) {
            (*this)[i] *= scalar;
        }
        return *this;
    }

private:
    friend struct vec;
    friend struct point;
    container data;
};

struct vec {

    vec() = default;

    vec(float x, float y, float z) : coords(x, y, z) {}

    vec(std::initializer_list<float> input) : coords(input) {}

    ~vec() = default;

    constexpr float operator[](std::size_t i) const {
        return coords[i];
    }

    constexpr float& operator[](std::size_t i) {
        return coords[i];
    }

    constexpr vec& operator+=(const vec& rhs) {
        coords += rhs.coords;
        return *this;
    }

    constexpr vec& operator-=(const vec& rhs) {
        coords -= rhs.coords;
        return *this;
    }

    constexpr vec& operator*=(const float scalar) {
        coords *= scalar;
        return *this;
    }

private:
    friend struct point;
    friend vec operator-(const point& p1, const point& p2);
    coordinate coords;
};

struct point {

    point() = default;

    point(float x, float y, float z) : coords(x, y, z) {}

    point(std::initializer_list<float> input) : coords(input) {}

    ~point() = default;

    constexpr float operator[](std::size_t i) const {
        return coords[i];
    }

    constexpr float& operator[](std::size_t i) {
        return coords[i];
    }

    constexpr point& operator+=(const vec& rhs) {
        coords += rhs.coords;
        return *this;
    }

    constexpr point& operator-=(const vec& rhs) {
        coords -= rhs.coords;
        return *this;
    }

    constexpr point& operator*=(const float scalar) {
        coords *= scalar;
        return *this;
    }

private:
    friend struct vec;
    friend vec operator-(const point& p1, const point& p2);
    coordinate coords;
};

vec operator-(const point& p1, const point& p2) {
    vec out(p1[0], p1[1], p1[2]);
    out.coords -= p2.coords;
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
    lhs -= rhs;
    return lhs;
}

point operator*(const float lhs, point rhs) {
    rhs *= lhs;
    return rhs;
}

bool operator==(const vec& lhs, const vec& rhs) {
    for (int i = 0; i < 3; ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

bool operator==(const point& lhs, const point& rhs) {
    for (int i = 0; i < 3; ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

//most probably mathematically incorrect, but suitable for our uses
int operator<=>(const vec& lhs, const vec& rhs) {
    for (int i = 0; i < 3; ++i) {
        if (lhs[i] < rhs[i]) {
            return -1;
        }
        if (lhs[i] > rhs[i]) {
            return 1;
        }
    }
    return 0;
}

int operator<=>(const point& lhs, const point& rhs) {
    for (int i = 0; i < 3; ++i) {
        if (lhs[i] < rhs[i]) {
            return -1;
        }
        if (lhs[i] > rhs[i]) {
            return 1;
        }
    }
    return 0;
}

} //namespace cube