#ifndef _POSITION_H_
#define _POSITION_H_

#include <cmath>

class Position {
private:
    double lat, lon;
public:
    double calcDist(const Position& pos2) const;

    void setLat(const double lat);
    void setLon(const double lon);
    double getLat() const;
    double getLon() const;
};

#endif