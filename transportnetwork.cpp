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
    addWalkConns(0.35);
}

bool TransportNetwork::getReadSuccess() const {
    return readSuccess;
}

bool TransportNetwork::exists(const std::string& code) const {
    return stopToInt.find(code) != stopToInt.end();
}

std::set<std::string> TransportNetwork::getLines() const {
    return lines;
}

double TransportNetwork::dijkstraDistance(const std::string& code1, const std::string& code2) {
    int a = stopToInt[code1], b = stopToInt[code2];
    const double notFound = -1.0;
    MinHeap<int, double> heap((int)stops.size(), notFound);
    for (int i = 1; i <= n; i++) {
        stops.at(i).dist = 100000.0;
        stops.at(i).visited = false;
    }
    stops.at(a).dist = 0.0;
    heap.insert(a, 0.0);
    while (heap.getSize() != 0) {
        int u = heap.removeMin();
        stops.at(u).visited = true;
        for (const auto& e : stops.at(u).adj) {
            int v = e.dest;
            if (!stops.at(v).visited && stops.at(u).dist + e.weight < stops.at(v).dist) {
                stops.at(v).dist = stops.at(u).dist + e.weight;
                if (!heap.hasKey(v)) heap.insert(v, stops.at(v).dist);
                else heap.decreaseKey(v, stops.at(v).dist);
            }
        }
    }
    if (stops.at(b).dist == 100000.0) return -1.0;
    else return stops.at(b).dist;
}

std::list<std::pair<std::string, std::set<std::string>>> TransportNetwork::dijkstraPath(const std::string& code1, const std::string& code2) {
    int a = stopToInt[code1], b = stopToInt[code2];
    std::list<std::pair<std::string, std::set<std::string>>> path;
    if (a == b) return path;
    const double notFound = -1.0;
    MinHeap<int, double> heap((int)stops.size(), notFound);
    for (int i = 1; i <= n; i++) {
        stops.at(i).dist = 100000.0;
        stops.at(i).visited = false;
    }
    stops.at(a).dist = 0.0;
    stops.at(a).pred = a;
    heap.insert(a, 0.0);
    while (heap.getSize() != 0) {
        int u = heap.removeMin();
        stops.at(u).visited = true;
        for (const auto& e : stops.at(u).adj) {
            int v = e.dest;
            if (!stops.at(v).visited && stops.at(u).dist + e.weight < stops.at(v).dist) {
                stops.at(v).dist = stops.at(u).dist + e.weight;
                stops.at(v).pred = u;
                stops.at(v).lines = e.lineCodes;
                if (!heap.hasKey(v)) heap.insert(v, stops.at(v).dist);
                else heap.decreaseKey(v, stops.at(v).dist);
            }
        }
    }
    if (stops.at(b).dist == 100000.0) return path;
    int pred = b;
    path.push_front({stops.at(b).code, stops.at(b).lines});
    while (pred != a) {
        pred = stops.at(pred).pred;
        path.push_front({stops.at(pred).code, stops.at(pred).lines});
    }
    return path;
}

double TransportNetwork::bfsDistance(const std::string& code1, const std::string& code2) {
    int a = stopToInt[code1], b = stopToInt[code2];
    for (int v = 1; v <= n; v++) stops.at(v).visited = false;
    std::queue<int> q;
    q.push(a);
    stops.at(b).dist = -1;
    stops.at(a).dist = 0.0;
    stops.at(a).visited = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto& e : stops.at(u).adj) {
            int w = e.dest;
            if (!stops.at(w).visited) {
                q.push(w);
                stops.at(w).visited = true;
                stops.at(w).dist = stops.at(u).dist + e.weight;
            }
        }
    }
    return stops.at(b).dist;
} 

std::list<std::pair<std::string, std::set<std::string>>> TransportNetwork::bfsPath(const std::string& code1, const std::string& code2) {
    int a = stopToInt[code1], b = stopToInt[code2];
    std::list<std::pair<std::string, std::set<std::string>>> path;
    if (a == b) return path;
    for (int v = 1; v <= n; v++) stops.at(v).visited = false;
    std::queue<int> q;
    q.push(a);
    stops.at(b).dist = -1;
    stops.at(a).dist = 0;
    stops.at(a).visited = true;
    stops.at(a).pred = a;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto& e : stops.at(u).adj) {
            int w = e.dest;
            if (!stops.at(w).visited) {
                q.push(w);
                stops.at(w).visited = true;
                stops.at(w).dist = stops.at(u).dist + 1;
                stops.at(w).pred = u;
                stops.at(w).lines = e.lineCodes;
            }
        }
    }
    if (stops.at(b).dist == -1) return path;
    int pred = b;
    path.push_front({stops.at(b).code, stops.at(b).lines});
    while (pred != a) {
        pred = stops.at(pred).pred;
        path.push_front({stops.at(pred).code, stops.at(pred).lines});
    }
    return path;
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
    n = (int)stops.size() - 1;
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
    lines.insert(lineCode);
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
        addConnection(src, dest, lineCode, stops.at(dest).position.calcDist(stops.at(src).position));
        prev = stopCode;
    }
    return true;
}

void TransportNetwork::addWalkConns(const double wDist) {
    for (int i = 1; i <= n - 1; i++) {
        std::set<int> dests;
        for (const auto& e : stops.at(i).adj)
            dests.insert(e.dest);
        for (int j = i+1; j <= n; j++) {
            if (dests.find(j) != dests.end()) continue;
            if (stops.at(i).position.calcDist(stops.at(j).position) <= wDist) {
                addConnection(i, j, "WALK", stops.at(i).position.calcDist(stops.at(j).position));
                addConnection(j, i, "WALK", stops.at(i).position.calcDist(stops.at(j).position));
            }
        }
    }
}

void TransportNetwork::addConnection(const int src, const int dest, const std::string& code, double weight) {
    if (src<1 || src>n || dest<1 || dest>n) return;
    bool hasConn = false;
    for (auto& a : stops.at(src).adj)
        if (a.dest == dest) {
            a.lineCodes.insert(code);
            hasConn = true;
            break;
        }
    if (!hasConn) stops.at(src).adj.push_back({dest, weight, {code}});
}
