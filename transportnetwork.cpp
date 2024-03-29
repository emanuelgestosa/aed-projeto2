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
    addWalkConns(0.3);
}

bool TransportNetwork::getReadSuccess() const {
    return readSuccess;
}

bool TransportNetwork::exists(const std::string& code) const {
    return stopToInt.find(code) != stopToInt.end();
}

std::vector<std::string> TransportNetwork::findNearestStops(Position pos) const {
    double shortestDist1 = DBL_MAX;
    double shortestDist2 = DBL_MAX;
    double shortestDist3 = DBL_MAX;
    double shortestDist4 = DBL_MAX;
    double shortestDist5 = DBL_MAX;
    std::string closestStop1, closestStop2, closestStop3, closestStop4, closestStop5;
    for (int i = 1; i <= n; i++) {
        double dist = pos.calcDist(stops.at(i).position);
        if (dist < shortestDist1) {
            shortestDist5 = shortestDist4;
            closestStop5 = closestStop4;
            shortestDist4 = shortestDist3;
            closestStop4 = closestStop3;
            shortestDist3 = shortestDist2;
            closestStop3 = closestStop2;
            shortestDist2 = shortestDist1;
            closestStop2 = closestStop1;
            shortestDist1 = dist;
            closestStop1 = stops.at(i).code;
        } else if (dist < shortestDist2) {
            shortestDist5 = shortestDist4;
            closestStop5 = closestStop4;
            shortestDist4 = shortestDist3;
            closestStop4 = closestStop3;
            shortestDist3 = shortestDist2;
            closestStop3 = closestStop2;
            shortestDist2 = dist;
            closestStop2 = stops.at(i).code;
        } else if (dist < shortestDist3) {
            shortestDist5 = shortestDist4;
            closestStop5 = closestStop4;
            shortestDist4 = shortestDist3;
            closestStop4 = closestStop3;
            shortestDist3 = dist;
            closestStop3 = stops.at(i).code;
        } else if (dist < shortestDist4) {
            shortestDist5 = shortestDist4;
            closestStop5 = closestStop4;
            shortestDist4 = dist;
            closestStop4 = stops.at(i).code;
        } else if (dist < shortestDist5) {
            shortestDist5 = dist;
            closestStop5 = stops.at(i).code;
        }
    }
    return {closestStop1, closestStop2, closestStop3, closestStop4, closestStop5};
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
        std::set<std::string> empty;
        std::swap(stops.at(i).lines, empty);
    }
    stops.at(a).dist = 0.0;
    heap.insert(a, 0.0);
    while (heap.getSize() != 0) {
        int u = heap.removeMin();
        stops.at(u).visited = true;
        for (const auto& e : stops.at(u).adj) {
            int v = e.dest;
            if (!stops.at(v).visited && stops.at(u).dist + e.weight < stops.at(v).dist &&
                (stops.at(u).lines.find("WALK") == stops.at(u).lines.end() || e.lineCodes.find("WALK") == e.lineCodes.end())) {
                stops.at(v).dist = stops.at(u).dist + e.weight;
                stops.at(v).lines = e.lineCodes;
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
        std::set<std::string> empty;
        std::swap(stops.at(i).lines, empty);
    }
    stops.at(a).dist = 0.0;
    stops.at(a).pred = a;
    heap.insert(a, 0.0);
    while (heap.getSize() != 0) {
        int u = heap.removeMin();
        stops.at(u).visited = true;
        for (const auto& e : stops.at(u).adj) {
            int v = e.dest;
            if (!stops.at(v).visited && stops.at(u).dist + e.weight < stops.at(v).dist &&
                (stops.at(u).lines.find("WALK") == stops.at(u).lines.end() || e.lineCodes.find("WALK") == e.lineCodes.end())) {
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
    for (int v = 1; v <= n; v++) {
        stops.at(v).visited = false;
        std::set<std::string> empty;
        std::swap(stops.at(v).lines, empty);
    }
    std::queue<int> q;
    q.push(a);
    stops.at(b).dist = -1;
    stops.at(a).dist = 0.0;
    stops.at(a).visited = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (const auto& e : stops.at(u).adj) {
            int w = e.dest;
            if (!stops.at(w).visited && 
                (stops.at(u).lines.find("WALK") == stops.at(u).lines.end() || e.lineCodes.find("WALK") == e.lineCodes.end())) {
                q.push(w);
                stops.at(w).visited = true;
                stops.at(w).dist = stops.at(u).dist + e.weight;
                stops.at(w).lines = e.lineCodes;
            }
        }
    }
    return stops.at(b).dist;
} 

std::list<std::pair<std::string, std::set<std::string>>> TransportNetwork::bfsPath(const std::string& code1, const std::string& code2) {
    int a = stopToInt[code1], b = stopToInt[code2];
    std::list<std::pair<std::string, std::set<std::string>>> path;
    if (a == b) return path;
    for (int v = 1; v <= n; v++) {
        stops.at(v).visited = false;
        std::set<std::string> empty;
        std::swap(stops.at(v).lines, empty);
    }
    std::queue<int> q;
    q.push(a);
    stops.at(b).dist = -1;
    stops.at(a).dist = 0;
    stops.at(a).visited = true;
    stops.at(a).pred = a;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (const auto& e : stops.at(u).adj) {
            int w = e.dest;
            if (!stops.at(w).visited &&
                (stops.at(u).lines.find("WALK") == stops.at(u).lines.end() || e.lineCodes.find("WALK") == e.lineCodes.end())) {
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

double TransportNetwork::mstCost(const std::string& start) {
    int a = stopToInt[start];
    for (int v = 1; v <= n; v++) stops.at(v).dist = 100000.0;
    stops.at(a).dist = 0;
    const double notFound = -1.0;
    MinHeap<int, double> heap((int)stops.size(), notFound);
    for (int v = 1; v <= n; v++) heap.insert(v, stops.at(v).dist);
    while (heap.getSize() > 0) {
        int u = heap.removeMin();
        for (int v = 1; v <= n; v++) {
            double dist = stops.at(v).position.calcDist(stops.at(u).position);
            if (heap.hasKey(v) && dist < stops.at(v).dist) {
                stops.at(v).dist = dist;
                heap.decreaseKey(v, dist);
            }
        }
    }
    double totalDist = 0;
    for (int v = 1; v <= n; v++) totalDist += stops.at(v).dist;
    return totalDist;
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
    for (int i = 1; i <= n; i++) {
        std::set<int> dests;
        for (const auto& e : stops.at(i).adj)
            dests.insert(e.dest);
        for (int j = 1; j <= n; j++) {
            if (i == j) continue;
            if (dests.find(j) != dests.end()) continue;
            if (stops.at(i).position.calcDist(stops.at(j).position) <= wDist)
                addConnection(i, j, "WALK", stops.at(i).position.calcDist(stops.at(j).position));
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
