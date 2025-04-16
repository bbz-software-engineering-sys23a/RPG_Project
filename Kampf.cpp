#include "Kampf.h"           // Eigenen Header einbinden
#include "Datenstrukturen.h" // Für Player, Character-Methoden (via Charaktere.h), Utils
#include <iostream>
#include <vector>
#include <string>
#include <chrono>           // Für sleepMilliseconds
#include <thread>           // Für sleepMilliseconds
#include <algorithm>        // Für std::max, std::swap
#include <limits>           // Für getIntegerInput Fehlerbehandlung (indirekt)
#include <cctype>           // Für std::tolower
#include <stdexcept>        // Für std::stoi Fehler
#include <cstdlib>          // Für exit()
#include <utility>          // Für std::swap
#include <windows.h> // Für UTF-8 Darstellung

// --- Implementierung Siegesanimation ---
void displayVictoryFireworks() {
    // ASCII-Art für ein einfaches Feuerwerk
    std::cout << R"(

          .''.      * .''`.         *
         :_\\_:   _\(/)_   :_\\_:   .''.
        //o\\    / / | \ \  //o\\  :_\\_:
       (_.-._)    * * * (_.-._) //o\\
        `-' ____ / \ | / \ ____ `-' (_.-._)
           |   | `.' | `.` |   |    `-'  *
           | S |     |     | I |    .''.
           | I |    \|/    | E |   :_\\_:
           | E |   --*--   | G |   //o\\
           | G |    /|\    | ! |  (_.-._)
           | ! |     |     | ! |   `-'
           | ! |           | ! |
          ======         ======

    )" << std::endl;
    sleepMilliseconds(2500); // Pause, um das Feuerwerk anzusehen
}


// --- Implementierung Kampflogik ---
void startCombat(Player& player1, Player& player2) {
    // Setze die Codepage auf UTF-8
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "\n==================== KAMPFBEGINN ====================" << std::endl;
    player1.character_data.displayInfo();
    std::cout << "                    VS" << std::endl;
    player2.character_data.displayInfo();
    std::cout << "=====================================================" << std::endl;
    sleepMilliseconds(2500);

    Player* currentPlayer = (rollD20() <= 10) ? &player1 : &player2;
    Player* opponentPlayer = (currentPlayer == &player1) ? &player2 : &player1;

    std::cout << "\n" << currentPlayer->name << " (" << currentPlayer->character_data.getName() << ") beginnt!" << std::endl;
    sleepMilliseconds(1500);

    int round = 1;
    while (!player1.character_data.isDefeated() && !player2.character_data.isDefeated()) {
        std::cout << "\n---------- Runde " << round << ": " << currentPlayer->name << " (" << currentPlayer->character_data.getName() << ") ist am Zug ----------" << std::endl;
        std::cout << currentPlayer->name << " HP: " << currentPlayer->character_data.getHP() << "/" << currentPlayer->character_data.max_hp
                  << "  |  " << opponentPlayer->name << " HP: " << opponentPlayer->character_data.getHP() << "/" << opponentPlayer->character_data.max_hp << std::endl;
        sleepMilliseconds(1000);

        // --- Angriff auswählen (mit Exit-Option) ---
        std::cout << "\nWähle deinen Angriff:" << std::endl;
        currentPlayer->character_data.displayAttackOptions();
        // --- Angriff auswählen (ANGEPASSTE LOGIK) ---
        std::cout << "\nWähle deinen Angriff:" << std::endl;
        currentPlayer->character_data.displayAttackOptions(); // Zeigt Optionen 1 und 2 an

        int attackChoice = 0;
        std::string inputLine;

        while (attackChoice == 0) {
            std::cout << "Deine Wahl (1 oder 2, oder 'Exit' zum Beenden): ";
            if (!std::getline(std::cin, inputLine)) {
                std::cerr << "Fehler bei der Eingabe. Breche ab." << std::endl;
                exit(1);
            }

            // Kopie für Umwandlung in Kleinbuchstaben
            std::string lowerInput = inputLine;
            // Wandle Eingabe in Kleinbuchstaben um für Vergleich
            std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(),
                           [](unsigned char c){ return std::tolower(c); });

            // Prüfe auf "exit"
            if (lowerInput == "exit") {
                std::cout << "\nSpiel wird auf Wunsch beendet..." << std::endl;
                exit(0);
            }

            // Versuche, die Eingabe in eine Zahl umzuwandeln
            try {
                size_t processedChars = 0;
                int choiceNum = std::stoi(inputLine, &processedChars);
                if (processedChars == inputLine.length()) {
                     if (choiceNum == 1 || choiceNum == 2) {
                        attackChoice = choiceNum;
                    } else {
                        std::cout << "Ungültige Zahl. Bitte 1 oder 2 eingeben (oder 'Exit')." << std::endl;
                    }
                } else {
                     std::cout << "Ungültige Eingabe. Bitte nur 1, 2 oder 'Exit' eingeben." << std::endl;
                }
            } catch (const std::invalid_argument& e) {
                std::cout << "Ungültige Eingabe. Bitte 1, 2 oder 'Exit' eingeben." << std::endl;
                // Fehler bei stoi: Eingabe war keine Zahl (und nicht "exit")
                std::cout << "Ungültige Eingabe. Bitte 1, 2 oder 'Exit' eingeben." << std::endl;
            } catch (const std::out_of_range& e) {
                 std::cout << "Zahl ausserhalb des gültigen Bereichs." << std::endl;
                // Fehler bei stoi: Zahl zu gross/klein für int
                 std::cout << "Zahl ausserhalb des gültigen Bereichs." << std::endl;
            }
            // Wenn attackChoice immer noch 0 ist, wird die Schleife wiederholt
        }
        // --- Ende Angriff auswählen ---

        // Angriff durchführen (Rest der Logik bleibt gleich)
        const AttackData& chosenAttack = (attackChoice == 1) ? currentPlayer->character_data.attack1 : currentPlayer->character_data.attack2;
        std::cout << "\n" << currentPlayer->name << " setzt '" << chosenAttack.name << "' ein!" << std::endl;
        sleepMilliseconds(1200);

        int attackRoll = rollD20();
        int defenseRoll = rollD20();
        std::cout << currentPlayer->name << " würfelt eine " << attackRoll << " (Angriff)." << std::endl;
        sleepMilliseconds(600);
        std::cout << opponentPlayer->name << " würfelt eine " << defenseRoll << " (Verteidigung)." << std::endl;
        sleepMilliseconds(1200);

        int potentialDamage = std::max(0, chosenAttack.base_damage + attackRoll - defenseRoll);

        std::cout << opponentPlayer->name << " versucht: \"" << opponentPlayer->character_data.defense_action_text << "\"" << std::endl;
        sleepMilliseconds(1200);

        if (potentialDamage > 0) {
             opponentPlayer->character_data.takeDamage(potentialDamage);
        } else {
             std::cout << opponentPlayer->name << " weicht aus oder der Angriff war zu schwach! (0 Schaden)" << std::endl;
        }

        std::cout << opponentPlayer->name << " hat noch " << opponentPlayer->character_data.getHP() << " HP." << std::endl;
        sleepMilliseconds(2500);

        // Siegbedingung prüfen
        if (opponentPlayer->character_data.isDefeated()) {
            std::cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            std::cout << opponentPlayer->name << " (" << opponentPlayer->character_data.getName() << ") wurde besiegt!" << std::endl;
            std::cout << currentPlayer->name << " (" << currentPlayer->character_data.getName() << ") hat gewonnen!" << std::endl;
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

            // !!! HIER WIRD DAS FEUERWERK AUFGERUFEN !!!
            displayVictoryFireworks();

            break; // Kampfschleife verlassen
        }

        // Spieler wechseln für die nächste Runde
        std::swap(currentPlayer, opponentPlayer);
        round++;
    }
     std::cout << "\n==================== KAMPFENDE ======================" << std::endl;
}
