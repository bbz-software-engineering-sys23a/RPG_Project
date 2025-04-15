#include "StartScreen.h" // Binde den eigenen Header ein

#include <iostream>      // Nötige Includes für die Implementierung der Funktion
#include <string>
#include <limits>

// Die eigentliche Implementierung der Funktion
void displayStartScreen() {
    // Extrem einfache ASCII-Art für "Extreme Combat" für maximale Kompatibilität
    std::cout << R"(
###########################################################################
#                                                                         #
#                          EXTREME COMBAT                                 #
#                                                                         #
###########################################################################
    )" << std::endl; // R"(...)" ist ein Raw String Literal

    std::cout << std::endl; // Leerzeile für Abstand
    std::cout << std::endl; // Noch eine Leerzeile

    // Namen der Ersteller anzeigen
    // (Leerzeichen am Anfang angepasst für die neue Breite)
    std::cout << "                 Ein Spiel von: Gabriel, Felix und Florin" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    // Kurze Pause, damit der Spieler den Bildschirm sehen kann
    std::cout << "                 Druecke Enter um fortzufahren...";
    // Ignoriert vorherige Eingaben im Buffer und wartet dann auf Enter
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    // Alternative: std::cin.get();

    // Optional: Bildschirm löschen nach Enter (systemabhängig)
    // system("cls"); // Für Windows
    // system("clear"); // Für Linux/macOS
}