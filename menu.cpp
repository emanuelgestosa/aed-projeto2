#include "menu.h"

Menu::Menu(const TransportNetwork& network) { this->network = network; }

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

MainMenu::MainMenu(const TransportNetwork& network) : Menu(network) {};

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
        case 1: return this;
        default: return this;
    }
}