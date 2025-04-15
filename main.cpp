#include <iostream>
#include <vector>
#include <string>
#include <utility> // Für std::pair
#include <stdexcept> // Für std::exception
#include <limits> // Für numeric_limits
#include <cstdio> // Für getchar()

// Eigene Header einbinden
#include "StartScreen.h"
#include "Datenstrukturen.h" // Inkludiert Charaktere.h indirekt
#include "Kampf.h"           // !!! NEU: Header für die Kampffunktion !!!

int main() {
    // Zufallsgenerator einmalig initialisieren
    initializeRandomSeed(); // Aus Datenstrukturen.h/.cpp

    // Startbildschirm anzeigen
    displayStartScreen(); // Aus StartScreen.h/.cpp

    // Charakterdaten laden
    std::vector<Character> available_characters;
    try {
        available_characters = initializeCharacters(); // Aus Charaktere.h/.cpp
        if (available_characters.empty()) {
             std::cerr << "FEHLER: Keine Charakterdaten initialisiert. Programm wird beendet." << std::endl;
             return 1; // Fehlercode
        }
        std::cout << "\nCharakterdaten geladen." << std::endl;
        sleepMilliseconds(1000); // Aus Datenstrukturen.h/.cpp
    } catch (const std::exception& e) {
         std::cerr << "FEHLER beim Initialisieren der Charaktere: " << e.what() << std::endl;
         return 1;
    }


    // Hauptmenü
    int menuChoice = 0;
    do {
        std::cout << "\n--- Hauptmenue ---" << std::endl;
        std::cout << "1: Spiel Starten" << std::endl;
        std::cout << "2: Beenden" << std::endl;

        menuChoice = getIntegerInput("Deine Wahl: "); // Aus Datenstrukturen.h/.cpp

        switch (menuChoice) {
            case 1: {
                std::cout << "\nSpiel wird gestartet..." << std::endl;
                sleepMilliseconds(500);
                try {
                    // Charakterauswahl durchführen
                    std::pair<Player, Player> players = selectCharacters(available_characters); // Aus Datenstrukturen.h/.cpp
                    Player player1 = players.first;
                    Player player2 = players.second;

                    // Kampf starten
                    startCombat(player1, player2); // !!! Ruft jetzt Funktion aus Kampf.h/.cpp auf !!!

                } catch (const std::exception& e) {
                    std::cerr << "\nEin Fehler ist aufgetreten: " << e.what() << std::endl;
                    std::cout << "Kehre zum Hauptmenue zurueck." << std::endl;
                    sleepMilliseconds(2000);
                }
                 std::cout << "\nDruecke Enter um zum Hauptmenue zurueckzukehren...";
                 getchar(); // Einfaches Warten auf Enter
                break;
            }
            case 2:
                std::cout << "Spiel wird beendet. Auf Wiedersehen!" << std::endl;
                break;
            default:
                std::cout << "Ungueltige Wahl. Bitte 1 oder 2 eingeben." << std::endl;
                sleepMilliseconds(1000);
                break;
        }
    } while (menuChoice != 2);


    return 0; // Programm erfolgreich beenden
}
