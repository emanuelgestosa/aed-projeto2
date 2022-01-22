#ifndef MENU_H
#define MENU_H

#include <limits>

#include "transportnetwork.h"

class Menu {
protected:

    TransportNetwork network;

    /**
     * Gets an integer from the user,
     * returns -1 for invalid inputs and
     * 0 for end of file.
     * 
     * @return An integer inserted by the user.
     */
    int getInt() const;

public:

    Menu(const TransportNetwork& network);
    virtual ~Menu() {}

    /**
     * Displays the current menu.
     */
    virtual void display() const = 0;

    /**
     * Gets the next menu to be displayed
     * based on an input given by the user.
     * Will return nullptr to go back.
     * 
     * @return A pointer to the next menu.
     */
    virtual Menu* getNext() = 0;
};

class MainMenu : public Menu {
public:
    MainMenu(const TransportNetwork& network);
    virtual ~MainMenu() {}
    void display() const override;
    Menu* getNext() override;
};

class TravelMenu : public Menu {
public:
    TravelMenu(const TransportNetwork& network);
    virtual ~TravelMenu() {}
    void display() const override;
    Menu* getNext() override;
};

class ByCodeMenu : public Menu {
private:
    const std::string& getCode() const;
public:
    ByCodeMenu(const TransportNetwork& network);
    virtual ~ByCodeMenu() {}
    void display() const override;
    Menu* getNext() override;
}

#endif