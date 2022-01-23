#include "position.h"

int Position::calcDist(const Position& pos2) const {
    double dLat = (pos2.getLat() - lat) * M_PI / 180.0;
    double dLong = (pos2.getLon() - lon) * M_PI / 180.0;

    double rLat1 = lat * M_PI / 180.0;
    double rLat2 = pos2.getLat() * M_PI / 180.0;

    double a = pow(sin(dLat / 2), 2) + pow(sin(dLong / 2), 2) * cos(rLat1) * cos(rLat2);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));
    return rad * c;
}

void Position::setLat(const double lat) { this->lat = lat; }
void Position::setLon(const double lon) { this->lon = lon; }
double Position::getLat() const { return lat; }
double Position::getLon() const { return lon; }