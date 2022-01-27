#include "menu.h"

Menu::Menu(TransportNetwork* network) : network(network) {}

int Menu::getInt() const {

    int userInput;
    std::cin >> userInput;

    if(std::cin.fail())
    {
        if(std::cin.eof())
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
              << "1. Travel" << std::endl;
}

Menu* MainMenu::getNext() {
    int choice = getInt();
    switch (choice) {
        case 0: return nullptr;
        case 1: return new TravelMenu(network);
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
              << "2. Enter stop name." << std::endl
              << "3. Enter location." << std::endl;
}

Menu* TravelMenu::getNext() {
    int choice = getInt();
    switch (choice) {
        case 0: return nullptr;
        case 1: return new ByCodeMenu(network, false);
        case 2: return this;
        case 3: return this;
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
    std::cout << "(Enter the zone code)" << std::endl;
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

RouteMenu::RouteMenu(TransportNetwork* network, const std::string& stop1, std::string& stop2) : Menu(network) {
    this->stop1 = stop1;
    this->stop2 = stop2;
}

void RouteMenu::display() const {
    std::cout << "Travel from " << stop1 << " to " << stop2 << std::endl
              << "(Please choose an option)" << std::endl
              << "0. Back to travel menu." << std::endl
              << "1. Least distance travelled." << std::endl
              << "2. Least stops visited" << std::endl;
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

LeastDistMenu::LeastDistMenu(TransportNetwork* network, const std::string& stop1, std::string& stop2) : Menu(network) {
    this->stop1 = stop1;
    this->stop2 = stop2;
}

void LeastDistMenu::display() const {
    auto path = network->dijkstraPath(stop1, stop2);
    double dist = network->dijkstraDistance(stop1, stop2);

    std::cout << "From " << stop1 << " to " << stop2 << std::endl
              << "you travelled " << dist << " km" << std::endl
              << "and passed through " << path.size() << " stops" << std::endl
              <<"(Enter '0' to go back)" << std::endl;
}

Menu* LeastDistMenu::getNext() {
    getInt();
    return nullptr;
}

/*************************************************************************/

LeastStopsMenu::LeastStopsMenu(TransportNetwork* network, const std::string& stop1, std::string& stop2) : Menu(network) {
    this->stop1 = stop1;
    this->stop2 = stop2;
}

void LeastStopsMenu::display() const {
    auto path = network->bfsPath(stop1, stop2);
    double dist = network->bfsDistance(stop1, stop2);

    std::cout << "From " << stop1 << " to " << stop2 << std::endl
              << "you travelled " << dist << " km" << std::endl
              << "and passed through " << path.size() << " stops" << std::endl
              <<"(Enter '0' to go back)" << std::endl;
}

Menu* LeastStopsMenu::getNext() {
    getInt();
    return nullptr;
}