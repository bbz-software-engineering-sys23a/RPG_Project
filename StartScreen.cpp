#include "StartScreen.h" // Eigenen Header einbinden
#include <iostream>      // Für std::cout
#include <string>        // Wird hier nicht direkt verwendet
#include <limits>        // Für Puffer leeren (falls getchar() Probleme macht)
#include <cstdio>        // Für getchar()
#include <windows.h>     // Für SetConsoleOutputCP (Windows-spezifisch)

/**
 * @brief Implementierung der Funktion zur Anzeige des Startbildschirms.
 */
void displayStartScreen() {
    // Windows-spezifisch: Stelle UTF-8 für korrekte Umlautdarstellung sicher
    SetConsoleOutputCP(CP_UTF8);

    // Titel als ASCII-Art ausgeben (Raw String Literal R"(...)")
    std::cout << R"(
###########################################################################
#                                                                         #
#                          EXTREME COMBAT                                 #
#                                                                         #
###########################################################################
    )" << std::endl;

    // Leerzeilen für Abstand
    std::cout << std::endl;
    std::cout << std::endl;

    // Credits anzeigen
    std::cout << "                 Ein Spiel von: Gabriel, Felix und Florin" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    // Warte auf Benutzereingabe
    std::cout << "                 Drücke Enter um fortzufahren...";

    // Leere Puffer vor getchar(), um sicherzustellen, dass es auf neues Enter wartet
    // int c; while ((c = getchar()) != '\n' && c != EOF); // Eine Möglichkeit
    // getchar(); // Wartet auf Enter

    // Alternative mit std::cin (oft robuster)
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

}
