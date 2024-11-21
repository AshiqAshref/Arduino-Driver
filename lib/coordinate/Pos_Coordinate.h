#ifndef POS_COORDINATE_H
#define POS_COORDINATE_H
class Pos_Coordinate {
    unsigned long x_=0;
    unsigned long y_=0;
    unsigned long z_=0;
public:
    Pos_Coordinate(const unsigned long &x, const unsigned long &y, const unsigned long &z)
        : x_(x), y_(y), z_(z) {}
    Pos_Coordinate() = default;

    unsigned long x() const{return x_;}
    unsigned long y() const{return y_;}
    unsigned long z() const{return z_;}

};
#endif //POS_COORDINATE_H
