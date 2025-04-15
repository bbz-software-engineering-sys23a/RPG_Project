#include "StartScreen.h"
#include <iostream>
#include <string>
#include <limits>
#include <cstdio> // Für getchar()

void displayStartScreen() {
    // Extrem einfache ASCII-Art für "Extreme Combat" für maximale Kompatibilität
    std::cout << R"(
###########################################################################
#                                                                         #
#                          EXTREME COMBAT                                 #
#                                                                         #
###########################################################################
    )" << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "                 Ein Spiel von: Gabriel, Felix und Florin" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "                 Druecke Enter um fortzufahren...";
    getchar(); // Wartet auf Enter

}
