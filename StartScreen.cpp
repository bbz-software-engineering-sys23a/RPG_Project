#include "StartScreen.h"
#include <iostream>
#include <string>
#include <limits>
#include <cstdio> // Für getchar()
#include <windows.h> // Für UTF-8 Darstellung

void displayStartScreen() {
    // Setze die Codepage auf UTF-8
    SetConsoleOutputCP(CP_UTF8);
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

    std::cout << "                 Drücke Enter um fortzufahren...";
    getchar(); // Wartet auf Enter

}
