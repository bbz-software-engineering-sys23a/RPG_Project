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
#include "Kampf.h"           // Header für die Kampffunktion
#include "Charakterinformationen.h" // Header für die Info-Anzeige

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
        std::cout << "2: Charakter Info Anzeigen" << std::endl; // Menüpunkt wieder da
        std::cout << "3: Beenden" << std::endl;                 // Beenden ist wieder 3

        menuChoice = getIntegerInput("Deine Wahl: "); // Aus Datenstrukturen.h/.cpp

        switch (menuChoice) {
            case 1: { // Spiel starten / Charakterauswahl + KAMPF
                std::cout << "\nSpiel wird gestartet..." << std::endl;
                sleepMilliseconds(500);
                try {
                    std::pair<Player, Player> players = selectCharacters(available_characters); // Aus Datenstrukturen.h/.cpp
                    Player player1 = players.first;
                    Player player2 = players.second;

                    std::cout << "\nCharaktere ausgewaehlt:" << std::endl;
                    std::cout << "Spieler 1 (" << player1.name << "): " << player1.character_data.getName() << std::endl;
                    std::cout << "Spieler 2 (" << player2.name << "): " << player2.character_data.getName() << std::endl;

                    // --- KAMPF STARTEN ---
                    startCombat(player1, player2); // Ruft Funktion aus Kampf.h/.cpp auf

                } catch (const std::exception& e) {
                    std::cerr << "\nEin Fehler ist aufgetreten: " << e.what() << std::endl;
                    std::cout << "Kehre zum Hauptmenue zurueck." << std::endl;
                    sleepMilliseconds(2000);
                }
                 std::cout << "\nDruecke Enter um zum Hauptmenue zurueckzukehren...";
                 getchar(); // Einfaches Warten auf Enter
                break;
            }
            case 2: { // Charakter Info
                displayAllCharacterInfo(available_characters); // Ruft Funktion aus Charakterinformationen.h/.cpp auf
                // Die Funktion wartet intern auf Enter.
                break;
            }
            case 3: { // Beenden
                std::cout << "Spiel wird beendet. Auf Wiedersehen!" << std::endl;
                break;
            }
            default:
                std::cout << "Ungueltige Wahl. Bitte 1, 2 oder 3 eingeben." << std::endl; // Angepasste Meldung
                sleepMilliseconds(1000);
                break;
        }
    } while (menuChoice != 3); // Bedingung wieder auf 3


    return 0; // Programm erfolgreich beenden
}
