#include "Kampf.h"           // Eigenen Header einbinden
#include "Datenstrukturen.h" // Für Player, Character-Methoden (via Charaktere.h), Utils
#include "Dice.h"            // Für die Dice-Klasse
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


// Hilfsfunktion zur Konvertierung von int zu CombatAction (intern, unverändert)
namespace {
    CombatAction intToCombatAction(int choice) {
        switch (choice) {
            case 1: return CombatAction::Attack1;
            case 2: return CombatAction::Attack2;
            case 3: return CombatAction::Defend;
            default: return CombatAction::Invalid;
        }
    }
}

// --- Implementierung Kampflogik (angepasst an Dice-Parameter) ---
void startCombat(Player& player1, Player& player2, const Dice& dice) {
    std::cout << "\n==================== KAMPFBEGINN ====================" << std::endl;
    player1.getCharacterData().displayInfo();
    std::cout << "                    VS" << std::endl;
    player2.getCharacterData().displayInfo();
    std::cout << "=====================================================" << std::endl;
    sleepMilliseconds(2500);

    // Bestimme Startspieler mit dem übergebenen Würfel
    // Annahme: Höherer Wurf gewinnt Initiative? Oder 50/50? Hier 50/50 bei D20, passt nicht für andere.
    // Besser wäre vielleicht: if (dice.roll() > dice.getMaxValue() / 2) ... oder ähnliches.
    // Wir lassen es vorerst bei der einfachen Logik: <= 10 gewinnt Spieler 1
    Player* currentPlayer = (dice.roll() <= 10) ? &player1 : &player2;
    Player* opponentPlayer = (currentPlayer == &player1) ? &player2 : &player1;

    player1.resetDefenseBonus();
    player2.resetDefenseBonus();

    std::cout << "\n" << currentPlayer->getName() << " (" << currentPlayer->getCharacterData().getName() << ") beginnt!" << std::endl;
    sleepMilliseconds(1500);

    int round = 1;
    while (!player1.getCharacterData().isDefeated() && !player2.getCharacterData().isDefeated()) {
        std::cout << "\n---------- Runde " << round << ": " << currentPlayer->getName() << " (" << currentPlayer->getCharacterData().getName() << ") ist am Zug ----------" << std::endl;

        currentPlayer->getCharacterData().addOwp(10);
        sleepMilliseconds(800);

        const Character& currentChara = currentPlayer->getCharacterData();
        const Character& opponentChara = opponentPlayer->getCharacterData();
        std::cout << currentPlayer->getName() << " HP: " << currentChara.getHP() << "/" << currentChara.getMaxHP()
                  << " OWP: " << currentChara.getOwp() << "/" << currentChara.getMaxOwp()
                  << "  |  " << opponentPlayer->getName() << " HP: " << opponentChara.getHP() << "/" << opponentChara.getMaxHP()
                  << " OWP: " << opponentChara.getOwp() << "/" << opponentChara.getMaxOwp()
                  << std::endl;
        sleepMilliseconds(1000);

        std::cout << "\nWähle deine Aktion:" << std::endl;
        currentChara.displayAttackOptions();
        std::cout << "  3: Verteidigen (Verbessert Verteidigung nächste Runde, +5 OWP)" << std::endl;

        CombatAction chosenAction = CombatAction::Invalid;
        std::string inputLine;
        bool affordable = false;

        while (chosenAction == CombatAction::Invalid || !affordable) {
            // ... (Input-Logik bleibt gleich) ...
             chosenAction = CombatAction::Invalid;
            affordable = false;
            std::cout << "Deine Wahl (1, 2, 3 oder 'Exit' zum Beenden): ";
            if (!std::getline(std::cin, inputLine)) {
                std::cerr << "Fehler bei der Eingabe. Breche ab." << std::endl; exit(1);
            }
            std::string lowerInput = inputLine;
            std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(),
                           [](unsigned char c){ return std::tolower(c); });

            if (lowerInput == "exit") {
                chosenAction = CombatAction::Exit; affordable = true;
            } else {
                try {
                    size_t processedChars = 0;
                    int choiceNum = std::stoi(inputLine, &processedChars);
                    if (processedChars == inputLine.length()) {
                        chosenAction = intToCombatAction(choiceNum);
                        if (chosenAction != CombatAction::Invalid) {
                            if (chosenAction == CombatAction::Attack1) {
                                const int ATTACK1_COST = 5;
                                if (currentChara.canAfford(ATTACK1_COST)) { affordable = true; }
                                else { std::cout << "Nicht genug OWP für '" << currentChara.getAttack1().name << "'! (...)\n"; sleepMilliseconds(1500); }
                            } else if (chosenAction == CombatAction::Attack2) {
                                const AttackData& attack2_data = currentChara.getAttack2();
                                if (currentChara.canAfford(attack2_data.base_damage)) { affordable = true; }
                                else { std::cout << "Nicht genug OWP für '" << attack2_data.name << "'! (...)\n"; sleepMilliseconds(1500); }
                            } else { affordable = true; } // Defend
                        } else { std::cout << "Ungültige Zahl.\n"; }
                    } else { std::cout << "Ungültige Eingabe.\n"; }
                } catch (const std::exception& e) { std::cout << "Ungültige Eingabe.\n"; }
            }
        } // Ende Input-Schleife

        if (chosenAction == CombatAction::Exit) {
            std::cout << "\nSpiel wird auf Wunsch beendet..." << std::endl; exit(0);
        }

        Character& currentCharaMod = currentPlayer->getCharacterData();
        Character& opponentCharaMod = opponentPlayer->getCharacterData();

        if (chosenAction == CombatAction::Attack1 || chosenAction == CombatAction::Attack2) {
            const AttackData& chosenAttackData = (chosenAction == CombatAction::Attack1) ? currentChara.getAttack1() : currentChara.getAttack2();
            int owpCost = (chosenAction == CombatAction::Attack1) ? 5 : chosenAttackData.base_damage;

            bool paid = currentCharaMod.useOwp(owpCost);
            if (paid) {
                 std::cout << currentPlayer->getName() << " verbraucht " << owpCost << " OWP. Verbleibend: " << currentCharaMod.getOwp() << std::endl;
                 sleepMilliseconds(1000);
            } else { continue; }

            std::cout << "\n" << currentPlayer->getName() << " setzt '" << chosenAttackData.name << "' ein!" << std::endl;
            sleepMilliseconds(1200);

            int attackRoll = dice.roll();
            int defenseRoll = dice.roll();

            // *** HIER die angepasste Krit-Logik ***
            bool is_crit = (attackRoll == dice.getMaxValue()); // Krit bei Maximalwurf des aktuellen Würfels
            bool is_fumble = (attackRoll == 1); // Fumble bleibt bei 1

            std::cout << currentPlayer->getName() << " würfelt eine " << attackRoll << " (Angriff)." << std::endl;
            sleepMilliseconds(600);
            std::cout << opponentPlayer->getName() << " würfelt eine " << defenseRoll << " (Verteidigung)." << std::endl;
            sleepMilliseconds(1200);

            int current_defense_bonus = opponentPlayer->getDefenseBonus();
            opponentPlayer->resetDefenseBonus();

            int potentialDamage = 0;
            if (is_fumble) {
                std::cout << ">>> FEHLSCHLAG! <<< Der Angriff geht daneben!" << std::endl;
                potentialDamage = 0; sleepMilliseconds(800);
            } else {
                 potentialDamage = std::max(0, chosenAttackData.base_damage + attackRoll - defenseRoll - current_defense_bonus);
                 if (current_defense_bonus > 0) { std::cout << opponentPlayer->getName() << " verteidigt sich verbessert! (Bonus: " << current_defense_bonus << ")" << std::endl; sleepMilliseconds(800); }
                 if (is_crit) { std::cout << ">>> KRITISCHER TREFFER! <<<" << std::endl; potentialDamage = static_cast<int>(potentialDamage * 1.5); sleepMilliseconds(800); }
            }

            std::cout << opponentPlayer->getName() << " versucht: \"" << opponentChara.getDefenseActionText() << "\"" << std::endl;
            sleepMilliseconds(1200);

            if (potentialDamage > 0) {
                 opponentCharaMod.takeDamage(potentialDamage);
            } else {
                 std::cout << opponentPlayer->getName() << " weicht aus oder der Angriff war zu schwach! (0 Schaden)" << std::endl;
            }

        } else if (chosenAction == CombatAction::Defend) {
            int defense_bonus_amount = 5;
            int owp_gain_defense = 5;
            currentPlayer->setDefenseBonus(defense_bonus_amount);
            currentCharaMod.addOwp(owp_gain_defense);
            std::cout << "\n" << currentPlayer->getName() << " geht in Verteidigungshaltung! (Nächster Verteidigungswurf +" << defense_bonus_amount << ")" << std::endl;
            std::cout << currentPlayer->getName() << " sammelt " << owp_gain_defense << " Ominöses weißes Pulver! Aktuell: " << currentCharaMod.getOwp() << std::endl;
            sleepMilliseconds(1500);
        }

        std::cout << opponentPlayer->getName() << " hat noch " << opponentChara.getHP() << " HP." << std::endl;
        sleepMilliseconds(2500);

        if (opponentChara.isDefeated()) {
            std::cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            std::cout << opponentPlayer->getName() << " (" << opponentChara.getName() << ") wurde besiegt!" << std::endl;
            std::cout << currentPlayer->getName() << " (" << currentChara.getName() << ") hat gewonnen!" << std::endl;
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            displayVictoryFireworks();
            break;
        }

        std::swap(currentPlayer, opponentPlayer);
        round++;
    } // Ende while Kampfschleife

     std::cout << "\n==================== KAMPFENDE ======================" << std::endl;
}

// --- Implementierung Siegesanimation (unverändert) ---
void displayVictoryFireworks() {
    // ASCII-Art: Pokal
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
