//
// Created by user on 6/07/2024.
//

#ifndef LEDCOORDINATE_H
#define LEDCOORDINATE_H
#include <Arduino.h>
class Led_Coordinate {
    const byte address=0;
    const byte row=0;
    const byte color=0;

public:
    Led_Coordinate(const byte address, const byte row, const byte color)
        : address(address),
          row(row),
          color(color) {
    }

    Led_Coordinate() = default;

    byte get_address() const {return address;}
    byte get_row() const {return row;}
    byte color_red() const {return color;}
    byte color_blue() const {return color+1;}
    byte color_green() const {return color+2;}

};

#endif //LEDCOORDINATE_H
