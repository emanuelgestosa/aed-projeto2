#ifndef MENU_H
#define MENU_H

#include <limits>
#include <bits/stdc++.h>

#include "transportnetwork.h"

#define INVALID_STRING_INPUT "INVALID"

class Menu {
protected:

    TransportNetwork* network;

    /**
     * Gets an integer from the user,
     * returns -1 for invalid inputs and
     * 0 for end of file.
     * 
     * @return An integer inserted by the user.
     */
    int getInt() const;

public:

    Menu(TransportNetwork* network);
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
    MainMenu(TransportNetwork* network);
    virtual ~MainMenu() {}
    void display() const override;
    Menu* getNext() override;
};

class TravelMenu : public Menu {
public:
    TravelMenu(TransportNetwork* network);
    virtual ~TravelMenu() {}
    void display() const override;
    Menu* getNext() override;
};

class ByCodeMenu : public Menu {
private:
    bool goBack;
    const std::string getCode() const;
public:
    ByCodeMenu(TransportNetwork* network, bool goBack = true);
    virtual ~ByCodeMenu() {}
    void display() const override;
    Menu* getNext() override;
};

class ByPosMenu : public Menu {
private:
    bool goBack;
    Position getPos() const;
public:
    ByPosMenu(TransportNetwork* network, bool goBack = true);
    virtual ~ByPosMenu() {}
    void display() const override;
    Menu* getNext() override;
};

class RouteMenu : public Menu {
    std::string stop1, stop2;
public:
    RouteMenu(TransportNetwork* network, const std::string& stop1, const std::string& stop2);
    virtual ~RouteMenu() {}
    void display() const override;
    Menu* getNext() override;
};

class LeastDistMenu : public Menu {
    std::string stop1, stop2;
public:
    LeastDistMenu(TransportNetwork* network, const std::string& stop1, const std::string& stop2);
    virtual ~LeastDistMenu() {}
    void display() const override;
    Menu* getNext() override;
};

class LeastStopsMenu : public Menu {
    std::string stop1, stop2;
public:
    LeastStopsMenu(TransportNetwork* network, const std::string& stop1, const std::string& stop2);
    virtual ~LeastStopsMenu() {}
    void display() const override;
    Menu* getNext() override;
};

#endif
