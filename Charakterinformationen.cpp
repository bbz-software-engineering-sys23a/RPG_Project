#include "Charakterinformationen.h" // Eigenen Header einbinden
#include "Charaktere.h"           // Für die Character-Klasse und ihre Methoden
#include <iostream>
#include <vector>
#include <cstdio>                // Für getchar()
#include <windows.h> // Für UTF-8 Darstellung

// Implementierung der Funktion zur Anzeige aller Charakter-Infos
void displayAllCharacterInfo(const std::vector<Character>& available_chars) {
    // Setze die Codepage auf UTF-8
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "\n--- Charakter Informationen ---" << std::endl;
    if (available_chars.empty()) {
        std::cout << "Keine Charaktere vorhanden." << std::endl;
    } else {
        // Geht durch alle Charaktere im Vektor
        for (const auto& character : available_chars) {
            character.displayInfo(); // Ruft die displayInfo-Methode des Charakters auf
            std::cout << "-----------------------------------------" << std::endl;
        }
    }
    std::cout << "\nDrücke Enter um zum Hauptmenü zurückzukehren...";
    // Wartet auf Enter, bevor zum Menü zurückgekehrt wird
    getchar();
}
