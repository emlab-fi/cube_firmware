#include "common/coordinate.hpp"

namespace cube {

vec::vec(const point& p) : coords(p.coords) {}

point::point(const vec& v) : coords(v.coords) {}

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

}