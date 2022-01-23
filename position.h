#ifndef _POSITION_H_
#define _POSITION_H_

#include <cmath>

class Position {
private:
    double lat, long;
public:
    int calcDist(const Position& pos2) const;

    void setLat(const int lat);
    void setLong(const int long);
    int getLat() const;
    int getLong() const;
};

#endif