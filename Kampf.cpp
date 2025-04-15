#include "Kampf.h"           // Eigenen Header einbinden
#include "Datenstrukturen.h" // Für Player, Character-Methoden und Utils
#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // Für std::max, std::swap
#include <utility>   // Für std::pair (obwohl hier nicht direkt gebraucht)

// Implementierung der Kampflogik (Code aus der alten startCombat Funktion)
void startCombat(Player& player1, Player& player2) {
    std::cout << "\n==================== KAMPFBEGINN ====================" << std::endl;
    player1.character_data.displayInfo();
    std::cout << "                    VS" << std::endl;
    player2.character_data.displayInfo();
    std::cout << "=====================================================" << std::endl;
    sleepMilliseconds(2500); // Funktion aus Datenstrukturen.h/.cpp

    // Zufällig Startspieler bestimmen
    Player* currentPlayer = (rollD20() <= 10) ? &player1 : &player2; // rollD20 aus Datenstrukturen.h/.cpp
    Player* opponentPlayer = (currentPlayer == &player1) ? &player2 : &player1;

    std::cout << "\n" << currentPlayer->name << " (" << currentPlayer->character_data.getName() << ") beginnt!" << std::endl;
    sleepMilliseconds(1500);

    int round = 1;
    // Kampfschleife
    while (!player1.character_data.isDefeated() && !player2.character_data.isDefeated()) {
        std::cout << "\n---------- Runde " << round << ": " << currentPlayer->name << " (" << currentPlayer->character_data.getName() << ") ist am Zug ----------" << std::endl;
        std::cout << currentPlayer->name << " HP: " << currentPlayer->character_data.getHP() << "/" << currentPlayer->character_data.max_hp
                  << "  |  " << opponentPlayer->name << " HP: " << opponentPlayer->character_data.getHP() << "/" << opponentPlayer->character_data.max_hp << std::endl;
        sleepMilliseconds(1000);

        // Angriff auswählen
        std::cout << "\nWaehle deinen Angriff:" << std::endl;
        currentPlayer->character_data.displayAttackOptions(); // Methode aus Character (Charaktere.h/.cpp)
        int attackChoice = -1;
        while (attackChoice != 1 && attackChoice != 2) {
             attackChoice = getIntegerInput("Deine Wahl (1 oder 2): "); // Funktion aus Datenstrukturen.h/.cpp
             if (attackChoice != 1 && attackChoice != 2) {
                 std::cout << "Ungueltige Wahl." << std::endl;
             }
        }

        // Angriff durchführen
        const AttackData& chosenAttack = (attackChoice == 1) ? currentPlayer->character_data.attack1 : currentPlayer->character_data.attack2;
        std::cout << "\n" << currentPlayer->name << " setzt '" << chosenAttack.name << "' ein!" << std::endl;
        sleepMilliseconds(1200);

        // Würfeln
        int attackRoll = rollD20();
        int defenseRoll = rollD20();
        std::cout << currentPlayer->name << " wuerfelt eine " << attackRoll << " (Angriff)." << std::endl;
        sleepMilliseconds(600);
        std::cout << opponentPlayer->name << " wuerfelt eine " << defenseRoll << " (Verteidigung)." << std::endl;
        sleepMilliseconds(1200);

        // Schaden berechnen (Potentialschaden)
        int potentialDamage = std::max(0, chosenAttack.base_damage + attackRoll - defenseRoll);

        // Verteidigungstext anzeigen
        std::cout << opponentPlayer->name << " versucht: \"" << opponentPlayer->character_data.defense_action_text << "\"" << std::endl;
        sleepMilliseconds(1200);

        // Schaden anwenden (takeDamage berücksichtigt Basis-Verteidigung)
        if (potentialDamage > 0) {
             opponentPlayer->character_data.takeDamage(potentialDamage); // Methode aus Character (Charaktere.h/.cpp)
        } else {
             std::cout << opponentPlayer->name << " weicht aus oder der Angriff war zu schwach! (0 Schaden)" << std::endl;
        }

        std::cout << opponentPlayer->name << " hat noch " << opponentPlayer->character_data.getHP() << " HP." << std::endl;
        sleepMilliseconds(2500);

        // Siegbedingung prüfen
        if (opponentPlayer->character_data.isDefeated()) { // Methode aus Character (Charaktere.h/.cpp)
            std::cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            std::cout << opponentPlayer->name << " (" << opponentPlayer->character_data.getName() << ") wurde besiegt!" << std::endl;
            std::cout << currentPlayer->name << " (" << currentPlayer->character_data.getName() << ") hat gewonnen!" << std::endl;
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            break; // Kampfschleife verlassen
        }

        // Spieler wechseln für die nächste Runde
        std::swap(currentPlayer, opponentPlayer);
        round++;
    }
     std::cout << "\n==================== KAMPFENDE ======================" << std::endl;
}
