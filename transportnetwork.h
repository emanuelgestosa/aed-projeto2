#ifndef _TRANSPORT_NETWORK_H_
#define _TRANSPORT_NETWORK_H_

#include "minHeap.h"
#include <vector>
#include <list>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>

#define STOPS_FILE "dataset/stops.csv"
#define LINES_FILE "dataset/lines.csv"
#define LINE_FILE "dataset/line_"

struct Position {
    double latitude;
    double longitude; 
};

class TransportNetwork {

    struct Connection {
        int dest, weight;
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
    void addConnection(const int src, const int dest, const std::string& code, const std::string& name, int weight = 1);

public:

    TransportNetwork();
    bool getReadSuccess() const;

};

#endif