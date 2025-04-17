#include "Charakterinformationen.h" // Eigenen Header einbinden
#include "Charaktere.h"           // Für die Character-Klasse und ihre displayInfo()-Methode
#include <iostream>               // Für std::cout
#include <vector>                 // Für std::vector
#include <cstdio>                 // Für getchar() zum Warten auf Enter
#include <windows.h>              // Für SetConsoleOutputCP (Windows-spezifisch)

/**
 * @brief Implementierung der Funktion zur Anzeige aller Charakter-Infos.
 */
void displayAllCharacterInfo(const std::vector<Character>& available_chars) {
    // Windows-spezifisch: Stelle UTF-8 für korrekte Umlautdarstellung sicher
    SetConsoleOutputCP(CP_UTF8);

    std::cout << "\n--- Charakter Informationen ---" << std::endl;
    // Prüfen, ob überhaupt Charaktere vorhanden sind
    if (available_chars.empty()) {
        std::cout << "Keine Charaktere vorhanden." << std::endl;
    } else {
        // Iteriere durch den Vektor und rufe displayInfo für jeden Charakter auf
        for (const Character& character : available_chars) { // Range-based for loop
            character.displayInfo(); // Ruft die Methode des Character-Objekts auf
            std::cout << "-----------------------------------------" << std::endl; // Trennlinie
        }
    }
    // Warte auf Benutzereingabe, bevor zum Menü zurückgekehrt wird
    std::cout << "\nDrücke Enter um zum Hauptmenü zurückzukehren...";
    // Leere den Puffer, falls noch etwas drin ist (z.B. von vorheriger Menüeingabe)
    // int c; while ((c = getchar()) != '\n' && c != EOF); // Eine Möglichkeit, Puffer zu leeren
    // getchar(); // Wartet auf das nächste Enter

    // Alternative mit std::cin (oft robuster)
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Verwirft alles bis zum nächsten Enter

}
