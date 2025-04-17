#include <iostream>
#include <vector>
#include <string>
#include <utility>   // Für std::pair
#include <stdexcept> // Für std::exception
#include <limits>    // Für numeric_limits
#include <cstdio>    // Für getchar()
#include <memory>    // Für std::unique_ptr, std::make_unique

// Eigene Header einbinden
#include "StartScreen.h"
#include "Datenstrukturen.h" // Enthält MainMenuOption Enum
#include "Kampf.h"           // Enthält startCombat Deklaration
#include "Charakterinformationen.h"
#include "Dice.h"            // Enthält Dice Klassen
#include <windows.h>         // Für UTF-8 Darstellung

// Funktion zur Auswahl des Würfels
std::unique_ptr<Dice> selectDiceType() {
    std::cout << "\n--- Würfeltyp auswählen ---" << std::endl;
    std::cout << "1: D10 (Würfelt 1-10)" << std::endl;
    std::cout << "2: D20 (Würfelt 1-20)" << std::endl;
    std::cout << "3: D40 (Würfelt 1-40)" << std::endl;

    int choice = 0;
    while (choice < 1 || choice > 3) {
        choice = getIntegerInputSimple("Wähle den Würfeltyp (1-3): ");
        if (choice < 1 || choice > 3) {
            std::cout << "Ungültige Auswahl." << std::endl;
        }
    }

    switch (choice) {
        case 1:
            std::cout << "D10 ausgewählt." << std::endl;
            return std::make_unique<D10Dice>();
        case 3:
            std::cout << "D40 ausgewählt." << std::endl;
            return std::make_unique<D40Dice>();
        case 2:
        default: // Fallback auf D20
            std::cout << "D20 ausgewählt." << std::endl;
            return std::make_unique<D20Dice>();
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    // initializeRandomSeed(); // Wird jetzt in Dice::initializeRNG() gemacht, wenn nötig

    displayStartScreen();

    std::vector<Character> available_characters;
    try {
        available_characters = initializeCharacters();
        if (available_characters.empty()) {
             std::cerr << "FEHLER: Keine Charakterdaten initialisiert..." << std::endl; return 1;
        }
        std::cout << "\nCharakterdaten geladen." << std::endl;
        sleepMilliseconds(1000);
    } catch (const std::exception& e) {
         std::cerr << "FEHLER beim Initialisieren der Charaktere: " << e.what() << std::endl; return 1;
    }

    // Aktuellen Würfel initialisieren (Standard: D20)
    std::unique_ptr<Dice> currentDice = std::make_unique<D20Dice>();
    std::string currentDiceName = "D20"; // Name für Anzeige

    MainMenuOption menuChoice = MainMenuOption::Invalid;
    do {
        std::cout << "\n--- Hauptmenü ---" << std::endl;
        std::cout << "1: Spiel Starten (Aktueller Würfel: " << currentDiceName << ")" << std::endl;
        std::cout << "2: Charakter Info Anzeigen" << std::endl;
        std::cout << "3: Würfel ändern" << std::endl; // Neue Option
        std::cout << "4: Beenden" << std::endl;       // Exit ist jetzt 4

        menuChoice = getMainMenuInput("Deine Wahl: ");

        switch (menuChoice) {
            case MainMenuOption::Start: {
                std::cout << "\nSpiel wird gestartet..." << std::endl;
                sleepMilliseconds(500);
                try {
                    std::pair<Player, Player> players = selectCharacters(available_characters);
                    Player player1 = players.first;
                    Player player2 = players.second;

                    std::cout << "\nCharaktere ausgewählt:" << std::endl;
                    std::cout << "Spieler 1 (" << player1.getName() << "): " << player1.getCharacterData().getName() << std::endl;
                    std::cout << "Spieler 2 (" << player2.getName() << "): " << player2.getCharacterData().getName() << std::endl;

                    // --- KAMPF STARTEN ---
                    // Übergebe das aktuell ausgewählte Dice-Objekt (dereferenziert)
                    startCombat(player1, player2, *currentDice);

                } catch (const std::exception& e) {
                    std::cerr << "\nEin Fehler ist aufgetreten: " << e.what() << std::endl;
                    std::cout << "Kehre zum Hauptmenü zurück." << std::endl;
                    sleepMilliseconds(2000);
                }
                 std::cout << "\nDrücke Enter um zum Hauptmenü zurückzukehren...";
                 std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            case MainMenuOption::Info: {
                displayAllCharacterInfo(available_characters);
                break;
            }
            case MainMenuOption::SelectDice: { // Neue Option: Würfel auswählen
                std::unique_ptr<Dice> newDice = selectDiceType();
                // Dynamische Typüberprüfung, um den Namen zu setzen (optional, aber nett für UI)
                if (dynamic_cast<D10Dice*>(newDice.get())) {
                    currentDiceName = "D10";
                } else if (dynamic_cast<D20Dice*>(newDice.get())) {
                    currentDiceName = "D20";
                } else if (dynamic_cast<D40Dice*>(newDice.get())) {
                    currentDiceName = "D40";
                }
                currentDice = std::move(newDice); // Übertrage Besitz des neuen Würfels
                sleepMilliseconds(1000);
                break;
            }
            case MainMenuOption::Exit: {
                std::cout << "Spiel wird beendet. Auf Wiedersehen!" << std::endl;
                break;
            }
            default: // Behandelt MainMenuOption::Invalid
                std::cout << "Ungültige Wahl. Bitte 1, 2, 3 oder 4 eingeben." << std::endl;
                sleepMilliseconds(1000);
                break;
        }
    } while (menuChoice != MainMenuOption::Exit);


    return 0;
}
