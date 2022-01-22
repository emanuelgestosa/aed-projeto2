#ifndef _TRANSPORT_NETWORK_H_
#define _TRANSPORT_NETWORK_H_

#include "minHeap.h"
#include <vector>
#include <list>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>

struct Position {
    double latitude;
    double longitude; 
};

class TransportNetwork {

    struct Connection {
        int dest, weight;
        std::string line;
    };

    struct Stop {
        std::list<Connection> adj;
        int distance, pred;
        bool visited;
        std::string name, code, zone;
        Position position;
    };

    int n;
    std::vector<Stop> stops;
    std::unordered_map<std::string, int> stopToInt;

    bool readStops();
    void addConnection(int src, int dest, int weight = 1);

public:

    TransportNetwork();


};

#endif