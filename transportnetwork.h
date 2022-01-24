#ifndef _TRANSPORT_NETWORK_H_
#define _TRANSPORT_NETWORK_H_

#include <vector>
#include <list>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>

#include "minHeap.h"
#include "position.h"

#define STOPS_FILE "dataset/stops.csv"
#define LINES_FILE "dataset/lines.csv"
#define LINE_FILE "dataset/line_"

class TransportNetwork {

    struct Connection {
        int dest;
        double weight;
        std::string lineCode, lineName;
    };

    struct Stop {
        std::list<Connection> adj;
        int distance, pred;
        bool visited;
        std::string name, code, zone;
        Position position;
    };

    int n;
    bool readSuccess;
    std::vector<Stop> stops;
    std::unordered_map<std::string, int> stopToInt;

    bool readStops();
    bool readLines();
    bool readLine(const std::string &lineCode, const std::string &lineName);
    bool readLine(const std::string &lineCode, const std::string &lineName, const std::string& fileName);
    void addConnection(const int src, const int dest, const std::string& code, const std::string& name, double weight = 1.0);

public:

    TransportNetwork();
    bool getReadSuccess() const;
    bool exists(const std::string& code) const;

};

#endif