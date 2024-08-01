#ifndef POS_COORDINATE_H
#define POS_COORDINATE_H
class Pos_Coordinate {
    unsigned long x=0;
    unsigned long y=0;
    unsigned long z=0;
public:
    Pos_Coordinate(const unsigned long &x, const unsigned long &y, const unsigned long &z)
        : x(x), y(y), z(z) {}
    Pos_Coordinate() = default;

    unsigned long & get_x() {return x;}
    unsigned long & get_y() {return y;}
    unsigned long & get_z() {return z;}

    void set_x(const unsigned long &x) {this->x = x;}
    void set_y(const unsigned long &y) {this->y = y;}
    void set_z(const unsigned long &z) {this->z = z;}
};
#endif //POS_COORDINATE_H
