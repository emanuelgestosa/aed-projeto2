#ifndef _TRANSPORT_NETWORK_H_
#define _TRANSPORT_NETWORK_H_

#include "minHeap.h"
#include <vector>
#include <list>
#include <string>

class TransportNetwork {

    struct Connection {
        int dest;
        int weight;
        std::string line;
    };

    struct Stop {
        list<Connection> adj;
        int distance;
        int pred;
        bool visited;
        std::string name;
    };

    int n;
    std::vector<Stop> stops;

    void addEdge(int src, int dest, int weight = 1);

public:

    TransportNetwork(int nodes);


};

#endif