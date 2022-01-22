#include "transportnetwork.h"

TransportNetwork::TransportNetwork() {
    if(!readStops()) {
        std::cout << "Failed to read file!" << std::endl;
        return;
    }
    std::cout << stops.at(1).zone << std::endl;
}

bool TransportNetwork::readStops() {
    std::ifstream stopsFile("dataset/stops.csv");
    if (!stopsFile.is_open()) return false;
    stops.push_back({});
    int i = 0;
    while (!stopsFile.eof()) {
        std::string line;
        std::getline(stopsFile, line);
        if (line.empty()) break;
        if (i++ == 0) continue; // Ignore the first line
        Stop stop;
        Position position;
        stop.code = line.substr(0, line.find(","));
        stop.name = line.substr(line.find(",") + 1, line.find(",", line.find(",") + 1) - (line.find(",") + 1));
        stop.zone = line.substr(line.find(",", line.find(",") + 1) + 1, line.find(",", line.find(",", line.find(",") + 1) + 1) - (line.find(",", line.find(",") + 1) + 1));
        position.latitude = stod(line.substr(line.find(",41") + 1, line.find(",-8") - line.find(",41")));
        position.longitude = stod(line.substr(line.find(",-8") + 1));
        stop.position = position;
        stops.push_back(stop);
    }
    n = (int)stops.size();
    return true;
}

void TransportNetwork::addConnection(int src, int dest, int weight) {
    if (src<1 || src>n || dest<1 || dest>n) return;
    stops[src].adj.push_back({dest, weight});
}