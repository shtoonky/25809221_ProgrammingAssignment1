#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include "rtweekend_utils.hpp"

class Interval {
    public: 
        double min, max;

        Interval() : min(+infinity), max(-infinity) {} // Default interval is empty
        Interval(double min, double max) : min(min), max(max) {}

        // bool size() const {
        //     return max - min;
        // }

        double Clamp(double x) const {
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }
};

#endif