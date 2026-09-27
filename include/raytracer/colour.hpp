#ifndef COLOUR_HPP
#define COLOUR_HPP

#include "interval.hpp"
#include "vec3.hpp"

using Colour = Vec3;

inline void WriteColour(std::ostream& out, const Colour& pixel_colour) {
    auto r = pixel_colour.x();
    auto g = pixel_colour.y();
    auto b = pixel_colour.z();

    // Translate [0, 1] component values to the byte range [0, 255].
    static const Interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.Clamp(r));
    int gbyte = int(256 * intensity.Clamp(g));
    int bbyte = int(256 * intensity.Clamp(b));

    // Write out the pixel colour components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif