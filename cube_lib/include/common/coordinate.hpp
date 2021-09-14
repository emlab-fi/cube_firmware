#pragma once
#include <array>
#include <initializer_list>
//#include <compare>

namespace cube {

struct vec;
struct point;

struct coordinate {
    static constexpr std::size_t dim = 3;
    using container = std::array<float, dim>;

    coordinate() = default;

    coordinate(const coordinate& other) = default;

    coordinate(coordinate&& other) = default;

    coordinate(const float x, const float y, const float z) : data({x, y, z}) {}

    coordinate(std::initializer_list<float> input) {
        auto iter = input.begin();
        for (auto i = data.begin(); i != data.end(); ++i) {
            *i = *iter;
            ++iter;
        }
    }

    ~coordinate() = default;

    coordinate& operator=(const coordinate& other) = default;

    coordinate& operator=(coordinate&& other) = default;

    //GCC 10 and ARM-GCC Q4-2020 cannot yet synthetize those
    //they might not act as we want them too though
    //friend std::strong_ordering operator<=>(const coordinate&, const coordinate&) = default;
    //friend bool operator==(const coordinate&, const coordinate&) = default;

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
    container data{0};
};

struct vec {

    vec() = default;

    vec(const vec& other) = default;

    vec(vec&& other) = default;

    vec(float x, float y, float z) : coords(x, y, z) {}

    vec(std::initializer_list<float> input) : coords(input) {}

    explicit vec(const point& p);

    explicit vec(point&& p);

    ~vec() = default;

    vec& operator=(const vec& other) = default;

    vec& operator=(vec&& other) = default;

    //GCC 10 and ARM-GCC Q4-2020 cannot yet synthetize those
    //they might not act as we want them too though
    //friend std::strong_ordering operator<=>(const vec&, const vec&) = default;
    //friend bool operator==(const vec&, const vec&) = default;

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

    point(const point& other) = default;

    point(point&& other) = default;

    point(float x, float y, float z) : coords(x, y, z) {}

    point(std::initializer_list<float> input) : coords(input) {}

    explicit point(const vec& v);

    explicit point(vec&& v);

    ~point() = default;

    point& operator=(const point& other) = default;

    point& operator=(point&& other) = default;

    //GCC 10 and ARM-GCC Q4-2020 cannot yet synthetize those
    //they might not act as we want them too though
    //friend std::strong_ordering operator<=>(const point&, const point&) = default;
    //friend bool operator==(const point&, const point&) = default;

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

vec operator-(const point& p1, const point& p2);

vec operator+(vec lhs, const vec& rhs);

vec operator-(vec lhs, const vec& rhs);

vec operator*(const float lhs, vec rhs);

point operator+(point lhs, const vec& rhs);

point operator-(point lhs, const vec& rhs);

point operator*(const float lhs, point rhs);


bool operator==(const vec& lhs, const vec& rhs);

bool operator==(const point& lhs, const point& rhs);


int operator<(const vec& lhs, const vec& rhs);
int operator>(const vec& lhs, const vec& rhs);

int operator<(const point& lhs, const point& rhs);
int operator>(const point& lhs, const point& rhs);

} //namespace cube