#include "menu.h"

Menu::Menu(TransportNetwork* network) : network(network) {}

int Menu::getInt() const {

    int userInput;
    std::cin >> userInput;

    if (std::cin.fail()) {
        if (std::cin.eof())
            return 0;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }

    return userInput;

}

/*************************************************************************/

MainMenu::MainMenu(TransportNetwork* network) : Menu(network) {};

void MainMenu::display() const {

    std::cout << "Welcome to the STCP helper!" << std::endl
              << "(Please choose an option)" << std::endl
              << "0. Exit." << std::endl
              << "1. Travel." << std::endl
              << "2. Special journey." << std::endl;
}

Menu* MainMenu::getNext() {
    int choice = getInt();
    switch (choice) {
        case 0: return nullptr;
        case 1: return new TravelMenu(network);
        case 2: return new SpecialMenu(network);
        default: return this;
    }
}

/*************************************************************************/

TravelMenu::TravelMenu(TransportNetwork* network) : Menu(network) {}

void TravelMenu::display() const {

    std::cout << "Welcome to the STCP travel section!" << std::endl
              << "(Please choose an option)" << std::endl
              << "0. Back to main menu." << std::endl
              << "1. Enter stop code." << std::endl
              << "2. Enter location." << std::endl;
}

Menu* TravelMenu::getNext() {
    int choice = getInt();
    switch (choice) {
        case 0: return nullptr;
        case 1: return new ByCodeMenu(network, false);
        case 2: return new ByPosMenu(network, false);
        default: return this;
    }
}

/*************************************************************************/

ByCodeMenu::ByCodeMenu(TransportNetwork* network, bool goBack) : Menu(network) {
    this->goBack = goBack;
}

const std::string ByCodeMenu::getCode() const {
    std::string userInput;
    std::cin >> userInput;
    if (std::cin.eof()) return "";
    if (!network->exists(userInput)) return INVALID_STRING_INPUT;
    return userInput;
}

void ByCodeMenu::display() const {
    if (goBack) return;
    std::cout << "(Enter the stop code)" << std::endl;
}

Menu* ByCodeMenu::getNext() {
    if (goBack) return nullptr;
    std::cout << "From: ";
    std::string src = getCode();
    if (src == "") return nullptr;
    if (src == INVALID_STRING_INPUT) {
        std::cout << "Invalid stop." << std::endl;
        return nullptr;
    }
    std::cout << "To:   ";
    std::string dest = getCode();
    if (dest == "" || dest == "0") return nullptr;
    if (dest == INVALID_STRING_INPUT) {
        std::cout << "Invalid stop." << std::endl;
        return nullptr;
    }
    goBack = true;
    return new RouteMenu(network, src, dest);
}

/*************************************************************************/

ByPosMenu::ByPosMenu(TransportNetwork* network, bool goBack) : Menu(network) {
    this->goBack = goBack;
}

Position ByPosMenu::getPos() const {
    Position eofPos; eofPos.setLat(0); eofPos.setLon(0);
    Position invPos; invPos.setLat(-1); invPos.setLon(-1);
    double lat, lon;
    std::cin >> lat >> lon;
    if(std::cin.fail()) {
        if(std::cin.eof())
            return eofPos;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return invPos;
    }
    Position pos; pos.setLat(lat); pos.setLon(lon);
    return pos;
}

void ByPosMenu::display() const {
    if (goBack) return;
    std::cout << "(Enter the position)" << std::endl;
}

Menu* ByPosMenu::getNext() {
    if (goBack) return nullptr;
    std::cout << "From (latitude longitude): ";
    Position src = getPos();
    if (src.getLat() == -1 && src.getLon() == -1) {
        std::cout << "Invalid position." << std::endl;
        return nullptr;
    } if (src.getLat() == 0 && src.getLon() == 0) return nullptr;

    std::cout << "To (latitude longitude): ";
    Position dest = getPos();
    if (dest.getLat() == -1 && dest.getLon() == -1) {
        std::cout << "Invalid position." << std::endl;
        return nullptr;
    } if (dest.getLat() == 0 && dest.getLon() == 0) return nullptr;
    goBack = true;
    return new ChooseStopMenu(network, src, dest, false);
}

/*************************************************************************/

ChooseStopMenu::ChooseStopMenu(TransportNetwork* network, Position pos1, Position pos2, bool goBack) : Menu(network) {
    this->goBack = goBack;
    this->pos1 = pos1;
    this->pos2 = pos2;
}

void ChooseStopMenu::display() const {
    if (goBack) return;
}

Menu* ChooseStopMenu::getNext() {
    if (goBack) return nullptr;
    std::cout << "From:" << std::endl
              << "(Please choose an option)" << std::endl;
    auto srcStops = network->findNearestStops(pos1);
    auto destStops = network->findNearestStops(pos2);
    std::cout << "0. Exit." << std::endl;
    for (int i = 0; i < srcStops.size(); i++) std::cout << i+1 << ". " << srcStops.at(i) << std::endl;
    int choice1 = getInt();
    if (choice1 == 0) return nullptr;
    if (choice1 == -1) return this;

    std::cout << "To:" << std::endl
              << "(Please choose an option)" << std::endl << std::endl;
    std::cout << "0. Exit." << std::endl;
    for (int i = 0; i < destStops.size(); i++) std::cout << i+1 << ". " << destStops.at(i) << std::endl;
    int choice2 = getInt();
    if (choice2 == 0) return nullptr;
    if (choice2 == -1) return this;
    return new RouteMenu(network, srcStops.at(choice1 - 1), destStops.at(choice2 - 1));
}

