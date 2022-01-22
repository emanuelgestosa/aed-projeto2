#include "transportnetwork.h"

int main() {
    TransportNetwork network;
    if (!network.getReadSuccess()) {
        std::cout << "Missing files." << std::endl;
        return 1;
    }
    return 0;
}