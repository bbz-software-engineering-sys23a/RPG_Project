#include "Kampf.h"           // Eigenen Header einbinden
#include "Datenstrukturen.h" // Für Player, Character-Methoden (via Charaktere.h), Utils
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <algorithm> // Für std::max, std::swap, std::transform, std::min
#include <limits>
#include <cctype>    // Für std::tolower
#include <stdexcept> // Für std::stoi Fehler
#include <cstdlib>   // Für exit()
#include <utility>   // Für std::swap


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

    // Sicherstellen, dass der Verteidigungsbonus zu Beginn 0 ist
    player1.defense_bonus = 0;
    player2.defense_bonus = 0;

    std::cout << "\n" << currentPlayer->name << " (" << currentPlayer->character_data.getName() << ") beginnt!" << std::endl;
    sleepMilliseconds(1500);

    int round = 1;
    while (!player1.character_data.isDefeated() && !player2.character_data.isDefeated()) {
        std::cout << "\n---------- Runde " << round << ": " << currentPlayer->name << " (" << currentPlayer->character_data.getName() << ") ist am Zug ----------" << std::endl;

        // OWP Regeneration zu Beginn des Zuges
        std::cout << currentPlayer->name << " regeneriert OWP." << std::endl;
        currentPlayer->character_data.addOwp(10);
        sleepMilliseconds(800);

        // Zeige HP und OWP beider Spieler an
        std::cout << currentPlayer->name << " HP: " << currentPlayer->character_data.getHP() << "/" << currentPlayer->character_data.max_hp
                  << " OWP: " << currentPlayer->character_data.getOwp() << "/" << currentPlayer->character_data.max_owp
                  << "  |  " << opponentPlayer->name << " HP: " << opponentPlayer->character_data.getHP() << "/" << opponentPlayer->character_data.max_hp
                  << " OWP: " << opponentPlayer->character_data.getOwp() << "/" << opponentPlayer->character_data.max_owp
                  << std::endl;
        sleepMilliseconds(1000);


        // --- Aktion auswählen (Angriff 1/2, Verteidigen, Exit) ---
        std::cout << "\nWähle deine Aktion:" << std::endl;
        currentPlayer->character_data.displayAttackOptions(); // Zeigt Optionen 1 und 2 mit Kosten an
        std::cout << "  3: Verteidigen (Verbessert Verteidigung nächste Runde, +5 OWP)" << std::endl;

        int choice = 0;
        std::string inputLine;
        bool affordable = false;

        while (choice == 0 || !affordable) { // Schleife bis gültige UND bezahlbare Wahl
            choice = 0;
            affordable = false;

            std::cout << "Deine Wahl (1, 2, 3 oder 'Exit' zum Beenden): ";
            if (!std::getline(std::cin, inputLine)) {
                std::cerr << "Fehler bei der Eingabe. Breche ab." << std::endl;
                exit(1);
            }
            std::string lowerInput = inputLine;
            std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(),
                           [](unsigned char c){ return std::tolower(c); });

            if (lowerInput == "exit") {
                std::cout << "\nSpiel wird auf Wunsch beendet..." << std::endl;
                exit(0);
            }
            try {
                size_t processedChars = 0;
                int choiceNum = std::stoi(inputLine, &processedChars);
                if (processedChars == inputLine.length()) {
                     if (choiceNum >= 1 && choiceNum <= 3) {
                        choice = choiceNum;

                        // Kostenprüfung für Angriffe
                        if (choice == 1) {
                            const int ATTACK1_COST = 5;
                            if (currentPlayer->character_data.canAfford(ATTACK1_COST)) {
                                affordable = true;
                            } else {
                                std::cout << "Nicht genug Ominöses weißes Pulver für '" << currentPlayer->character_data.attack1.name << "'! (Benötigt: "
                                          << ATTACK1_COST << ", Vorhanden: " << currentPlayer->character_data.getOwp() << ")" << std::endl;
                                sleepMilliseconds(1500);
                            }
                        } else if (choice == 2) {
                            const AttackData& attack2_data = currentPlayer->character_data.attack2;
                            if (currentPlayer->character_data.canAfford(attack2_data.base_damage)) {
                                affordable = true;
                            } else {
                                std::cout << "Nicht genug Ominöses weißes Pulver für '" << attack2_data.name << "'! (Benötigt: "
                                          << attack2_data.base_damage << ", Vorhanden: " << currentPlayer->character_data.getOwp() << ")" << std::endl;
                                sleepMilliseconds(1500);
                            }
                        } else { // choice == 3 (Verteidigen)
                             affordable = true;
                        }
                    } else {
                        std::cout << "Ungültige Zahl. Bitte 1, 2, 3 eingeben (oder 'Exit')." << std::endl;
                    }
                } else {
                     std::cout << "Ungültige Eingabe. Bitte 1, 2, 3 oder 'Exit' eingeben." << std::endl;
                }
            } catch (const std::invalid_argument& e) {
                std::cout << "Ungültige Eingabe. Bitte 1, 2, 3 oder 'Exit' eingeben." << std::endl;
            } catch (const std::out_of_range& e) {
                 std::cout << "Zahl außerhalb des gültigen Bereichs." << std::endl;
            }
        } // Ende der Aktionsauswahl-Schleife

        // --- Aktion ausführen ---
        if (choice == 1 || choice == 2) { // Angriff gewählt
            const AttackData& chosenAttack = (choice == 1) ? currentPlayer->character_data.attack1 : currentPlayer->character_data.attack2;
            int owpCost = (choice == 1) ? 5 : chosenAttack.base_damage;

            bool paid = currentPlayer->character_data.useOwp(owpCost);
            if (paid) {
                 std::cout << currentPlayer->name << " verbraucht " << owpCost << " OWP. Verbleibend: " << currentPlayer->character_data.getOwp() << std::endl;
                 sleepMilliseconds(1000);
            } else {
                 std::cerr << "[FEHLER] Konnte OWP nicht verbrauchen trotz canAfford Check!" << std::endl;
                 continue;
            }


            std::cout << "\n" << currentPlayer->name << " setzt '" << chosenAttack.name << "' ein!" << std::endl;
            sleepMilliseconds(1200);

            int attackRoll = rollD20();
            int defenseRoll = rollD20();
            bool is_crit = (attackRoll == 20);
            bool is_fumble = (attackRoll == 1);

            std::cout << currentPlayer->name << " würfelt eine " << attackRoll << " (Angriff)." << std::endl;
            sleepMilliseconds(600);
            std::cout << opponentPlayer->name << " würfelt eine " << defenseRoll << " (Verteidigung)." << std::endl;
            sleepMilliseconds(1200);

            int current_defense_bonus = opponentPlayer->defense_bonus;
            opponentPlayer->defense_bonus = 0;

            int potentialDamage = 0;
            if (is_fumble) {
                std::cout << ">>> FEHLSCHLAG! <<< Der Angriff geht daneben!" << std::endl;
                potentialDamage = 0;
                sleepMilliseconds(800);
            } else {
                 potentialDamage = std::max(0, chosenAttack.base_damage + attackRoll - defenseRoll - current_defense_bonus);
                 if (current_defense_bonus > 0) {
                      std::cout << opponentPlayer->name << " verteidigt sich verbessert! (Bonus: " << current_defense_bonus << ")" << std::endl;
                      sleepMilliseconds(800);
                 }
                 if (is_crit) {
                     std::cout << ">>> KRITISCHER TREFFER! <<<" << std::endl;
                     potentialDamage = static_cast<int>(potentialDamage * 1.5);
                     sleepMilliseconds(800);
                 }
            }

            std::cout << opponentPlayer->name << " versucht: \"" << opponentPlayer->character_data.defense_action_text << "\"" << std::endl;
            sleepMilliseconds(1200);

            if (potentialDamage > 0) {
                 opponentPlayer->character_data.takeDamage(potentialDamage);
            } else {
                 std::cout << opponentPlayer->name << " weicht aus oder der Angriff war zu schwach! (0 Schaden)" << std::endl;
            }

            // OWP Generierung bei Angriff 1 wurde entfernt

        } else if (choice == 3) { // Verteidigen gewählt
            int defense_bonus_amount = 5;
            int owp_gain_defense = 5;
            currentPlayer->defense_bonus = defense_bonus_amount;
            currentPlayer->character_data.addOwp(owp_gain_defense);
            std::cout << "\n" << currentPlayer->name << " geht in Verteidigungshaltung! (Nächster Verteidigungswurf +" << defense_bonus_amount << ")" << std::endl;
            std::cout << currentPlayer->name << " sammelt " << owp_gain_defense << " Ominöses weißes Pulver! Aktuell: " << currentPlayer->character_data.getOwp() << std::endl;
            sleepMilliseconds(1500);
        }
        // --- Ende Aktion ausführen ---


        std::cout << opponentPlayer->name << " hat noch " << opponentPlayer->character_data.getHP() << " HP." << std::endl;
        sleepMilliseconds(2500);

        // Siegbedingung prüfen
        if (opponentPlayer->character_data.isDefeated()) {
            std::cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            std::cout << opponentPlayer->name << " (" << opponentPlayer->character_data.getName() << ") wurde besiegt!" << std::endl;
            std::cout << currentPlayer->name << " (" << currentPlayer->character_data.getName() << ") hat gewonnen!" << std::endl;
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            displayVictoryFireworks(); // Zeigt jetzt den Pokal an
            break;
        }

        // Spieler wechseln für die nächste Runde
        std::swap(currentPlayer, opponentPlayer);
        round++;
    }
     std::cout << "\n==================== KAMPFENDE ======================" << std::endl;
}

// --- Implementierung Siegesanimation (Jetzt am Ende) ---
void displayVictoryFireworks() {
    // ASCII-Art: Pokal (Vorschlag C)
    std::cout << R"(

          ___________
         '._==_==_.'
         .-\:      /-.
        | (|:.     |) |
         '-|:.     |-'
           \::.    /
            '::. .'
              ) (
            _.' '._
           `-------`
         * * S I E G * *

    )" << std::endl;
    sleepMilliseconds(2500); // Pause, um die Grafik anzusehen
}
