#include <stack>

#include "transportnetwork.h"
#include "menu.h"

int main() {
    const TransportNetwork network;
    if (!network.getReadSuccess()) {
        std::cout << "Missing files." << std::endl;
        return 1;
    }
    std::stack<Menu*> menus;
    menus.push(new MainMenu(&network));
    while (!menus.empty())
    {
        menus.top()->display();
        Menu* nextMenu = menus.top()->getNext();
        if (nextMenu != nullptr) {
            if (nextMenu != menus.top()) menus.push(nextMenu);
        }
        else {
            delete menus.top();
            menus.pop();
        }
    }
    return 0;
}