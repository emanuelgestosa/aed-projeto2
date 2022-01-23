#include "transportnetwork.h"

TransportNetwork::TransportNetwork() {
    readSuccess = true;
    if (!readStops()) {
        readSuccess = false;
        return;
    }
    if (!readLines()) {
        readSuccess = false;
        return;
    }
}

bool TransportNetwork::getReadSuccess() const {
    return readSuccess;
}

bool TransportNetwork::exists(const std::string& code) const {
    return stopToInt.find(code) != stopToInt.end();
}

bool TransportNetwork::readStops() {
    std::ifstream stopsFile(STOPS_FILE);
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
        position.setLat(stod(line.substr(line.find(",41") + 1, line.find(",-8") - line.find(",41"))));
        position.setLon(stod(line.substr(line.find(",-8") + 1)));
        stop.position = position;
        stops.push_back(stop);
        stopToInt[stop.code] = i - 1;
    }
    n = (int)stops.size();
    return true;
}

bool TransportNetwork::readLines() {
    std::ifstream linesFile(LINES_FILE);
    if (!linesFile.is_open()) return false;
    int i = 0;
    while (!linesFile.eof()) {
        std::string line;
        std::getline(linesFile, line);
        if (line.empty()) break;
        if (i++ == 0) continue; // Ignore the first line
        if (!readLine(line.substr(0, line.find(",")), line.substr(line.find(",") + 1))) return false;
    }
    return true;
}

bool TransportNetwork::readLine(const std::string &lineCode, const std::string &lineName) {
    if (!readLine(lineCode, lineName, LINE_FILE + lineCode + "_0.csv")) return false;
    if (!readLine(lineCode, lineName, LINE_FILE + lineCode + "_1.csv")) return false;
    return true;
}

bool TransportNetwork::readLine(const std::string &lineCode, const std::string &lineName, const std::string& fileName) {
    std::ifstream lineFile(fileName);
    if (!lineFile.is_open()) return false;
    std::string n;
    std::getline(lineFile, n);
    int nStops = stoi(n);
    std::string prev = "";
    for (int i = 0; i < nStops; i++) {
        std::string stopCode;
        std::getline(lineFile, stopCode);
        if (stopCode.empty()) break;
        if (prev.empty()) {
            prev = stopCode;
            continue;
        }
        int dest = stopToInt[stopCode];
        int src = stopToInt[prev];
        addConnection(src, dest, lineCode, lineName);
        prev = stopCode;
    }
    return true;
}

void TransportNetwork::addConnection(const int src, const int dest, const std::string& code, const std::string& name, int weight) {
    if (src<1 || src>n || dest<1 || dest>n) return;
    stops[src].adj.push_back({dest, weight, code, name});
}