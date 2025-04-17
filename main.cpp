#include <iostream>
#include <vector>
#include <string>
#include <utility> // Für std::pair
#include <stdexcept> // Für std::exception
#include <limits> // Für numeric_limits
#include <cstdio> // Für getchar()

// Eigene Header einbinden
#include "StartScreen.h"
#include "Datenstrukturen.h" // Inkludiert Charaktere.h indirekt, enthält jetzt MainMenuOption Enum
#include "Kampf.h"           // Header für die Kampffunktion
#include "Charakterinformationen.h" // Header für die Info-Anzeige
#include <windows.h> // Für UTF-8 Darstellung

int main() {
    // Setze die Codepage auf UTF-8
    SetConsoleOutputCP(CP_UTF8);
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
    MainMenuOption menuChoice = MainMenuOption::Invalid; // Verwende Enum
    do {
        std::cout << "\n--- Hauptmenü ---" << std::endl;
        std::cout << "1: Spiel Starten" << std::endl;
        std::cout << "2: Charakter Info Anzeigen" << std::endl;
        std::cout << "3: Beenden" << std::endl;

        // Verwendet neue Funktion aus Datenstrukturen, die Enum zurückgibt
        menuChoice = getMainMenuInput("Deine Wahl: ");

        switch (menuChoice) {
            case MainMenuOption::Start: { // Spiel starten / Charakterauswahl + KAMPF
                std::cout << "\nSpiel wird gestartet..." << std::endl;
                sleepMilliseconds(500);
                try {
                    // selectCharacters gibt std::pair<Player, Player> zurück
                    std::pair<Player, Player> players = selectCharacters(available_characters);
                    Player player1 = players.first; // Kopie oder Move, je nach Compiler/Optimierung
                    Player player2 = players.second;

                    std::cout << "\nCharaktere ausgewählt:" << std::endl;
                    // Verwende Getter für Namen und Charakterdaten
                    std::cout << "Spieler 1 (" << player1.getName() << "): " << player1.getCharacterData().getName() << std::endl;
                    std::cout << "Spieler 2 (" << player2.getName() << "): " << player2.getCharacterData().getName() << std::endl;

                    // --- KAMPF STARTEN ---
                    // Übergabe per Referenz, damit Änderungen im Kampf erhalten bleiben
                    startCombat(player1, player2);

                } catch (const std::exception& e) {
                    std::cerr << "\nEin Fehler ist aufgetreten: " << e.what() << std::endl;
                    std::cout << "Kehre zum Hauptmenü zurück." << std::endl;
                    sleepMilliseconds(2000);
                }
                 std::cout << "\nDrücke Enter um zum Hauptmenü zurückzukehren...";
                 // Sicherstellen, dass der Puffer leer ist, bevor auf Enter gewartet wird
                 // (Nötig, da getchar() sonst evtl. ein übrig gebliebenes '\n' liest)
                 std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                 // getchar(); // Alternative: Puffer leeren und dann getchar()
                break;
            }
            case MainMenuOption::Info: { // Charakter Info
                displayAllCharacterInfo(available_characters); // Ruft Funktion auf
                // Die Funktion displayAllCharacterInfo wartet intern auf Enter.
                break;
            }
            case MainMenuOption::Exit: { // Beenden
                std::cout << "Spiel wird beendet. Auf Wiedersehen!" << std::endl;
                break;
            }
            default: // Behandelt MainMenuOption::Invalid
                std::cout << "Ungültige Wahl. Bitte 1, 2 oder 3 eingeben." << std::endl;
                sleepMilliseconds(1000);
                break;
        }
    } while (menuChoice != MainMenuOption::Exit); // Bedingung verwendet Enum


    return 0; // Programm erfolgreich beenden
}
