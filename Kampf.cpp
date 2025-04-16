#include "Kampf.h"           // Eigenen Header einbinden
#include "Datenstrukturen.h" // Für Player, Character-Methoden (via Charaktere.h), Utils
#include <iostream>
#include <vector>
#include <string>           // Für std::string und std::getline, std::stoi
#include <chrono>           // Für sleepMilliseconds
#include <thread>           // Für sleepMilliseconds
#include <algorithm>        // Für std::max, std::swap, std::transform, std::min
#include <limits>           // Für getIntegerInput (obwohl hier nicht mehr direkt verwendet)
#include <cctype>           // Für std::tolower
#include <stdexcept>        // Für std::stoi Fehler
#include <cstdlib>          // Für exit()
#include <utility>          // Für std::swap

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

        // OWP Regeneration zu Beginn des Zuges
        std::cout << currentPlayer->name << " regeneriert OWP." << std::endl;
        currentPlayer->character_data.addOwp(10); // Fügt 10 OWP hinzu (max 100)
        sleepMilliseconds(800);

        // Zeige HP und OWP beider Spieler an
        std::cout << currentPlayer->name << " HP: " << currentPlayer->character_data.getHP() << "/" << currentPlayer->character_data.max_hp
                  << " OWP: " << currentPlayer->character_data.getOwp() << "/" << currentPlayer->character_data.max_owp // OWP Anzeige
                  << "  |  " << opponentPlayer->name << " HP: " << opponentPlayer->character_data.getHP() << "/" << opponentPlayer->character_data.max_hp
                  << " OWP: " << opponentPlayer->character_data.getOwp() << "/" << opponentPlayer->character_data.max_owp // OWP Anzeige
                  << std::endl;
        sleepMilliseconds(1000);


        // Angriff auswählen (mit Exit-Option und OWP-Prüfung)
        std::cout << "\nWaehle deinen Angriff:" << std::endl;
        currentPlayer->character_data.displayAttackOptions(); // Zeigt Optionen 1 und 2 mit Kosten an

        int attackChoice = 0;
        std::string inputLine;
        bool affordable = false; // Flag, ob der Angriff bezahlbar ist

        while (attackChoice == 0 || !affordable) { // Schleife bis gültige UND bezahlbare Wahl
            attackChoice = 0; // Zurücksetzen für den Fall, dass nur die Kostenprüfung fehlschlug
            affordable = false; // Zurücksetzen

            std::cout << "Deine Wahl (1 oder 2, oder 'Exit' zum Beenden): ";
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
                     if (choiceNum == 1 || choiceNum == 2) {
                        attackChoice = choiceNum; // Gültige Nummer gewählt

                        // Kostenprüfung
                        if (attackChoice == 1) {
                            affordable = true; // Angriff 1 kostet nichts
                        } else { // attackChoice == 2
                            const AttackData& attack2_data = currentPlayer->character_data.attack2;
                            if (currentPlayer->character_data.canAfford(attack2_data.base_damage)) {
                                affordable = true; // Genug OWP für Angriff 2
                            } else {
                                std::cout << "Nicht genug Ominoeses weisses Pulver fuer '" << attack2_data.name << "'! (Benoetigt: "
                                          << attack2_data.base_damage << ", Vorhanden: " << currentPlayer->character_data.getOwp() << ")" << std::endl;
                                sleepMilliseconds(1500);
                                // Bleibe in der Schleife, um neue Eingabe zu fordern
                            }
                        }
                    } else {
                        std::cout << "Ungueltige Zahl. Bitte 1 oder 2 eingeben (oder 'Exit')." << std::endl;
                    }
                } else {
                     std::cout << "Ungueltige Eingabe. Bitte nur 1, 2 oder 'Exit' eingeben." << std::endl;
                }
            } catch (const std::invalid_argument& e) {
                std::cout << "Ungueltige Eingabe. Bitte 1, 2 oder 'Exit' eingeben." << std::endl;
            } catch (const std::out_of_range& e) {
                 std::cout << "Zahl ausserhalb des gueltigen Bereichs." << std::endl;
            }
        } // Ende der Angriffsauswahl-Schleife

        const AttackData& chosenAttack = (attackChoice == 1) ? currentPlayer->character_data.attack1 : currentPlayer->character_data.attack2;

        // OWP verbrauchen, WENN Angriff 2 gewählt wurde
        if (attackChoice == 2) {
            bool success = currentPlayer->character_data.useOwp(chosenAttack.base_damage);
             if(success) {
                std::cout << currentPlayer->name << " verbraucht " << chosenAttack.base_damage << " OWP. Verbleibend: " << currentPlayer->character_data.getOwp() << std::endl;
                sleepMilliseconds(1000);
             } else {
                // Sollte nie passieren wegen der canAfford Prüfung oben
                std::cerr << "[FEHLER] Konnte OWP nicht verbrauchen trotz canAfford Check!" << std::endl;
             }
        }

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

        // Siegbedingung prüfen
        if (opponentPlayer->character_data.isDefeated()) {
            std::cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            std::cout << opponentPlayer->name << " (" << opponentPlayer->character_data.getName() << ") wurde besiegt!" << std::endl;
            std::cout << currentPlayer->name << " (" << currentPlayer->character_data.getName() << ") hat gewonnen!" << std::endl;
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

            displayVictoryFireworks(); // Feuerwerk anzeigen

            break; // Kampfschleife verlassen
        }

        // Spieler wechseln für die nächste Runde
        std::swap(currentPlayer, opponentPlayer);
        round++;
    }
     std::cout << "\n==================== KAMPFENDE ======================" << std::endl;
}
