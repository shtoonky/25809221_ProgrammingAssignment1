#ifndef COLOUR_HPP
#define COLOUR_HPP

#include "vec3.hpp"

using Colour = Vec3;

inline void WriteColour(std::ostream& out, const Colour& pixel_colour) {
    auto r = pixel_colour.x();
    auto g = pixel_colour.y();
    auto b = pixel_colour.z();

    // Translate [0, 1] component values to the byte range [0, 255].
    int rbyte = int(255.999 * r); // ints round towards zero, so 255.999 provides an extra bucket for the inital values to be mapped to
    int gbyte = int(255.999 * g); // if it was only 255, we would only get rbyte = 255 when exactly r = 1.0
    int bbyte = int(255.999 * b);

    // Write out the pixel colour components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif