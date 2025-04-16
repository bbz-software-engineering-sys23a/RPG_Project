#include "Kampf.h"           // Eigenen Header einbinden
#include "Datenstrukturen.h" // Für Player, Character-Methoden (via Charaktere.h), Utils
#include <iostream>
#include <vector>
#include <string>           // Für std::string und std::getline, std::stoi
#include <chrono>           // Für sleepMilliseconds
#include <thread>           // Für sleepMilliseconds
#include <algorithm>        // Für std::max, std::swap, std::transform
#include <limits>           // Für getIntegerInput (obwohl hier nicht mehr direkt verwendet)
#include <cctype>           // Für std::tolower
#include <stdexcept>        // Für std::stoi Fehler
#include <cstdlib>          // Für exit()

// --- Implementierung Kampflogik ---
void startCombat(Player& player1, Player& player2) {
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

        // --- Angriff auswählen (ANGEPASSTE LOGIK) ---
        std::cout << "\nWaehle deinen Angriff:" << std::endl;
        currentPlayer->character_data.displayAttackOptions(); // Zeigt Optionen 1 und 2 an

        int attackChoice = 0; // 0 bedeutet ungültig/noch nicht gewählt
        std::string inputLine;

        while (attackChoice == 0) {
            std::cout << "Deine Wahl (1 oder 2, oder 'Exit' zum Beenden): ";
            // Lese die gesamte Zeile ein, um "Exit" zu erkennen
            if (!std::getline(std::cin, inputLine)) {
                // Fehler beim Lesen der Eingabe (z.B. Dateiende)
                std::cerr << "Fehler bei der Eingabe. Breche ab." << std::endl;
                exit(1); // Beende das Programm bei schwerem Eingabefehler
            }

            // Kopie für Umwandlung in Kleinbuchstaben
            std::string lowerInput = inputLine;
            // Wandle Eingabe in Kleinbuchstaben um für Vergleich
            std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(),
                           [](unsigned char c){ return std::tolower(c); });

            // Prüfe auf "exit"
            if (lowerInput == "exit") {
                std::cout << "\nSpiel wird auf Wunsch beendet..." << std::endl;
                exit(0); // Beendet das gesamte Programm
            }

            // Versuche, die Eingabe in eine Zahl umzuwandeln
            try {
                size_t processedChars = 0;
                int choiceNum = std::stoi(inputLine, &processedChars); // Versucht Umwandlung

                // Prüfen, ob die *gesamte* Eingabe eine Zahl war und im gültigen Bereich liegt
                if (processedChars == inputLine.length()) {
                     if (choiceNum == 1 || choiceNum == 2) {
                        attackChoice = choiceNum; // Gültige Wahl (1 oder 2)
                    } else {
                        std::cout << "Ungueltige Zahl. Bitte 1 oder 2 eingeben (oder 'Exit')." << std::endl;
                    }
                } else {
                     std::cout << "Ungueltige Eingabe. Bitte nur 1, 2 oder 'Exit' eingeben." << std::endl;
                }
            } catch (const std::invalid_argument& e) {
                // Fehler bei stoi: Eingabe war keine Zahl (und nicht "exit")
                std::cout << "Ungueltige Eingabe. Bitte 1, 2 oder 'Exit' eingeben." << std::endl;
            } catch (const std::out_of_range& e) {
                // Fehler bei stoi: Zahl zu gross/klein für int
                 std::cout << "Zahl ausserhalb des gueltigen Bereichs." << std::endl;
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
        std::cout << currentPlayer->name << " wuerfelt eine " << attackRoll << " (Angriff)." << std::endl;
        sleepMilliseconds(600);
        std::cout << opponentPlayer->name << " wuerfelt eine " << defenseRoll << " (Verteidigung)." << std::endl;
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

        if (opponentPlayer->character_data.isDefeated()) {
            std::cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            std::cout << opponentPlayer->name << " (" << opponentPlayer->character_data.getName() << ") wurde besiegt!" << std::endl;
            std::cout << currentPlayer->name << " (" << currentPlayer->character_data.getName() << ") hat gewonnen!" << std::endl;
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            break;
        }

        std::swap(currentPlayer, opponentPlayer);
        round++;
    }
     std::cout << "\n==================== KAMPFENDE ======================" << std::endl;
}