/*************************************************************************/

RouteMenu::RouteMenu(TransportNetwork* network, const std::string& stop1, const std::string& stop2) : Menu(network) {
    this->stop1 = stop1;
    this->stop2 = stop2;
}

void RouteMenu::display() const {
    std::cout << "Travel from " << stop1 << " to " << stop2 << std::endl
              << "(Please choose an option)" << std::endl
              << "0. Back to travel menu." << std::endl
              << "1. Least distance travelled." << std::endl
              << "2. Least stops visited." << std::endl;
}

Menu* RouteMenu::getNext() {
    int choice = getInt();
    switch (choice) {
        case 0: return nullptr;
        case 1: return new LeastDistMenu(network, stop1, stop2);
        case 2: return new LeastStopsMenu(network, stop1, stop2);
        default: return this;
    }
}

/*************************************************************************/

LeastDistMenu::LeastDistMenu(TransportNetwork* network, const std::string& stop1, const std::string& stop2) : Menu(network) {
    this->stop1 = stop1;
    this->stop2 = stop2;
}

void LeastDistMenu::display() const {
    auto path = network->dijkstraPath(stop1, stop2);
    double dist = network->dijkstraDistance(stop1, stop2);

    std::cout << "From " << stop1 << " to " << stop2 << std::endl
              << "you will travel " << dist << " km" << std::endl
              << "and pass through " << path.size() << " stops" << std::endl
              <<"(Enter '0' to go back)" << std::endl;

    std::list<std::string> linePath;
    std::set<std::string> lines = network->getLines();
    std::set<std::string> oldLines;
    std::string lastStop;
    std::vector<std::string> intersect;
    for (auto i : path) {
        if (i.second.empty()) i.second = network->getLines();
        intersect.clear();
        std::set_intersection(lines.begin(), lines.end(), 
                              i.second.begin(), i.second.end(), 
                              std::back_inserter(intersect));
        lines.clear();
        for (auto a : intersect) lines.insert(a);
        if (lines.empty()) {
            lines = i.second;
            if (linePath.size() > 1) {
                std::cout << *(oldLines.begin()) << " ";
                for (auto l : linePath) std::cout << l << " ";
                std::cout << std::endl;
            }
            linePath.clear();
            if (!lastStop.empty()) linePath.push_back(lastStop);
        } 
        linePath.push_back(i.first);
        oldLines = lines;
        lastStop = i.first;
    }
    if (linePath.size() > 1) {
        std::cout << *(path.back().second.begin()) << " ";
        for (auto l : linePath) std::cout << l << " ";
        std::cout << std::endl;
    }
}

Menu* LeastDistMenu::getNext() {
    getInt();
    return nullptr;
}

/*************************************************************************/

LeastStopsMenu::LeastStopsMenu(TransportNetwork* network, const std::string& stop1, const std::string& stop2) : Menu(network) {
    this->stop1 = stop1;
    this->stop2 = stop2;
}

void LeastStopsMenu::display() const {
    auto path = network->bfsPath(stop1, stop2);
    double dist = network->bfsDistance(stop1, stop2);

    std::cout << "From " << stop1 << " to " << stop2 << std::endl
              << "you will travel " << dist << " km" << std::endl
              << "and pass through " << path.size() << " stops" << std::endl
              <<"(Enter '0' to go back)" << std::endl;

    std::list<std::string> linePath;
    std::set<std::string> lines = network->getLines();
    std::set<std::string> oldLines;
    std::string lastStop;
    std::vector<std::string> intersect;
    for (auto i : path) {
        if (i.second.empty()) i.second = network->getLines();
        intersect.clear();
        std::set_intersection(lines.begin(), lines.end(), 
                              i.second.begin(), i.second.end(), 
                              std::back_inserter(intersect));
        lines.clear();
        for (auto a : intersect) lines.insert(a);
        if (lines.empty()) {
            lines = i.second;
            if (linePath.size() > 1) {
                std::cout << *(oldLines.begin()) << " ";
                for (auto l : linePath) std::cout << l << " ";
                std::cout << std::endl;
            }
            linePath.clear();
            if (!lastStop.empty()) linePath.push_back(lastStop);
        } 
        linePath.push_back(i.first);
        oldLines = lines;
        lastStop = i.first;
    }
    if (linePath.size() > 1) {
        std::cout << *(path.back().second.begin()) << " ";
        for (auto l : linePath) std::cout << l << " ";
        std::cout << std::endl;
    }
}

Menu* LeastStopsMenu::getNext() {
    getInt();
    return nullptr;
}

/*************************************************************************/

SpecialMenu::SpecialMenu(TransportNetwork* network) : Menu(network) {}

void SpecialMenu::display() const {
    std::cout << "Welcome to the special journey menu!" << std::endl
              << "Here you can choose a stop as start point" << std::endl
              << "and see how much distance you would cover" << std::endl
              << "in order to visit every single stop in our network!" << std::endl
              << "(Please choose an option)" << std::endl
              << "0. Back to main menu." << std::endl
              << "1. Pick starting point." << std::endl;
}

Menu* SpecialMenu::getNext() {
    int choice = getInt();
    switch (choice) {
        case 0: return nullptr;
        case 1: std::cout << network->mstCost("ALFG1") << std::endl; return this;
        default: return this;
    }
}