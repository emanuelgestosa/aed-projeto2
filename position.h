#ifndef _POSITION_H_
#define _POSITION_H_

#include <cmath>

/**
 * @brief Contains information about a position
 */
class Position {
private:
    double lat, lon;
public:
    /**
     * Calculate the distance between two
     * positions using Haversine's formula.
     * 
     * @param pos2 The position to calculate the distance to.
     * 
     * @return The distance between the two points in km.
     */
    double calcDist(const Position& pos2) const;

    /**
     * Sets the latitute.
     * 
     * @param lat The new latitutde.
     */
    void setLat(const double lat);

    /**
     * Sets the longitude.
     * 
     * @param lon The new longitude.
     */
    void setLon(const double lon);

    /**
     * Gets the latitude.
     * 
     * @return The latitude.
     */
    double getLat() const;

    /**
     * Gets the longitude.
     * 
     * @return The new longitude.
     */
    double getLon() const;
};

#endif