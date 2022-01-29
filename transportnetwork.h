#ifndef _TRANSPORT_NETWORK_H_
#define _TRANSPORT_NETWORK_H_

#include <vector>
#include <list>
#include <string>
#include <queue>
#include <set>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <float.h>
#include <limits.h>

#include "minHeap.h"
#include "position.h"

#define STOPS_FILE "dataset/stops.csv"
#define LINES_FILE "dataset/lines.csv"
#define LINE_FILE "dataset/line_"

/**
 * @brief Contains and manages all the information about the STCP network.
 */
class TransportNetwork {

    struct Connection {
        int dest;
        double weight;
        std::set<std::string> lineCodes;
    };

    struct Stop {
        std::list<Connection> adj;
        int pred;
        double dist;
        bool visited;
        std::string name, code, zone;
        Position position;
        std::set<std::string> lines;
    };

    int n;
    bool readSuccess;
    std::vector<Stop> stops;
    std::unordered_map<std::string, int> stopToInt;
    std::set<std::string> lines;

    /**
     * Reads from disk information about the stops.
     * 
     * @return Read successfull.
     */
    bool readStops();

    /**
     * Reads, using the other two readLine functions as auxiliars,
     * from disk information about the lines.
     * 
     * @return Read successfull.
     */
    bool readLines();

    /**
     * Auxiliar function to readLines.
     * 
     * @return Read successfull.
     */
    bool readLine(const std::string &lineCode, const std::string &lineName);

    /**
     * Auxiliar function to readLines.
     * 
     * @return Read successfull.
     */
    bool readLine(const std::string &lineCode, const std::string &lineName, const std::string& fileName);

    /**
     * Adds connections between stops within walking distance.
     * 
     * @param wDist The maximum walking distance.
     */
    void addWalkConns(const double wDist);

    /**
     * Adds a connection between two stops.
     * 
     * @param src The source of the connection.
     * @param dest The destination of the connection.
     * @param code The line code of the connection.
     * @param weight The weight of the connection, usually used to represent a distance.
     */
    void addConnection(const int src, const int dest, const std::string& code, double weight = 1.0);

public:

    /**
     * Default constructor reads all stops and lines information from disk and stores it in memory.
     */
    TransportNetwork();

    /**
     * Gets readSucess.
     * 
     * @return All disk accesses were successfull.
     */
    bool getReadSuccess() const;

    /**
     * Checks if a certain stop code exists.
     * 
     * @param code The code of the stop to check.
     * 
     * @return The stop code exists.
     */
    bool exists(const std::string& code) const;

    /**
     * Finds the top 5 nearest stops to a certain position.
     * 
     * @param pos The position to find stops near to.
     * 
     * @return A vector containing the top 5 nearest positions.
     */
    std::vector<std::string> findNearestStops(Position pos) const;

    /**
     * Gets all line codes.
     * 
     * @return A set containing all the line codes.
     */
    std::set<std::string> getLines() const;

    /**
     * Computes the minimum distance to travel between two stops using the dijkstra algorithm.
     * 
     * @param code1 The starting stop.
     * @param code2 The destination stop.
     * 
     * @return The distance to travel between the two stops.
     */
    double dijkstraDistance(const std::string& code1, const std::string& code2); 

    /**
     * Computes the shortest path to take from one stop to another using the dijkstra algorithm.
     * 
     * @param code1 The starting stop.
     * @param code2 The destination stop.
     * 
     * @return A list containing pairs of a stop code and a set of lines that can be used to access that stop.
     */
    std::list<std::pair<std::string, std::set<std::string>>> dijkstraPath(const std::string& code1, const std::string& code2); 

    /**
     * Compute distance to travel between two stops while
     * passing through the minimum amount of stops possible
     * using breadth-first search.
     * 
     * @param code1 The starting stop.
     * @param code2 The destination stop.
     * 
     * @return The distance to travel.
     */
    double bfsDistance(const std::string& code1, const std::string& code2);

    /**
     * Computes the path between two stops which
     * passes through the least amount of stops
     * using breadth-first search.
     * 
     * @param code1 The starting stop.
     * @param code2 The destination stop.
     * 
     * @return A list containing pairs of a stop code and a set of lines that can be used to access that stop.
     */
    std::list<std::pair<std::string, std::set<std::string>>> bfsPath(const std::string& code1, const std::string& code2);

    /**
     * Computes the minimum-spanning-tree cost of the
     * entire network assuming every stop is connected to each other.
     * 
     * @param start The starting stop. (Used just for testing purposes, since the cost should be the same for every starting point)
     * 
     * @return The MST cost.
     */
    double mstCost(const std::string& start);
};

#endif
